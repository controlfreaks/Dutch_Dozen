
/*  
 * File:                    Misc_Macro.h
 * Author:                  James Vlasblom
 * Date:                    September 29, 2016
 * Revision history:        1.0
 * 
 * Comments:                This file contains all the flags which are in
 *                          a structure format. There are 32 flags in two 
 *                          structures of 16.
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MISC_MACRO_H
#define	MISC_MACRO_H

#include <xc.h> // include processor files - each processor file is guarded.

// *** Definitions start here ***

//*** Structures Declared here ***
  struct tagFLAGS_A{       // FLAG structure for setting up flags
    unsigned FLG0:1;
    unsigned FLG1:1;
    unsigned FLG2:1;
    unsigned FLG3:1;
    unsigned FLG4:1;
    unsigned FLG5:1;
    unsigned FLG6:1;
    unsigned FLG7:1;
    unsigned FLG8:1;
    unsigned FLG9:1;
    unsigned FLG10:1;
    unsigned FLG11:1;
    unsigned FLG12:1;
    unsigned FLG13:1;
    unsigned FLG14:1;
    unsigned FLG15:1;
  }FLAGBITS_A = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
#define FIRE_FLG FLAGBITS_A.FLG0  // 1 when in a firing condtion, 0 when not. 
#define FIRE_HIGH_FLG FLAGBITS_A.FLG1
#define FIRE_FIRST_FLG FLAGBITS_A.FLG2 // denotes the first time in a firing condition
#define ARM_FLG FLAGBITS_A.FLG3
#define ARM_ALERT_FLG  FLAGBITS_A.FLG4
#define RESET_FLG  FLAGBITS_A.FLG5
#define SM_FLG  FLAGBITS_A.FLG6    // 0 = single mode, 1 =  multi mode
#define VER_FLG  FLAGBITS_A.FLG7  //  1 = first time through, noticed at int0 routine.
#define MULTI_FIRST_FLG  FLAGBITS_A.FLG8
#define CLOCK_FLG  FLAGBITS_A.FLG9
#define ENCDR_FLG  FLAGBITS_A.FLG10 // 
#define UNLATCH_FLG  FLAGBITS_A.FLG11
#define GALV_FLG  FLAGBITS_A.FLG12    // 0 = GALV off, 1 = GALV on.
#define ARM_GLV_FLG  FLAGBITS_A.FLG13 // 0 = no ARM/GALV conflict, 1 = there is.
#define MENU_FLG  FLAGBITS_A.FLG14    // 0 = not in MENU, 1 = in MENU (no fire)
#define CHG_COM_FLG  FLAGBITS_A.FLG15 // Charge complete.
  
   struct tagFLAGS_B{       // FLAG structure for setting up flags
    unsigned FLG16:1;
    unsigned FLG17:1;
    unsigned FLG18:1;
    unsigned FLG19:1;
    unsigned FLG20:1;
    unsigned FLG21:1;
    unsigned FLG22:1;
    unsigned FLG23:1;
    unsigned FLG24:1;
    unsigned FLG25:1;
    unsigned FLG26:1;
    unsigned FLG27:1;
    unsigned FLG28:1;
    unsigned FLG29:1;
    unsigned FLG30:1;
    unsigned FLG31:1;
  }FLAGBITS_B = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
#define LOW_VOLT_FIRST_FLG FLAGBITS_B.FLG16 //Flag is set when measured voltage > 10.
#define BATT_SETTLE_FLG FLAGBITS_B.FLG17  // Set when charging voltage reached.
#define FLAG18_FLG FLAGBITS_B.FLG18
#define FLAG19_FLG FLAGBITS_B.FLG19
#define FLAG20_FLG  FLAGBITS_B.FLG20
#define FLAG21_FLG  FLAGBITS_B.FLG21
#define FLAG22_FLG  FLAGBITS_B.FLG22    //
#define FLAG23_FLG  FLAGBITS_B.FLG23    //  
#define FLAG24_FLG  FLAGBITS_B.FLG24
#define FLAG25_FLG  FLAGBITS_B.FLG25
#define FLAG26_FLG  FLAGBITS_B.FLG26
#define FLAG27_FLG  FLAGBITS_B.FLG27
#define FLAG28_FLG  FLAGBITS_B.FLG28    // 
#define FLAG29_FLG  FLAGBITS_B.FLG29    //
#define FLAG30_FLG  FLAGBITS_B.FLG30    // 
#define FLAG31_FLG  FLAGBITS_B.FLG31 
  

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MISC_MACRO_H */

