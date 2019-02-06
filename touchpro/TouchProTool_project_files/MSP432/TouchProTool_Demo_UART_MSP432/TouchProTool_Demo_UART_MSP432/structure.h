//******************************************************************************
// structure.h
//
//! \brief This file contains both application dependent and independent pieces.  The
//!        dependent piece must be updated along with structure.c, and carries the
//!        naming conventions found in structure.c to the rest of the library.
//!
//******************************************************************************

//******************************************************************************
// The following elements need to be configured by the user.
//******************************************************************************
#ifndef CTS_STRUCTURE_H_
#define CTS_STRUCTURE_H_


#include "msp.h"
#include <stdint.h>

/* Public Globals */
extern const struct Element volume_down;   // Element of slider wheel
extern const struct Element volume_up;     // Element of slider wheel
extern const struct Element right;         // Element of slider wheel
extern const struct Element left;          // Element of slider wheel

extern const struct Sensor wheel;          // Struct that contains element references and sensor configuration info

//****** RAM ALLOCATION ********************************************************
// TOTAL_NUMBER_OF_ELEMENTS defines the total number of elements used, even if
// they are going to be segmented into seperate groups.
// If the RAM_FOR_FLASH definition is removed, then the appropriate HEAP size 
// must be allocated. 2 bytes * MAXIMUM_NUMBER_OF_ELEMENTS_PER_SENSOR + 2 bytes
// of overhead.
#define TOTAL_NUMBER_OF_ELEMENTS 4
#define RAM_FOR_FLASH

//****** Structure Array Definition ********************************************
// This defines the array size in the sensor strucure.  In the event that 
// RAM_FOR_FLASH is defined, then this also defines the amount of RAM space
// allocated (global variable) for computations.
#define MAXIMUM_NUMBER_OF_ELEMENTS_PER_SENSOR  4

//****** Choosing a  Measurement Method ****************************************
// These variables are references to the definitions found in structure.c and
// must be generated per the application.
// possible values for the method field

// MSP432 OSCILLATOR DEFINITIONS
#define RO_CTIO_TA2_WDTA            73
//#define RO_CTIO_TA2_TA3           74

// MSP432 FAST RO DEFINITIONS
//#define fRO_CTIO_TA2_TA3          23


//****** WHEEL and SLIDER ******************************************************
// Are wheel or slider representations used?
//#define SLIDER
#define ILLEGAL_SLIDER_WHEEL_POSITION		0xFFFF
#define WHEEL

//******************************************************************************
// End of user configuration section.
//******************************************************************************
//******************************************************************************
//******************************************************************************

//possible timer source clock dividers, different from clock module dividers
#define TIMER_TxCLK 	   TASSEL__TACLK
#define TIMER_ACLK  	   TASSEL__ACLK
#define TIMER_SMCLK 	   TASSEL__SMCLK
#define TIMER_INCLK 	   TASSEL__INCLK

#define TIMER_SOURCE_DIV_0     ID_0		// Timer A Source/1
#define TIMER_SOURCE_DIV_1     ID_1     // Timer A Source/2
#define TIMER_SOURCE_DIV_2     ID_2     // Timer A Source/4
#define TIMER_SOURCE_DIV_3     ID_3     // Timer A Source/8

#define GATE_WDTA_SMCLK    WDTSSEL__SMCLK
#define GATE_WDTA_ACLK     WDTSSEL__ACLK
#define GATE_WDTA_VLO      WDTSSEL__VLOCLK
#define GATE_WDTA_BCLK     WDTSSEL__BCLK

#define WDTA_GATE_2G       WDTIS_0  // watchdog source/2G
#define WDTA_GATE_128M     WDTIS_1  // watchdog source/128M
#define WDTA_GATE_8192K    WDTIS_2  // watchdog source/8192K
#define WDTA_GATE_512K     WDTIS_3  // watchdog source/512K
#define WDTA_GATE_32768    WDTIS_4  // watchdog source/32768
#define WDTA_GATE_8192     WDTIS_5  // watchdog source/8192
#define WDTA_GATE_512      WDTIS_6  // watchdog source/512
#define WDTA_GATE_64       WDTIS_7  // watchdog source/64


