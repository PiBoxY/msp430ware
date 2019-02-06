#include "msp430g2553.h"
#include "IIC.h"

#define NUM_SPEED	16


/*************** Function declaration ******************/
// Return ADC value from A4
unsigned int get_ADC_value(void);

// Set motor rotating direction
// Value "direction": DIR_STOP, DIR_CW, DIR_CCW
void set_motor_direction(unsigned char direction);

// Set motor speed
// Value "speed": 0 to (NUM_SPEED - 1)
void set_motor_by_VR(void);
void set_motor_speed(unsigned char speed);

/*************** Variable declaration ******************/
// increment when current_direction = DIR_CW
// decrement when current_direction = DIR_CCW
// no change when current_direction = DIR_STOP

#define DIR_STOP	0
#define DIR_CW		1
#define DIR_CCW		2

// For speed calculation based on ADC value
volatile unsigned int current_speed;
volatile unsigned char current_direction = DIR_STOP;

// Define revolution per second for each stage
#define RPS_S1		1
#define RPS_S2		4
#define RPS_S3		7
#define RPS_S4		11
#define RPS_S5		14
#define RPS_S6		17
#define RPS_S7		21
#define RPS_S8		24
#define RPS_S9		27
#define RPS_S10		31
#define RPS_S11		35
#define RPS_S12		40
#define RPS_S13		45
#define RPS_S14		50
#define RPS_S15		55	// 3000rpm

#define SLOW_SPEED	3

// SMCLK = 1MHz / 4 = 250KHz

// PWM frequency = 250KHz / 1024 ~240 Hz
#define PWM_CCR0 1024

// For calculating revolution per second (RPS)
// Timer A CLK = 250KHz / 8 = 31250
#define RPS_TIMER_A_FREQ	31250
const unsigned int RPS_TimerA_Table[NUM_SPEED] = 
{
	0,									// Stop
	RPS_TIMER_A_FREQ / RPS_S1,
	RPS_TIMER_A_FREQ / RPS_S2,
	RPS_TIMER_A_FREQ / RPS_S3,
	RPS_TIMER_A_FREQ / RPS_S4,
	RPS_TIMER_A_FREQ / RPS_S5,
	RPS_TIMER_A_FREQ / RPS_S6,
	RPS_TIMER_A_FREQ / RPS_S7,
	RPS_TIMER_A_FREQ / RPS_S8,
	RPS_TIMER_A_FREQ / RPS_S9,
	RPS_TIMER_A_FREQ / RPS_S10,
	RPS_TIMER_A_FREQ / RPS_S11,
	RPS_TIMER_A_FREQ / RPS_S12,
	RPS_TIMER_A_FREQ / RPS_S13,
	RPS_TIMER_A_FREQ / RPS_S14,
	RPS_TIMER_A_FREQ / RPS_S15,
//	RPS_TIMER_A_FREQ / RPS_S16,
//	RPS_TIMER_A_FREQ / RPS_S17,
};

volatile int acc_error;
volatile char ignore_first_error = 0;

