
/* 
 * File:            TFT_ILI9341.h
 * Author:          James Vlasblom
 * Date:            Feb 21, 2017  
 * Comments:     
 * This file contains drivers for the 240RGB x 320 TFT LCD display. Because this
 * can use either serial or parallel connections, both serial and parallel 
 * functions are included in this one header file. Theses functions are 
 * essentially the same except for the prefix 'Para_' in front of the 
 * parallel functions. 
 * 
 * Revision history: 
 * May 14, 2018:
 * Added initialization and driver functions for 8-bit parallel connection to the 
 * display.
 * 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TFT_ILI9341_H
#define	TFT_ILI9341_H


#include <xc.h> // include processor files - each processor file is guarded.  
#include "MyFunctions_V5.0.h"
#include "Fonts_V5.0.h"

// *** Definitions here


#define BLACK 0
#define COMMAND 0
#define WHITE 1
#define DATA 1

// #define statements for serial connection to the display.
#define CS _LATG9            // Chip select for OLED - Serial Mode.
#define RES _LATG1           // Reset for OLED - Serial Mode.
#define DC _LATG0            // Data / Command for OLED (LATG0 for pictail) - Serial Mode.
#define SCLK _LATG6          // Clock for OLED - Serial Mode.
#define MOSI _LATG8          // Master out, serial in for OLED - Serial Mode.

#define ILI9341_TFTWIDTH  320
#define ILI9341_TFTHEIGHT 240

#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0D
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1
/*
#define ILI9341_PWCTR6  0xFC

 */


// Line definitions.

#define Line0   0
#define Line1   30
#define Line2   60
#define Line3   90
#define Line4   120
#define Line5   150
#define Line6   180
#define Line7   210

// Color definitions
#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F
#define ILI9341_PHOSPHORGREEN 0x07C7

// *** Function Prototypes here ***


void CharWrite_XY_ILI9341_16x25(int digit, long int x_start, long int y_start,
        long int fore_colour, long int back_colour);
void DrawPixel_ILI9341(long int x, long int y, long int colour);
void FillRec_ILI9341(long int x, long int y, long int w, long int h,
        long int colour);
void FillScreen_ILI9341(long int colour);
void Initialize_TFT_ILI9341(void);
//void LineWrite_XY_ILI9341(char *digit, long int x, long int y, long int fore_colour,
//        long int back_colour);
void LineWrite_XY_ILI9341_16x25(char *digit, long int x, long int line, long int fore_colour,
        long int back_colour);
void SetAddrWindow_ILI9341(long int X_Start, long int Y_Start, long int X_End,
        long int Y_End);
void WriteCommand_ILI9341(unsigned char Command);
void WriteData_ILI9341(unsigned char Data);
//void WriteGRAM(int Colour);



// ****  Insert code here. *****

