/* 
 * File:   Dutch_Dozen.c
 * Author: james_vlasblom
 *
 * Created on February 22, 2016, 5:37 PM
 * 
 * 
 * About: This version used the mennu system for the Multi / Single and Galv
 * modes.
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

#include "ADCInit.h"
#include "Fonts.h"
#include "Misc_Macro.h"
#include "OLED.h"
#include "PortInit.h"
#include "InterruptISR.h"
#include "INTInit.h"
#include "TimerInit.h"
#include "SPIInit.h"
#include "String.h"


// *****************************************************************************
// *****************************************************************************
// Section: File Scope Variables and Functions
// *****************************************************************************
// *****************************************************************************

#define CHG_BATT_HIGH 820   // Level for <CHARGING> message.(14.0V)
#define CHG_BATT_LOW 800 //(13.75)
#define CHG_BATT_COM 660    // Level for <CHARGE COMNPLETE> message.
#define LOW_BATT 580    // Level for <LOW BATTERY> message.(10.0V)
#define CHG_BATT 525    // Level for <CHARGE BATT> message.(9.0V)

void Arm_Alert(void);
void Fire_Alert(void);
void Latch_Test(void);
void SplashDisplay(void);

extern int Skip;

int PortB_Temp = 0;
int PortB_Mask = 0x001c;
int MenuNo = 0;
int count = 0;

//******************************************************************************
// MCU Configuration Section
//******************************************************************************

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCDIV           // Oscillator Select (Fast RC Oscillator with Postscaler (FRCDIV))
#pragma config IESO = OFF               // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) disabled)


// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
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


    // *** Initialize the PORTS ***
    PortInit();

    // This flag is set once the charging voltage is reached, cleared once
    // charge is complete.
    BATT_SETTLE_FLG = 0;

    // Denotes initial startup, allows Version screen to be displayed if the
    // <RESET> button is pressed.
    VER_FLG = 1;

    // *** Reset system from start ***
    Reset();

    // *** Initialization SHIFT REGISTER to know quantity ***  
    SERIAL = 0;
    SRCLK = 0;
    SR_LATCH = 0;
    SR_CLEAR = 1; // active low so a 1 is disabled

    // *** Initialize Interrupts and Modules *** 
    // Note: The ADC and interrupts are not enabled yet as not to interfere
    // With the opening screens.
    TimerInit();
    SPIInit();

    // *** Initialize OLED ***
    ResetOLED();
    InitializeOLED();
    ClearOLED(); // Clear the OLED.

    // *** Display splash screen ***
    SplashDisplay();

    // *** Initialize the interrupts
    IntInit();

    // *** Initialize ADC ***
    ADCInit();

    //Latch_Test();   // Determine if klunker is to be put into LATCH mode.

    // Fire_Alert routine checks to see if FIRE button activated upon start-up.
    // Freezes system until <FIRE> button is depressed.
    Fire_Alert();

    // Arm_Alert routine checks to see if ARM switch is  on during start-up.
    // Freezes system until <ARM> switch is turned off.
    Arm_Alert();


    // Enable the output of the shift register (active low).
    // Initially port F3 set to 1 in 'PortInit' header.
    SHIFT_REG_EN = 0;

    // *** Set FIRE Mode and GALV Mode setting ***
    SM_FLG = 1; // Set to single mode.
    GALV_FLG = 0; // Set no Galv off.


    // *** Display Start Menu Routine ****

    MenuNo = 10; // Initialize menu to start Menu
    MENU_FLG = 1; // Set to MENU mode.
    Menu();


    // *** Initial Firing Mode conditions
    SM_LED = 0; // turn SM LED off
    _TON = 0; // Timer1 off.
    TMR1 = 0; // Clear Timer 1 register.
    CLOCK_FLG = 0; // Clear CLOCK flag. RESET_FLG = 1; // Trigger a RESET condition before entering FLAG checking
    Reset();


    // *** Grab an image of PORTB before Change Notification Interrupt enabled.
    PortB_Temp = PORTB & PortB_Mask; // get initial PORTB value 

    _ADON = 1; // Turn A/C on


    /*
        // Attempt to Vertical scrolling (left to right)
    
        WriteCommand(0x29); // Set vertical scroll
        WriteCommand(0x00); // No horizontal scroll
        WriteCommand(0x00); // dummy byte for start page address
        WriteCommand(0x01); // 6 frames between each scroll step
        WriteCommand(0x00); // dummy byte for end page address
        WriteCommand(0x03); // 1 byte offset for scroll
     * 
        // Activate scroll
        WriteCommand(0x2F);
     
     */
    // *** Main endless program loop
    while (1) // Forever loop.
    {
        while (MENU_FLG == 1) {
            // Hold in Menu screen the first time around until MenuNo = 40
            // MenuNo will be 40 upon selecting <EXIT> from the menu.
        }

        // start sampling
        _SAMP = 1; // Manually start the ADC sampling
        Nop();
        _SAMP = 0;

        // Test for <CHARGING> condition, charging condition is set to >=14V. 
        if (ADC1BUF1 >= CHG_BATT_HIGH) {
            PageWrite("<Charging>", 3);
            BATT_SETTLE_FLG = 1;
        }// Test for <CHARGE COMPLETE> condition
            // The BATT_SETTLE_FDLG ensures the message is only displayed coming
            // off of charge, not on the way up.
            // *** Note: pressing the reset button will clear this message
        else if ((ADC1BUF1 < CHG_BATT_LOW) && (ADC1BUF1 >= CHG_BATT_COM) &&
                (BATT_SETTLE_FLG == 1)) {
            PageWrite("<Chg Comp>", 3);

            // Set charge complete flag so message can only be cleared by 
            // pressing the rotary encoder.
            CHG_COM_FLG = 1;
            BATT_SETTLE_FLG = 0;

        }
            // Test for <LOW BATT> condition.
        else if ((ADC1BUF1 <= LOW_BATT) && (ADC1BUF1 > CHG_BATT)) {
            CHG_COM_FLG = 0;
            PageWrite("<Low Batt>", 3);
        }
            // Test for <CHARGE BATT> condition.
        else if (ADC1BUF1 < CHG_BATT) {

            CHG_COM_FLG = 0;

            PageWrite("<Chg Batt>", 3);
            DelayMs(50);
            RV_PageWrite("<Chg Batt>", 3);
            DelayMs(50);
            PageWrite("<Chg Batt>", 3);
            DelayMs(50);
            RV_PageWrite("<Chg Batt>", 3);
            DelayMs(50);
            PageWrite("<Chg Batt>", 3);
            DelayMs(50);
            RV_PageWrite("<Chg Batt>", 3);
            DelayMs(50);


        } else if (CHG_COM_FLG == 0) // Only clear message if not <CHG COM>.
            PageWrite("                ", 3);
        DelayMs(500); // This delay keeps the menu display from glitching.
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
        ClearOLED();
        PageWrite("*** ARM Switch on ***", 0);
        PageWrite(" Turn off to continue", 3);
        while (ARM_SW == 1) // While ARM switch still prematurely on:
        {

            ARM_LED = ~ARM_LED; // toggle ARM LED.
            DelayMs(200);
        }
        ARM_LED = 0; //Turn off ARM LED

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

void Fire_Alert(void) {
    if (FIRE_SW == 1) {

        // disable all interrupts
        IEC0bits.INT0IE = 0; // disable INT0 ISR
        IEC1bits.INT1IE = 0; // disable INT1 ISR
        IEC1bits.INT2IE = 0; // disable INT2 ISR
        IEC3bits.INT3IE = 0; // disable INT3 ISR
        IEC3bits.INT4IE = 0; // disable INT4 ISR
        IEC0bits.T1IE = 0; // disable Time1 interrupt

        // *** Warning message here ***
        ClearOLED();
        PageWrite("  *** FIRE Button ***", 0);
        PageWrite("       is Pressed.", 1);
        PageWrite(" Turn off to continue", 3);

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
    int a;
    for (a = 0; a <= 527; a++) // Loop because characters are 5 wide
        WriteData(Keeler_Logo[a]);
    WriteCommand(SSD1305_DISPLAYON);
    
    DelayMs(2000);
    ClearOLED();
       
}