// The below variables are used to excluded portions of code not needed by
// the method chosen by the user. Uncomment the type used prior to compilation.
// Multiple types can be chosen as needed.
// What Method(s) are used in this application?

#ifdef RO_CTIO_TA2_WDTA
    #define RO_TYPE
    #define WDT_GATE
    #define HAL_DEFINITION
#endif

#ifdef RO_CTIO_TA2_TA3
    #define RO_TYPE
    #define TIMER_SCALE
    #define TIMERA3_0GATE
    #define HAL_DEFINITION
#endif

#ifdef fRO_CTIO_TA2_TA3
    #define RO_TYPE
    #define TIMER_SCALE
    #define TIMERA2_0GATE
    #define HAL_DEFINITION
#endif


#ifdef SLIDER
	#define SLIDER_WHEEL
#endif

#ifdef WHEEL
	#define SLIDER_WHEEL
#endif

#define RO_MASK         0xC0        // 1100 0000

/*
 *  The element structure identifies port input definitions for
 *  each element.
 */
struct Element{
 		     
  uint16_t inputBits;                 // FastRO+RO: bit
                                      // definition
                                      
  uint16_t threshold;                   // specific threshold for each button
  uint16_t maxResponse;                 // Special Case: Slider max counts
};

/*
 *  The sensor structure identifies HAL and timing definitions for
 *  each sensor.
 */

struct Sensor{
  // the method acts as the switch to determine which HAL is called
  uint8_t halDefinition;           // RO
                                   // FAST_SCAN_RO

/*
 *  This register address definition is needed to indicate which CTIOxCTL
 *  register is associated with the Timer identified in the HAL.
 */
  uint16_t *inputCaptioctlRegister;

  uint8_t numElements;             // number of elements within group
  uint8_t baseOffset;              // the offset within the global 
                                   // base_cnt array

  struct Element const *arrayPtr[MAXIMUM_NUMBER_OF_ELEMENTS_PER_SENSOR];    
                                   // an array of pointers

//*****************************************************************************
// Timer definitions
//  The basic premise is to count a number of clock cycles within a time
//  period, where either the clock source or the timer period is a function
//  of the element capacitance.
//
// RO Method:
//          Period: accumulationCycles*measGateSource/sourceScale 
//                  (with WDT sourceScale = 1, accumulationCycles is WDT control
//                   register settings)
//
//          clock source: relaxation oscillator where freq is a function of C
//
// fRO Method:
//          Period: accumulationCycles * 1/freq, freq is a function of C
//
//          clock source: measGateSource/sourceScale
     
  uint16_t measGateSource;         // FastRO: measurement timer source,
                                   // {ACLK, TACLK, SMCLK}
  	  	  	  	  	  	  	  	   // RO: gate timer source,
                                   // {ACLK, TACLK, SMCLK}
#ifdef TIMER_SCALE
  uint16_t sourceScale;            // RO: gate timer,
                                   // TA, scale: 1,1/2,1/4,1/8
                                   // FastRO: measurement timer, TA
  	  	  	  	  	  	  	  	   // scale:
                                   // Not used for WDTA
#endif
    
  uint16_t accumulationCycles;
  
//*****************************************************************************
// Other definitions

#ifdef SLIDER_WHEEL  
  uint8_t points;                   // Special Case: Number of points
                                    // along slider or wheel  
  uint8_t sensorThreshold;
#endif                               

};

/*
 *  The GCC language extension within CCS is needed, otherwise a warning will
 *  be generated during compilation when no problems exist or an error will be
 *  generated (instead of a warning) when a problem does exist.
 */
#ifndef TOTAL_NUMBER_OF_ELEMENTS
 #warning "WARNING: TOTAL_NUMBER_OF_ELEMENTS is not defined in structure.h. Only TI_CAPT_RAW function is enabled."
#endif

#ifndef RAM_FOR_FLASH
 #warning "WARNING: The HEAP must be set appropriately.  Please refer to SLAA490 for details."
#endif

#ifndef HAL_DEFINITION
 #warning "WARNING: At least one HAL definition must be made in structure.h."
#endif

#endif /* CTS_STRUCTURE_H_ */
