

/* 
 * File:             ADCInit.h
 * Author:           James Vlasblom
 * Date:             Sept 28, 2016
 * Revision history: 1.0
 * 
 * Comments: This file contains the initialization code for the ADC. The ADC
 *           is enabled, however, in main().
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADCINIT_H
#define	ADCINIT_H

#include <xc.h> // include processor files - each processor file is guarded.  

void ADCInit(void);

void ADCInit(void) {
    
    // ***************************************
    // *** AD1CON1: A/D Control Register 1 ***
    // ***************************************
    
    
    //AD1CON1 = 0x0000;      // Set entire register.
    
    _ADON = 1;          // 1 = A/D Converter module is operating.
                        // 0 = A/D converter is disabled.
    
    _ADSIDL = 0;        // 0 = Continue operation in idle mode.
                        // 1 = Discontinue module operation in idle mode.
    
    _FORM = 0b00;        // Data output format: 00 = Integer, 01 = Signed Integer
                        // 10 = Fractional, 11 = Signed fractional.
    
    _SSRC = 0x7;       // Conversion Trigger Source Select, 
                        // 000 = Clearing SAMP bit (full program control,    
                        // 001 = Active transition on INT0 pin.
                        // 010 = Timer3 compare match.
                        // 011 = Timer5 compare match.
                        // 100 = CTMU event.
                        // 101 = Reserved.
                        // 110 = CTMU event.
                        // 111 = Internal counter (auto-convert).
    
    _ASAM = 1;          // 0 = Sampling begins when SAMP bit is set.
                        // 1 = Sampling begins immediately after last conversion
                        // completes.
    
    _SAMP = 0;          // 0 = A/D S/H amplifier is holding.
                        // 1 = A/dD S/H amplifier is sampling.
    
    _DONE = 0;          // 0 = A/D conversion is not done or has not started.
                        // 1 = A/D conversion is done.
    
   
    // ***************************************
    // *** AD1CON2: A/D Control Register 2 ***
    // ***************************************
    
    //AD1CON2 = 0x0000;      // Set entire register.
    
    _VCFG = 0x0;        // Voltage reference config bits.
                        // 000 = VR+ = AVdd, -VR = AVss.
                        // 001 = VR+ = external Vref+ pin, -VR = AVss.
                        // 010 = VR+ = AVdd, -VR = external Vref- pin.
                        // 011 = VR+ = external Vref+ pin, -VR = external Vref- pin.
                        // 1xx = VR+ = AVdd, -VR = AVss.
    
    _CSCNA = 1;         // 0 = Use channel selected by CH0SA bits as the MUX A inputs.
                        // 1 = Scan inputs selected by the AD1SSL register as
                        // the MUX A inputs.
    
    _BUFS = 0;          // 0 = A/D currently filling ADCBUF0-7, user use ADCBUF8-F.
                        // 1 = A/D currently filling ADCBUF8-F, user use ADCBUF0-7.
    
    _SMPI = 0x1;        // Sample / Convert Sequences per interrupt selection bit
                        // 0000 = interrupts at the completion of conversion for
                        //        each sample convert sequence.
                        // .......
                        // 1111 = interrupts at the completion of conversion for
                        //        16th sample convert sequence.
    
    _BUFM = 0;          // 0 = Buffer configured as one 16-word buffer.
                        // 1 = Buffer configured as two 8-word buffers.
    
    _ALTS = 0;          // 0 = Always use MUX A input multiplexers settings.
                        // 1 = Alternate between MUX A and MUX B input multi-
                        // plexers settings on successive conversions.
    
    
    // ***************************************
    // *** AD1CON3: A/D Control Register 3 ***
    // ***************************************
    
    
    //AD1CON3 = 0x0F00;      // Set entire register.
    
    _ADRC = 0;        // A/D conversion clock source
                        // 0 = clock derived from system clock.
                        // 1= =A/D internal RC clock.
    
    _SAMC = 0b11111;       // Auto_Sample time bits.
                        // 00000 = 0 TAD (not recommended).
                        // ........
                        // 11111 = 31 TAD .
    
    _ADCS = 0x00;       // A/D conversion clock period select bits.
                        // 00000000 = TCY.
                        // 00000001 = 2 * TCY.
                        // .....
                        // 00111111 = 64 * TCY
                        // 01000000
                        // ......    reserved.
                        // 11111111
    
    
    // *************************************************
    // *** AD1CHS: A/D Input Channel Select Register ***
    // *************************************************
    
    
    //AD1CHS = 0x0000;        //Set entire register.
    
    _CH0NB = 0;         // 0 = Negative input is VR-.
                        // 1 = Negative input is AN0.
    
    _CH0SB = 0x00;      // S/H amplifier positive input select for MUX B.
                        // 00000 = Positive input is AN0.
                        // .........
                        // 01111 = Positive input is AN15.
    
    _CH0NA = 0x0;       // 0 = Negative input is VR-.
                        // 1 = Negative input is AN1.
    
    _CH0SA = 0;         // Implemented same as those for CH0SB.
    
    
    // *******************************************************
    // *** AD1PCFG(L): A/D Port Configuration Low Register ***
    // *******************************************************
    
    // AD1PCFG = 0x00;    
    // Note: Already configured in PortInit(), file PortInit.h, in the PORTB
    // section.
    
       
    // *********************************************************
    // *** AD1CSSL: Input Scan Select Low Register for MUX A ***
    // *********************************************************
    
    
    // AD1CSSL = 0x00;      // Set entire register.
    
    _CSSL0 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN0, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL1 = 1;             // A/D input channel scan selection bits 
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN1, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL2 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN2, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL3 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN3, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL4 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN4, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL5 = 1;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN5, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL6 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN6, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL7 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN7, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL8 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN8, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL9 = 0;             // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN9, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL10 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN10, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL11 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN111, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL12 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN12, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL13 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN13, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL14 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN14, is 
                            //     selected for sequential scanning on MUX A.
    
    _CSSL15 = 0;            // A/D input channel scan selection bits
                            // 0 = Corresponding analog channel ignored in
                            //     sequential scanning.
                            // 1 = Corresponding analog channel, AN15, is 
                            //     selected for sequential scanning on MUX A.
    
    
    
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

