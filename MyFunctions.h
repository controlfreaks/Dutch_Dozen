
/* 
 * File:                    MyFunctions.h
 * Author:                  James Vlasblom
 * Date:                    September 29, 2016.
 * Revision history:        1.0
 * 
 * Comments:            This file contains misc functions that may have broad 
 *                      appeal
 * 
 *                      Function List:  DelayUs()
 *                                      DelayMs()
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MYFUNCTIONS_H
#define	MYFUNCTIONS_H

#define OSC_CLOCK 8000000              // 8 MHz oscillator
#define INSTRUCT_CLOCK (OSC_CLOCK / 4) // 4 OSC per instruction cycle
#define CLOCKMS (0.001 * INSTRUCT_CLOCK)     // number represents clk / 2000 (ie 4MHz / 2000)
//#define CLOCKMS 2000
#define CLOCKUS 2
#define KLUNK_CH 11
#define _POS 0
#define _NEG 1
#define _OUTPUT 0
#define _INPUT 1

#include <xc.h> // include processor files - each processor file is guarded. 

// *** Declarations start here ***
void DelayMs( long time);
void DelayUs( long time);


  // *** Function definitions start here, all declarations above this point
void DelayMs( long time)
{
    long Temp= 0;           // temp 32 bit variable]
    Temp = time * CLOCKMS;
    PR2 = Temp;             // copy LSW into PR2
    PR3 = Temp >> 16;       // Shift 16 bits and copy MSW into PR3
    T2CON = 0x0000;         // stops Timer1
    T3CON = 0x00;           // stops any 16-bit Timer3 operation
    TMR3 = 0x00;            // clears contents of Timer3 Register 
    TMR2 = 0x00;            // clears contents of Timer2 register
    T2CONbits.TCKPS = 0;    // set clock prescaler to 1:1
    IFS0bits.T3IF = 0;      // clears timer3 interrupt status flag
    T2CONbits.T32 = 1;      // enable 32bit timer operation
    T2CONbits.TON = 1;      // start 32 bit timer
    
    while(IFS0bits.T3IF == 0)
    {}
}


// This Us routine is only accurate above 100Us due to system clock
void DelayUs(long time)
{
    PR2 = time * CLOCKUS;             // copy LSW into PR2
    T2CON = 0x0000;         // stops Timer1
    TMR2 = 0x00;            // clears contents of Timer2 register
    //T2CONbits.TCKPS = 0;    // set clock prescaler to 1:1
    IFS0bits.T2IF = 0;      // clears timer3 interrupt status flag
    T2CONbits.TON = 1;      // start 32 bit timer
    
    while(IFS0bits.T2IF == 0)
    {}
}



// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

