
/* 
 * File:        PortInit.h
 * Author:      James Vlasblom
 * Date:        September 29, 2016
 * Comments:    This file contains the function 'PortInit()', 
 *              which contains all the code for all the PORT initialization
 *              including input / output, open drain, and initial value. PortB
 *              also has Analog / Digital designation.
 * 
 * Revision history: 1.0
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PORTINIT_50_Shot_H
#define	PORTINIT_50_Shot_H

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

void PortInit(void);

void PortInit(void) {

    //*** All PORTS are inputs at POR ***

    //************************
    //************************
    //* PortA initialization *
    //************************
    //************************

    //*** INPUT / OUTPUT Configuration***

    //TRISA = 0x00;      // Set entire PORT

    _TRISA0 = 0; // 0 = output, 1 = input 
    _TRISA1 = 0; // 0 = output, 1 = input
    _TRISA2 = 0; // 0 = output, 1 = input
    _TRISA3 = 0; // 0 = output, 1 = input 
    _TRISA4 = 0; // 0 = output, 1 = input
    _TRISA5 = 0; // 0 = output, 1 = input
    _TRISA6 = 0; // 0 = output, 1 = input
    _TRISA7 = 0; // 0 = output, 1 = input
    // NO _TRISA8
    _TRISA9 = 0; // 0 = output, 1 = input
    _TRISA10 = 0; // 0 = output, 1 = input
    // no _TRISA11
    // no _TRISA12
    // no _TRISA13
    _TRISA14 = 1; // 0 = output, 1 = input
    _TRISA15 = 1; // 0 = output, 1 = input

    // *** Open Drain Configuration ***

    //ODCA = 0x00;         // Set entire PORT

    _ODA0 = 0; // 0 = normal, 1 = open drain
    _ODA1 = 0; // 0 = normal, 1 = open drain
    _ODA2 = 0; // 0 = normal, 1 = open drain        
    _ODA3 = 0; // 0 = normal, 1 = open drain        
    _ODA4 = 0; // 0 = normal, 1 = open drain 
    _ODA5 = 0; // 0 = normal, 1 = open drain         
    _ODA6 = 0; // 0 = normal, 1 = open drain  
    _ODA7 = 0; // 0 = normal, 1 = open drain 
    //no ODA8
    _ODA9 = 0; // 0 = normal, 1 = open drain
    _ODA10 = 0; // 0 = normal, 1 = open drain
    // no _ODA11
    // no _ODA12
    // no _ODA13          // 0 = normal, 1 = open drain
    _ODA14 = 0; // 0 = normal, 1 = open drain
    _ODA15 = 0; // 0 = normal, 1 = open drain

    // *** Initial Port settings, entire of individual ***

    //LATTA = 0x8000;   // initial port settings

    _LATA0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA5 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA6 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RA8
    _LATA9 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATA10 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RA11
    // no _RA12
    // no _RA13
    //_LATA14 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATA15 = 0;       // 0 = low(0v), 1 = high(+3.3V)


    // ****************************
    // *** Redefine PORT MACROS ***
    // ****************************

    // *** OUTPUTS ***
#define RESET_LED _LATA0
#define ARM_LED LATAbits.LATA1
#define FIRE_LED LATAbits.LATA2    
#define SERIAL LATAbits.LATA3
#define SRCLK LATAbits.LATA4 // Clocks (enables) the output to relays via '245.
#define SR_LATCH LATAbits.LATA5  // RCLK on the 74HC595, clocks the SR
#define SR_CLEAR LATAbits.LATA6
#define ON_LED LATAbits.LATA7  
#define SM_LED LATAbits.LATA9  
#define GALV_OUT LATEbits.LATE0
    

    //*** INPUTS ***
#define ENCDR_SW PORTAbits.RA14
#define ENCDR_A PORTAbits.RA15

#define TOUCH PORTBbits.RB2
#define ARM_SW PORTBbits.RB3
#define FIRE_SW PORTBbits.RB4
#define AD_VOLTAGE PORTBbits.RB5

#define ON_SW PORTEbits.RE4

#define ENCDR_B PORTFbits.RF0
#define RESET_SW PORTFbits.RF6
    






    //***********************
    //***********************
    // PORTB initialization *
    //***********************
    //***********************

    //*** INPUT / OUTPUT Configuration***

    //TRISB = 0x00;      // Set entire PORT
    
    // Note: Must be set for analog inputs for A/D.

    _TRISB0 = 0; // 0 = output, 1 = input 
    _TRISB1 = 1; // 0 = output, 1 = input
    _TRISB2 = 1; // 0 = output, 1 = input
    _TRISB3 = 1; // 0 = output, 1 = input 
    _TRISB4 = 1; // 0 = output, 1 = input
    _TRISB5 = 1; // 0 = output, 1 = input
    _TRISB6 = 0; // 0 = output, 1 = input
    _TRISB7 = 0; // 0 = output, 1 = input
    _TRISB8 = 0; // 0 = output, 1 = input
    _TRISA9 = 0; // 0 = output, 1 = input
    _TRISA10 = 0; // 0 = output, 1 = input
    _TRISB11 = 0; // 0 = output, 1 = input
    _TRISB12 = 0; // 0 = output, 1 = input
    _TRISB13 = 0; // 0 = output, 1 = input
    _TRISB14 = 0; // 0 = output, 1 = input
    _TRISB15 = 0; // 0 = output, 1 = input

    // *** Open Drain Configuration ***

    //ODCB = 0x00;         // Set entire PORT

    _ODB0 = 0; // 0 = normal, 1 = open drain
    _ODB1 = 0; // 0 = normal, 1 = open drain
    _ODB2 = 0; // 0 = normal, 1 = open drain        
    _ODB3 = 0; // 0 = normal, 1 = open drain        
    _ODB4 = 0; // 0 = normal, 1 = open drain 
    _ODB5 = 0; // 0 = normal, 1 = open drain         
    _ODB6 = 0; // 0 = normal, 1 = open drain
    _ODB7 = 0; // 0 = normal, 1 = open drain  
    _ODB8 = 0; // 0 = normal, 1 = open drain 
    _ODB9 = 0; // 0 = normal, 1 = open drain 
    _ODB10 = 0; // 0 = normal, 1 = open drain 
    _ODB11 = 0; // 0 = normal, 1 = open drain 
    _ODB12 = 0; // 0 = normal, 1 = open drain 
    _ODB13 = 0; // 0 = normal, 1 = open drain 
    _ODB14 = 0; // 0 = normal, 1 = open drain 
    _ODB15 = 0; // 0 = normal, 1 = open drain 

    // *** Initial Port settings, entire of individual ***

    LATB = 0x00; // Set entire PORT

    //_LATB0 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB1 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB2 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB3 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB4 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB5 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB6 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB7 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB8 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB9 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    //_LATB10 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB11 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB12 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB13 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB14 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    //_LATB15 = 0;       // 0 = low(0v), 1 = high(+3.3V)


    // *** Setting Inputs as Analog or Digital (PORTB only) ***

    //AD1PCFG = 0xFF;     // set entire PORT digital

    _PCFG0 = 1; // 0 = analog, 1 = digital
    _PCFG1 = 0; // 0 = analog, 1 = digital
    _PCFG2 = 1; // 0 = analog, 1 = digital
    _PCFG3 = 1; // 0 = analog, 1 = digital
    _PCFG4 = 1; // 0 = analog, 1 = digital
    _PCFG5 = 0; // 0 = analog, 1 = digital
    _PCFG6 = 1; // 0 = analog, 1 = digital
    _PCFG7 = 1; // 0 = analog, 1 = digital
    _PCFG8 = 1; // 0 = analog, 1 = digital
    _PCFG9 = 1; // 0 = analog, 1 = digital
    _PCFG10 = 1; // 0 = analog, 1 = digital
    _PCFG11 = 1; // 0 = analog, 1 = digital
    _PCFG12 = 1; // 0 = analog, 1 = digital
    _PCFG13 = 1; // 0 = analog, 1 = digital
    _PCFG14 = 1; // 0 = analog, 1 = digital
    _PCFG15 = 1; // 0 = analog, 1 = digital




    //************************
    //************************
    //* PortC initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISC = 0x00;         // Set entire PORT

    // no_TRISC0 = 0
    _TRISC1 = 0; // 0 = output, 1 = input
    _TRISC2 = 0; // 0 = output, 1 = input
    _TRISC3 = 0; // 0 = output, 1 = input 
    _TRISC4 = 0; // 0 = output, 1 = input
    // no _TRISC5
    // no _TRISC6
    // no _TRISC7
    // no _TRISC8
    // no _TRISC9
    // no _TRISC10
    // no _TRISC11
    _TRISC12 = 0; // 0 = output, 1 = input
    _TRISC13 = 0; // 0 = output, 1 = input
    _TRISC14 = 0; // 0 = output, 1 = input
    _TRISC15 = 0; // 0 = output, 1 = input

    // *** Open Drain Configuration ***

    //ODCC = 0x00;         // Set entire PORT

    // no _ODC0
    _ODC1 = 0; // 0 = normal, 1 = open drain
    _ODC2 = 0; // 0 = normal, 1 = open drain        
    _ODC3 = 0; // 0 = normal, 1 = open drain        
    _ODC4 = 0; // 0 = normal, 1 = open drain 
    // no _ODC5    
    // no _ODC6 
    // no _ODC7 
    // no _ODC8
    // no _ODC9
    // no _ODC10
    // no _ODC11
    _ODC12 = 0; // 0 = normal, 1 = open drain
    _ODC13 = 0; // 0 = normal, 1 = open drain
    _ODC14 = 0; // 0 = normal, 1 = open drain
    _ODC15 = 0; // 0 = normal, 1 = open drain

    // *** Initial Port settings, entire of individual ***

    //LATTC = 0x6a;   // initial port settings

    // no _RC0
    _LATC1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RC5
    // no _RC6
    // no _RC7
    // no _RC8
    // no _RC9
    // no _RC10
    // no _RC11
    _LATC12 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC13 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC14 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATC15 = 0; // 0 = low(0v), 1 = high(+3.3V)



    //************************
    //************************
    //* PortD initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISD = 0x00;         // Set entire PORT

    _TRISD0 = 0; // 0 = output, 1 = input
    _TRISD1 = 0; // 0 = output, 1 = input
    _TRISD2 = 0; // 0 = output, 1 = input
    _TRISD3 = 0; // 0 = output, 1 = input 
    _TRISD4 = 0; // 0 = output, 1 = input
    _TRISD5 = 0; // 0 = output, 1 = input
    _TRISD6 = 0; // 0 = output, 1 = input
    _TRISD7 = 0; // 0 = output, 1 = input
    _TRISD8 = 0; // 0 = output, 1 = input
    _TRISD9 = 0; // 0 = output, 1 = input
    _TRISD10 = 0; // 0 = output, 1 = input
    _TRISD11 = 0; // 0 = output, 1 = input
    _TRISD12 = 0; // 0 = output, 1 = input
    _TRISD13 = 0; // 0 = output, 1 = input
    _TRISD14 = 0; // 0 = output, 1 = input
    _TRISD15 = 0; // 0 = output, 1 = input

    // *** Open Drain Configuration ***

    //ODCD = 0x00;         // Set entire PORT

    _ODD0 = 0; // 0 = normal, 1 = open drain
    _ODD1 = 0; // 0 = normal, 1 = open drain
    _ODD2 = 0; // 0 = normal, 1 = open drain        
    _ODD3 = 0; // 0 = normal, 1 = open drain        
    _ODD4 = 0; // 0 = normal, 1 = open drain 
    _ODD5 = 0; // 0 = normal, 1 = open drain  
    _ODD6 = 0; // 0 = normal, 1 = open drain  
    _ODD7 = 0; // 0 = normal, 1 = open drain   
    _ODD8 = 0; // 0 = normal, 1 = open drain  
    _ODD9 = 0; // 0 = normal, 1 = open drain  
    _ODD10 = 0; // 0 = normal, 1 = open drain  
    _ODD11 = 0; // 0 = normal, 1 = open drain  
    _ODD12 = 0; // 0 = normal, 1 = open drain
    _ODD13 = 0; // 0 = normal, 1 = open drain
    _ODD14 = 0; // 0 = normal, 1 = open drain
    _ODD15 = 0; // 0 = normal, 1 = open drain

    // *** Initial Port settings, entire of individual ***

    //LATD = 0x00;   // initial port settings

    _LATD0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD5 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD6 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD8 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD9 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD10 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD11 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD12 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD13 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD14 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATD15 = 0; // 0 = low(0v), 1 = high(+3.3V)


    //************************
    //************************
    //* PortE initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISE = 0x00;         // Set entire PORT

    _TRISE0 = 0; // 0 = output, 1 = input
    _TRISE1 = 0; // 0 = output, 1 = input
    _TRISE2 = 0; // 0 = output, 1 = input
    _TRISE3 = 0; // 0 = output, 1 = input 
    _TRISE4 = 0; // 0 = output, 1 = input
    _TRISE5 = 0; // 0 = output, 1 = input
    _TRISE6 = 0; // 0 = output, 1 = input
    _TRISE7 = 0; // 0 = output, 1 = input
    _TRISE8 = 1; // 0 = output, 1 = input
    _TRISE9 = 0; // 0 = output, 1 = input
    //no _TRISE10
    //no _TRISE11
    //no _TRISE12
    //no _TRISE13
    //no _TRISE14
    //no _TRISE15

    // *** Open Drain Configuration ***

    //ODCE = 0x00;         // Set entire PORT

    _ODE0 = 0; // 0 = normal, 1 = open drain
    _ODE1 = 0; // 0 = normal, 1 = open drain
    _ODE2 = 0; // 0 = normal, 1 = open drain        
    _ODE3 = 0; // 0 = normal, 1 = open drain        
    _ODE4 = 0; // 0 = normal, 1 = open drain 
    _ODE5 = 0; // 0 = normal, 1 = open drain  
    _ODE6 = 0; // 0 = normal, 1 = open drain  
    _ODE7 = 0; // 0 = normal, 1 = open drain   
    _ODE8 = 0; // 0 = normal, 1 = open drain  
    _ODE9 = 0; // 0 = normal, 1 = open drain  
    //no _ODE10 
    //no _ODE11
    //no _ODE12
    //no _ODE13
    //no _ODE14
    //no _ODE15

    // *** Initial Port settings, entire of individual ***

    //LATE = 0x00;   // initial port settings

    _LATE0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE5 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE6 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATE7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    //_LATE8 = 0;       // 0 = low(0v), 1 = high(+3.3V)
    _LATE9 = 0; // 0 = low(0v), 1 = high(+3.3V)
    //no _RE10 = 0
    //no _RE11 = 0
    //no _RE12 = 0
    //no _RE13 = 0
    //no _RE14 = 0
    //no _RE15 = 0



    //************************
    //************************
    //* PortF initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISF = 0x00;         // Set entire PORT

    _TRISF0 = 1; // 0 = output, 1 = input
    _TRISF1 = 0; // 0 = output, 1 = input
    _TRISF2 = 0; // 0 = output, 1 = input
    _TRISF3 = 0; // 0 = output, 1 = input 
    _TRISF4 = 0; // 0 = output, 1 = input
    _TRISF5 = 0; // 0 = output, 1 = input
    _TRISF6 = 1; // 0 = output, 1 = input
    _TRISF7 = 1; // 0 = output, 1 = input
    _TRISF8 = 0; // 0 = output, 1 = input
    // no _TRISF9
    // no _TRISF10
    // no _TRISF11
    _TRISF12 = 0; // 0 = output, 1 = input
    _TRISF13 = 0; // 0 = output, 1 = input
    // no _TRISF14
    // no _TRISF15

    // *** Open Drain Configuration ***

    //ODCF = 0x00;         // Set entire PORT

    _ODF0 = 0; // 0 = normal, 1 = open drain
    _ODF1 = 0; // 0 = normal, 1 = open drain
    _ODF2 = 0; // 0 = normal, 1 = open drain        
    _ODF3 = 0; // 0 = normal, 1 = open drain        
    _ODF4 = 0; // 0 = normal, 1 = open drain 
    _ODF5 = 0; // 0 = normal, 1 = open drain  
    _ODF6 = 0; // 0 = normal, 1 = open drain  
    _ODF7 = 0; // 0 = normal, 1 = open drain   
    _ODF8 = 0; // 0 = normal, 1 = open drain  
    // no _ODF9 
    // no _ODF10
    // no _ODF11 
    _ODF12 = 0; // 0 = normal, 1 = open drain
    _ODF13 = 0; // 0 = normal, 1 = open drain
    // no _ODF14
    // no _ODF15

    // *** Initial Port settings, entire of individual ***

    //LATF = 0x00;   // initial port settings

    //_LATF0 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    _LATF1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATF2 = 1; // 0 = low(0v), 1 = high(+3.3V)
    _LATF3 = 1; // 0 = low(0v), 1 = high(+3.3V)
    _LATF4 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATF5 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATF6 = 0;        // 0 = low(0v), 1 = high(+3.3V)
    _LATF7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATF8 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RF9
    // no _RF10
    // no _RF11
    _LATF12 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATF13 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RF14
    // no _RF15

    // ****************************
    // *** Redefine PORT MACROS ***
    // ****************************

    // *** OUTPUTS ***
    #define SHIFT_REG_EN LATFbits.LATF3

    //************************
    //************************
    //* PortG initialization *
    //************************
    //************************


    //*** INPUT / OUTPUT Configuration***

    //TRISG = 0x00;         // Set entire PORT

    _TRISG0 = 0; // 0 = output, 1 = input
    _TRISG1 = 0; // 0 = output, 1 = input
    _TRISG2 = 0; // 0 = output, 1 = input
    _TRISG3 = 0; // 0 = output, 1 = input 
    // no _TRISG4
    // no _TRISG5
    _TRISG6 = 0; // 0 = output, 1 = input
    _TRISG7 = 0; // 0 = output, 1 = input
    _TRISG8 = 0; // 0 = output, 1 = input
    _TRISG9 = 0; // 0 = output, 1 = input
    // no _TRISG10
    // no _TRISG11
    _TRISG12 = 0; // 0 = output, 1 = input
    _TRISG13 = 0; // 0 = output, 1 = input
    _TRISG14 = 0; // 0 = output, 1 = input
    _TRISG15 = 0; // 0 = output, 1 = input

    // *** Open Drain Configuration ***

    //ODCG = 0x00;         // Set entire PORT

    _ODG0 = 0; // 0 = normal, 1 = open drain
    _ODG1 = 0; // 0 = normal, 1 = open drain
    _ODG2 = 0; // 0 = normal, 1 = open drain        
    _ODG3 = 0; // 0 = normal, 1 = open drain        
    // no _ODG4
    // no _ODG5
    _ODG6 = 0; // 0 = normal, 1 = open drain  
    _ODG7 = 0; // 0 = normal, 1 = open drain   
    _ODG8 = 0; // 0 = normal, 1 = open drain  
    _ODG9 = 0; // 0 = normal, 1 = open drain  
    // no _ODG10 
    // no _ODG11
    _ODG12 = 0; // 0 = normal, 1 = open drain
    _ODG13 = 0; // 0 = normal, 1 = open drain
    _ODG14 = 0; // 0 = normal, 1 = open drain
    _ODG15 = 0; // 0 = normal, 1 = open drain

    // *** Initial Port settings, entire of individual ***

    //LATG = 0x00;   // initial port settings

    _LATG0 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG1 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG2 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG3 = 0; // 0 = low(0v), 1 = high(+3.3V)
    // no _RG4
    // no _RG5
    _LATG6 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG7 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG8 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG9 = 1; // 0 = low(0v), 1 = high(+3.3V)
    // no _RG10
    // no _RG11
    _LATG12 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG13 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG14 = 0; // 0 = low(0v), 1 = high(+3.3V)
    _LATG15 = 0; // 0 = low(0v), 1 = high(+3.3V)
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