void Initialize_TFT_ILI9341(void) {
    RES = 0; // Reset the TFT.
    DelayMs(120);
    RES = 1; // Take TFT out of reset.

    WriteCommand_ILI9341(ILI9341_SWRESET);
    DelayMs(10);


    WriteCommand_ILI9341(0xEF);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x80);
    WriteData_ILI9341(0x02);

    WriteCommand_ILI9341(0xCF);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0XC1);
    WriteData_ILI9341(0X30);

    WriteCommand_ILI9341(0xED);
    WriteData_ILI9341(0x64);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0X12);
    WriteData_ILI9341(0X81);

    WriteCommand_ILI9341(0xE8);
    WriteData_ILI9341(0x85);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x78);

    WriteCommand_ILI9341(0xCB);
    WriteData_ILI9341(0x39);
    WriteData_ILI9341(0x2C);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x34);
    WriteData_ILI9341(0x02);

    WriteCommand_ILI9341(0xF7);
    WriteData_ILI9341(0x20);

    WriteCommand_ILI9341(0xEA);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_PWCTR1); //Power control 
    WriteData_ILI9341(0x23); //VRH[5:0] 

    WriteCommand_ILI9341(ILI9341_PWCTR2); //Power control 
    WriteData_ILI9341(0x10); //SAP[2:0];BT[3:0] 

    WriteCommand_ILI9341(ILI9341_VMCTR1); //VCM control 
    WriteData_ILI9341(0x3e); //Â¶Ã?Â±Ã?Â¶Ã?ÂµÃ·Â½Ã?
    WriteData_ILI9341(0x28);

    WriteCommand_ILI9341(ILI9341_VMCTR2); //VCM control2 
    WriteData_ILI9341(0x86); //--

    WriteCommand_ILI9341(ILI9341_MADCTL); // Memory Access Control 
    WriteData_ILI9341(0x28);

    WriteCommand_ILI9341(ILI9341_PIXFMT); // DPI and DBI set to 16 bits/pixel
    WriteData_ILI9341(0x55); // 16 bits/pixel (65k colour, RGB 5-6-6 bit input)
    //WriteData_ILI9341(0x66); // 18 bits/pixel (262k colour, RGB 6-6-6 bit input)

    WriteCommand_ILI9341(ILI9341_FRMCTR1);
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x18);

    WriteCommand_ILI9341(ILI9341_DFUNCTR); // Display Function Control 
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x82);
    WriteData_ILI9341(0x27);

    WriteCommand_ILI9341(0xF2); // 3Gamma Function Disable 
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_GAMMASET); //Gamma curve selected 
    WriteData_ILI9341(0x01);

    WriteCommand_ILI9341(ILI9341_GMCTRP1); //Set Gamma 
    WriteData_ILI9341(0x0F);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0x2B);
    WriteData_ILI9341(0x0C);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x4E);
    WriteData_ILI9341(0xF1);
    WriteData_ILI9341(0x37);
    WriteData_ILI9341(0x07);
    WriteData_ILI9341(0x10);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x09);
    WriteData_ILI9341(0x00);

    WriteCommand_ILI9341(ILI9341_GMCTRN1); //Set Gamma 
    WriteData_ILI9341(0x00);
    WriteData_ILI9341(0x0E);
    WriteData_ILI9341(0x14);
    WriteData_ILI9341(0x03);
    WriteData_ILI9341(0x11);
    WriteData_ILI9341(0x07);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0xC1);
    WriteData_ILI9341(0x48);
    WriteData_ILI9341(0x08);
    WriteData_ILI9341(0x0F);
    WriteData_ILI9341(0x0C);
    WriteData_ILI9341(0x31);
    WriteData_ILI9341(0x36);
    WriteData_ILI9341(0x0F);

    WriteCommand_ILI9341(ILI9341_SLPOUT); //Exit Sleep 
    DelayMs(120);
    WriteCommand_ILI9341(ILI9341_DISPON); //Display on 
    DelayMs(120);
}

void DrawPixel_ILI9341(long int x, long int y, long int colour) {

    int colour_hi;
    int colour_low;

    if ((x < 0) || (x >= ILI9341_TFTWIDTH) || (y < 0) || (y >= ILI9341_TFTHEIGHT)) return;
    SetAddrWindow_ILI9341(x, y, x, y);

    colour_hi = colour >> 8;
    colour_low = colour;
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop();      // Added for interrupt disable timing.

    DC = DATA; // Write Command, leave low
    CS = 0; // Activate ~CS   
    SPI2BUF = colour_hi;
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    SPI2BUF = colour_low;
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    CS = 1; // deactivate ~CS.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
}



void FillRec_ILI9341(long int x, long int y, long int w, long int h, long int colour) {

    int colour_hi;
    int colour_low;

    //rudimentary clipping(drawChar w / big text requires this)
    if ((x >= ILI9341_TFTWIDTH) || (y >= ILI9341_TFTHEIGHT)) return;
    if ((x + w - 1) >= ILI9341_TFTWIDTH) w = ILI9341_TFTWIDTH - x;
    if ((y + h - 1) >= ILI9341_TFTHEIGHT) h = ILI9341_TFTHEIGHT - y;

    SetAddrWindow_ILI9341(x, y, x + w - 1, y + h - 1);

    colour_hi = colour >> 8;
    colour_low = colour;

    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop();      // Added for interrupt disable timing.

    DC = DATA; // Write Command, leave low
    CS = 0; // Activate ~CS   

    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            SPI2BUF = colour_hi;
            Nop(), Nop(), Nop();
            SPI2BUF = colour_low;
        }
    }
    CS = 1; // deactivate ~CS.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void FillScreen_ILI9341(long int colour) {
    FillRec_ILI9341(0, 0, ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, colour);
}

void SetAddrWindow_ILI9341(long int X_Start, long int Y_Start, long int X_End,
        long int Y_End) {
    WriteCommand_ILI9341(ILI9341_CASET); // Column addr set (Horizontal, 480 Max.)
    WriteData_ILI9341(X_Start >> 8);
    WriteData_ILI9341(X_Start & 0xFF); // XSTART 
    WriteData_ILI9341(X_End >> 8);
    WriteData_ILI9341(X_End & 0xFF); // XEND

    WriteCommand_ILI9341(ILI9341_PASET); // Page addr set (vertical, 320 Max))
    WriteData_ILI9341(Y_Start >> 8);
    WriteData_ILI9341(Y_Start); // YSTART
    WriteData_ILI9341(Y_End >> 8);
    WriteData_ILI9341(Y_End); // YEND

    WriteCommand_ILI9341(ILI9341_RAMWR); // write to RAM
}




