/* 
 * File:   Dutch_Dozen_V5.0.c
 * Author: James_Vlasblom
 *
 * Created on January 22, 2018.
 * 
 * 
 * About: This version used the menu system for the Multi / Single and Galv
 * modes. Also it uses a 320RGB x 480 dot 16M TFT display controlled by a 
 * HX8357-D00/D01 display controller.
 * 
 * Revision history: 
 * Version 5.0 now includes software for a AT93C96 EPROM. 
 * 
 * Feb 15/18:
 * Modified InterruptISR_V5.0.h to eliminate *HexDelayPt and instead used 
 * Array_Count for both arrays. 
 * 
 * Feb 19/18:
 * Modified Dutch_Dozen_V5.0 & InterruptISR_V5.0.h
 * Added an EEPROM initialization routine which loads the EEPROM with legitimate
 * initial values. This routine is only executed once by means of a flag stored
 * in the EEPROM. 
 * 
 * Feb 21/18:
 * Modified Dutch_Dozen_V5.0 & InterruptISR_V5.0.h
 * Added software for storing the channel klunker is currently on. On power up,
 * the klunker will advance to the channel last fired.
 * 
 * Modified Dutch_Dozen_V5.0 & InterruptISR_V5.0.h
 * Added software for a 'Version' screen. When klunker is off, press both the
 * <RESET> and <FIRE> buttons while klunker is turned on to enter the Version 
 * screen. Release either button to exit Version screen.
 * 
 * May 11, 2018
 * Fixed glitch that occurred when powering up in a non-RESET condition.
 * 
 * May 14, 2018
 * Starting parallel display routine.
 * 
 * Fixed glitch that occurred
 */


// *****************************************************************************
// *****************************************************************************
// Section: Included Files and DEFINE statements
// *****************************************************************************
// *****************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <p24FJ128GA010.h>
#include <xc.h>
#include "ADCInit_V5.0.h"
#include "EEPROM_AT93C96D.h"
#include "Fonts_V5.0.h"
#include "Fonts_Dutch_Logo.h"
#include "Misc_Macro_V5.0.h"
#include "OLED_V3.0.h"
#include "PortInit_V5.0.h"
#include "InterruptISR_V5.0.h"
#include "INTInit_V5.0.h"
#include "TimerInit_V5.0.h"
#include "TFT_ILI9341.h"
#include "SPIInit_V5.0.h"
#include "String.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************

#define CHG_BATT_HIGH 820   // Level for <CHARGING> message.(14.0V)
#define CHG_BATT_LOW 800 //(13.75)
#define CHG_BATT_COM 660    // Level for <CHARGE COMPLETE> message.
#define LOW_BATT 580    // Level for <LOW BATTERY> message.(10.0V)
#define CHG_BATT 525    // Level for <CHARGE BATT> message.(9.0V)


// This functions displays the battery voltage and returns it.
static float Disp_Battery_Voltage(void);
void Arm_Alert(void);
void Fire_Alert(void);
void Latch_Test(void);
void SplashDisplay(void);


static float Batt_Volt = 0; // Variable which stores the battery voltage
int Array_Count = ARRAY_MID; // Count to keep track of the array index.;
int place = 85; // Right justification for the delay count, changes with 2 or 3 digits
int MenuNo = 40; // Set MenuNo initially to running display.
int Count = KLUNK_CH; // Number of Klunker channels + 1.


//******************************************************************************
// MCU Configuration Section
//******************************************************************************


// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF              // Watchdog Timer Disable (Watchdog Timer is disabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(int argc, char** argv) {

    int Channel_count = 0; // Local channel count var. Set to RESET condition.

    // **************************  
    // **** Initialize PORTS ****
    // **************************
    PortInit();

    // ****************************
    // *** Initialize Constants ***
    // ****************************

    // This flag is set because multi-mode delay starts @ 200 (3 digits).
    THREE_DIGIT_FLG = 1;

    // This flag is set once the charging voltage is reached, cleared once
    // charge is complete.
    BATT_SETTLE_FLG = 0;

    // Set initial Button state as 'pressed' because the first press toggles
    // the flag in CN ISR.
    BUTT_PRESS_FLG = 1;

    // *** Initialization SHIFT REGISTER to know quantity ***  
    SERIAL = 0;
    SRCLK = 0;
    SR_LATCH = 0;
    SR_CLEAR = 1; // active low so a 1 is disabled

    // **********************************
    // *** Initialize Various Modules *** 
    // **********************************
    // Note: The ADC and interrupts are not enabled yet as not to interfere
    // With the opening screens.

    // **********************
    // *** Initialize SPI ***
    // **********************
    SPIInit();

    // **********************************
    // *** Initialization TFT_ILI9341 *** 
    // **********************************
    Initialize_TFT_ILI9341();

    // **********************
    // *** Version Screen ***
    // **********************
    // If both the <RESET> and <FIRE> buttons are pressed, version screen will
    // be displayed. Not using the interrupts, just checking the bits.
    // <RESET> = _RF6, <FIRE> = _RB4. Active high.
    if ((_RF6) && (_RB4)) { //  Both buttons are pressed.
        Version();
    }

    // *****************************
    // *** Display splash screen ***
    // *****************************
    SplashDisplay();
    FillScreen_ILI9341(ILI9341_BLACK);

    // *****************************
    // *** EEPROM Initialization ***
    // *****************************
    // Note: EWEN() is called from within EEEPROM_Setup().
    //***EEPROM MOD, remove between bookends***
    //EEPROM_Setup(); // Initialize the EEPROM memory variables.
    //***EEPROM MOD, remove between bookends***


    // **********************************
    // *** Initial EEPROM programming ***
    // **********************************
    // Fill EEPROM with initial settings, (running display, delay = 200ms)
    // This is only done once, on the 1st programming of the board.

    //***EEPROM MOD, remove between bookends***
    // while ((E_Read(EInit_FLG)) != 0x1235) {
    // E_Write(EDelay_Count, ARRAY_MID); // Initialize memory location. (24)
    // E_Write(EDelay_Place, 85); //Initialize memory location. (3 digits)
    // E_Write(EMenu, 40); //Initialize memory location. (running display)
    // E_Write(EDelay_3D_FLG, THREE_DIGIT_FLG); //Initialize memory location.
    // E_Write(EChannel, KLUNK_CH); // Initialize to reset condition.
    // E_Write(EInit_FLG, 0x1235); // set flag so above while() only executes once.
    //}
    //***EEPROM MOD, remove between bookends***

    // *****************************
    // *** Initialize Interrupts ***
    // *****************************
    IntInit();

    // **********************
    // *** Initialize ADC ***
    // **********************
    ADCInit();


    // ************************
    // *** Initialize Timer ***
    // ************************
    TimerInit();
    _TON = 0; // Timer1 off.
    TMR1 = 0; // Clear Timer 1 register.
    CLOCK_FLG = 0; // Clear CLOCK flag. 
    RESET_FLG = 1; // Trigger a RESET condition before entering FLAG checking
    CHANNEL_FLG = 0; // Assume reset condition at power up.

    // Fire_Alert routine checks to see if FIRE button activated upon start-up.
    // Freezes system until <FIRE> button is depressed.
    Fire_Alert();

    // Arm_Alert routine checks to see if ARM switch is  on during start-up.
    // Freezes system until <ARM> switch is turned off.
    Arm_Alert();



    // *** Set FIRE Mode and GALV Mode setting ***
    // ***Next line is commented out because single/multi is now recovered from
    // memory.
    SM_FLG = 0; // Set to single mode (1) for intial setup.
    GALV_FLG = 0; // Set no Galv off.




    /* *** MEMORY RECOVERY SECTION****
     * This section recovers the four sections being stores in memory:
     * 1. single/multi mode (SM_FLG: 0 = multi mode, 1 =  single mode) <EMode>
     * 2. multi mode delay <EDelay>
     * 3. channel number (reset or the last channel fired) <EChannel>
     * 4. menu number (see below for menu descriptions) <EMenu>
     * 
     * *** Menu number/description:
     * M0   -   MAIN selection menu.   
     * M10  -   MAIN selection menu, <fire mode> highlighted.
     * M11  -   FIRE mode menu, <single> highlighted.
     * M12  -   FIRE mode menu, <multi> highlighted.
     * M20  -   Main selection menu, <galv> highlighted.
     * M21  -   GALV menu, <off> highlighted.
     * M22  -   GALV menu, <on> highlighted.
     * M30  -   MAIN menu, <exit> highlighted.
     * M40  -   RUNNING menu.
     * M70  -   ARM conflict warning.
     */

    // ***************************************
    // *** Mode Delay **** MEMORY RECOVERY ***
    // ***************************************
    //***EEPROM MOD, remove between bookends***

    //Array_Count = E_Read(EDelay_Count);
    //place = E_Read(EDelay_Place);
    //THREE_DIGIT_FLG = E_Read(EDelay_3D_FLG);
    //***EEPROM MOD, remove between bookends***

    // ************************************
    // *** Channel **** MEMORY RECOVERY ***
    // ************************************
    // SRCLK controls the output buffers of '245.
    // SR_LATCH shifts the shift register.
    //***EEPROM MOD, remove between bookends***

    //Channel_count = E_Read(EChannel); // Retrieve stored channel value
    //Count = (Channel_count);
    //***EEPROM MOD, remove between bookends***

    // Test to see if channel advancing is necessary.
    if (Channel_count != KLUNK_CH) { // Non reset condition.
        FIRE_FIRST_FLG = 1;
        RESET_LED = 0;
        CHANNEL_FLG = 1; //  Non-reset condition, used in Rotate(), determines bit set for shifting.
        while ((KLUNK_CH - Channel_count) >= 1) {
            Nop();
            SRCLK = 0;
            SERIAL = 0;
            if (FIRE_FIRST_FLG == 1)
                SERIAL = 1;

            SRCLK = 1; // Shift pulse to nest channel/ activate '245.
            Nop();

            SR_LATCH = 1; // Output to SR.
            Nop(); // Timing delay for SR.
            SR_LATCH = 0; // Kill latch pulse.
            Nop();
            FIRE_FIRST_FLG = 0; // Clear FIRE_FIRST flag.
            Channel_count++;
        }
        FIRE_FIRST_FLG = 1;
        SRCLK = 0;
    } else {
        Reset(); // Reset if not in channel recovery mode.
    }

    // Enable the output of the shift register (active low).
    // Initially port F3 set to 1 in 'PortInit' header.
    SHIFT_REG_EN = 0;

    // ****************************************
    //  *** Single/Mode *** MEMORY RECOVERY ***
    // ****************************************
    // Make sure this is the last test before main.
    //***EEPROM MOD, remove between bookends***
    //SM_FLG = E_Read(EMode);
    //***EEPROM MOD, remove between bookends***

    if (!SM_FLG) { // Call Multi() to turn on SM_LED. (if Multi mode)
        Multi();
    }
    _ADON = 1; // Turn A/C on


    // ***********************************
    // *** Display Start Menu Routine ****
    // ***********************************

    Disp_Battery_Voltage();

    // MenuNo is not saved in memory at this time. Because of the slow menu
    // update speed, not all of the menu lines are drawn. This causes issues
    // when jumping to a saved menu. This saved feature will be implemented
    // in future when faster clock speeds and parallel display writing
    // is employed.
    // Therefore we jump straight to running display.

    MenuNo = 40;
    MENU_FLG = 0;
    Menu();


    // *** Main endless program loop
    while (1) // Forever loop.
    {
        // Display and return the battery voltage

        Batt_Volt = Disp_Battery_Voltage();

        // Test for <CHARGING> condition, charging condition is set to >=14V. 
        if (Batt_Volt >= CHG_BATT_HIGH) {
            LineWrite_XY_ILI9341_16x25("<Charging>", 150, Line7, ILI9341_YELLOW, ILI9341_BLACK);
            BATT_SETTLE_FLG = 1;
        }// Test for <CHARGE COMPLETE> condition
        else if ((Batt_Volt < CHG_BATT_LOW) && (Batt_Volt >= CHG_BATT_COM) &&
                (BATT_SETTLE_FLG == 1)) {
            LineWrite_XY_ILI9341_16x25("<Chg Comp>", 150, Line7, ILI9341_WHITE, ILI9341_BLACK);
            // Set charge complete flag so message can only be cleared by 
            // pressing the rotary encoder.
            CHG_COM_FLG = 1;
            BATT_SETTLE_FLG = 0;

        }// Test for <LOW BATT> condition.
        else if ((Batt_Volt <= LOW_BATT) && (Batt_Volt > CHG_BATT)) {
            CHG_COM_FLG = 0;
            LineWrite_XY_ILI9341_16x25("<Low Batt>", 150, Line7, ILI9341_PINK, ILI9341_BLACK);
        }// Test for <CHARGE BATT> condition.
        else if (Batt_Volt < CHG_BATT) {

            CHG_COM_FLG = 0;
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_RED, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_BLACK, ILI9341_RED);
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_RED, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_BLACK, ILI9341_RED);
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_RED, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("<Chg Batt>", 150, Line7, ILI9341_BLACK, ILI9341_RED);

        } else if (CHG_COM_FLG == 0) // Only clear message if not <CHG COM>.
            LineWrite_XY_ILI9341_16x25("          ", 150, Line7, ILI9341_BLACK, ILI9341_BLACK);
        FillRec_ILI9341(135, 177, 1, 60, ILI9341_GREEN); // Replace vertical line.

        //DelayMs(500); // This delay keeps the menu display from glitching.
    }

    return (EXIT_SUCCESS);
} // End of main program loop.

