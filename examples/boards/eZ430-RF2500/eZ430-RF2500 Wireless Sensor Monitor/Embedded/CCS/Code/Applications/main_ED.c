//******************************************************************************
// THIS PROGRAM IS PROVIDED "AS IS". TI MAKES NO WARRANTIES OR
// REPRESENTATIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
// INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
// COMPLETENESS OF RESPONSES, RESULTS AND LACK OF NEGLIGENCE.
// TI DISCLAIMS ANY WARRANTY OF TITLE, QUIET ENJOYMENT, QUIET
// POSSESSION, AND NON-INFRINGEMENT OF ANY THIRD PARTY
// INTELLECTUAL PROPERTY RIGHTS WITH REGARD TO THE PROGRAM OR
// YOUR USE OF THE PROGRAM.
//
// IN NO EVENT SHALL TI BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
// CONSEQUENTIAL OR INDIRECT DAMAGES, HOWEVER CAUSED, ON ANY
// THEORY OF LIABILITY AND WHETHER OR NOT TI HAS BEEN ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGES, ARISING IN ANY WAY OUT
// OF THIS AGREEMENT, THE PROGRAM, OR YOUR USE OF THE PROGRAM.
// EXCLUDED DAMAGES INCLUDE, BUT ARE NOT LIMITED TO, COST OF
// REMOVAL OR REINSTALLATION, COMPUTER TIME, LABOR COSTS, LOSS
// OF GOODWILL, LOSS OF PROFITS, LOSS OF SAVINGS, OR LOSS OF
// USE OR INTERRUPTION OF BUSINESS. IN NO EVENT WILL TI'S
// AGGREGATE LIABILITY UNDER THIS AGREEMENT OR ARISING OUT OF
// YOUR USE OF THE PROGRAM EXCEED FIVE HUNDRED DOLLARS
// (U.S.$500).
//
// Unless otherwise stated, the Program written and copyrighted
// by Texas Instruments is distributed as "freeware".  You may,
// only under TI's copyright in the Program, use and modify the
// Program without any charge or restriction.  You may
// distribute to third parties, provided that you transfer a
// copy of this license to the third party and the third party
// agrees to these terms by its first use of the Program. You
// must reproduce the copyright notice and any other legend of
// ownership on each copy or partial copy, of the Program.
//
// You acknowledge and agree that the Program contains
// copyrighted material, trade secrets and other TI proprietary
// information and is protected by copyright laws,
// international copyright treaties, and trade secret laws, as
// well as other intellectual property laws.  To protect TI's
// rights in the Program, you agree not to decompile, reverse
// engineer, disassemble or otherwise translate any object code
// versions of the Program to a human-readable form.  You agree
// that in no event will you alter, remove or destroy any
// copyright notice included in the Program.  TI reserves all
// rights not specifically granted under this license. Except
// as specifically provided herein, nothing in this agreement
// shall be construed as conferring by implication, estoppel,
// or otherwise, upon you, any license or other right under any
// TI patents, copyrights or trade secrets.
//
// You may not use the Program in non-TI devices.
//
//******************************************************************************
//   eZ430-RF2500 Temperature Sensor End Device
//
//   Description: This is the End Device software for the eZ430-2500RF
//                Temperature Sensing demo
//
//
//   Z. Shivers
//   Version    1.05
//   Texas Instruments, Inc
//   July 2010
//   Known working builds:
//     IAR Embedded Workbench Kickstart (Version: 5.10.4)
//     Code Composer Studio (Version 4.1.2.00027)
//******************************************************************************
//Change Log:
//******************************************************************************
//Version:  1.05
//Comments: Added support for various baud rates dependent on CPU frequencies
//Version:  1.04
//Comments: Added support for SimpliciTI 1.1.1
//          Moved radio wakeup in linkTo() to after ADC code to save power
//          Replaced delays with __delay_cylces() instrinsic
//          Replaced toggleLED with BSP functions
//          Added more comments
//Version:  1.03
//Comments: Added support for SimpliciTI 1.1.0
//          Added support for Code Composer Studio
//          Added security (Enabled with -DSMPL_SECURE in smpl_nwk_config.dat)
//          Added acknowledgement (Enabled with -DAPP_AUTO_ACK in smpl_nwk_config.dat)
//          Based the modifications on the AP_as_Data_Hub example code
//Version:  1.02
//Comments: Changed Port toggling to abstract method
//          Fixed comment typos
//Version:  1.01
//Comments: Added support for SimpliciTI 1.0.3
//          Added Flash storage/check of Random address
//          Moved LED toggle to HAL
//Version:  1.00
//Comments: Initial Release Version
//******************************************************************************
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "vlo_rand.h"

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
/* How many times to try a TX and miss an acknowledge before doing a scan */
#define MISSES_IN_A_ROW  2

