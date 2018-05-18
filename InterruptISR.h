
/* 
 * File:            InterruptISR.h
 * Author:          James Vlasblom
 * Date:            September 29, 2016
 * Comments:        This file contains all the ISR prototypes and function
 *                  definitions for the interrupt service routines.
 *                  ***Note: Initialization of each module and each modules     
 *                  interrupt settings is contained in its own header file.
 *                  
 *                  The file also contains the menu logic for displaying and
 *                  controlling the menu system.
 *                     
 * 
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTERRUPTISR_50_Shot_H
#define	INTERRUPTISR_50_Shot_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdlib.h>
#include <string.h>
#include "ADCInit_50_Shot.h"
#include "INTInit_50_Shot.h"
#include "Misc_Macro_50_Shot.h"
#include "OLED_50_Shot.h"


// *** Local define statements here ***
#define MASK 0x0001
#define PORTB_OFFSET 2
#define CW 0
#define CCW 1
#define ONE 1
#define ZERO 0
// Dimension of Array_DelayMs[].
// 1 - the number of elements in array.
#define ARRAY_MAX 39                   

// This number represents where in the array the program starts in multi mode.
#define ARRAY_MID 24           

// Resistor ratio for Internal voltage (R1 & R2)
#define RESISTOR_RATIO_CH1  0.1875 

// Resistor ratio for Firing Battery voltage (R16 & R17)
#define RESISTOR_Ratio_CH2 1     

// Step voltage for ADC with resolution of 2^10 (1024) and assumed max input
// voltage of 3.3 V (3.3 / (2^10 - 1)
#define V_Step 3.225806         

#define VIN_MAX_CH1 16   // Assumed maximum input voltage on channel 1 is 16V
#define VIN_MAX_CH2 28   // Assumed maximum input voltage on channel 2 is 28V

// **********************
// *** ISR Prototypes ***
// **********************

void __attribute__((interrupt)) _ADC1Interrupt(void);
void __attribute__((interrupt)) _CNInterrupt(void);
void __attribute__((interrupt)) _INT0Interrupt(void);
void __attribute__((interrupt)) _INT1Interrupt(void);
void __attribute__((interrupt)) _INT2Interrupt(void);
void __attribute__((interrupt)) _INT3Interrupt(void);
void __attribute__((interrupt)) _INT4Interrupt(void);
void __attribute__((interrupt)) _T1Interrupt(void);
void __attribute__((interrupt)) _T4Interrupt(void);

void Arm(void);
void Check_Mark(void);
void X_Mark (void);
void Clear_SR(void);
void Firing(void);
void Menu(void);
void Multi(void);
void Reset(void);
void Rotate(void);
void RunningDisplay();
void Shift(void);
void Single(void);
void Version(void);

//*** Extern statements go here ***
extern int PortB_Temp; // variable holding PortB_Temp ^ PORTB
extern int PortB_Mask;
extern int MenuNo;
extern int SendReg;

int Array_Count = ARRAY_MID; // Count to keep track of the array index.

// Register which holds one more than the # of hits
int Result = 0;
int SendReg = 0; // Register which is outputted to the Shift Reg.
int Count = KLUNK_CH; // Number of Klunker channels + 1.
int Multi_Delay = 0x30d; // Originally set to 500mS. Changes from F42
int Multi_Delay_Step = 0x009c; // Multi delay increment;
int Multi_Delay_Max = 0x061A;
int Multi_Delay_Min = 0x009C;
int Skip = 1; // used for skipping ClearOLED(), eliminates blanking. 0 = skip.

int Col_High = 0x15;
int Col_Low = 0x05;

// Array dimension is kept track of using the Array_Count variable.
// NOTE: Because when decreasing the time constant, we subtract two from 
// DelayHexPt, the first entry in both arrays is a dummy value and is not read.
char *Array_DelayMs [] = {
    "10", "10", "15", "20", "25",
    "30", "35", "40", "45", "50",
    "60", "70", "80", "90", "100",
    "110", "120", "130", "140", "150",
    "160", "170", "180", "190", "200",
    "220", "240", "260", "280", "300",
    "340", "380", "420", "460", "500",
    "550", "600", "650", "700", "750"
};

int Array_DelayHex [] = {
    0x9C, 0x9C, 0xEA, 0x138, 0x186,
    0x1D4, 0x222, 0x271, 0x2BF, 0x30D,
    0x3A9, 0x445, 0x4E2, 0x57E, 0x61A,
    0x6B6, 0x753, 0x7EF, 0x88B, 0x927,
    0x9C4, 0xA60, 0xAFC, 0xB98, 0xC35,
    0xD6D, 0xEA6, 0xFDE, 0x1117, 0x124F,
    0x14C0, 0x1731, 0x19A2, 0x1C13, 0x1E84,
    0x2191, 0x249F, 0x27AC, 0x2AB9, 0x2DC6
};

// Array pointers for the mode choices in the menu system
char *FireModePt [] = {"-MULTI-", "-SINGLE-"};
char *GalvStatePt [] = {"-OFF-", "-ON-"};

// Pointer for the middle of the mS array.
int *DelayHexPt = (Array_DelayHex + ARRAY_MID); // Set in middle of array.

// ***********************
// *** ISR Definitions ***
// ***********************

// *** A/D Conversion Done Interrupt ***

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {

    int count = 0;
    int a = 0;
    int num = 0;
    static float Final_ADC = 0;
    static float Temp_ADC;
    static int Count_ADC = 0;

    // The next few lines takes the average of 2 samples of the ADC
    Temp_ADC = Temp_ADC + ADC1BUF1;
    Count_ADC++;
    if (Count_ADC >= 2) {
        Final_ADC = (Temp_ADC / 2);
        Count_ADC = 0;
        Temp_ADC = 0;
    }

    float Vin_CH1 = ((Final_ADC * V_Step) / 0.1875);

    // Buffer for Int to ASCII conversion, ADC channel 1.
    char buf1 [5] = "";

    // Buffer for Int to ASCII conversion, ADC channel 2.
    //char buf2 [5] = ""; 

    // When _ASAM = 0, sampling stops automatic mode and sampling conversions
    // only happen under software control via the _SAMP = 1 command.
    _ASAM = 0;

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
        Col_Low = 0x05;
    }

    // Set lower limit (0.1 V) of voltage to accept.
    // If below, set display to 0.00 V.
    if (num <= 100) {
        buf1[0] = '0';
        buf1[1] = '0';
        buf1[2] = '0';
    }

    if ((num <= 9999) && (LOW_VOLT_FIRST_FLG == 0)) {
        PageWriteCol("        ", 3, 0x05, 0x15); // write blank
        PageWriteCol("V", 3, 0x07, 0x017);
        LOW_VOLT_FIRST_FLG = 1;
        Col_Low = 0x0B;
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

    PageWriteCol(buf1, 3, Col_Low, Col_High); // Write results in HEX.
    DelayMs(10); // delay to stop flicker

    IFS0bits.AD1IF = 0; // reset A/D Conversion Done interrupt flag
}

// *** Input Change Notification Interrupt ***

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    int Result_bit = 0;
    int Result_Temp = 0;

    Result = PortB_Temp ^ PORTB; // determine which pin has changed
    Result_Temp = Result; // keep a copy of Result_bit for other switches
    Result = Result >> PORTB_OFFSET; // shift potential change bits to end
    Result_bit = Result & MASK; // so can test bit individually

    // ** This first test isn't done because the MULTI / SINGLE mode is now
    // done within the menu system. (next 5 lines)
    // Testing for SM switch
    //if (Result_bit == 1) {
    // IFS1bits.CNIF = 0; // reset CN interrupt flag
    //  Multi();
    // }

    // Shift the Result_Temp over for next interrupt test
    Result_bit = (Result_Temp >> (PORTB_OFFSET + 1)) & MASK;

    // Testing for ARM switch
    if (Result_bit == 1) {
        IFS1bits.CNIF = 0; // reset CN interrupt flag
        Arm();
    }

    // Shift the Result_Temp over for next interrupt test
    Result_bit = (Result_Temp >> (PORTB_OFFSET + 2)) & MASK;

    // Testing for FIRE switch
    if (Result_bit == 1) {
        IFS1bits.CNIF = 0; // reset CN interrupt flag

        // Only allow FIRE mode when not in MENU mode. The MENU_FLG is 
        // changed entering and leaving MENU mode and initially.
        if (MENU_FLG == 0)
            Firing();
    }

    PortB_Temp = PORTB; // get initial PORTB value 
    PortB_Temp = PortB_Temp & PortB_Mask;

    IFS1bits.CNIF = 0; // reset CN interrupt flag
}


// *** External INT0 Interrupt - RESET INTERRUPT ***

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {

    // If the <RESET> button is pressed while in the menu system the version
    // will display, but only this time. The VERSION_FLG initially set in main()
    if (VER_FLG == 1) {
        Version();
    }

    // Tests if the <Chg Comp> message is displayed, if so, clears it. 
    // When the <Chg Comp> is displayed it only clears when the <RESET> button
    // is pressed.
    if (CHG_COM_FLG == 1) {
        PageWrite("             ", 3);
        DelayMs(1); // Delay for OLED to settle
        CHG_COM_FLG = 0;
    }

    // Otherwise RESET (finally.)
    Reset();
    IFS0bits.INT0IF = 0; // reset INT0 interrupt flag
}


// *** External INT1 Interrupt - ON-SWITCH INTERRUPT ***

void __attribute__((interrupt, auto_psv)) _INT1Interrupt(void) {

    _LATE0 = ~_LATE0;
    IFS1bits.INT1IF = 0; // reset INT1 interrupt flag
}


// *** External INT2 Interrupt ***

void __attribute__((interrupt, auto_psv)) _INT2Interrupt(void) {
    //*** not being used presently ***
}


// *** External INT3 Interrupt - ENCODER BUTTON ***
// This interrupt function only sets the new menu (based on the current menu
// and the rotary encoder's action) and initializes conditions for that
// particular action.
// All display commands are done in the switch statement in the Menu() function.

void __attribute__((interrupt, auto_psv)) _INT3Interrupt(void) {

    // Change from M10 to M11.
    if (MenuNo == 10) {
        MenuNo = 11;
        Skip = 1;
        Menu();
    }// Change from M11 back to M10.
        // This puts klunker into SINGLE mode.
    else if (MenuNo == 11) {
        MenuNo = 10;
        Skip = 1; // Dont skip ClearOLED().
        Single();
        Menu();
    }// Change from M12 back to M10.
        // This puts klunker into MULTI mode.
    else if (MenuNo == 12) {
        MenuNo = 10;
        Skip = 1; // Dont skip ClearOLED().
        Multi();
        Menu();
    }// Change from M21 back to M20.
        // This action also turns GALV function off.
    else if (MenuNo == 21) {
        MenuNo = 20;
        Skip = 1;
        GALV_FLG = 0; // Set GALV flag.
        GALV_OUT = 0; // Turn GALV function and LED off.
        Menu();
    }// Change from M22 back to M20.
        // This action also turns GALV function on.
    else if (MenuNo == 22) {

        // Test for ARMA_FLG here.
        if (ARM_FLG == 1) {
            ARM_GLV_FLG = 1;
            MenuNo = 70; // Menu 70 is the ARM/GALV conflict message.
            Menu();
        }
        Reset(); // reset before entering GALV mode.
        MenuNo = 22; // ** For full function uncomment Menu() and Change
        // MenuNo = 21 instead of MenuNo = 22.
        Skip = 1;
        GALV_FLG = 1; // Set GALV flag.
        GALV_OUT = 1; // Turn GALV function and LED on.
        // Menu();
    }// Change from M20 to M21.
    else if (MenuNo == 20) {
        MenuNo = 21;
        Skip = 1; // Dont skip ClearOLED().
        Menu();
    }// Change from M30 to Running Display
    else if (MenuNo == 30) {
        MenuNo = 40;
        VER_FLG = 0;
        MENU_FLG = 0; // leaving MENU mode (OK to fire).
        Skip = 1; // Do not skip ClearOLED().
        IEC0bits.AD1IE = 1; // enable A/D Conversion Done interrupt
        Menu();
    }// Change back to main menu.
    else if (MenuNo == 40) {
        MENU_FLG = 1; // entering MENU mode (Not OK to fire).
        MenuNo = 10; // Enter menu in M10 screen.
        Skip = 1; // Do not skip ClearOLED().
        IEC0bits.AD1IE = 0; // disable A/D Conversion Done interrupt
        Menu();
    }
    IFS3bits.INT3IF = 0; // reset INT3 interrupt flag
}


// *** External INT4 Interrupt - ROTORY ENCODER A ***
void __attribute__((interrupt, auto_psv)) _INT4Interrupt(void) {

    long time = 15;    // Delay time for encode ton settle.
    int Encoder_Dir = CW; // Encoder state, 0 = CW, 1 = CCW.
    Encoder_Dir = _RF0;
    DelayMs(time); // Delay for encoder B to settle

    // *** This series of conditional statements assume Running Display Screen
    // in MULTI mode.
    if ((MenuNo == 40) && (SM_FLG == 0)) {// Done with menu, goto running display.

        if (Encoder_Dir == CW) { // *** INCREASING DELAY (CCW)***
            ON_LED = ~ON_LED;
            Array_Count++; // Increase the count of both arrays.
            DelayHexPt++;
            if (Array_Count > ARRAY_MAX) { // Check if hit upper limit of array.
                Array_Count = ARRAY_MAX; // Oops, went above, hold at upper limit.
                DelayHexPt--; // Oops, went above, subtract one.
            }
            
            PageWriteCol("            ", 1, 0x04, 0x12); // Write to display.
            PageWriteCol("ms", 1, 0x0A, 0x013);
            PageWriteCol(Array_DelayMs[Array_Count], 1, 0x04, 0x12);

        } else if (Encoder_Dir == CCW) { // *** DECREASING DELAY (CW)***
            ON_LED = ~ON_LED;
            DelayHexPt--; // Decrease both arrays.
            Array_Count--;
            if (TMR1 >= *DelayHexPt) // Check if TMR is above PRx, if
                TMR1 = *(DelayHexPt - 2); // so, stick TMR below PRx.

            if (Array_Count < 1) { // Check for lower array limit.
                Array_Count = 1; // Oops, went below, hold at lower limit.
                DelayHexPt++; // Oops, went below, add one back. 
            }
            
            PageWriteCol("            ", 1, 0x04, 0x12); // Write to display.
            PageWriteCol("ms", 1, 0x0A, 0x013);
            PageWriteCol(Array_DelayMs[Array_Count], 1, 0x04, 0x12);
        }
        PR1 = *DelayHexPt;
    }


    // NOTE: encoder_B = 0: CW, encoder_B = 1: CCW
    // *** This series of conditional statements assume Menu Screen ***
    // Start with CCW then move to CW conditions.

    if (Encoder_Dir == CCW) {

        // Menu 0 to 10 (CCW)
        if (MenuNo == 0) {
            MenuNo = 10;
            Skip = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 11 to 12 (CCW)
        else if (MenuNo == 11) {
            MenuNo = 12;
            Skip = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 10 to 20 (CCW)
        else if (MenuNo == 10) {
            MenuNo = 20;
            Menu();
            Skip = 0; // Skip Next ClearOLED().
        }// Menu 21 to 22 (CCW)
        else if (MenuNo == 21) {
            MenuNo = 22;
            Skip = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 20 to 30 (CCW)
        else if (MenuNo == 20) {
            MenuNo = 30;
            Skip = 0; // Skip Next ClearOLED().
            Menu();
        }


        // Now determine actions for CW actions
    } else if (Encoder_Dir == CW) {

        // Menu 30 to 20 (CW)
        if (MenuNo == 30) {
            MenuNo = 20;
            Skip = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 20 to 10 (CW)
        else if (MenuNo == 20) {
            MenuNo = 10;
            Menu();
            Skip = 0; // Skip next ClearOLED().
        }// This menu is skipped because viewed as redundant, MENU does not have
        // to start highlighted.
        // Menu 10 to 0 (CW)
            //  else if (MenuNo == 10) {
            //      MenuNo = 0;
            //      Skip = 0; // Skip Next ClearOLED().
            //      Menu();
            //  }
             
            // Menu 12 to 11 (CW)
        else if (MenuNo == 12) {
            MenuNo = 11;
            Skip = 0; // Skip next ClearOLED().
            Menu();
        }// Menu 22 to 21 (CCW)
        else if (MenuNo == 22) {
            // The disables the rotary encode during ARM/GLV conflict.
            if (ARM_GLV_FLG == 0) {
                MenuNo = 21;
                Skip = 0; // Skip Next ClearOLED().
                Menu();
            }
        }
    }
    IFS3bits.INT4IF = 0; // reset INT4 interrupt flag
}


// *** Timer1 TI1 Interrupt ***
// This timer controls the timing of the MULTI LED and the MULTI function.
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {

    TMR1 = 0x00;                // clears contents of time register
    SM_LED = ~SM_LED;            // toggle the SM LED.
    CLOCK_FLG = ~CLOCK_FLG;     // Toggle CLOCK flag.
    
    // 
    if ((FIRE_FLG == 1) && (CLOCK_FLG == 1)) {  
        if (Array_Count <= 0)
            Reset();
        Rotate();
    } else {
        //SRCLK = 0;            // Reset output.
        Nop();
        FIRE_LED = 0;
    }
    IFS0bits.T1IF = 0; // reset Timer1 interrupt flag  
}

void __attribute__((interrupt, auto_psv)) _T4Interrupt(void) {
    // Not being used currently.
    TMR4 = 0x00; // clears contents of time register
    IFS1bits.T4IF = 0; // reset Timer4 interrupt flag 
}

// ***************************************************************************
// *************                   Non IRS FUNCTIONS             *************
// ***************************************************************************

void Arm(void) {
    _CNARM = 0;
    if (ARM_SW == 1) { // If the ARM switch is activated:
        ARM_FLG = 1; // SET the ARM flag.
        ARM_LED = 1; // SET the ARM LED.
    } else { // The ARM switch is deactivated:
        if (ARM_GLV_FLG == 1) {
            ARM_GLV_FLG = 0;
            MenuNo = 22;
            GALV_OUT = 1;
            Menu();

        }// conflict now resolved
        ARM_FLG = 0; // CLEAR the ARM flag.
        ARM_LED = 0; // CLEAR the ARM flag.
    }

}

void Clear_SR(void) {
    SR_CLEAR = 0; // clear Shift Register
    Nop();
    SR_CLEAR = 1;
    Nop();
    SR_LATCH = 1;
    Nop();
    SR_LATCH = 0;
    Nop();
}

void Firing(void) {
    _CNFIRE = 0; // Clear FIRE EVENT indicator.
    if (FIRE_SW == 1) // SM=0 - multi, SM=1 - single
    {
        RESET_LED = 0; // Clear RESET_LED;
        Nop();
        FIRE_FLG = 1; // Not first time through this function
        if (FIRE_HIGH_FLG == 0) // indicates a first time +ve going FIRE pulse:
        { // Set appropriate flags.
            FIRE_FIRST_FLG = 1;
            FIRE_HIGH_FLG = 1;
        }

        Shift();

    } else {
        FIRE_FLG = 0; // Clear the FIRE flag.
        FIRE_LED = 0; // Clear the FIRE LED.
        Nop();
        SRCLK = 0; // Turn SR clock off *** effectively turning output
        Nop(); // relays off.
    }

}

// This function changes the menu displayed depending on the MenuNo variable.
void Menu(void) {
    switch (MenuNo) {
        case 0: // Display Menu: M0.
            if (Skip)
                ClearOLED();
            PageWrite("           ", 0);
            RV_PageWriteCol("MENU", 0, 0x06, 0x13);
            PageWrite("FIRE Mode:", 1);
            PageWriteCol(FireModePt[SM_FLG], 1, 0x0A, 0x14);
            PageWrite("GALV:", 2);
            PageWriteCol(GalvStatePt[GALV_FLG], 2, 0x0A, 0x14);
            PageWrite("EXIT Menu", 3);
            break;

        case 10: // Display Menu: M10.
            if (Skip)
                ClearOLED();
            PageWriteCol("MENU", 0, 0x06, 0x13);
            RV_PageWrite("FIRE Mode:", 1);
            PageWriteCol(FireModePt[SM_FLG], 1, 0x0A, 0x14);
            PageWrite("GALV:", 2);
            PageWriteCol(GalvStatePt[GALV_FLG], 2, 0x0A, 0x14);
            PageWrite("EXIT Menu", 3);
            Skip = 0; // Reset Skip to 0.
            break;

        case 11: // Display Menu: M11.
            if (Skip) // eliminates blanking effects from ClearOLED().
                ClearOLED();
            PageWriteCol("Fire Mode:", 0, 0x0D, 0x11);
            RV_PageWrite("SINGLE", 1);
            PageWrite("MULTI", 2);
            break;

        case 12: // Display Menu: M12.
            PageWrite("SINGLE", 1);
            RV_PageWrite("MULTI", 2);
            break;

        case 20: // Display Menu: M20.
            if (Skip)
                ClearOLED();
            PageWrite("           ", 0);
            PageWriteCol("MENU", 0, 0x06, 0x13);
            PageWrite("FIRE Mode:", 1);
            PageWriteCol(FireModePt[SM_FLG], 1, 0x0A, 0x14);
            RV_PageWrite("GALV:", 2);
            PageWriteCol(GalvStatePt[GALV_FLG], 2, 0x0A, 0x14);
            PageWrite("EXIT Menu", 3);
            Skip = 0; // Reset Skip to 0.
            break;

        case 21: // Display Menu: M21.
            if (Skip)
                ClearOLED();
            PageWriteCol("Galv:", 0, 0x0D, 0x11);
            RV_PageWrite("OFF", 1);
            PageWrite("ON", 2);
            Check_Mark();
            X_Mark();
            break;

        case 22: // Display Menu: M22.
            if (Skip)
                ClearOLED();
            PageWriteCol("Galv:", 0, 0x0D, 0x11);
            PageWrite("OFF", 1);
            RV_PageWrite("ON", 2);
            Check_Mark();
            X_Mark();
            break;

        case 30: // Display Menu: M30.
            if (Skip)
                ClearOLED();
            PageWrite("GALV:", 2);
            RV_PageWrite("EXIT Menu", 3);
            break;

        case 40: // Display Running Display.
            if (Skip)
                ClearOLED();

            // Decide which mode to display based on SM_FLG.

            if (SM_FLG) { // SINGLE mode condition
                PageWriteCol("  ", 1, 0x0A, 0x013);

            } else { // MULTI mode condition
                PageWriteCol("Delay:", 1, 0x00, 0x010);
                PageWriteCol("ms", 1, 0x0A, 0x013);
                PageWriteCol(Array_DelayMs[Array_Count], 1, 0x04, 0x12);
            }
            // Always display these gems
            PageWriteCol("Mode: ", 0, 0x00, 0x10); // Top left hand corner
            PageWriteCol(FireModePt[SM_FLG], 0, 0x0E, 0x12);
            PageWriteCol("Batt:", 2, 0x0A, 0x15);
            PageWriteCol("V", 3, 0x07, 0x017);
            break;

        case 70: // case of trying to GALV with the ARM switch on.
            ClearOLED();
            PageWrite("      * Conflict *", 0);
            PageWrite("     Turn off <ARM> ", 2);
            PageWrite("     switch to GALV", 3);
            break;
    }
}

void Multi(void) {

    // New Multi
    PR1 = *DelayHexPt; // load period register with interrupt match value
    SM_FLG = 0; // Set SM flag to multi mode
    SM_LED = 1; // Turn SM LED on.
    _TON = 1; // Turn Timer 1 on.
    CLOCK_FLG = 1; // Set CLOCK flag.
}

void Reset(void) {
    SendReg = 0x8000; // Register which is outputted to the Shift Reg.
    Count = KLUNK_CH; // Register which holds one more than the 
    // number of hits channels for the klunker.
    SERIAL = 0;
    FIRE_FIRST_FLG = 0; // Clear FIRE_FIRST flag.
    FIRE_HIGH_FLG = 0; // Clear FIRE_HIGH flag.
    FIRE_FLG = 0;
    Clear_SR();
    FIRE_LED = 0; // Clear FIRE LED.
    Nop(); //  Because of port timing.
    RESET_LED = 1; // Set RESET LED.
    RESET_FLG = 0;

    IEC1bits.CNIE = 1; // enable CN ISR
}

void Rotate(void) {

    if (UNLATCH_FLG == 1) {
        SRCLK = 0;
        UNLATCH_FLG = 0;
    }
    FIRE_LED = 1; // turn FIRE LED on
    Nop();
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
    Count--;
    if (Count <= 0) // check if full sequence achieved, if yes, reset
        Reset();

    UNLATCH_FLG++;
}

void Shift(void) {

    if (SM_FLG == 0) // If this is true (SM_FLG = 0) then in multi mode
    {
        _TON = 0; // Resync the Timer and CLOCK flag to start fresh:
        TMR1 = 0x00; // Clears contents of Timer1 register
        _TON = 1; // Turn timer on.    
        CLOCK_FLG = 1;
        _LATD0 = 1;
        SM_LED = 1;
    }

    Rotate(); // in single mode

}

// Function for SINGLE mode.
void Single(void) {

    SM_FLG = 1; // Put into SINGLE mode.
    _TON = 0; // Timer1 off.
    SM_LED = 0; // Turn SM LED off.

    TMR1 = 0; // Clear Timer 1 register.
    CLOCK_FLG = 0; // Clear CLOCK flag.
    _LATD0 = 0; // turn of testing LED.
    //  PageWriteCol("SINGLE", 0, 0x0E, 0x11);
    //  PageWriteCol("    ", 1, 0x04, 0x12);
    //  PageWriteCol("      ", 1, 0x00, 0x010); // blank out "Delay" from Multi mode.
    //  PageWriteCol("   ", 1, 0x0D, 0x014);
}

void RunningDisplay() {
    ClearOLED();
    PageWriteCol("Mode: ", 0, 0x00, 0x10); // Top left hand corner
    PageWriteCol("Delay:", 1, 0x00, 0x010);
    PageWriteCol("ms", 1, 0x0A, 0x013);
    //PageWriteCol("Internal Batt:", 2, 0x00, 0x010);
    PageWriteCol("         Batt:", 3, 0x00, 0x010);
    //  PageWriteCol("0000", 2, 0x0A, 0x015);
    //  PageWriteCol("0000", 3, 0x0A, 0x015);
    PageWriteCol("       ", 2, 0x05, 0x15); // Write results in HEX.
    PageWriteCol("V", 3, 0x07, 0x017);
}

char *DecimalInsert(char *buf) {

    return buf;
}


// This screen only appears during the menu screen if the RESET button is
// pressed. Once the menu screen is passed, the Version Screen is no more.
// See RESET ISR INT0, for the code for this.

// The screen contains all the version numbers for the software and circuit
// boards.


void Check_Mark (void) {
     int a;
     
     PageWriteCol("GREEN:", 1, 0x0B, 0x14);
     // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | 0);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x01); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x17); // Set high column to 0x00.
    
    for (a = 0; a <= 9; a++) // Loop because characters are 5 wide
        WriteData(Check_Mk[a]);
    
     // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | 1);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x01); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x17); // Set high column to 0x00.
    
    for (a = 10; a <= 19; a++) // Loop because characters are 5 wide
    WriteData(Check_Mk[a]);
    
    WriteCommand(SSD1305_DISPLAYON);
}

void X_Mark (void) {
     int a;
     
     PageWriteCol("RED:", 3, 0x07, 0x15);
     // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | 2);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x01); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x17); // Set high column to 0x00.
    
    for (a = 0; a <= 9; a++) // Loop because characters are 5 wide
        WriteData(X_Mk[a]);
    
     // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | 3);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x01); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x17); // Set high column to 0x00.
    
    for (a = 10; a <= 19; a++) // Loop because characters are 5 wide
    WriteData(X_Mk[a]);
    
    WriteCommand(SSD1305_DISPLAYON);
}

void Version(void) {
    VER_FLG = 0;
    ClearOLED();
    PageWrite("       Dutch Dozen", 0);
    PageWrite("", 1);
    PageWrite("     Designed By:", 2);
    PageWrite("    Control Freaks ", 3);
    CharWrite(1);
    DelayMs(2000);
    ClearOLED();
    PageWrite("Dutch_Dozen_V1.0.pjc", 0);
    PageWrite("dutch_dozen_cpu_V5.0", 1);
    PageWrite("dutch_dozen_clipped", 2);
    PageWrite("_relay_V5.0.brd", 3);
    DelayMs(10000);
    ClearOLED();
    MenuNo = 10;
    Menu();
}
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

