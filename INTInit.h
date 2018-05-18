
/* 
 * File:            INTInit.h
 * Author:          James Vlasblom  
 * Date:            September 29, 2016
 * Comments:        This file contains the function 'IntInit()' which
 *                  contains all the interrupt configuration bits for all
 *                  of the different modules. Each module's configuration bits
 *                  can be found in the header file for each individual  
 *                  module.
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef INTINIT_H
#define	INTINIT_H

#include <xc.h> // include processor files - each processor file is guarded.
#include "ADCInit.h"
#include "Misc_Macro.h"
#include "MyFunctions.h"


// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

void IntInit(void);
    // Configure change structure
    
 
struct tagCHANGE{
  unsigned CN0:1;
  unsigned CN1:1;
  unsigned CN2:1;
  unsigned CN3:1;
  unsigned CN4:1;
  unsigned CN5:1;
  unsigned CN6:1;
  unsigned CN7:1;
  unsigned CN8:1;
  unsigned CN9:1;
  unsigned CN10:1;
  unsigned CN11:1;
  unsigned CN12:1;
  unsigned CN13:1;
  unsigned CN14:1;
  unsigned CN15:1;
}CHANGEBITS = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


#define _CN0 CHANGEBITS.CN0
#define _CN1 CHANGEBITS.CN1
#define _CN2 CHANGEBITS.CN2
#define _CN3 CHANGEBITS.CN3
#define _CNSM CHANGEBITS.CN4
#define _CNARM CHANGEBITS.CN5
#define _CNFIRE CHANGEBITS.CN6
#define _CN7 CHANGEBITS.CN7
#define _CN8 CHANGEBITS.CN8
#define _CN9 CHANGEBITS.CN9
#define _CN10 CHANGEBITS.CN10
#define _CN11 CHANGEBITS.CN11
#define _CN12 CHANGEBITS.CN12
#define _CN13 CHANGEBITS.CN13
#define _CN14 CHANGEBITS.CN14
#define _CN15 CHANGEBITS.CN15

 

void IntInit(void)
{
    // **********************************************
    // *** Configuration for 10 bit A/D converter ***
    // **********************************************
    
    // Note: The configuration bits for the A/D module can be found in the
    // header file for that module.
    
    IFS0bits.AD1IF = 0;     // reset A/D Conversion Done interrupt flag
    IEC0bits.AD1IE = 0;     // disable A/D Conversion Done interrupt
    IPC3bits.AD1IP = 1;     // set A/D Conversion Done priority to low
    
    
    // *********************************************************
    // *** Configuration for External Interrupts (INT0-INT4) ***
    // *********************************************************
    
    
    _NSTDIS = 0;            // enable nested interrupts
    
    // configure external interrupt edge detect polarity
    _INT0EP = _POS;         // set INT0 edge polarity (0 = +ve, 1 = -ve)
    _INT1EP = _NEG;         // set INT1 edge polarity (0 = +ve, 1 = -ve)
    _INT2EP = _POS;          // set INT2 edge polarity (0 = +ve, 1 = -ve)  
    _INT3EP = _NEG;         // set INT3 edge polarity (0 = +ve, 1 = -ve)
    _INT4EP = _NEG;          // set INT4 edge polarity (0 = +ve, 1 = -ve)
    
    // ***Note: Function Alarm_Alert disables all these interrupts when an
    //          illegal ARM condition occurs. Any changes in these interrupts
    //          must be reflected in function AlarmAlert as well.
    
    
    // Priority level '0' = low (effectively disabled)
    // Priority level '7' = highest
    
    // configure INT0
    IFS0bits.INT0IF = 0;    // reset INT0 interrupt flag
    IEC0bits.INT0IE = 1;    // enable INT0 ISR
    IPC0bits.INT0IP = 4;    // set low priority
    
    // configure INT1
    IFS1bits.INT1IF = 0;    // reset INT1 interrupt flag
    IEC1bits.INT1IE = 1;    // enable INT1 ISR
    IPC5bits.INT1IP = 4;    // set low priority
    
    // configure INT2
    IFS1bits.INT2IF = 0;    // reset INT2 interrupt flag
    IEC1bits.INT2IE = 0;    // disable INT2 ISR
    IPC7bits.INT2IP = 4;    // set low priority
    
    // configure INT3
    IFS3bits.INT3IF = 0;    // reset INT3 interrupt flag
    IEC3bits.INT3IE = 1;    // enable INT3 ISR
    IPC13bits.INT3IP = 4;    // set low priority
    
    // configure INT4
    IFS3bits.INT4IF = 0;    // reset INT4 interrupt flag
    IEC3bits.INT4IE = 1;    // enable INT4 ISR
    IPC13bits.INT4IP = 7;    // set low high
    
    
    // configure timer 1 interrupt T1
    IFS0bits.T1IF = 0;     // reset Timer1 interrupt flag
    IEC0bits.T1IE = 1;     // enable Time1 interrupt
    IPC0bits.T1IP = 4;     // set Timer1 priority to low
    
        // configure timer 4 interrupt T4
    IFS1bits.T4IF = 0;     // reset Timer1 interrupt flag
    IEC1bits.T4IE = 1;     // enable Time1 interrupt
    IPC6bits.T4IP = 4;     // set Timer1 priority to low
     
    // *********************************************************
    // *** Configuration for Change Notification Interrupts  ***
    // *********************************************************   
     
    // Enable on Input Change Notification Bits
     
    // CNEN1 = 0x0000         // set the entire register (CN0 - CN15)
     
    _CN0IE = 0;              // 0 = CN0 disabled, 1 = CN0 enabled
    _CN1IE = 0;              // 0 = CN1 disabled, 1 = CN1 enabled
    _CN2IE = 0;              // 0 = CN2 disabled, 1 = CN2 enabled
    _CN3IE = 0;              // 0 = CN3 disabled, 1 = CN3 enabled 
    _CN4IE = 1;              // 0 = CN4 disabled, 1 = CN4 enabled
    _CN5IE = 1;              // 0 = CN5 disabled, 1 = CN5 enabled
    _CN6IE = 1;              // 0 = CN6 disabled, 1 = CN6 enabled
    _CN7IE = 0;              // 0 = CN7 disabled, 1 = CN7 enabled
    _CN8IE = 0;              // 0 = CN8 disabled, 1 = CN8 enabled
    _CN9IE = 0;              // 0 = CN9 disabled, 1 = CN9 enabled
    _CN10IE = 0;             // 0 = CN10 disabled, 1 = CN10 enabled
    _CN11IE = 0;             // 0 = CN11 disabled, 1 = CN11 enabled
    _CN12IE = 0;             // 0 = CN12 disabled, 1 = CN12 enabled
    _CN13IE = 0;             // 0 = CN13 disabled, 1 = CN13 enabled
    _CN14IE = 0;             // 0 = CN14 disabled, 1 = CN14 enabled
    _CN15IE = 0;             // 0 = CN15 disabled, 1 = CN15 enabled
    
     // CNEN2 = 0x0000         // set the entire register (CN16 - CN21)
    
    _CN16IE = 0;             // 0 = CN16 disabled, 1 = CN16 enabled
    _CN17IE = 0;             // 0 = CN17 disabled, 1 = CN17 enabled
    _CN18IE = 0;             // 0 = CN18 disabled, 1 = CN18 enabled
    _CN19IE = 0;             // 0 = CN19 disabled, 1 = CN19 enabled
    _CN20IE = 0;             // 0 = CN20 disabled, 1 = CN20 enabled
    _CN21IE = 0;             // 0 = CN21 disabled, 1 = CN21 enabled
    
   
    // Input CHANGE Notification Interrupt section
     IFS1bits.CNIF = 0;     // reset CN interrupt flag
     IEC1bits.CNIE = 1;     // enable CN ISR
     IPC4bits.CNIP = 4;     // set CN interrupt priority to low
     
     
     
     
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