/*------------------------------------------------------------------------------
 * Prototypes
 *----------------------------------------------------------------------------*/
static void linkTo(void);
void createRandomAddress(void);
__interrupt void ADC10_ISR(void);
__interrupt void Timer_A (void);

/*------------------------------------------------------------------------------
* Globals
------------------------------------------------------------------------------*/
static linkID_t sLinkID1 = 0;
/* Temperature offset set at production */
volatile int * tempOffset = (int *)0x10F4;
/* Initialize radio address location */
char * Flash_Addr = (char *)0x10F0;
/* Work loop semaphores */
static volatile uint8_t sSelfMeasureSem = 0;

/*------------------------------------------------------------------------------
 * Main
 *----------------------------------------------------------------------------*/
void main (void)
{
  addr_t lAddr;

  /* Initialize board-specific hardware */
  BSP_Init();

  /* Check flash for previously stored address */
  if(Flash_Addr[0] == 0xFF && Flash_Addr[1] == 0xFF &&
     Flash_Addr[2] == 0xFF && Flash_Addr[3] == 0xFF )
  {
    createRandomAddress(); // Create and store a new random address
  }

  /* Read out address from flash */
  lAddr.addr[0] = Flash_Addr[0];
  lAddr.addr[1] = Flash_Addr[1];
  lAddr.addr[2] = Flash_Addr[2];
  lAddr.addr[3] = Flash_Addr[3];

  /* Tell network stack the device address */
  SMPL_Ioctl(IOCTL_OBJ_ADDR, IOCTL_ACT_SET, &lAddr);

  /* Initialize TimerA and oscillator */
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 12000;                           // ~ 1 sec
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode

  /* Keep trying to join (a side effect of successful initialization) until
   * successful. Toggle LEDS to indicate that joining has not occurred.
   */
  while (SMPL_SUCCESS != SMPL_Init(0))
  {
    BSP_TOGGLE_LED1();
    BSP_TOGGLE_LED2();
    /* Go to sleep (LPM3 with interrupts enabled)
     * Timer A0 interrupt will wake CPU up every second to retry initializing
     */
    __bis_SR_register(LPM3_bits+GIE);  // LPM3 with interrupts enabled
  }

  /* LEDs on solid to indicate successful join. */
  BSP_TURN_ON_LED1();
  BSP_TURN_ON_LED2();

  /* Unconditional link to AP which is listening due to successful join. */
  linkTo();

  while(1);
}

