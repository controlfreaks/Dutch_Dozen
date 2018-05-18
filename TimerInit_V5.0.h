
/* 
 * File:                TimerInit.h
 * Author:              James Vlasblom
 * Date:                September 29, 1016
 * Comments:            This file contains initialization configuration bits
 *                      for all the timer modules in their respective functions.
 *                      (Timer1Init() - Timer6Init())
 * Revision history:    1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef TIMERINIT_50_Shot_H
#define	TIMERINIT_50_Shot_H

#include <xc.h> // include processor files - each processor file is guarded.  

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


// *** Timer1 configuration function ***
void TimerInit(void);

void TimerInit(void)
{
    
    // *** Timer 1 Initialization *** ///
        
    _TON = 0;               // 0 = stops timer, 1 = starts timer
    _TSIDL = 0;             // 0 = Time continues in IDLE mode, 1 = Timer stops 
                            // in IDLE mode.
    _TGATE = 0;             // 0 = Gated time accumulation is disabled
                            // 1 = GAted time accumulation is enabled:
                            // When TCS = 1.
    _TCKPS = 3;             // Timer 1 Input Clock Prescale Select Bits.
                            // 0 = 1:1, 1 = 1:8, 2 = 1:64, 3 = 1:256.
    _TSYNC = 0;             // 0 = Does not synchronize external clock input.
                            // 1 =  Synchronizes external clock input.
    _TCS = 0;               // 0 = Internal clock (Fosc/2).
                            // 1 = External clock from pin T1CK (on rising edge)
    
    TMR1 = 0x00;            // clears contents of Timer1 register
    PR1 = 0x2000;           // load period register with interrupt match value
    T1CONbits.TON = 1;      // turn on Timer1
    
    
    
      // *** Timer 4 Initialization *** ///
    
      // _TON  
    T4CONbits.TON = 0;      // 0 = stops timer, 1 = starts timer
    
    // _TSIDL
    T4CONbits.TSIDL = 0;    // 0 = Time continues in IDLE mode, 1 = Timer stops 
                            // in IDLE mode.
    //_TGATE
    T4CONbits.TGATE = 0;    // 0 = Gated time accumulation is disabled
                            // 1 = GAted time accumulation is enabled:
                            // When TCS = 1.
    //_TCKPS
    T4CONbits.TCKPS = 0;    // Timer 4 Input Clock Prescale Select Bits.
                            // 0 = 1:1, 1 = 1:8, 2 = 1:64, 3 = 1:256.
    //_TCS
    T4CONbits.TCS= 0;       // 0 = Internal clock (Fosc/2).
                            // 1 = External clock from pin T1CK (on rising edge)
    
    TMR4 = 0x00;            // clears contents of Timer4 register
    
    PR4 = 0x2000;           // load period register with interrupt match value
    T4CONbits.TON = 1;      // turn on Timer4  
    


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */
}
