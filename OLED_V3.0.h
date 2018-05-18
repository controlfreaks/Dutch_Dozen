
/* 
 * File:                OLED.h
 * Author:              James Vlasblom
 * Date:                September 29, 2016
 * Revision history:    1.0
 * 
 * Comments:            This file contains the initialization function of the
 *                      of the OLEDS, InitializeOLED(), and the various 
 *                      functions necessary to display information on the OLED.
 *                      The OLED used in these functions used the SSD1305
 *                      controller chip.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef OLED_50_Shot_H
#define	OLED_50_Shot_H

// *** Insert includes here ***

#include <xc.h> // include processor files - each processor file is guarded.  
#include "MyFunctions_V5.0.h"
#include <stdlib.h>


// *** Insert definitions here *** 

#define BLACK 0
#define COMMAND 0
#define WHITE 1
#define DATA 1

#define CS _LATG9            // Chip select for OLED
//#define RES _LATG1           // Reset for OLED
#define DC _LATG0            // Data / Command for OLED (LATG0 for pictail)
#define SCLK _LATG6          // Clock for OLED
#define MOSI _LATG8          // Master out, serial in for OLED


// *** Definition of control words for the SSSD1305 controller
#define SSD1305_SETLOWCOLUMN 0x00
#define SSD1305_SETHIGHCOLUMN 0x10
#define SSD1305_SETCOLUMNADDRESS_HVMODE 0x21
#define SSD1305_SETSTARTPAGE_HVMODE 0x22
#define SSD1305_MEMORYMODE 0x20
#define SSD1305_SETSTARTLINE 0x40
#define SSD1305_SETCONTRAST 0x81
#define SSD1305_SETBRIGHTNESS 0x82
#define SSD1305_SETLUT 0x91
#define SSD1305_SEGREMAP 0xA0
#define SSD1305_SEGREMAP1 0xA1
#define SSD1305_DISPLAYALLON_RESUME 0xA4
#define SSD1305_DISPLAYALLON 0xA5
#define SSD1305_NORMALDISPLAY 0xA6
#define SSD1305_INVERTDISPLAY 0xA7
#define SSD1305_SETMULTIPLEX 0xA8
#define SSD1305_DISPLAYDIM 0xAC
#define SSD1305_MASTERCONFIG 0xAD
#define SSD1305_DISPLAYOFF 0xAE
#define SSD1305_DISPLAYON 0xAF
#define SSD1305_SETPAGE_PAGEMODE 0xB0 
#define SSD1305_COMSCANINC 0xC0
#define SSD1305_COMSCANDEC 0xC8
#define SSD1305_SETDISPLAYOFFSET 0xD3
#define SSD1305_SETDISPLAYCLOCKDIV 0xD5
#define SSD1305_SETAREACOLOUR 0xD8
#define SSD1305_SETPRECHARGE 0xD9
#define SSD1305_SETCOMPINS 0xDA
#define SSD1305_SETVCOMDETECTLVL 0xDB
#define SSD1305_SETMODIFY 0xE0
#define SSD1305_CLRMODIFY 0xEE
#define SSD1305_NOP 0xE3

// *** Definition of control words for the SSSD1325 controller
//
#define SSD1325_SETCOLADDR 0x15
#define SSD1325_SETROWADDR 0x75
#define SSD1325_SETCONTRAST 0x81
#define SSD1325_SETCURRENT 0x86

#define SSD1325_SETREMAP 0xA0
#define SSD1325_SETSTARTLINE 0xA1
#define SSD1325_SETOFFSET 0xA2
#define SSD1325_NORMALDISPLAY 0xA4
#define SSD1325_DISPLAYALLON 0xA5
#define SSD1325_DISPLAYALLOFF 0xA6
#define SSD1325_INVERTDISPLAY 0xA7
#define SSD1325_SETMULTIPLEX 0xA8
#define SSD1325_MASTERCONFIG 0xAD
#define SSD1325_DISPLAYOFF 0xAE
#define SSD1325_DISPLAYON 0xAF

#define SSD1325_STEPRECHARGECOMPENABLE 0xB0
#define SSD1325_SETPHASELEN 0xB1
#define SSD1325_SETROWPERIOD 0xB2
#define SSD1325_SETCLOCKDIV 0xB3
#define SSD1325_SETPRECHARGECOMP 0xB
#define SSD1325_SETGREYTABLE 0xB8
#define SSD1325_SETPRECHARGEVOLT 0xBC
#define SSD1325_SETCOMLEVEL 0xBE
#define SSD1325_SETVSL 0xBF

#define SSD1325_GFXACCEL 0x23
#define SSD1325_DRAWREC 0x24
#define SSD1325_COPY 0x25


/*

// *** Function Prototypes here ***

void Initialize_SSD1305_OLED(void);
void Initialize_SSD1325_OLED(void);
void ClearOLED(void);
void RV_ClearOLED(void);
void ResetOLED(void);
void WriteCommand(unsigned char Command);
void WriteData(unsigned char Data);
void CharWrite(int digit);
void RV_CharWrite(int digit);
void StringWrite(char *digit);
void RV_StringWrite(char *digit);
void PageWrite(char *digits, int page);
void RV_PageWrite(char *digits, int page);
void PageWriteCol(char *digits, int page, int lowadd, int highadd);
void RV_PageWriteCol(char *digits, int page, int lowadd, int highadd);

*/
// Global Variables
//int Page = 0;