/*************** Start of main ******************/
void main(void)
{
	unsigned char i, j;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	// Initialization
	if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF)
	{
		while(1);                               // If calibration constants erased
												// do not load, trap CPU!!
	}

	// DCO = 1MHz
	// MCLK = DCO
	// ACLK = VLO
	// SMCLK = DCO / 4
	
	//1Mhz
	DCOCTL = CALDCO_1MHZ;
	BCSCTL1 = XT2OFF + CALBC1_1MHZ;
	BCSCTL2 = SELM_0 + DIVS_2;
	BCSCTL3 = LFXT1S_2;

	// GPIO
	// P1.0	= NC
	// P1.1	= RXD
	// P1.2	= TXD
	// P1.3	= VR Out
	// P1.4	= VR in
	// P1.5	= BUT2
	// P1.6	= SCL
	// P1.7	= SDA
	// P2.0	= BUT1
	// P2.1	= Motor PWM2
	// P2.2	= Motor PWM1
	// P2.3	= Motor Sleep
	// P2.4	= Led
	// P2.5	= Rotation detection
	// P2.6	= NC
	// P2.7	= NC
	
	P1OUT	= BIT5;
	P1DIR	= BIT1 + BIT2 + BIT3;
	P1REN	= BIT5;					// Pull up button input
	P1IES	= BIT5;					// Hi to Lo transition
	P1IFG	= 0;
	
	P2OUT	= BIT0 + BIT3;
	P2DIR	= BIT1 + BIT2 + BIT3 + BIT4 + BIT6 + BIT7;
	P2REN	= BIT0;					// Pull up button input
	P2IES	= BIT0 + BIT5;			// Hi to Lo transition
	P2IFG	= 0;
	P2SEL	= BIT1 + BIT2;

	// Timer A1. For motor PWM
	// CCR0 --> PWM frequency
	// CCR1 --> PWM pulse-width
	TA1CCR0		= PWM_CCR0;
	TA1CCR1		= 0;
	TA1CCTL1	= OUTMOD_7;
	TA1CTL		= TASSEL_2 + MC_1 + TACLR;

	// ADC10
	ADC10CTL0 = ADC10SHT_3 + ADC10SR + ADC10ON;// + ADC10IE;
	ADC10CTL1 = INCH_4 + ADC10SSEL_1;
	ADC10AE0 = BIT4;

	__enable_interrupt();

	// Flash the LED
	WDTCTL = WDTPW + WDTCNTCL + WDTTMSEL + WDTSSEL + WDTIS1;	// Set Watchdog Timer interval to ~30ms
	IE1		|= WDTIE;
	for (i = 8; i > 0; i--)
	{
		LPM0;
		P2OUT ^= BIT4;
	}
	IE1 &= ~WDTIE;
	
	// Enable button and rotate_counter interrupts
	P1IE |= BIT5;
	P2IE |= BIT0 + BIT5;

	Setup_IIC_Slave();

	while (1)
	{
		// Watchdog timer wakes up from LPM0 and then scan the VR using ADC
		LPM0;
		set_motor_by_VR();
	}
}

unsigned int get_ADC_value(void)
{
	P1OUT |= BIT3;					// Enable VR
	ADC10CTL0 &= ~ADC10IFG;
	ADC10CTL0 |= ENC + ADC10SC;		// Enable AD conversion
	while (!(ADC10CTL0 & ADC10IFG));
	//LPM0;
	P1OUT &= ~BIT3;					// Disable VR
	return ADC10MEM;				// Return ADC value
}

unsigned int adc_value = 0;
const unsigned int adc_zone_interval = 1023 / NUM_SPEED;

#define ADC_HYSTERESIS	5

void set_motor_by_VR(void)
{
	unsigned int new_speed;
	unsigned int adc_border = adc_zone_interval * current_speed;
	
	// simple averaging for adc_value
	adc_value += get_ADC_value();
	adc_value >>=1;

	new_speed = adc_value / adc_zone_interval;

	if (new_speed > current_speed)
	{
		if (adc_value > (adc_border + ADC_HYSTERESIS))
		{
			set_motor_speed(new_speed);
		}
	}
	else if (new_speed < current_speed)
	{
		if (adc_value < (adc_border - ADC_HYSTERESIS))
		{
			set_motor_speed(new_speed);
		}
	}
}

void set_motor_direction(unsigned char direction)
{
	current_direction = direction;
	switch (direction)
	{
		case DIR_STOP:
			P2OUT |= BIT1 + BIT2;
			P2SEL &= ~(BIT1 + BIT2);
			current_speed = 0;
			break;
			
		case DIR_CW:
			P2OUT &= ~(BIT1 + BIT2);
#ifdef REVERSED
			P2SEL &= ~BIT1;
			P2SEL |= BIT2;
#else
			P2SEL &= ~BIT2;
			P2SEL |= BIT1;
#endif
			break;
			
		case DIR_CCW:
			P2OUT &= ~(BIT1 + BIT2);
#ifdef REVERSED
			P2SEL &= ~BIT2;
			P2SEL |= BIT1;
#else
			P2SEL &= ~BIT1;
			P2SEL |= BIT2;
#endif
			break;
	}
		acc_error = 0;
}

void set_motor_speed(unsigned char speed)
{
	if (speed < NUM_SPEED)
	{
		// Timer A0. For measuring rpm
		// Set time-out for TA0CCR0
		// If rotation is too slow, timer overflow interrupt will be triggered to increase PWM

		
		if (speed > 0)
		{
			if (speed < SLOW_SPEED)
			{
				TA0CCR0 = RPS_TimerA_Table[speed] /2;
				ignore_first_error = 2;
				TA0CCTL0	= CCIE;
			}
			else
			{
				TA0CCR0 = RPS_TimerA_Table[speed];
				ignore_first_error = 1;
			}
			
			if (current_speed == 0)
			{
				TA1CCR1 = 200;		// Set an initial PWM to rotate the motor from station
			}
			TA0CTL		= TASSEL_2 + ID_3 + MC_2 + TACLR;
		}
		else
		{
			TA1CCR1		= 0;			// Stop the motor
			TA0CTL		= 0;			// Stop rotation detection
			TA0CCTL0	= 0;
			
		}
		P2IES |= BIT5;
		current_speed = speed;
		acc_error = 0;
	}
}