/*
void CharWrite_XY_ILI9341(int digit, long int x_start, long int y_start,
        long int fore_colour, long int back_colour) {
    int col;
    int row;
    int test = 0;
    int Mask = 0x01;
    int temp = 0;

    for (col = 0; col <= 4; col++) {
        temp = Font[(digit * 5) + col];
        for (row = 0; row <= 8; row++) // Loop because characters are 5 wide
        {
            test = (temp & Mask);
            if (test == 1) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp >> 1;
        }
        // This little bit adds a background coloured column at the end of a 
        // character. (AKA blank space)
        for (row = 0; row <= 8; row++) {// Loop because characters are 5 wide

            DrawPixel_ILI9341((col + x_start + 1), (row + y_start), back_colour);
        }
    }
}
 */
void CharWrite_XY_ILI9341_16x25(int digit, long int x_start, long int y_start,
        long int fore_colour, long int back_colour) {
    int col = 0;
    int row = 0;
    int test = 0;
    int Mask = 0x80;
    int temp = 0;
    for (row = 0; row <= 21; row++) {
        temp = Font_16x25[((digit - 32)* 50) + (2 * row)];
        for (col = 0; col <= 7; col++) {
            test = (temp & Mask);
            if (test == 0x80) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp << 1;
        }

        temp = Font_16x25[((digit - 32) * 50) + ((2 * row) + 1)];
        for (col = 8; col <= 15; col++) {
            test = (temp & Mask);
            if (test == 0x80) {
                DrawPixel_ILI9341((col + x_start), (row + y_start), fore_colour);
            } else {
                DrawPixel_ILI9341((col + x_start), (row + y_start), back_colour);
            }
            temp = temp << 1;
        }
    }
}

/*
void LineWrite_XY_ILI9341(char *digit, long int x, long int y, long int fore_colour,
        long int back_colour) {

    long int xtemp = x;

    SetAddrWindow_ILI9341(x, y, 5, 8);

    for (; *digit != NULL;) {
        CharWrite_XY_ILI9341(*digit, xtemp, y, fore_colour, back_colour);
        digit++;
        xtemp = xtemp + 6; // Make 6 wide to account for the blank
        // at the end of the character.
        SetAddrWindow_ILI9341(xtemp, y, 5, 8);
    }

}
 */
void LineWrite_XY_ILI9341_16x25(char *digit, long int x, long int line, long int fore_colour,
        long int back_colour) {
    int xtemp = x;

    SetAddrWindow_ILI9341(x, line, 16, 22);

    for (; *digit != NULL;) {
        CharWrite_XY_ILI9341_16x25(*digit, xtemp, line, fore_colour, back_colour);

        // This next if statement checks if the digit is lower case, if so it
        // advances only 13 instead of 16, compressing the letters closer.
        // Looks better.

        if ((*digit >= 97) && (*digit <= 122)) {
            xtemp = xtemp + 13;
        } else {
            xtemp = xtemp + 16; // Make 6 wide to account for the blank
        }
        // at the end of the character.
        digit++;
        SetAddrWindow_ILI9341(xtemp, line, 16, 22);
    }
}





void WriteCommand_ILI9341(unsigned char Command) {
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop();      // Added for interrupt disable timing.

    
    DC = COMMAND; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Command;
    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    Nop(), Nop();
    CS = 1; // deactivate ~CS.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
}

void WriteData_ILI9341(unsigned char Data) {
    IEC1bits.CNIE = 0; // disable CN ISR
    IEC3bits.INT3IE = 0; // disable INT3 ISR
    IEC3bits.INT4IE = 0; // disable INT4 ISR
    IEC0bits.INT0IE = 0; // disable INT0 ISR
    Nop();      // Added for interrupt disable timing.

    
    DC = DATA; // Write Command, leave low
    //Original delay of 12 NOPs approx 1.5uS.
    CS = 0; // Activate ~CS
    //Original delay of 15 NOPs approx 1.9uS.
    SPI2BUF = Data;
    //Original delay of 23 NOPs approx 2.8uS.
    Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop(), Nop();
    Nop(), Nop();
    CS = 1; // deactivate ~CS.

    IEC1bits.CNIE = 1; // enable CN ISR
    IEC3bits.INT3IE = 1; // enable INT3 ISR
    IEC3bits.INT4IE = 1; // enable INT4 ISR
    IEC0bits.INT0IE = 1; // enable INT0 ISR
}
/*
void WriteGRAM(int Colour) {


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