// *** Function definitions here ***
// ---------------------------------------

// *** List of OLED display functions ***
// CharWrite()   - Write character in the next space.
// StringWrite()    - Writes string on the next line.
// PageWrite()  - Writes string on the specified page (P0-P7).
// PageWriteXY() - Writes string on specified page and specified start address.
// MultiUpdate() accepts a string pointer to indicate mode, Multi or single.
// CharWrite() accepts an ASCII character with in single quotes or else an
// ASCII number 32-255. ASCII numbers between 0 - 32 are bonus characters 
// courtesy from Adafruit.
// Note: The offset of this font table is cleverly equal to the ASCII value * 5
// For example, '0' is ASCII 48, offset is 48*5 = 240.
/*
void CharWrite(int digit) {
    int a;
    for (a = 0; a <= 4; a++) // Loop because characters are 5 wide
        WriteData(Font[(digit * 5) + a]);
    WriteCommand(SSD1305_DISPLAYON);
}


// RV_CharWrite() accepts an ASCII character with in single quotes or else an
// ASCII number 32-255. ASCII numbers between 0 - 32 are bonus characters 
// courtesy from Adafruit.
// Note: The offset of this font table is cleverly equal to the ASCII value * 5
// For example, '0' is ASCII 48, offset is 48*5 = 240.

void RV_CharWrite(int digit) {
    int a;
    for (a = 0; a <= 4; a++) // Loop because characters are 5 wide
        WriteData(RV_Font[(digit * 5) + a]);
    WriteCommand(SSD1305_DISPLAYON);
}

// StringWrite() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// advances the page and reset the column counter to zero thereby starting at
// the beginning of the next line. After the last line the page counter resets
// back to zero.

void StringWrite(char *digits) {
    // Start at column zero.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x00); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x10); // Set high column to 0x00.

    WriteCommand(SSD1305_SETPAGE_PAGEMODE | Page); // Print at the next page.
    for (; *digits != NULL;) {
        CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0x00); // character was a space.
        digits++;
    }
    if (Page <= 6) // Advance the page number
        Page++;
    else
        Page = 0;
}

// RV_StringWrite() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// advances the page and reset the column counter to zero thereby starting at
// the beginning of the next line. After the last line the page counter resets
// back to zero.

void RV_StringWrite(char *digits) {
    // Start at column zero.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x00); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x10); // Set high column to 0x00.

    WriteCommand(SSD1305_SETPAGE_PAGEMODE | Page); // Print at the next page.
    for (; *digits != NULL;) {
        RV_CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0xFF); // character was a space(RV).
        digits++;
    }
    if (Page <= 6) // Advance the page number
        Page++;
    else
        Page = 0;
}

// PageWrite() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// accepts an integer (0-7) representing the page number to write on.

void PageWrite(char *digits, int page) {
    // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Reset the column pointer to zero.
    WriteCommand(SSD1305_SETPAGE_PAGEMODE); // Set start page 0 for page mode.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x00); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x10); // Set high column to 0x00.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | page);

    for (; *digits != NULL;) {
        CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0x00); // character was a space.
        digits++;
    }
}

// RV_PageWrite() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// accepts an integer (0-7) representing the page number to write on.

void RV_PageWrite(char *digits, int page) {
    // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Reset the column pointer to zero.
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | page); // ****Set start page 0 for page mode.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x00); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x10); // Set high column to 0x00.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | page);

    for (; *digits != NULL;) {
        RV_CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0xFF); // character was a space.(RV))
        digits++;
    }
}

// PagwWriteCol() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// accepts an integer (0-7) representing the page number to write on. 
// As well it also accepts two ints which represents the lower and upper nibble
// of the starting column (1 - 131).

void PageWriteCol(char *digits, int page, int lowadd, int highadd) {
    // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | page);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | lowadd); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | highadd); // Set high column to 0x00.

    for (; *digits != NULL;) {
        CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0x00); // character was a space.
        digits++;
    }
}

// RV_PagwWriteCol() accepts a string in quotes or a string array. It automatically
// adds a space between letters except if the last letter was a space. It also 
// accepts an integer (0-7) representing the page number to write on. 
// As well it also accepts two ints which represents the lower and upper nibble
// of the starting column (1 - 131).

void RV_PageWriteCol(char *digits, int page, int lowadd, int highadd) {
    // Set OLED into page mode and set the page.
    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x02); // Set to Page memory mode.

    // Set page (0 - 7)
    WriteCommand(SSD1305_SETPAGE_PAGEMODE | page);
    // Set column address.
    WriteCommand(SSD1305_SETLOWCOLUMN | lowadd); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | highadd); // Set high column to 0x00.

    for (; *digits != NULL;) {
        RV_CharWrite(*digits);
        if (*digits != 32) // Don't add a space if the last 
            WriteData(0xFF); // character was a space (RV).
        digits++;
    }
}

void Initialize_SSD1305_OLED(void) {

    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x00); // Set to Horizontal memory mode.

    // *** The next three commands are for setting up PAGE mode. Comment out if
    //     using either Horizontal or vertical mode.

    WriteCommand(SSD1305_SETPAGE_PAGEMODE); // Set start page 0 for page mode.
    WriteCommand(SSD1305_SETLOWCOLUMN | 0x00); // Set low column to 0x00.
    WriteCommand(SSD1305_SETHIGHCOLUMN | 0x10); // Set high column to 0x00.


    // *** The next six commands are for setting up PAGE mode. Comment out if   
    //     using PAGE mode.

    WriteCommand(SSD1305_SETCOLUMNADDRESS_HVMODE); // 0x21
    WriteCommand(0x00); // Start address of 0x00.
    WriteCommand(0x7F); // End address of 131d.
    WriteCommand(SSD1305_SETSTARTPAGE_HVMODE); // 0x22
    WriteCommand(0x00); // Start address of 0d.
    WriteCommand(0x07); // End address of 7d. 

    WriteCommand(SSD1305_SETSTARTLINE); // Start line at zero

    WriteCommand(SSD1305_SETCONTRAST); // 0x81
    WriteCommand(0x80); // POR Contrast(0xFF is high)

    WriteCommand(SSD1305_SETBRIGHTNESS); // 0x82
    WriteCommand(0x80); // POR Contrast(0xFF is high)
    WriteCommand(SSD1305_SEGREMAP); // Segment remap Col 0 = Seg 0 
    WriteCommand(SSD1305_NORMALDISPLAY); // 0xA6
    WriteCommand(SSD1305_SETMULTIPLEX); // 0xA8
    WriteCommand(0x3F); // Ratio of 64 (POR)
    WriteCommand(SSD1305_MASTERCONFIG); // 0xAD
    WriteCommand(0x8E); // External Vcc supply
    WriteCommand(SSD1305_COMSCANINC); // Column scan in default direction
    WriteCommand(SSD1305_SETDISPLAYOFFSET); // 0xD3
    WriteCommand(0x00); // No offset
    WriteCommand(SSD1305_SETDISPLAYCLOCKDIV); // 0xD5
    WriteCommand(0x80); // Suggested ratio of 0x80
    WriteCommand(SSD1305_SETAREACOLOUR); // 0xD8
    WriteCommand(0x00); // Monochrome / normal power.
    WriteCommand(SSD1305_SETPRECHARGE); // 0xD9
    WriteCommand(0xF1); // Both periods of 3  
    WriteCommand(SSD1305_SETCOMPINS); // 0xDA
    WriteCommand(0x12); // Disable COM left/right map
    WriteCommand(SSD1305_SETLUT); // 0x91
    WriteCommand(0x3F);
    WriteCommand(0x3F);
    WriteCommand(0x3F);
    WriteCommand(0x3F);
    WriteCommand(SSD1305_DISPLAYOFF); //0xAE

    DelayMs(10);
}

void Initialize_SSD1325_OLED(void) {
    // Commands for initializing the SSD1325 OLED display.

    // Set Column Address.
    WriteCommand(SSD1325_DISPLAYOFF);

    WriteCommand(SSD1325_SETCLOCKDIV); // (0xB3)
    WriteCommand(0x61); //  = Default, divide ratio = 2

    // Set Multiplex Ratio.
    WriteCommand(SSD1325_SETMULTIPLEX); // (0xA8)
    WriteCommand(0x4F); // Default = 0x4F.

    // Set display offset
    WriteCommand(SSD1325_SETOFFSET); // (0xA2)
    WriteCommand(0x00); // 0x00 Default

    // Set start line.
    WriteCommand(SSD1325_SETSTARTLINE); // (0xA1)
    WriteCommand(0x00); // Default = 0x00.

    // Set Master Config
    WriteCommand(SSD1325_MASTERCONFIG); // (0xAD)
    WriteCommand(0x02); // 0x00 = Default.

    // Set Remap.
    WriteCommand(SSD1325_SETREMAP); // (0xA0).
    WriteCommand(0x052); // Default: Disable comm addr remap, Disable nibble 
    // remap, Horizontal increment, Disable COMM remap
    // Disable COMM split odd/even.

    // Set Current.
    WriteCommand(SSD1325_SETCURRENT); // (0x84, Quarter current)

    WriteCommand(SSD1325_SETGREYTABLE); // (0xB8))
    WriteCommand(0x01);
    WriteCommand(0x11);
    WriteCommand(0x22);
    WriteCommand(0x32);
    WriteCommand(0x43);
    WriteCommand(0x54);
    WriteCommand(0x65);
    WriteCommand(0x76);

    // Set contrast
    WriteCommand(SSD1325_SETCONTRAST); // (0x81)
    WriteCommand(0x40); // level 1 - 128, 

    WriteCommand(SSD1325_SETROWPERIOD); // (0xB2)
    WriteCommand(0x25); // 0x25 = Default.

    WriteCommand(SSD1325_SETPHASELEN); // (0xB1)
    WriteCommand(0x53); // 0x03 = Default.
    //WriteCommand(0x05); // 0x05 = Default.

    WriteCommand(SSD1325_SETPRECHARGEVOLT); // (0xBC)
    WriteCommand(0x18); // 0x18 = Default.

    WriteCommand(SSD1325_SETPRECHARGECOMP); // (0xB4)
    WriteCommand(0x00); // 0x03 recommended.

    WriteCommand(SSD1325_STEPRECHARGECOMPENABLE); // (0xB0)
    WriteCommand(0x08); // 0x08 = Default.

    WriteCommand(SSD1325_SETCOMLEVEL); // (0xBE)
    WriteCommand(0x18); // 0x18 = Default.




    WriteCommand(SSD1325_SETCOLADDR); // (0x15)
    WriteCommand(0x00); // col start address = 0x00.
    WriteCommand(0x3F); // col end address 0x3f = 63d.

    // Set Row Address.,
    WriteCommand(SSD1325_SETROWADDR); // (0x75)
    WriteCommand(0x00); // Row start address = 0x00.
    WriteCommand(0x7F); // Row end address = 0x7F (127).

    WriteCommand(SSD1325_SETVSL); // (0xBF)
    WriteCommand(0x0D | 0x02); // 0x0E = Default.

    // Set display mode.
    WriteCommand(SSD1325_NORMALDISPLAY); // (0xA4 = normal display.

    WriteCommand(SSD1325_DISPLAYON);




}

void ClearOLED(void) {
    // Clear entire OLED, all 8 pages
    // NOTE: It is important to change the Column and Page start addresses to
    // Zero, and the length and width to maximum when clearing the display so
    // as to clear the entire thing; hence the five lines.

    // NOTE: Also remember to place the clear screen before the Initialization
    // routine or else the CearOLERD() setting will take precedence).

    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x00); // Set to Horizontal memory 
    WriteCommand(SSD1305_SETCOLUMNADDRESS_HVMODE | 0x00); // 0x21
    WriteCommand(0x00); // Start address of 0x00.
    WriteCommand(0x83); // End address of 131d.

    int data = 0x00;
    int a;

    for (a = 0; a <= 1055; a++) {
        WriteData(data);
    }
    WriteCommand(SSD1305_DISPLAYON);
    //DelayMs(1);
}

void RV_ClearOLED(void) {
    // Clear entire OLED, all 8 pages
    // NOTE: It is important to change the Column and Page start addresses to
    // Zero, and the length and width to maximum when clearing the display so
    // as to clear the entire thing; hence the five lines.

    // NOTE: Also remember to place the clear screen before the Initialization
    // routine or else the CearOLERD() setting will take precedence).

    WriteCommand(SSD1305_MEMORYMODE); // 0x20
    WriteCommand(0x00); // Set to Horizontal memory 
    WriteCommand(SSD1305_SETCOLUMNADDRESS_HVMODE | 0x00); // 0x21
    WriteCommand(0x00); // Start address of 0x00.
    WriteCommand(0x83); // End address of 131d.

    int data = 0xFF;
    int a;

    for (a = 0; a <= 1055; a++) {
        WriteData(data);
    }
    WriteCommand(SSD1305_DISPLAYON);
    //DelayMs(1);
}

// RESET() drives the active-low reset pin on the OLED (SSD1305) chip. Sets
//OLED into page mode, the default mode.

void ResetOLED(void) {

   // RES = 0; // Reset
    DelayUs(1); // Wait
   // RES = 1; // Come out of reset.
    DelayUs(1);
}

void WriteCommand(unsigned char Command) {
    DC = COMMAND; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Command;
    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    CS = 1; // deactivate ~CS.
}

void WriteData(unsigned char Data) {
    DC = DATA; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Data;
    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    CS = 1; // deactivate ~CS.
}
*/

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

