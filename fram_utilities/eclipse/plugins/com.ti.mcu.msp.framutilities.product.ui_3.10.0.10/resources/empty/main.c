#include <msp430.h>

#include <ctpl.h>

/*
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // Stop watchdog timer

    return 0;
}