#define ADJ_DIV				40
#define ADJ_HYSTERSIS_DIV	100
void adjust_pwm(unsigned int measured_tar)
{
	int error;
	int adjust;
	int hystersis = (int)RPS_TimerA_Table[current_speed] / ADJ_HYSTERSIS_DIV;
	
	if (current_speed < SLOW_SPEED)
	{
		if (ignore_first_error)
		{
			ignore_first_error--;
		}
		else
		{
			error = (int)measured_tar - (int)(RPS_TimerA_Table[current_speed] >> 1);

			if (error > 0)
			{
				TA1CCR1++;
			}
			else
			{
				ignore_first_error++;
				TA1CCR1--;
			}
		}
	}
	else
	{
		error = (int)measured_tar - (int)RPS_TimerA_Table[current_speed];
		if (ignore_first_error)
		{
			ignore_first_error--;
		}
		else
		{
			acc_error += error/8;
			acc_error /= 4;
		}

		if (error > hystersis)
		{
			adjust = (acc_error / ADJ_DIV) + 1;
			// if (adjust > 10) adjust = 10;
			// else adjust = 1;
			if (TA1CCR1 + adjust > PWM_CCR0)
				TA1CCR1 = PWM_CCR0;
			else
				TA1CCR1 += adjust;
		}
		else if (acc_error < -hystersis)
		{
			adjust = (-acc_error / ADJ_DIV) + 1;
			// if (adjust > 5) adjust = 5;
			// else adjust = 1;
			if (adjust > TA1CCR1)
				TA1CCR1 = 0;
			else
				TA1CCR1 -= adjust;
		}
	}	
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TimerA0_ISR(void)
{
	// Timer A0 overflow.
	// The motor is probably stopped.
	// Increase PWM to make it rotate.
	
	TA1CCR1++;
}

// ADC10 interrupt service routine
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	LPM0_EXIT;
}

// Watchdog interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR (void)
{
	LPM0_EXIT;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
	unsigned char i = 255;

	if (P1IFG & BIT5)		// BUT2
	{
		// de-bouncing
		do
		{
			if (P1IN & BIT5)
			{
				P1IFG	&= ~BIT5;
				return;
			}
		}while (i--);

		switch (current_direction)
		{
			case DIR_CW:
				set_motor_direction(DIR_STOP);
				break;
			case DIR_STOP:
			case DIR_CCW:
				set_motor_direction(DIR_CCW);
				break;
		}
		IE1		|= WDTIE;
		P1IFG	&= ~BIT5;
	}
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
	unsigned int tar = TA0R;
	unsigned char i = 255;

	if (P2IFG & BIT0)		// BUT1
	{
		// de-bouncing
		do
		{
			if (P2IN & BIT0)
			{
				P2IFG	&= ~BIT0;
				return;
			}
		}while (i--);

		switch (current_direction)
		{
			case DIR_CCW:
				set_motor_direction(DIR_STOP);
				break;
			case DIR_STOP:
			case DIR_CW:
				set_motor_direction(DIR_CW);
				break;
		}
		IE1		|= WDTIE;
		P2IFG	&= ~BIT0;
	}

	if (P2IFG & BIT5)		// Rotate Counter
	{
		if (current_direction == DIR_CW)		rotate_counter--;
		else if (current_direction == DIR_CCW)	rotate_counter++;

		// Adjust motor speed
		if (current_speed > 0)
		{
			adjust_pwm(tar);
			TA0CTL		= TASSEL_2 + ID_3 + MC_2 + TACLR;	// reset Timer;
			TA0CCTL0	= CCIE;
			if (current_speed < SLOW_SPEED)
			{
				P2IES ^= BIT5;
			}
		}
		else
		{
			TA0CTL		= 0;
			TA0CCTL0	&= ~CCIE;
		}
		P2IFG &= ~BIT5;
	}
}