void Arm_Alert(void) {
    if (ARM_SW == 1) {

        // disable all interrupts
        IEC0bits.INT0IE = 0; // disable INT0 ISR
        IEC1bits.INT1IE = 0; // disable INT1 ISR
        IEC1bits.INT2IE = 0; // disable INT2 ISR
        IEC3bits.INT3IE = 0; // disable INT3 ISR
        IEC3bits.INT4IE = 0; // disable INT4 ISR
        IEC0bits.T1IE = 0; // disable Time1 interrupt
        IEC1bits.CNIE = 0; // disable CN ISR

        // *** Warning message here ***
        LineWrite_XY_ILI9341_16x25("*** <ARM> on ***", 35, Line3, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25(" Turn off to continue.", 0, Line4, ILI9341_WHITE, ILI9341_BLACK);

        while (ARM_SW == 1) // While ARM switch still prematurely on:
        {
            ARM_LED = ~ARM_LED; // toggle ARM LED.
            DelayMs(200);
        }
        ARM_LED = 0; //Turn off ARM LED
        // Erase the caution message.
        LineWrite_XY_ILI9341_16x25("*** <ARM> on ***", 35, Line3, ILI9341_BLACK, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25(" Turn off to continue.", 0, Line4, ILI9341_BLACK, ILI9341_BLACK);

        // *** Keep CN Interrupts disabled until FIRE_)ALERT routine run
        // re-enable all interrupts
        IEC0bits.INT0IE = 1; // enable INT0 ISR
        IEC1bits.INT1IE = 1; // enable INT1 ISR
        IEC1bits.INT2IE = 0; // disable INT2 ISR (not being used)
        IEC3bits.INT3IE = 1; // enable INT3 ISR
        IEC3bits.INT4IE = 1; // enable INT4 ISR
        IEC0bits.T1IE = 1; // enable Time1 interrupt
        // Note: dont enable CN Interrupts yet, first check FIRE_ALERT
    }
}

static float Disp_Battery_Voltage(void) {

    int count = 0;
    int a = 0;
    int sum = 0;
    int num = 0;
    int *ADCPtr;
    static float Final_ADC = 0;

    for (count = 0; count <= 3; count++) { // sample 4 buffers.
        // start sampling
        _SAMP = 1; // Manually start the ADC sampling
        while (_SAMP == 0) { // Waiting for sampling ton be done.
        }
        _SAMP = 0;
        DelayUs(1); // Another delay to help things settle.
    }
    while (!_DONE) {
    } // Wait for A/D conversion to finish.

    ADCPtr = &ADC1BUF1;
    for (count = 0; count <= 3; count++) {
        sum = sum + *ADCPtr++;
    }
    sum = sum >> 2; // divide by 4

    Final_ADC = sum;
    float Vin_CH1 = ((Final_ADC * V_Step) / 0.1875);

    // Buffer for Int to ASCII conversion, ADC channel 1.
    char buf1 [5] = "";
    // **** Convert Hex to Dec ASCII
    // Always a 5 digit string because of how the buffer is defined.
    // Format: char *  itoa ( int value, char * str, int base );
    itoa(buf1, (Vin_CH1), 10);

    // Convert the 
    num = atoi(buf1); // Convert string back to a int.

    // This checks if the ADC string will start as 4 ot 5 digits and 
    // consequently right justifies accordingly.
    if (num >= 10000) {
        LOW_VOLT_FIRST_FLG = 0;
        column = 222;
    }

    // Set lower limit (0.1 V) of voltage to accept.
    // If below, set display to 0.00 V.
    if (num <= 100) {
        buf1[0] = '0';
        buf1[1] = '0';
        buf1[2] = '0';
    }

    if ((num <= 9999) && (LOW_VOLT_FIRST_FLG == 0)) {
        LineWrite_XY_ILI9341_16x25(" ", 222, Line6, ILI9341_BLACK, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("V", 286, Line6, ILI9341_CYAN, ILI9341_BLACK);
        LOW_VOLT_FIRST_FLG = 1;
        column = 238;

    }

    // Note string is 4 for voltages under 10V, 5 for over 10V
    count = strlen(buf1);
    a = count;

    // Place the decimal place with two decimal places, losing one decimal
    // place in precision. eg. 12345 = 12.34, 9345 = 9.34
    while (a >= (count - 2)) {
        buf1[a - 1] = buf1[a - 2];
        a--;
    }
    buf1[a] = '.';

    // This if statement determines how many characters in the string and
    // shifts accordingly while adding a leading zero.
    if (a <= 0) {
        buf1[3] = buf1[2];
        buf1[2] = buf1[1];
        buf1[1] = buf1[0];
        buf1[0] = '0';
    }

    LineWrite_XY_ILI9341_16x25(buf1, column, Line6, ILI9341_CYAN, ILI9341_BLACK);

    return (Final_ADC);

}

void Fire_Alert(void) {
    if (FIRE_SW == 1) {

        // disable all interrupts
        IEC0bits.INT0IE = 0; // disable INT0 ISR
        IEC1bits.INT1IE = 0; // disable INT1 ISR
        IEC1bits.INT2IE = 0; // disable INT2 ISR
        IEC3bits.INT3IE = 0; // disable INT3 ISR
        IEC3bits.INT4IE = 0; // disable INT4 ISR
        IEC0bits.T1IE = 0; // disable Time1 interrupt

        LineWrite_XY_ILI9341_16x25("*** <FIRE> Button ***", 0, Line3, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25("is pressed.", 90, Line4, ILI9341_WHITE, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25(" Turn off to continue.", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);

        while (FIRE_SW == 1) // While ARM switch still prematurely on:
        {
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.                    
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.
            DelayMs(50);
            FIRE_LED = ~FIRE_LED; // toggle ARM LED.

            DelayMs(500);
        }
        FIRE_LED = 0; //Turn off ARM LED
        // Erase the caution message.
        LineWrite_XY_ILI9341_16x25("*** <FIRE> Button ***", 0, Line3, ILI9341_BLACK, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("is pressed.", 90, Line4, ILI9341_BLACK, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25(" Turn off to continue.", 0, Line5, ILI9341_BLACK, ILI9341_BLACK);
        // re-enable all interrupts
        IEC0bits.INT0IE = 1; // enable INT0 ISR
        IEC1bits.INT1IE = 1; // enable INT1 ISR
        IEC1bits.INT2IE = 0; // disable INT2 ISR (not being used)
        IEC3bits.INT3IE = 1; // enable INT3 ISR
        IEC3bits.INT4IE = 1; // enable INT4 ISR
        IEC0bits.T1IE = 1; // enable Time1 interrupt
        // Note: CN interrupt enabled in RESET().
    }
}

void Latch_Test(void) {
    // not sure how to initiate this
}

void SplashDisplay(void) {
    long int count = 0; // count for the logo array.
    int row = 0; // Display rows, 58-125 (68 total).
    int column = 0; // Display columns, 0-319 (320 total).
    int octet = 0; // Byte in each row, 1 - 40 (40 total).
    int o_bit = 0; // Bit of each octet, 0-7, (8 total).
    int temp = 0; // load each array element into temp space.
    int mask = 0x80; // Mask for testing MSB for.

    // Start out with white screen.
    FillRec_ILI9341(0, 0, 320, 239, ILI9341_WHITE);

    // Read each octet (byte) from the array and test each bit. If the bit
    // is set draw a pixel of chosen colour at column, row (x, y) and increment 
    // row.

    for (row = 78; row <= 145; row++) { // Adjusted for centre.
        for (octet = 0; octet <= 39; octet++) {// 40 Octets per row.
            for (o_bit = 0; o_bit <= 7; o_bit++) {// Each bit in each octet.
                temp = ILI9341_Dutchlogo [count] & mask;
                if (temp) { // Test if the result is not zero
                    // This test a bit of the left for a different colour.
                    if (column <= 64) {
                        DrawPixel_ILI9341(column, row, ILI9341_ORANGE);
                    } else {
                        DrawPixel_ILI9341(column, row, ILI9341_BLUE);
                    }
                }
                mask = mask / 2; // shift mask bit to the right;
                column++; //Has value of 0 - 320
            }
            count++; // Increment array count.
            mask = 0x80; // Reset mask to original value.
        }
        column = 0; // reset column after each row increment.
    }
    DelayMs(1500);
}
