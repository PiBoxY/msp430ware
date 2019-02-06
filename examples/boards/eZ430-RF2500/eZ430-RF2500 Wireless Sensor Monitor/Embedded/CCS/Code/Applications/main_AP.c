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
//   eZ430-RF2500 Temperature Sensor Access Point
//
//   Description: This is the Access Point software for the eZ430-2500RF
//                Temperature Sensing demo
//
//
//   Z. Shivers
//   Version    1.05
//   Texas Instruments, Inc
//   July 2010
//     IAR Embedded Workbench Kickstart (Version: 5.10.4)
//     Code Composer Studio (Version 4.1.2.00027)
//******************************************************************************
//Change Log:
//******************************************************************************
//Version:  1.05
//Comments: Added support for various baud rates dependent on CPU frequencies
//Version:  1.04
//Comments: Added support for SimpliciTI 1.1.1
//          Replaced delays with __delay_cycles() instrinsic
//          Added more comments
//Version:  1.03
//Comments: Added support for SimpliciTI 1.1.0
//          Added support for Code Composer Studio
//          Added security (Enabled with -DSMPL_SECURE in smpl_nwk_config.dat)
//          Added acknowledgement (Enabled with -DAPP_AUTO_ACK in smpl_nwk_config.dat)
//          Based the modifications on the AP_as_Data_Hub example code
//Version:  1.02
//Comments: Changed Port toggling to abstract method
//          Removed ToggleLED
//          Fixed comment typos/errors
//          Changed startup string to 1.02
//Version:  1.01
//Comments: Added support for SimpliciTI 1.0.3
//          Changed RSSI read method
//          Added 3 digit temperature output for 100+F
//          Changed startup string to 1.01
//Version:  1.00
//Comments: Initial Release Version
//******************************************************************************
#include <string.h>
#include "bsp.h"
#include "mrfi.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_frame.h"
#include "nwk.h"
#include "virtual_com_cmds.h"

/****************** COMMENTS ON ASYNC LISTEN APPLICATION ***********************
Summary:
  This AP build includes implementation of an unknown number of end device peers
  in addition to AP functionality. In this scenario all End Devices establish a
  link to the AP and only to the AP. The AP acts as a data hub. All End Device
  peers are on the AP and not on other distinct ED platforms.

  There is still a limit to the number of peers supported on the AP that is
  defined by the macro NUM_CONNECTIONS. The AP will support NUM_CONNECTIONS or
  fewer peers but the exact number does not need to be known at build time.

  In this special but common scenario SimpliciTI restricts each End Device
  object to a single connection to the AP. If multiple logical connections are
  required these must be accommodated by supporting contexts in the application
  payload itself.

Solution overview:
  When a new peer connection is required the AP main loop must be notified. In
  essence the main loop polls a semaphore to know whether to begin listening for
  a peer Link request from a new End Device. There are two solutions: automatic
  notification and external notification. The only difference between the
  automatic notification solution and the external notification solution is how
  the listen semaphore is set. In the external notification solution the
  sempahore is set by the user when the AP is stimulated for example by a button
  press or a commend over a serial link. In the automatic scheme the
  notification is accomplished as a side effect of a new End Device joining.

  The Rx callback must be implemented. When the callback is invoked with a
  non-zero Link ID the handler could set a semaphore that alerts the main work
  loop that a SMPL_Receive() can be executed successfully on that Link ID.

  If the callback conveys an argument (LinkID) of 0 then a new device has joined
  the network. A SMPL_LinkListen() should be executed.

  Whether the joining device supports ED objects is indirectly inferred on the
  joining device from the setting of the NUM_CONNECTIONS macro. The value of
  this macro should be non-zero only if ED objects exist on the device. This
  macro is always non-zero for ED-only devices. But Range Extenders may or may
  not support ED objects. The macro should be be set to 0 for REs that do not
  also support ED objects. This prevents the Access Point from reserving
  resources for a joinng device that does not support any End Device Objects and
  it prevents the AP from executing a SMPL_LinkListen(). The Access Point will
  not ever see a Link frame if the joining device does not support any
  connections.

  Each joining device must execute a SMPL_Link() after receiving the join reply
  from the Access Point. The Access Point will be listening.

******************* END COMMENTS ON ASYNC LISTEN APPLICATION ******************/