static void linkTo()
{
  uint8_t msg[3];
#ifdef APP_AUTO_ACK
  uint8_t misses, done;
#endif

  /* Keep trying to link... */
  while (SMPL_SUCCESS != SMPL_Link(&sLinkID1))
  {
    BSP_TOGGLE_LED1();
    BSP_TOGGLE_LED2();
    /* Go to sleep (LPM3 with interrupts enabled)
     * Timer A0 interrupt will wake CPU up every second to retry linking
     */
    __bis_SR_register(LPM3_bits+GIE);
  }

  /* Turn off LEDs. */
  BSP_TURN_OFF_LED1();
  BSP_TURN_OFF_LED2();

  /* Put the radio to sleep */
  SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);

  while (1)
  {
    /* Go to sleep, waiting for interrupt every second to acquire data */
    __bis_SR_register(LPM3_bits);

    /* Time to measure */
    if (sSelfMeasureSem) {
      volatile long temp;
      int degC, volt;
      int results[2];
#ifdef APP_AUTO_ACK
      uint8_t      noAck;
      smplStatus_t rc;
#endif

      /* Get temperature */
      ADC10CTL1 = INCH_10 + ADC10DIV_4;       // Temp Sensor ADC10CLK/5
      ADC10CTL0 = SREF_1 + ADC10SHT_3 + REFON + ADC10ON + ADC10IE + ADC10SR;
      /* Allow ref voltage to settle for at least 30us (30us * 8MHz = 240 cycles)
       * See SLAS504D for settling time spec
       */
      __delay_cycles(240);
      ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
      __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
      results[0] = ADC10MEM;                  // Retrieve result
      ADC10CTL0 &= ~ENC;

      /* Get voltage */
      ADC10CTL1 = INCH_11;                     // AVcc/2
      ADC10CTL0 = SREF_1 + ADC10SHT_2 + REFON + ADC10ON + ADC10IE + REF2_5V;
      __delay_cycles(240);
      ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
      __bis_SR_register(CPUOFF + GIE);        // LPM0 with interrupts enabled
      results[1] = ADC10MEM;                  // Retrieve result

      /* Stop and turn off ADC */
      ADC10CTL0 &= ~ENC;
      ADC10CTL0 &= ~(REFON + ADC10ON);

      /* oC = ((A10/1024)*1500mV)-986mV)*1/3.55mV = A10*423/1024 - 278
       * the temperature is transmitted as an integer where 32.1 = 321
       * hence 4230 instead of 423
       */
      temp = results[0];
      degC = ((temp - 673) * 4230) / 1024;
      if( (*tempOffset) != 0xFFFF )
      {
        degC += (*tempOffset);
      }

      /* message format,  UB = upper Byte, LB = lower Byte
      -------------------------------
      |degC LB | degC UB |  volt LB |
      -------------------------------
         0         1          2
      */
      temp = results[1];
      volt = (temp*25)/512;
      msg[0] = degC&0xFF;
      msg[1] = (degC>>8)&0xFF;
      msg[2] = volt;

      /* Get radio ready...awakens in idle state */
      SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_AWAKE, 0);

#ifdef APP_AUTO_ACK
      /* Request that the AP sends an ACK back to confirm data transmission
       * Note: Enabling this section more than DOUBLES the current consumption
       *       due to the amount of time IN RX waiting for the AP to respond
       */
      done = 0;
      while (!done)
      {
        noAck = 0;

        /* Try sending message MISSES_IN_A_ROW times looking for ack */
        for (misses=0; misses < MISSES_IN_A_ROW; ++misses)
        {
          if (SMPL_SUCCESS == (rc=SMPL_SendOpt(sLinkID1, msg, sizeof(msg), SMPL_TXOPTION_ACKREQ)))
          {
            /* Message acked. We're done. Toggle LED 1 to indicate ack received. */
            BSP_TURN_ON_LED1();
            __delay_cycles(2000);
            BSP_TURN_OFF_LED1();
            break;
          }
          if (SMPL_NO_ACK == rc)
          {
            /* Count ack failures. Could also fail becuase of CCA and
             * we don't want to scan in this case.
             */
            noAck++;
          }
        }
        if (MISSES_IN_A_ROW == noAck)
        {
          /* Message not acked */
          BSP_TURN_ON_LED2();
          __delay_cycles(2000);
          BSP_TURN_OFF_LED2();
#ifdef FREQUENCY_AGILITY
          /* Assume we're on the wrong channel so look for channel by
           * using the Ping to initiate a scan when it gets no reply. With
           * a successful ping try sending the message again. Otherwise,
           * for any error we get we will wait until the next button
           * press to try again.
           */
          if (SMPL_SUCCESS != SMPL_Ping(sLinkID1))
          {
            done = 1;
          }
#else
          done = 1;
#endif  /* FREQUENCY_AGILITY */
        }
        else
        {
          /* Got the ack or we don't care. We're done. */
          done = 1;
        }
      }
#else

      /* No AP acknowledgement, just send a single message to the AP */
      SMPL_SendOpt(sLinkID1, msg, sizeof(msg), SMPL_TXOPTION_NONE);

#endif /* APP_AUTO_ACK */

      /* Put radio back to sleep */
      SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SLEEP, 0);

      /* Done with measurement, disable measure flag */
      sSelfMeasureSem = 0;
    }
  }
}

void createRandomAddress()
{
  unsigned int rand, rand2;
  do
  {
    rand = TI_getRandomIntegerFromVLO();    // first byte can not be 0x00 of 0xFF
  }
  while( (rand & 0xFF00)==0xFF00 || (rand & 0xFF00)==0x0000 );
  rand2 = TI_getRandomIntegerFromVLO();

  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO to 1MHz
  DCOCTL = CALDCO_1MHZ;
  FCTL2 = FWKEY + FSSEL0 + FN1;             // MCLK/3 for Flash Timing Generator
  FCTL3 = FWKEY + LOCKA;                    // Clear LOCK & LOCKA bits
  FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

  Flash_Addr[0]=(rand>>8) & 0xFF;
  Flash_Addr[1]=rand & 0xFF;
  Flash_Addr[2]=(rand2>>8) & 0xFF;
  Flash_Addr[3]=rand2 & 0xFF;

  FCTL1 = FWKEY;                            // Clear WRT bit
  FCTL3 = FWKEY + LOCKA + LOCK;             // Set LOCK & LOCKA bit
}

/*------------------------------------------------------------------------------
 * ADC10 interrupt service routine
 *----------------------------------------------------------------------------*/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

/*------------------------------------------------------------------------------
 * Timer A0 interrupt service routine
 *----------------------------------------------------------------------------*/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  sSelfMeasureSem = 1;
  __bic_SR_register_on_exit(LPM3_bits);        // Clear LPM3 bit from 0(SR)
}
