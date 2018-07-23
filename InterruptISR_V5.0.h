
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
#define INTERRUPTISR_50_Shot_H

#include <xc.h> // include processor files - each processor file is guarded.
#include <stdlib.h>
#include <string.h>
#include "ADCInit_V5.0.h"
#include "EEPROM_AT93C96D.h"
#include "INTInit_V5.0.h"
#include "Misc_Macro_V5.0.h"
#include "TFT_ILI9341.h"


// *** Local define statements here ***
#define ARM_MASK 0x0008
#define FIRE_MASK 0x0010
#define PORTB_MASK  0x001C; // This masks off the 3 PortB CN bits, RB2, RB3, RB4.
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
#define ARRAY_MID 24    // Should be 24.          

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
void Encoder_Switch(void);
void Clear_SR(void);
void Current_Condition(void); // Store current conditions in EEPROM.
void Firing(void);
void Memory_Recovery(void); // When Powered up, restores from EEPROM.
void Menu(void);
void Multi(void);
void Parse_PortB(void);
void Reset(void);
void Rotate(void);
void Rotory_Encoder(void);
void Shift(void);
void Single(void);
void Version(void);

//*** Extern statements go here ***

extern int Array_Count;
extern int place;
extern int MenuNo;
extern int Count;


//int Array_Count = ARRAY_MID; // Count to keep track of the array index.;
// Register which holds one more than the # of hits
int PortB_New = 0;
int PortB_Last = 0; // Previous PortB stored here. 
int PortB_Temp = 0; // This is the latest snapshot of PortB
int Result = 0;
int SendReg = 0; // Register which is outputted to the Shift Reg.
int Multi_Delay = 0x30d; // Originally set to 500mS. Changes from F42
int Multi_Delay_Step = 0x009c; // Multi delay increment;
int Multi_Delay_Max = 0x061A;
int Multi_Delay_Min = 0x009C;
int DoUp = 0; // Used for skipping Line Writes when going back up the menu chain.
int DoDown = 1; // Used for skipping LineWrites, going down menu chain
int DoBack = 0; // Used for skipping LineWrites, going back from sub menu.
int DoForward = 0; // Used for skipping LineWrites, going into sub menu.
int column = 222;


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
char *MemStatePt [] = {"-OFF-", "-ON-"};

// Pointer for the middle of the mS array.

// ***********************
// *** ISR Definitions ***
// ***********************

// *** A/D Conversion Done Interrupt ***

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(void) {

    // ADC calculations are done inside the Disp_Batt_Voltage() function
    // in the Main() of the source file.

    IFS0bits.AD1IF = 0; // reset A/D Conversion Done interrupt flag
}

// *** Input Change Notification Interrupt ***
// Captures FIRE, ARM and S/M interrupts.

void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
    PortB_Temp = PORTB & PORTB_MASK; // Read a copy of the PORTB;
    Parse_PortB(); // Parse the Port B data and act on it
}


// *** External INT0 Interrupt - RESET INTERRUPT ***