/******  THIS SOURCE FILE REPRESENTS THE AUTOMATIC NOTIFICATION SOLUTION ******/

/*------------------------------------------------------------------------------
 * Prototypes
 *----------------------------------------------------------------------------*/
/* Frequency Agility helper functions */
static void    checkChangeChannel(void);
static void    changeChannel(void);

__interrupt void ADC10_ISR(void);
__interrupt void Timer_A (void);

/*------------------------------------------------------------------------------
 * Globals
 *----------------------------------------------------------------------------*/
/* reserve space for the maximum possible peer Link IDs */
static linkID_t sLID[NUM_CONNECTIONS] = {0};
static uint8_t  sNumCurrentPeers = 0;

/* callback handler */
static uint8_t sCB(linkID_t);

/* received message handler */
static void processMessage(linkID_t, uint8_t *, uint8_t);

/* work loop semaphores */
static volatile uint8_t sPeerFrameSem = 0;
static volatile uint8_t sJoinSem = 0;
static volatile uint8_t sSelfMeasureSem = 0;

/* blink LEDs when channel changes... */
static volatile uint8_t sBlinky = 0;

/* data for terminal output */
const char splash[] = {"\r\n--------------------------------------------------  \r\n     ****\r\n     ****           eZ430-RF2500\r\n     ******o****    Temperature Sensor Network\r\n********_///_****   Copyright 2009\r\n ******/_//_/*****  Texas Instruments Incorporated\r\n  ** ***(__/*****   All rights reserved.\r\n      *********     SimpliciTI1.1.1\r\n       *****\r\n        ***\r\n--------------------------------------------------\r\n"};
volatile int * tempOffset = (int *)0x10F4;

/*------------------------------------------------------------------------------
 * Frequency Agility support (interference detection)
 *----------------------------------------------------------------------------*/
#ifdef FREQUENCY_AGILITY

#define INTERFERNCE_THRESHOLD_DBM (-70)
#define SSIZE    25
#define IN_A_ROW  3
static int8_t  sSample[SSIZE];
static uint8_t sChannel = 0;

#endif  /* FREQUENCY_AGILITY */

/*------------------------------------------------------------------------------
 * Main
 *----------------------------------------------------------------------------*/
void main (void)
{
  bspIState_t intState;

#ifdef FREQUENCY_AGILITY
  memset(sSample, 0x0, sizeof(sSample));
#endif

  /* Initialize board */
  BSP_Init();

  /* Initialize TimerA and oscillator */
  BCSCTL3 |= LFXT1S_2;                      // LFXT1 = VLO
  TACCTL0 = CCIE;                           // TACCR0 interrupt enabled
  TACCR0 = 12000;                           // ~1 second
  TACTL = TASSEL_1 + MC_1;                  // ACLK, upmode

  /* Initialize serial port */
  COM_Init();

  //Transmit splash screen and network init notification
  TXString( (char*)splash, sizeof splash);
  TXString( "\r\nInitializing Network....", 26 );

  SMPL_Init(sCB);

  // network initialized
  TXString( "Done\r\n", 6);

  /* green and red LEDs on solid to indicate waiting for a Join. */
  BSP_TURN_ON_LED1();
  BSP_TURN_ON_LED2();

  /* main work loop */
  while (1)
  {
    /* Wait for the Join semaphore to be set by the receipt of a Join frame from
     * a device that supports an End Device.
     *
     * An external method could be used as well. A button press could be connected
     * to an ISR and the ISR could set a semaphore that is checked by a function
     * call here, or a command shell running in support of a serial connection
     * could set a semaphore that is checked by a function call.
     */
    if (sJoinSem && (sNumCurrentPeers < NUM_CONNECTIONS))
    {
      /* listen for a new connection */
      while (1)
      {
        if (SMPL_SUCCESS == SMPL_LinkListen(&sLID[sNumCurrentPeers]))
        {
          break;
        }
        /* Implement fail-to-link policy here. otherwise, listen again. */
      }

      sNumCurrentPeers++;

      BSP_ENTER_CRITICAL_SECTION(intState);
      sJoinSem--;
      BSP_EXIT_CRITICAL_SECTION(intState);
    }


    // if it is time to measure our own temperature...
    if(sSelfMeasureSem)
    {
      char msg [6];
      char addr[] = {"HUB0"};
      char rssi[] = {"000"};
      int degC, volt;
      volatile long temp;
      int results[2];

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

      temp = results[1];
      volt = (temp*25)/512;

      /* Package up the data */
      msg[0] = degC&0xFF;
      msg[1] = (degC>>8)&0xFF;
      msg[2] = volt;

      /* Send it over serial port */
      transmitDataString(1, addr, rssi, msg );

      BSP_TOGGLE_LED1();

      /* Done with measurement, disable measure flag */
      sSelfMeasureSem = 0;
    }

    /* Have we received a frame on one of the ED connections?
     * No critical section -- it doesn't really matter much if we miss a poll
     */
    if (sPeerFrameSem)
    {
      uint8_t     msg[MAX_APP_PAYLOAD], len, i;

      /* process all frames waiting */
      for (i=0; i<sNumCurrentPeers; ++i)
      {
        if (SMPL_SUCCESS == SMPL_Receive(sLID[i], msg, &len))
        {
          ioctlRadioSiginfo_t sigInfo;

          processMessage(sLID[i], msg, len);

          sigInfo.lid = sLID[i];

          SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_SIGINFO, (void *)&sigInfo);

          transmitData( i, sigInfo.sigInfo.rssi, (char*)msg );
          BSP_TOGGLE_LED2();

          BSP_ENTER_CRITICAL_SECTION(intState);
          sPeerFrameSem--;
          BSP_EXIT_CRITICAL_SECTION(intState);
        }
      }
    }
    if (BSP_BUTTON1())
    {
      __delay_cycles(2000000);  /* debounce (0.25 seconds) */
      changeChannel();
    }
    else
    {
      checkChangeChannel();
    }
    BSP_ENTER_CRITICAL_SECTION(intState);
    if (sBlinky)
    {
      if (++sBlinky >= 0xF)
      {
        sBlinky = 1;
        BSP_TOGGLE_LED1();
        BSP_TOGGLE_LED2();
      }
    }
    BSP_EXIT_CRITICAL_SECTION(intState);
  }

}

