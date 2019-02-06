//*****************************************************************************
// ussSwLibCommon.c
//
// version __LIB_VERSION__
// Released on __LIB_RELEASE_DATE__
//
//*****************************************************************************
#include "ussSwLibCommonTimer.h"

static void commonTimerWaitEvent(uint32_t flag, USS_low_power_mode_option lpmMode);

void commonTimerGenerateLowPowerDelay(
		USS_SW_Library_configuration *config,
		uint16_t delay,USS_low_power_mode_option lpmMode)
{
	// Save global interrupt status
	uint16_t gieStatus = ( __get_SR_register() & GIE);

	commonTimerConfigureDelay(config, delay);

	commonClearCCFlagStart(config);

	commonTimerEnableInterrupt(config);

	commonTimerStart(config);

	__disable_interrupt();
	while(false==(USSSWLIB_USS_interrupt_status & USS_TIMER_EXPIRED_INTERRUPT))
	{
		__bis_SR_register(lpmMode + GIE);
		__disable_interrupt();
	}

	commonTimerStop(config);

	commonClearCCFlagStart(config);

	commonTimerDisableInterrupt(config);

	USSSWLIB_USS_interrupt_status &= ~(USS_TIMER_EXPIRED_INTERRUPT);

	// Restore global interrupts status
	if(GIE == gieStatus)
	{
		__bis_SR_register(GIE);
	}
}

void commonTimerConfigureDelay(USS_SW_Library_configuration *config,
		uint16_t delay)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCR0) =
				delay;
}

void commonTimerConfigurePeriodMeasurement(USS_SW_Library_configuration *config,
        uint16_t delay1, uint16_t delay2)
{
    // CCR1 will be triggered on first event
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCR1) =
                delay1;
    // CCR0 will be a timeout for 2nd event
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCR0) =
                delay2;
}

void commonTimerPeriodMeasurementClear(USS_SW_Library_configuration *config)
{
    USSSWLIB_USS_interrupt_status &= ~USS_TIMER_EXPIRED_INTERRUPT;
    USSSWLIB_USS_interrupt_status &= ~USS_TIMER_EXPIRED_EVENT1_INTERRUPT;
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) &= ~(CCIFG);
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL1) &= ~(CCIFG);
}

void commonTimerPeriodMeasurementEnableInterrupts(USS_SW_Library_configuration *config)
{
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) |= (CCIE);
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL1) |= (CCIE);
}

void commonTimerPeriodMeasurementDisableInterrupts(USS_SW_Library_configuration *config)
{
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) &= ~(CCIE);
    HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL1) &= ~(CCIE);
}

void commonTimerPeriodMeasurementStart(USS_SW_Library_configuration *config)
{
    commonTimerPeriodMeasurementClear(config);
    commonTimerPeriodMeasurementEnableInterrupts(config);
    commonTimerStart(config);
}

void commonTimerPeriodMeasurementWaitEvent1(USS_SW_Library_configuration *config,
                                             USS_low_power_mode_option lpmMode)
{
    commonTimerWaitEvent(USS_TIMER_EXPIRED_EVENT1_INTERRUPT, lpmMode);
}

void commonTimerPeriodMeasurementWaitEvent2(USS_SW_Library_configuration *config,
                                             USS_low_power_mode_option lpmMode)
{
    commonTimerWaitEvent(USS_TIMER_EXPIRED_INTERRUPT, lpmMode);

}

static void commonTimerWaitEvent(uint32_t flag,
                                 USS_low_power_mode_option lpmMode)
{
    // Save global interrupt status
    uint16_t gieStatus = ( __get_SR_register() & GIE);

    __disable_interrupt();
    while(false==(USSSWLIB_USS_interrupt_status & flag))
    {
        __bis_SR_register(lpmMode + GIE);
        __disable_interrupt();
    }

    USSSWLIB_USS_interrupt_status &= ~(flag);

    // Restore global interrupts status
    if(GIE == gieStatus)
    {
        __bis_SR_register(GIE);
    }
}

void commonClearCCFlagStart(USS_SW_Library_configuration *config)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCTL) &= ~(TAIFG);
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) &= ~(CCIFG);
}

void commonTimerStart(USS_SW_Library_configuration *config)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCTL) |=
					(MC__UP | TACLR);
}

void commonTimerStop(USS_SW_Library_configuration *config)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCTL) &=
					~(MC__STOP);
}

void commonTimerClear(USS_SW_Library_configuration *config)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCTL) |=
						(TACLR);
}

void commonTimerEnableInterrupt(USS_SW_Library_configuration *config)
{
	HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) |=
				(CCIE);
}

void commonTimerDisableInterrupt(USS_SW_Library_configuration *config)
{
	 HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) &=
				~(CCIE);
}

bool commonTimerPollInterruptFlag(USS_SW_Library_configuration *config)
{
	if(HARDWAREWREG16(config->systemConfig->timerBaseAddress + OFS_TAxCCTL0) &
			CCIFG)
	{
		return true;
	}

	return false;
}

bool commonTimerConfigureTimer(USS_SW_Library_configuration *config)
{

	// Configure Timer to be used for delays
	uint16_t baseAdress = config->systemConfig->timerBaseAddress;
	HARDWAREWREG16(baseAdress + OFS_TAxCTL) =
		(TASSEL__ACLK | ID__1 | MC__STOP | TACLR);
	HARDWAREWREG16(baseAdress + OFS_TAxEX0) =
		(TAIDEX_0);
	HARDWAREWREG16(baseAdress + OFS_TAxCCTL0) =
			CCIE;

	// Check the oscillation fault flag if the flag is set then go to LPM3 mode
	// for 800 ms if there is fault in LFXT crystal then ACLK is being sourced by
	// LFMODCLK at ~37.5 KHz
	CSCTL0_H = CSKEY >> 8;
	CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG;

    if(SFRIFG1 & OFIFG)
	{
		do
		{
			commonTimerGenerateLowPowerDelay(config,4,
					USS_low_power_mode_option_low_power_mode_3);

			CSCTL5 &= ~LFXTOFFG;                // Clear XT1 fault flag
			SFRIFG1 &= ~OFIFG;

		}while ((SFRIFG1 & OFIFG));

	}
	CSCTL0_H = 0;

	return true;
}