void __attribute__((interrupt, auto_psv)) _INT0Interrupt(void) {

    // Tests if the <Chg Comp> message is displayed, if so, clears it. 
    // When the <Chg Comp> is displayed it only clears when the <RESET> button
    // is pressed.
    if (CHG_COM_FLG == 1) {
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


// Encoder switch

void __attribute__((interrupt, auto_psv)) _INT3Interrupt(void) {
    IEC3bits.INT4IE = 0; // disable INT4 ISR, enabled at the end of this ISR.
    Encoder_Switch();
    IEC3bits.INT4IE = 1; // enable INT4 ISR.
    IFS3bits.INT3IF = 0; // Reset INT3 interrupt flag

}


// *** External INT4 Interrupt - ROTORY ENCODER A ***

void __attribute__((interrupt, auto_psv)) _INT4Interrupt(void) {
    IEC3bits.INT3IE = 0; // disable INT3 ISR, enables at end of this ISR.
    Rotory_Encoder();
    IEC3bits.INT3IE = 1; // enable INT3 ISR.
    IFS3bits.INT4IF = 0; // reset INT4 interrupt flag
}


// *** Timer1 TI1 Interrupt ***
// This timer controls the timing of the MULTI LED and the MULTI function.

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {

    TMR1 = 0x00; // clears contents of time register
    SM_LED = ~SM_LED; // toggle the SM LED.
    CLOCK_FLG = ~CLOCK_FLG; // Toggle CLOCK flag.

    if ((FIRE_FLG == 1) && (CLOCK_FLG == 1)) {
        if (Array_Count <= 0)
            Reset();
        Rotate();
    } else {
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
    // _CNARM = 0;
    if (ARM_SW == 1) { // If the ARM switch is activated:
        ARM_FLG = 1; // SET the ARM flag.
        ARM_LED = 1; // SET the ARM LED.
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_RED, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_BLACK, ILI9341_RED);
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_RED, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_BLACK, ILI9341_RED);
    } else { // The ARM switch is deactivated:
        if (ARM_GLV_FLG == 1) {
            ARM_GLV_FLG = 0;
            LineWrite_XY_ILI9341_16x25("  *** CONFLICT ***", 18, Line4, ILI9341_BLACK, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("Turn off <ARM> to Galv.", 0, Line5, ILI9341_BLACK, ILI9341_BLACK);
            GALV_OUT = 1;
        }// conflict now resolved
        ARM_FLG = 0; // CLEAR the ARM flag.
        ARM_LED = 0; // CLEAR the ARM flag.
        LineWrite_XY_ILI9341_16x25("<ARMED>", 10, Line7, ILI9341_BLACK, ILI9341_BLACK);
        // Rewrites the "batt" and "V" 
        LineWrite_XY_ILI9341_16x25("Batt:", 148, Line6, ILI9341_CYAN, ILI9341_BLACK);
        LineWrite_XY_ILI9341_16x25("V", 302, Line6, ILI9341_CYAN, ILI9341_BLACK);
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

void Current_Condition(void) {
    E_Write(EMode, SM_FLG); // Save current sm mode value.
    E_Write(EChannel, Count); // save channel position.
}

void Encoder_Switch(void) {
    // This disables the encoder button while the encoder rotation is running.
    // Change from M10 to M11.
    if (MenuNo == 10) {
        MenuNo = 11;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoDown = 0;
        DoForward = 1;
        DoUp = 0;
        Menu();
    }// Change from M11 back to M10.
        // This puts klunker into SINGLE mode.
    else if (MenuNo == 11) {
        MenuNo = 10;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        Single();
        Menu();
    }// Change from M12 back to M10.
        // This puts klunker into MULTI mode.
    else if (MenuNo == 12) {
        MenuNo = 10;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoUp = 0;
        DoBack = 1;
        Multi();
        Menu();
    }// Change from M50 to M51

    else if (MenuNo == 50) {
        MenuNo = 51;
        DoDown = 0;
        DoForward = 1;
        DoUp = 0;
        Menu();
    }// Change from M51 back to M50
    else if (MenuNo == 51) {
        MenuNo = 50;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        MEMORY_MODE_FLG = OFF; // Set MEM flag.
        E_Write(EMemory_Mode, OFF); // Save Memory Condition.
        Menu();
    }// Change from M52 back to M50
    else if (MenuNo == 52) {
        MenuNo = 50;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
        MEMORY_MODE_FLG = ON; // Set MEM flag.
        E_Write(EMemory_Mode, ON); // Save Memory Condition.
        Current_Condition();
        Memory_Recovery();
        //Reset();
        //SM_FLG = 1;
        //Single();
        Memory_Recovery();
        Menu();
    }// Change from M21 back to M20.
        // This action also turns GALV function off.
    else if (MenuNo == 21) {
        MenuNo = 20;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoForward = 0;
        DoDown = 0;
        DoUp = 0;
        DoBack = 1;
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
            //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
            Menu();
        }
        Reset(); // reset before entering GALV mode.
        MenuNo = 22; // ** For full function uncomment Menu() and Change
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        // MenuNo = 21 instead of MenuNo = 22.
        LineWrite_XY_ILI9341_16x25("ON ", 0, Line3, ILI9341_BLACK, ILI9341_BLUE);
        Box_ILI9341(0, Line3, 55, 30, ILI9341_WHITE); // BOX <ON>.
        GALV_FLG = 1; // Set GALV flag.
        GALV_OUT = 1; // Turn GALV function and LED on.
    }// Change from M20 to M21.
    else if (MenuNo == 20) {
        MenuNo = 21;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        DoDown = 0;
        DoUp = 0;
        DoForward = 1;
        Menu();
    }// Change from M30 to Running Display
    else if (MenuNo == 30) {
        MenuNo = 40;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        VER_FLG = 0;
        MENU_FLG = 0; // leaving MENU mode (OK to fire).
        DoForward = 1; // Do not skip ClearOLED().
        IEC0bits.AD1IE = 1; // enable A/D Conversion Done interrupt
        Menu();
    }// Change back to main menu from RUNNING menu.
    else if (MenuNo == 40) {
        MENU_FLG = 1; // entering MENU mode (Not OK to fire).
        MenuNo = 10; // Enter menu in M10 screen.
        DoBack = 1;
        //E_Write(EMenu, MenuNo); // Write MenuNo to memory.
        // Do not skip ClearOLED().
        //IEC0bits.AD1IE = 0; // disable A/D Conversion Done interrupt
        Menu();
    }
}

void Firing(void) {
    // _CNFIRE = 0; // Clear FIRE EVENT indicator.
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

void Memory_Recovery(void) {// When Powered up, restores from EEPROM.

    int Channel_count = 0; // Local channel count var. Set to RESET condition.

    // ***************************************
    // *** Mode Delay **** MEMORY RECOVERY ***
    // ***************************************

    Array_Count = E_Read(EDelay_Count);
    place = E_Read(EDelay_Place);
    THREE_DIGIT_FLG = E_Read(EDelay_3D_FLG);

    // ************************************
    // *** Channel **** MEMORY RECOVERY ***
    // ************************************
    // SRCLK controls the output buffers of '245.
    // SR_LATCH shifts the shift register.


    // Initialize Shift Register.
    //SERIAL = 0;
    // SRCLK = 0;
    //SR_LATCH = 0;
    SR_CLEAR = 0; // Clear shift register.
    SR_CLEAR = 1; // active low so a 1 is disabled

    // Disable the output of the shift register (active low).
    // Initially port F3 set to 1 in 'PortInit' header.
    SHIFT_REG_EN = 1;

    Channel_count = E_Read(EChannel); // Retrieve stored channel value
    Count = (Channel_count);
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
    SM_FLG = E_Read(EMode);
    if (!SM_FLG) { // Call Multi() to turn on SM_LED. (if Multi mode)
        Multi();
    }
}

// This function changes the menu displayed depending on the MenuNo variable.

void Menu(void) {
    switch (MenuNo) {
        case 0: // Display Menu: M0.
            /*  PageWrite("           ", 0);
                RV_PageWriteCol("MENU", 0, 0x06, 0x13);
                LineWrite_XY_ILI9341_16x25("MENU", 128, Line0, ILI9341_GREEN, ILI9341_BLACK);
                PageWrite("FIRE Mode:", 1);
                LineWrite_XY_ILI9341_16x25("FIRE Mode:", 0, Line1, ILI9341_GREEN, ILI9341_BLACK);
                PageWriteCol(FireModePt[SM_FLG], 1, 0x0A, 0x14);
                PageWrite("GALV:", 2);
                PageWriteCol(GalvStatePt[GALV_FLG], 2, 0x0A, 0x14);
                PageWrite("EXIT Menu", 3);
             */
            break;

        case 10: // Display Menu: M10, <FIRE Mode:> highlighted.
            if (DoBack) {// Print M10, From either M40 Running menu or <FIRE> submenu.
                FillRec_ILI9341(0, 0, 480, 176, ILI9341_BLACK);

                LineWrite_XY_ILI9341_16x25("MENU", 128, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("FIRE Mode:", 0, Line2, ILI9341_GREEN, ILI9341_BLACK);
                Box_ILI9341(0, (Line2), 150, 30, ILI9341_WHITE); // Box <FIRE MODE:>.
                LineWrite_XY_ILI9341_16x25(FireModePt[SM_FLG], 176, Line2, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("MEM Mode:", 0, Line3, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(MemStatePt[MEMORY_MODE_FLG], 200, Line3, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("GALV:", 0, Line4, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(GalvStatePt[GALV_FLG], 200, Line4, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("EXIT", 0, Line5, ILI9341_GREEN, ILI9341_BLACK);
            }


            if (DoUp) { // M10 from M50, BOX <FIRE>, UnBOX <MEM>.
                Box_ILI9341(0, (Line3), 150, 30, ILI9341_BLACK); // UnBox <MEM MODE:>.
                Box_ILI9341(0, (Line2), 150, 30, ILI9341_WHITE); // BOX <FIRE MODE:>.
            }
            break;

        case 11: // Display Menu: M11.
            if (DoForward) {// Menu M11 from M10 or M12 create menu for first time
                FillRec_ILI9341(0, 0, 480, 170, ILI9341_BLACK); // increased to cover Line5         
                LineWrite_XY_ILI9341_16x25("FIRE Mode:", 0, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("SINGLE", 0, Line2, ILI9341_WHITE, ILI9341_BLACK);
                Box_ILI9341(0, Line2, 100, 30, ILI9341_WHITE); // BOX <SINGLE>.
                LineWrite_XY_ILI9341_16x25("MULTI", 0, Line3, ILI9341_WHITE, ILI9341_BLACK);
            }

            if (DoUp) {// Menu M12 to M11, unBOX <MULTI>, BOX <SINGLE>.
                Box_ILI9341(0, Line3, 100, 30, ILI9341_BLACK); // UnBOX <MULTI>.
                Box_ILI9341(0, Line2, 100, 30, ILI9341_WHITE); // BOX <SINGLE>.
            }
            break;

        case 12: // Display Menu: M12, UnBox <MULTI>, BOX <SINGLE>.
            Box_ILI9341(0, Line2, 100, 30, ILI9341_BLACK); // UnBOX <SINGLE>.
            Box_ILI9341(0, Line3, 100, 30, ILI9341_WHITE); // BOX <MULTI>.
            break;

        case 20: // Display Menu: M20, <GALV> highlighted.
            if (DoBack) {// Print M20, <Menu> from <GALV> submenu screen.
                FillRec_ILI9341(0, 0, 480, 176, ILI9341_BLACK);

                LineWrite_XY_ILI9341_16x25("MENU", 128, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("FIRE Mode:", 0, Line2, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(FireModePt[SM_FLG], 176, Line2, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("MEM Mode:", 0, Line3, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(MemStatePt[MEMORY_MODE_FLG], 200, Line3, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("GALV:", 0, Line4, ILI9341_GREEN, ILI9341_BLACK);
                Box_ILI9341(0, (Line4), 90, 30, ILI9341_WHITE); // Box <GALV MODE:>.
                LineWrite_XY_ILI9341_16x25(GalvStatePt[GALV_FLG], 200, Line4, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("EXIT", 0, Line5, ILI9341_GREEN, ILI9341_BLACK);
            }

            if (DoDown) {// Print M20 from M50, unBOX <MEM>, BOX <GALV> 
                Box_ILI9341(0, (Line3), 150, 30, ILI9341_BLACK); // UnBOX <MEM>.
                Box_ILI9341(0, (Line4), 90, 30, ILI9341_WHITE); // BOX <GALV>.
            }

            if (DoUp) {// Print M20 from M50, UnBOX <EXIT>, BOX <GALV>
                Box_ILI9341(0, (Line5), 90, 30, ILI9341_BLACK); // UnBox <EXIT>.
                Box_ILI9341(0, (Line4), 90, 30, ILI9341_WHITE); // Box <GALV>.
            }
            break;

        case 21: // Display Menu: M21, <GALV> submenu
            if (DoForward) {// Menu M21 from M20 or M22, create menu for first time
                FillRec_ILI9341(0, 0, 480, 170, ILI9341_BLACK); // increased to cover Line5         
                LineWrite_XY_ILI9341_16x25("GALV Mode:", 0, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("OFF", 0, Line2, ILI9341_WHITE, ILI9341_BLACK);
                Box_ILI9341(0, Line2, 55, 30, ILI9341_WHITE); // BOX <OFF>.
                LineWrite_XY_ILI9341_16x25("ON", 0, Line3, ILI9341_WHITE, ILI9341_BLACK);
            }

            if (DoUp) {// Menu M21 from M22, unBOX <ON>, BOX <OFF>.
                Box_ILI9341(0, Line3, 55, 30, ILI9341_BLACK); // UnBOX <ON>.
                Box_ILI9341(0, Line2, 55, 30, ILI9341_WHITE); // BOX <OFF>.
            }
            break;

        case 22: // Display Menu: M22, UnBox <OFF>, BOX <ON>.
            Box_ILI9341(0, Line2, 55, 30, ILI9341_BLACK); // UnBOX <OFF>.
            Box_ILI9341(0, Line3, 55, 30, ILI9341_WHITE); // BOX <ON>.
            break;

        case 30: // Display Menu M30.
            Box_ILI9341(0, (Line4), 90, 30, ILI9341_BLACK); // UnBOX <GALV>.
            Box_ILI9341(0, (Line5), 90, 30, ILI9341_WHITE); // BOX <EXIT>.
            break;

        case 40: // Display Running Display.
            if (DoForward)
                FillRec_ILI9341(0, 0, 480, 177, ILI9341_BLACK);
            // Decide which mode to display based on SM_FLG.

            if (SM_FLG) { // SINGLE mode condition
            } else { // MULTI mode condition
                LineWrite_XY_ILI9341_16x25("Delay:", 0, Line2, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("ms", 140, Line2, ILI9341_WHITE, ILI9341_BLACK);
                // Check if we need to right justify when coming back from menu.
                if (Array_DelayHex[Array_Count] <= 0x57E) {// number <= 90 *** changed Feb15/18
                    place = 101;
                } else {
                    place = 85;
                }
                LineWrite_XY_ILI9341_16x25(Array_DelayMs[Array_Count], place, Line2, ILI9341_WHITE, ILI9341_BLACK);
            }
            // Always display these gems

            // Outline at bottom screen
            FillRec_ILI9341(0, 177, 320, 1, ILI9341_GREEN); // Horizontal lines
            FillRec_ILI9341(0, 207, 320, 1, ILI9341_GREEN);
            FillRec_ILI9341(0, 237, 320, 1, ILI9341_GREEN);

            FillRec_ILI9341(0, 177, 1, 60, ILI9341_GREEN); // Vertical lines
            FillRec_ILI9341(319, 177, 1, 60, ILI9341_GREEN);
            FillRec_ILI9341(135, 177, 1, 60, ILI9341_GREEN);

            LineWrite_XY_ILI9341_16x25("Mode:", 0, Line1, ILI9341_GREEN, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25(FireModePt[SM_FLG], 85, Line1, ILI9341_WHITE, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("Batt:", 148, Line6, ILI9341_CYAN, ILI9341_BLACK);
            LineWrite_XY_ILI9341_16x25("V", 302, Line6, ILI9341_CYAN, ILI9341_BLACK);

            break;
        case 50: // Displaying menu M50, <MEM Mode:> highlighted.
            if (DoBack) {// Print M10, <Menu> from <MEM> submenu screen.
                FillRec_ILI9341(0, 0, 480, 176, ILI9341_BLACK);

                LineWrite_XY_ILI9341_16x25("MENU", 128, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("FIRE Mode:", 0, Line2, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(FireModePt[SM_FLG], 176, Line2, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("MEM Mode:", 0, Line3, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(MemStatePt[MEMORY_MODE_FLG], 200, Line3, ILI9341_WHITE, ILI9341_BLACK);
                Box_ILI9341(0, (Line3), 150, 30, ILI9341_WHITE); // Box <MEM MODE:>.
                LineWrite_XY_ILI9341_16x25("GALV:", 0, Line4, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25(GalvStatePt[GALV_FLG], 200, Line4, ILI9341_WHITE, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("EXIT", 0, Line5, ILI9341_GREEN, ILI9341_BLACK);
            }

            if (DoDown) {// Print M50 from M10, unBOX <FIRE>, BOX <MEM> 
                Box_ILI9341(0, (Line2), 150, 30, ILI9341_BLACK); // UnBOX <FIRE>.
                Box_ILI9341(0, (Line3), 150, 30, ILI9341_WHITE); // BOX <MEM>.
            }

            if (DoUp) {// Print M50 from M20, UnBOX <GALV:>, BOX <MEM>
                Box_ILI9341(0, (Line4), 90, 30, ILI9341_BLACK); // UnBox <GALV>.
                Box_ILI9341(0, (Line3), 150, 30, ILI9341_WHITE); // Box <MEM>.
            }
            break;

        case 51:// Display Menu: M51.
            if (DoForward) {// Menu M51 from M50 or M52, create menu for first time
                FillRec_ILI9341(0, 0, 480, 170, ILI9341_BLACK); // increased to cover Line5         
                LineWrite_XY_ILI9341_16x25("MEM Mode:", 0, Line0, ILI9341_GREEN, ILI9341_BLACK);
                LineWrite_XY_ILI9341_16x25("OFF", 0, Line2, ILI9341_WHITE, ILI9341_BLACK);
                Box_ILI9341(0, Line2, 55, 30, ILI9341_WHITE); // BOX <OFF>.
                LineWrite_XY_ILI9341_16x25("ON", 0, Line3, ILI9341_WHITE, ILI9341_BLACK);
            }

            if (DoUp) {// Menu M51 from M52, unBOX <ON>, BOX <OFF>.
                Box_ILI9341(0, Line3, 55, 30, ILI9341_BLACK); // UnBOX <ON>.
                Box_ILI9341(0, Line2, 55, 30, ILI9341_WHITE); // BOX <OFF>.
            }
            break;

        case 52: // Display Menu: M52, UnBox <OFF>, BOX <ON>.
            Box_ILI9341(0, Line2, 55, 30, ILI9341_BLACK); // UnBOX <OFF>.
            Box_ILI9341(0, Line3, 55, 30, ILI9341_WHITE); // BOX <ON>.
            break;


        case 70: // case of trying to GALV with the ARM switch on.
            LineWrite_XY_ILI9341_16x25("  *** CONFLICT ***", 18, Line4, ILI9341_BLACK, ILI9341_RED);
            LineWrite_XY_ILI9341_16x25("Turn off <ARM> to Galv.", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
            break;
    }
}

void Multi(void) {

    // New Multi
    PR1 = Array_DelayHex[Array_Count]; // load period register with interrupt match value
    SM_FLG = 0; // Set SM flag to multi mode
    E_Write(EMode, SM_FLG); // Save current sm mode value.
    SM_LED = 1; // Turn SM LED on.
    _TON = 1; // Turn Timer 1 on. Memory recovery also test and turns on
    CLOCK_FLG = 1; // Set CLOCK flag.
}

void Parse_PortB(void) {

    IFS1bits.CNIF = 0;
    int arm_bit = 0; // Gets the RB3 value;
    int fire_bit = 0; // Gets the RB4 value;

    // This section sets up touch_bit, arm_bit and fire_bit according
    // to their port values. Gives current state of that bit.
    //touch_bit = (PortB_Temp & TOUCH_MASK); // Mask off _RB2.
    arm_bit = (PortB_Temp & ARM_MASK); // Mask off _RB3.
    fire_bit = (PortB_Temp & FIRE_MASK); // Mask off _RB4.

    // This section loads masks off the 3 desired bits of the newest PortB
    // read and XOR's that with the previous PortB read setting the latest
    // bit that changed. Note Port_Last already has the 3 desired bits masked 
    // off.
    PortB_New = PortB_Temp & PORTB_MASK; // Mask off the 3 new desired  bits.
    PortB_New = PortB_New ^ PortB_Last; // Set the bit that has changed.

    // Test if FIRE bit, (FIRE button pressed or depressed).
    if (PortB_New & FIRE_MASK) { // FIRE button was the one to change
        if (MENU_FLG == 0) // Only safe to fire when not in menu mode.
            Firing();
    }

    // Test if ARM bit, (ARM switch toggled).
    if (PortB_New & ARM_MASK) { // FIRE button was the one to change
        Arm();
    }
    PortB_Last = PortB_Temp; // get initial PORTB value 
    PortB_Last = PortB_Last & PORTB_MASK; // Mask off the 3 pertinent values.
}

void Reset(void) {

    SendReg = 0x8000; // Register which is outputted to the Shift Reg.
    Count = KLUNK_CH; // Register which holds one more than the 
    E_Write(EChannel, Count); // save channel position.
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
    CHANNEL_FLG = 0; // Reset condition if powered down.
    IEC1bits.CNIE = 1; // enable CN ISR
}

void Rotate(void) {
    SRCLK = 0;
    FIRE_LED = 1; // turn FIRE LED on
    Nop();

    SERIAL = 0;
    if ((FIRE_FIRST_FLG == 1) && (CHANNEL_FLG == 0))
        SERIAL = 1;

    SRCLK = 1; // Shift pulse to nest channel/ activate '245.
    Nop();

    SR_LATCH = 1; // Output to SR.
    Nop(); // Timing delay for SR.
    SR_LATCH = 0; // Kill latch pulse.
    Nop();
    FIRE_FIRST_FLG = 0; // Clear FIRE_FIRST flag.
    Count--;
    E_Write(EChannel, Count); // save channel position.
    if (Count <= 0) // check if full sequence achieved, if yes, reset
        Reset();

    //CHANNEL_FLG = 0;
}

void Rotory_Encoder(void) {
    int Encoder_Dir = 0;
    long time = 15; // Delay time for encode ton settle.

    Encoder_Dir = _RF0; //'encoder pin B' determines CW or CCW, 0 = CCW, 1 = CW.
    DelayMs(time); // Delay for encoder B to settle

    // This disables the rotation while the button routine is running.

    // *** This series of conditional statements assume Running Display Screen
    // in MULTI mode.
    if ((MenuNo == 40) && (SM_FLG == 0)) {// Done with menu, goto running display.
        if (Encoder_Dir == CW) { // *** INCREASING DELAY (CCW)***
            Array_Count++; // Increase the count of both arrays.
            E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.

            if (Array_Count > ARRAY_MAX) { // Check if hit upper limit of array.
                Array_Count = ARRAY_MAX; // Oops, went above, hold at upper limit.
                E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            }
            // if count > 90 go into 3 digit mode, no right justify, 
            // three digit flag = 3.
            if (Array_DelayHex[Array_Count] > 0x57E) { // f count > 90. *** CHANGED FEB15/18 ***
                THREE_DIGIT_FLG = 1;
                E_Write(EDelay_3D_FLG, THREE_DIGIT_FLG); // Save place to EEPROM.
                place = 85;
                E_Write(EDelay_Place, place); // Save place to EEPROM.
            }
            LineWrite_XY_ILI9341_16x25(Array_DelayMs[Array_Count], place, Line2, ILI9341_WHITE, ILI9341_BLACK);

        } else if (Encoder_Dir == CCW) { // *** DECREASING DELAY (CW)***
            // Checking if count < 100 AND coming from 3 digits.
            if ((Array_DelayHex[Array_Count] <= 0x61A) && (THREE_DIGIT_FLG == 1)) {//****
                //  Effectively clears the digit no longer being printed. When 
                // working place one character at the proper location.
                place = 101; // Right justification
                E_Write(EDelay_Place, place); // Save place to EEPROM.
                // Places a blank at the MSB.
                LineWrite_XY_ILI9341_16x25(" ", 85, Line2, ILI9341_BLACK, ILI9341_BLACK);
                THREE_DIGIT_FLG = 0; // signals 2 digits
                E_Write(EDelay_3D_FLG, THREE_DIGIT_FLG); // Save THREE_DIGIT_FLG to EEPROM.
            }

            Array_Count--;
            E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            if (TMR1 >= Array_DelayHex[Array_Count]) // Check if TMR is above PRx, if
                TMR1 = Array_DelayHex[Array_Count - 2]; // so, stick TMR below PRx.; // so, stick TMR below PRx.

            if (Array_Count < 1) { // Check for lower array limit.
                Array_Count = 1; // Oops, went below, hold at lower limit.
                E_Write(EDelay_Count, Array_Count); // Save Array_Count to EEPROM.
            }
            // Only clear the delay once when going from 100 - 90. Stops blanking.
            // When THREE_DIGIT_FLG = 1, count is 3 digits.

            LineWrite_XY_ILI9341_16x25(Array_DelayMs[Array_Count], place, Line2, ILI9341_WHITE, ILI9341_BLACK);
        }
        PR1 = Array_DelayHex[Array_Count]; // Load delay value into Timer Period Register.
    }


    // NOTE: encoder_B = 0: CW, encoder_B = 1: CCW
    // *** This series of conditional statements assume Menu Screen ***
    // Start with CCW then move to CW conditions.

    if (Encoder_Dir == CCW) {
        if (MenuNo == 11) {
            MenuNo = 12;
            Menu();
        }// Menu 10 to 20 (CCW)

        else if (MenuNo == 10) {
            MenuNo = 50; // Changed from MenuNo = 20.
            DoDown = 1;
            DoUp = 0;
            DoBack = 0;
            Menu();
            // Skip Next ClearOLED().

        }// Menu 51 to 52 (CCW)
        else if (MenuNo == 50) {
            MenuNo = 20; // Changed from MenuNo = 20.
            DoDown = 1;
            DoUp = 0;
            DoBack = 0;
            Menu();
            // Skip Next ClearOLED().
        }// Menu 51 to 22 (CCW)
        else if (MenuNo == 51) {
            MenuNo = 52;
            Menu();
        }// Menu 21 to 22 (CCW)
        else if (MenuNo == 21) {
            MenuNo = 22;
            Menu();
        }// Menu 20 to 30 (CCW)


        else if (MenuNo == 20) {
            MenuNo = 30;
            // Skip Next ClearOLED().
            Menu();
        }

        // Now determine actions for CW actions
    } else if (Encoder_Dir == CW) {
        // Menu 30 to 20 (CW)
        if (MenuNo == 30) {
            MenuNo = 20;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0; // Skip Next ClearOLED().
            Menu();
        }// Menu 20 to 50 (CW)
        else if (MenuNo == 20) {
            MenuNo = 50;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0;
            Menu();

        }// Menu 50 to 10 (CW)
        else if (MenuNo == 50) {
            MenuNo = 10;
            DoUp = 1;
            DoDown = 0;
            DoBack = 0;
            Menu();

        } else if (MenuNo == 12) {
            MenuNo = 11;
            DoForward = 0;
            DoDown = 0;
            DoUp = 1;
            Menu();
        }// Menu 52 to 51 (CW)
        else if (MenuNo == 52) {
            MenuNo = 51;
            DoForward = 0;
            DoDown = 0;
            DoUp = 1;
            Menu();
        }// Menu 22 to 21 (CCW)
        else if (MenuNo == 22) {
            // The disables the rotary encode during ARM/GLV conflict.
            if (ARM_GLV_FLG == 0) {
                MenuNo = 21;
                DoForward = 0;
                DoDown = 0;
                DoUp = 1;
                Menu();
            }
        }
    }
}

void Shift(void) {

    if (SM_FLG == 0) // If this is true (SM_FLG = 0) then in multi mode
    {
        _TON = 0; // Resync the Timer and CLOCK flag to start fresh:
        TMR1 = 0x00; // Clears contents of Timer1 register
        //E_Write(EDelay_TMR1, TMR1);
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
    E_Write(EMode, SM_FLG); // Save current sm mode value.
    _TON = 0; // Timer1 off.
    SM_LED = 0; // Turn SM LED off.
    TMR1 = 0; // Clear Timer 1 register.
    CLOCK_FLG = 0; // Clear CLOCK flag.
    _LATD0 = 0; // turn of testing LED.
}

char *DecimalInsert(char *buf) {

    return buf;
}

// Fill this out for each klunker programmed.

void Version(void) {

    FillScreen_ILI9341(ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Serial: DD0301017-008", 0, Line0, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Software Version: ", 0, Line1, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Dutch-Dozen-V5.0 ", 0, Line2, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("Hardware Version: ", 0, Line3, ILI9341_GREENYELLOW, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("dutch-dozen-cpu-V7.0", 0, Line4, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("dutch-dozen-clipped-", 0, Line5, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("relay-V5.0", 0, Line6, ILI9341_WHITE, ILI9341_BLACK);
    LineWrite_XY_ILI9341_16x25("dutch-dozen-output-V2.0", 0, Line7, ILI9341_WHITE, ILI9341_BLACK);

    while ((_RF6 == 1) && (_RB4)) {
    } // Wait in Version screen until <RESET> is pressed. 
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