/* Runs in ISR context. Reading the frame should be done in the */
/* application thread not in the ISR thread. */
static uint8_t sCB(linkID_t lid)
{
  if (lid)
  {
    sPeerFrameSem++;
    sBlinky = 0;
  }
  else
  {
    sJoinSem++;
  }

  /* leave frame to be read by application. */
  return 0;
}

static void processMessage(linkID_t lid, uint8_t *msg, uint8_t len)
{
  /* do something useful */
  if (len)
  {
    BSP_TOGGLE_LED1();
  }
  return;
}

static void changeChannel(void)
{
#ifdef FREQUENCY_AGILITY
  freqEntry_t freq;

  if (++sChannel >= NWK_FREQ_TBL_SIZE)
  {
    sChannel = 0;
  }
  freq.logicalChan = sChannel;
  SMPL_Ioctl(IOCTL_OBJ_FREQ, IOCTL_ACT_SET, &freq);
  BSP_TURN_OFF_LED1();
  BSP_TURN_OFF_LED2();
  sBlinky = 1;
#endif
  return;
}

/* implement auto-channel-change policy here... */
static void checkChangeChannel(void)
{
#ifdef FREQUENCY_AGILITY
  int8_t dbm, inARow = 0;

  uint8_t i;

  memset(sSample, 0x0, SSIZE);
  for (i=0; i<SSIZE; ++i)
  {
    /* quit if we need to service an app frame */
    if (sPeerFrameSem || sJoinSem)
    {
      return;
    }
    NWK_DELAY(1);
    SMPL_Ioctl(IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RSSI, (void *)&dbm);
    sSample[i] = dbm;

    if (dbm > INTERFERNCE_THRESHOLD_DBM)
    {
      if (++inARow == IN_A_ROW)
      {
        changeChannel();
        break;
      }
    }
    else
    {
      inARow = 0;
    }
  }
#endif
  return;
}

/*------------------------------------------------------------------------------
* ADC10 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
  __bic_SR_register_on_exit(CPUOFF);        // Clear CPUOFF bit from 0(SR)
}

/*------------------------------------------------------------------------------
* Timer A0 interrupt service routine
------------------------------------------------------------------------------*/
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
  sSelfMeasureSem = 1;
}
