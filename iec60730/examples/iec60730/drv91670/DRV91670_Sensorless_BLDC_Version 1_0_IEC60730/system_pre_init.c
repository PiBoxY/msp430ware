/*******************************************************************************
 *
 *  system_pre_init.c - system_pre_init.c
 *
 ******************************************************************************/
 
/*
 * The function _system_pre_init it called by the start-up code before
 * "main" is called, and before data segment initialization is
 * performed.
 *
 * This is a template file, modify to perform any initialization that
 * should take place early.
 *
 * The return value of this function controls if data segment
 * initialization should take place. If 0 is returned, it is bypassed.
 *
 * For the MSP430 microcontroller family, please consider disabling
 * the watchdog timer here, as it could time-out during the data
 * segment initialization.
 */
 
#include "DRV91670.h"

int _system_pre_init(void)
{
  /* Insert your low-level initializations here */

  /* Disable Watchdog timer to prevent reset during */
  /* long variable initialization sequences. */
	WDTCTL = WDTPW + WDTHOLD;
  
  /*==================================*/
  /* Choose if segment initialization */
  /* should be done or not.           */
  /* Return: 0 to omit initialization */
  /* 1 to run initialization          */
  /*==================================*/
  return 1;
}
