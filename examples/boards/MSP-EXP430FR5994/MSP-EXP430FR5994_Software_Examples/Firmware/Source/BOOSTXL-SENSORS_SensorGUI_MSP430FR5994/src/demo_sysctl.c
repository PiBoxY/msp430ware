
#include <stdbool.h>
#include <stdint.h>
#include "driverlib.h"
#include "demo_sysctl.h"

//*****************************************************************************
//
//! Provides a small delay.
//!
//! \param ui32Count is the number of delay loop iterations to perform.
//!
//! This function provides a means of generating a delay by executing a simple
//! 3 instruction cycle loop a given number of times.  It is written in
//! assembly to keep the loop instruction count consistent across tool chains.
//!
//! It is important to note that this function does NOT provide an accurate
//! timing mechanism.  Although the delay loop is 3 instruction cycles long,
//! the execution time of the loop will vary dramatically depending upon the
//! application's interrupt environment (the loop will be interrupted unless
//! run with interrupts disabled and this is generally an unwise thing to do)
//! and also the current system clock rate and flash timings (wait states and
//! the operation of the prefetch buffer affect the timing).
//!
//! For better accuracy, the ROM version of this function may be used.  This
//! version will not suffer from flash- and prefect buffer-related timing
//! variability but will still be delayed by interrupt service routines.
//!
//! For best accuracy, a system timer should be used with code either polling
//! for a particular timer value being exceeded or processing the timer
//! interrupt to determine when a particular time period has elapsed.
//!
//! \return None.
//
//*****************************************************************************

//
// For CCS implement this function in pure assembly.  This prevents the TI
// compiler from doing funny things with the optimizer.
//
#if 0 // defined(__TI_COMPILER_VERSION__)
__asm("    .sect \".text:SysCtlDelay\"\n"
      "    .clink\n"
      "    .global SysCtlDelay\n"
      "SysCtlDelay:\n"
	  "    sub.w     #1,r12\n"
	  "    cmp.w	 #0,r12\n"
	  "    jne       SysCtlDelay\n"
      "    reta\n");
#endif

#if defined(__TI_COMPILER_VERSION__)
__asm("    .sect \".text:SysCtlDelay\"\n"
      "    .clink\n"
      "    .global SysCtlDelay\n"
      "SysCtlDelay:\n"
	  "    sub.w     #1,r12\n"
	  "    subc.w    #0,r13\n"
	  "    jne       SysCtlDelay\n"
	  "    tst.w     r12\n"
	  "    jne       SysCtlDelay\n"
      "    reta\n");
#endif

// #define DelayMs(ulClockMS) {}
extern uint32_t MCLKValue;
void DelayMs (uint32_t ui32ClockMS)
{
//	uint32_t ui32LocCount;

	if (ui32ClockMS != 0)
	{
		SysCtlDelay((ui32ClockMS)*(MCLKValue / (4 * 1000)));
	}
}

