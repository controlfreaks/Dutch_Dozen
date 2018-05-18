
/* 
 * File:        EEPROM_AT93C96D.h
 * Author:      James Vlasblom  
 * Date:        January 31, 2018
 * Comments:    
 *  This file contains the setup, void EEPROM_Setup(), and driver files for the
 * Atmel AT93C96D, serial EEPROM, 128x8 or 64x16 bit.
 * 
 * 
 * Revision history: 
 * 
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef EEPROM_AT93C46D_H
#define	EEPROM_AT93C46D_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "MyFunctions_V5.0.h"       // Has the delay functions.


// Write to LATCHes, Read from PORTS


// EEPROM Inputs THERFORE MCU Outputs
#define EEPROM_CS _LATE4
#define EEPROM_SK _LATE3
#define EEPROM_DI _LATG3
#define EEPROM_ORG _LATG2

//EEPROM Outputs THERFORE MCU Inputs
#define EEPROM_DO _RF7

// *** Function Prototypes here ***
void EEPROM_Setup();
int E_Read(int address);
void E_EWEN();
void E_Erase();
void E_Write(int write_add, int write_data);
void E_Eral();
//void E_Wral(int data);
void E_Ewds();

//EEPROM definitions / memory locations
int EMode = 0x00; // EEPROM memory location for mode setting.
int EMenu = 0x01; // EEPROM memory location for menu setting.
int EDelay_Count = 0x02; // EEPROM memory location for "ms" array.
int EInit_FLG = 0x03; // EEPROM initialization flag. 0x0000 = 1st time through,
//                          0x1234 = ignore initialization.
int EDelay_3D_FLG = 0x04;  // EEPROM memory location of # of digits.
int EDelay_Place = 0x05;    // EEPROM memory location place justification.
int EChannel = 0x08; // EEPROM memory location for Channel setting.
// Function Definitions here.

void EEPROM_Setup() {

    //*** Note: DO & DI are from the MCU's perspective. DO is the output to 
    // the EEPROM's input, DI is input from the EEPROM's output.

    // Set everything initially to zero except EEPROM_ORG = 1.
    // Pin directions WRT EEPROM.
    EEPROM_CS = 0; // (MCU pin RE4) 
    EEPROM_SK = 0; // (MCU pin RE3)
    EEPROM_DO = 0; // (MCU pin RF7)
    EEPROM_DI = 0; // (MCU pin RG3)
    EEPROM_ORG = 1; // 0 = 128 x 8,  1 = 64 x 16 of the EEPROM

    E_EWEN(); // Enable programming 
}

// **NOTE: The following Read and write routines automatically
// accommodates for EEPROM_ORG.

int E_Read(int read_add) {
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    IEC0bits.T1IE = 0; // enable Time1 interrupt
    IEC1bits.T4IE = 0; // enable Time1 interrupt
    Nop(); // Added for interrupt disable timing.

    int add_mask = 0x0020; // Mask for masking address bit.
    int count = 6; // Count of the 6 address bits.
    int data_in = 0; // Read variable.

    EEPROM_CS = 0; // Start with CS=0, low to high transition.
    EEPROM_CS = 1; // Start sequence, clock goes high.
    Nop(); // Wait 250 nS before clock goes high, tCSS>50nS

    EEPROM_DI = 1; // **** START BIT ****.
    Nop(); // Wait 250 nS before clock goes high, tDIS>100nS
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // tSKH 250 nS min but tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 1; // *** 1ST DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // Wait tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 0; // *** 2ND DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(), Nop(); // 250 nS delay.
    EEPROM_SK = 0; // Clock low.
    // Nop(); // 250 nS delay.

    // Put the address to be read from on the wire.
    while (count--) { // Count = 6 for 6 address bits.
        // Mask off address bit of interest then shift right 5 to put
        // bit A0 location.
        EEPROM_DI = ((read_add & add_mask) >> 5);
        Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
        EEPROM_SK = 1; // Clock high.
        read_add = read_add << 1; // Shift to the left one bit;
        EEPROM_SK = 0; // Clock low.
    }

    if (EEPROM_ORG == 0) { // Accommodate for 8 or 16 bit configuration.
        count = 8;
    } else {
        count = 15; // Variable for data read.
    }

    // An extra clock pulse for the *** DUMMY BIT **** (logic 0).
    EEPROM_SK = 1; // Clock high.
    Nop();
    EEPROM_SK = 0; // Clock high.
    Nop();

    // Read value at desired location into variable
    while (count) {
        EEPROM_SK = 1; // Clock high.
        data_in = data_in | EEPROM_DO; // Assume data_in = 0 to start.
        data_in = data_in << 1; // Shift to the left.     
        EEPROM_SK = 0; // Clock low.
        count--;
    }

    data_in = data_in | EEPROM_DO; // This attaches the last LSbit.;

    EEPROM_CS = 0; // End sequence.

    return data_in; // Return 16 bit value

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
    IEC0bits.T1IE = 1; // enable Time1 interrupt
    IEC1bits.T4IE = 1; // enable Time1 interrupt
}

void E_EWEN() {
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    IEC0bits.T1IE = 0; // enable Time1 interrupt
    IEC1bits.T4IE = 0; // enable Time1 interrupt
    Nop(); // Added for interrupt disable timing.


    int add_mask = 0x0020; // Mask for masking address bit.
    int count = 6; // Count of the 6 address bits.
    int read_add = 0xAB; // any arbitrary address.
    EEPROM_CS = 0; // Start with CS=0, low to high transition.
    EEPROM_CS = 1; // Start sequence, clock goes high.
    Nop(); // Wait 250 nS before clock goes high, tCSS>50nS
    EEPROM_DI = 1; // **** START BIT ****.
    Nop(); // Wait 250 nS before clock goes high, tDIS>100nS
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // tSKH 250 nS min but tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 0; // *** 1ST DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // Wait tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 0; // *** 2ND DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // 250 nS delay.
    EEPROM_SK = 0; // Clock low.

    EEPROM_DI = 1; // *** 1ST ADDRESS CODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // 250 nS delay.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 1; // *** 2ND 1ST ADDRESS CODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // 250 nS delay.
    EEPROM_SK = 0; // Clock low.

    // Write arbitrary 6 bit address. Not sure if this is really necessary.
    while (count--) { // Count = 6 for 6 address bits.
        // Mask off address bit of interest then shift right 5 to put
        // bit A0 location.
        EEPROM_DI = ((read_add & add_mask) >> 5);
        Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
        EEPROM_SK = 1; // Clock high.
        read_add = read_add << 1; // Shift to the left one bit;

        EEPROM_SK = 0; // Clock low.
    }

    Nop(), Nop();
    EEPROM_CS = 0; // End of sequence.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
    IEC0bits.T1IE = 1; // enable Time1 interrupt
    IEC1bits.T4IE = 1; // enable Time1 interrupt
}

void E_Erase() {
}

void E_Write(int write_add, int write_data) {
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    IEC0bits.T1IE = 0; // enable Time1 interrupt
    IEC1bits.T4IE = 0; // enable Time1 interrupt
    Nop(); // Added for interrupt disable timing.


    int write_data_mask = 0x8000; // Mask for masking data bit.
    int write_add_mask = 0x0020; // Mask for masking address bit.
    int count = 6; // Count of the 6 address bits.
    int temp = 0; // Read variable.
    EEPROM_CS = 0; // Start with CS=0, low to high transition.
    EEPROM_CS = 1; // Start sequence, clock goes high.
    Nop(); // Wait 250 nS before clock goes high, tCSS>50nS
    EEPROM_DI = 1; // **** START BIT ****.
    Nop(); // Wait 250 nS before clock goes high, tDIS>100nS
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // tSKH 250 nS min but tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 0; // *** 1ST DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // Wait tDIH>400nS.
    EEPROM_SK = 0; // Clock low.
    Nop();

    EEPROM_DI = 1; // *** 2ND DIGIT OPCODE ***.
    Nop(); // tDIS > 100nS.
    EEPROM_SK = 1; // Clock high.
    Nop(), Nop(); // 250 nS delay.
    EEPROM_SK = 0; // Clock low.

    // Write arbitrary 6 bit address. Not sure if this is really necessary.

    while (count--) { // Count = 6 for 6 address bits.
        // Mask off address bit of interest then shift right 5 to put
        // bit A0 location.
        EEPROM_DI = ((write_add & write_add_mask) >> 5);
        Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
        EEPROM_SK = 1; // Clock high.
        write_add = write_add << 1; // Shift to the left one bit;
        EEPROM_SK = 0; // Clock low.
    }

    // 16 bits of data to write stored in 'data'.
    // First determine which organization mode it is in.
    if (EEPROM_ORG == 0) { // Accommodate for 8 or 16 bit configuration.
        count = 8;
    } else {
        count = 16; // Variable for data read.
    }

    // Put the data to be written on EEPROM_DI, MSBit first.
    while (count--) { // Count
        // Mask off address bit of interest then shift right 5 to put
        // bit A0 location.
        temp = (write_data & write_data_mask);
        temp = temp >> 15;
        EEPROM_DI = temp;
        Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
        EEPROM_SK = 1; // Clock high.
        write_data = write_data << 1; // Shift to the left one bit;
        EEPROM_SK = 0; // Clock low.
    }

    EEPROM_CS = 0; // Pull CS low than high to trigger Busy/Ready signal.
    Nop(), Nop();
    EEPROM_CS = 1;

    // Now keep supplying clock pulses all the while testing EEPROM_DO.
    while (!EEPROM_DO) {
    }

    EEPROM_CS = 0; // End of sequence.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
    IEC0bits.T1IE = 1; // enable Time1 interrupt
    IEC1bits.T4IE = 1; // enable Time1 interrupt
}

void E_Eral() {
}

void E_Wral(int write_data) {
    /* *** Not working yet ***
        IEC1bits.CNIE = 0; // disable CN ISR
        IEC3bits.INT3IE = 0; // disable INT3 ISR
        IEC3bits.INT4IE = 0; // disable INT4 ISR
        IEC0bits.INT0IE = 0; // disable INT0 ISR
        IEC0bits.T1IE = 0; // enable Time1 interrupt
        IEC1bits.T4IE = 0; // enable Time1 interrupt
        Nop(); // Added for interrupt disable timing.

        int write_data_mask = 0x8000; // Mask for masking data bit.
        int write_add_mask = 0x0020; // Mask for masking address bit.
        int count = 6; // Count of the 6 address bits.
        int write_add = 0x0021;  // Any old address.
        int temp = 0; // Read variable.
        EEPROM_CS = 0; // Start with CS=0, low to high transition.
        EEPROM_CS = 1; // Start sequence, clock goes high.
        Nop(); // Wait 250 nS before clock goes high, tCSS>50nS
        EEPROM_DI = 1; // **** START BIT ****.
        Nop(); // Wait 250 nS before clock goes high, tDIS>100nS
        EEPROM_SK = 1; // Clock high.
        Nop(), Nop(); // tSKH 250 nS min but tDIH>400nS.
        EEPROM_SK = 0; // Clock low.
        Nop();

        EEPROM_DI = 0; // *** 1ST DIGIT OPCODE ***.
        Nop(); // tDIS > 100nS.
        EEPROM_SK = 1; // Clock high.
        Nop(), Nop(); // Wait tDIH>400nS.
        EEPROM_SK = 0; // Clock low.
        Nop();

        EEPROM_DI = 0; // *** 2ND DIGIT OPCODE ***.
        Nop(); // tDIS > 100nS.
        EEPROM_SK = 1; // Clock high.
        Nop(), Nop(); // 250 nS delay.
        EEPROM_SK = 0; // Clock low.

        EEPROM_DI = 0; // *** 1ST ADDRESS CODE ***.
        Nop(); // tDIS > 100nS.
        EEPROM_SK = 1; // Clock high.
        Nop(), Nop(); // 250 nS delay.
        EEPROM_SK = 0; // Clock low.
        Nop();

        EEPROM_DI = 1; // *** 2ND 1ST ADDRESS CODE ***.
        Nop(); // tDIS > 100nS.
        EEPROM_SK = 1; // Clock high.
        Nop(), Nop(); // 250 nS delay.
        EEPROM_SK = 0; // Clock low.

        // Write arbitrary 6 bit address. Not sure if this is really necessary.

        while (count--) { // Count = 6 for 6 address bits.

            // Mask off address bit of interest then shift right 5 to put
            // bit A0 location.
            EEPROM_DI = ((write_add & write_add_mask) >> 5);
            //temp = temp >> 5;
            //EEPROM_DI = temp;
            Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
            EEPROM_SK = 1; // Clock high.
            write_add = write_add << 1; // Shift to the left one bit;

            EEPROM_SK = 0; // Clock low.
            //Nop(), Nop(); // 250 nS delay.
        }

        // 16 bits of data to write stored in 'data'.
        // First determine which organization mode it is in.
        if (EEPROM_ORG == 0) { // Accommodate for 8 or 16 bit configuration.
            count = 8;
        } else {
            count = 16; // Variable for data read.
        }

        // Put the data to be written on EEPROM_DI, MSBit first.
        while (count--) { // Count

            // Mask off address bit of interest then shift right 5 to put
            // bit A0 location.
            temp = (write_data & write_data_mask);
            temp = temp >> 15;
            EEPROM_DI = temp;
            Nop(), Nop(); // Wait tSKH 250 nS min but tDIH>400nS.
            EEPROM_SK = 1; // Clock high.
            write_data = write_data << 1; // Shift to the left one bit;

            EEPROM_SK = 0; // Clock low.
            //Nop(), Nop(); // 250 nS delay.

        }


        EEPROM_CS = 0; // Pull CS low than high to trigger Busy/Ready signal.
        Nop(), Nop();
        EEPROM_CS = 1;
    // Now keep supplying clock pulses all the while testing EEPROM_DO.
    
        while (!EEPROM_DO) {
          //  EEPROM_SK = 1;
          //  Nop(), Nop();   // wait tSK > 250nS.
           // EEPROM_SK = 0;
        }

    EEPROM_CS = 0; // End of sequence.


        IEC1bits.CNIE = 1; // enable CN ISR
        IEC3bits.INT3IE = 1; // enable INT3 ISR
        IEC3bits.INT4IE = 1; // enable INT4 ISR
        IEC0bits.INT0IE = 1; // enable INT0 ISR
        IEC0bits.T1IE = 1; // enable Time1 interrupt
        IEC1bits.T4IE = 1; // enable Time1 interrupt
     */
}

void E_Ewds() {
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

