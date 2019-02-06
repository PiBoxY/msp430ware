/*
 * interrupts.c
 *
 * A LIST OF INTERRUPT VECTORS FOR THIS MSP430 FAMILY
 * THIS PREVENTS THE COMPILER FROM COMPLAINING ABOUT MISSING HANDLERS
 * CHANGE "#if 1" TO "#if 0" FOR HANDLERS DECLARED WITHIN YOUR CODE
 *
 */
#include "msp430.h"

#if defined (__MSP430G2553__)
#if 1
#pragma vector=PORT1_VECTOR
__interrupt void port1_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=PORT2_VECTOR
__interrupt void port2_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer0_a1_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER1_A1_VECTOR
__interrupt void timer1_a1_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer1_a0_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=ADC10_VECTOR
__interrupt void adc10_interrupt(void)
{
	_no_operation();
}
#endif

/********************************************/
#if 1
#pragma vector=USCIAB0TX_VECTOR
__interrupt void usciab0_tx_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=USCIAB0RX_VECTOR
__interrupt void usciab0_rx_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 0
// DISABLE FOR MSP430G2553
// ENABLE FOR MSP430G2452
#pragma vector=USI_VECTOR
__interrupt void usi_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 0
// DECLARED IN THE TOUCH TOOL DEMO
#pragma vector=WDT_VECTOR
__interrupt void watchDog_interrupt(void)
{
	_no_operation();
}
#endif

/********************************************/
#if 1
#pragma vector=COMPARATORA_VECTOR
__interrupt void comp_a_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=NMI_VECTOR
__interrupt void nmi_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/


#elif defined (__MSP430G2452__)

#if 1
#pragma vector=PORT1_VECTOR
__interrupt void port1_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer0_a1_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=PORT2_VECTOR
__interrupt void port2_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer0_a0_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=ADC10_VECTOR
__interrupt void adc10_interrupt(void)
{
	_no_operation();
}
#endif

/********************************************/
#if 1
#pragma vector=USI_VECTOR
__interrupt void usi_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 0
// DECLARED IN THE TOUCH TOOL DEMO
#pragma vector=WDT_VECTOR
__interrupt void watchDog_interrupt(void)
{
	_no_operation();
}
#endif

/********************************************/
#if 1
#pragma vector=COMPARATORA_VECTOR
__interrupt void comp_a_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#if 1
#pragma vector=NMI_VECTOR
__interrupt void nmi_interrupt(void)
{
	_no_operation();
}
#endif
/********************************************/
#else
#error "DEVICE NOT SUPPORTED"
#endif
