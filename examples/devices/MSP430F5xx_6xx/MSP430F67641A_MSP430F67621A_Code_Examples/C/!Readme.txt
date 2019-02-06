File Name                           Description
----------------------------------------------------------------------------------------
msp430f67641A_adc10_01.c            ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
msp430f67641A_adc10_02.c            ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V
msp430f67641A_adc10_03.c            ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C
msp430f67641A_adc10_04.c            ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc
msp430f67641A_adc10_05.c            ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
msp430f67641A_adc10_06.c            ADC10, Internal Reference toggle
msp430f67641A_adc10_07.c            ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
msp430f67641A_adc10_10.c            ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO
msp430f67641A_adc10_11.c            ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V
msp430f67641A_adc10_12.c            ADC10, Sample A5, 1.5V Ref, TA0.1 Trig, Ultra-Low Power
msp430f67641A_adc10_13.c            ADC10, DMA Sample A1 32x, 2.5V Ref, TA3 Trig, DCO
msp430f67641A_adc10_14.c            ADC10, DMA Sample A1-0 32x, AVcc, Repeat Seq, DCO
msp430f67641A_adc10_15.c            ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO
msp430f67641A_adc10_21.c            ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
msp430f67641A_adc10_22.c            ADC10, Sample A3, AVcc Ref, SD24_B Trig, Set P1.0 if A0 > 0.5*Avcc
msp430f67641A_dma_01.c              DMA0, Repeated Block to-from RAM, Software Trigger
msp430f67641A_dma_04.c              DMA0, Single Transfer Using ADC10 Triggered by TimerA
msp430f67641A_flashwrite_01.c       Flash In-System Programming, Single-Byte Copy SegC to SegD
msp430f67641A_flashwrite_02.c       Flash In-System Programming, Long-Word Write at 0x1800
msp430f67641A_lcdc_01.c             LCD_C, Display a Single Character
msp430f67641A_lcdc_02.c             LCD_C, Alternates Between Two Strings
msp430f67641A_lpm3_01.c             Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS disabled
msp430f67641A_lpm3_02.c             Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS default state
msp430f67641A_lpm3_03.c             Enters LPM3 with ACLK =VLO;  LFXT1, REF0 disabled, SVS default state
msp430f67641A_lpm4.c                Enters LPM4;  LFXT1, REF0 disabled, SVS disabled
msp430f67641A_of_lfxt1_nmi.c        LFXT1 Oscillator Fault Detection
msp430f67641A_p1_01.c               Software Poll P1.4, Set P1.0 if P1.4 = 1
msp430f67641A_p1_02.c               Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled
msp430f67641A_p1_05.c               Write a byte to Port 1
msp430f67641A_pa_05.c               Write a word to Port A (Port1+Port2)
msp430f67641A_portmap_01.c          Port Mapping, Port2; Single runtime configuration
msp430f67641A_portmap_02.c          Port Mapping, Single function to multiple pins; Single runtime configuration.
msp430f67641A_portmap_03.c          Port Mapping, Port3; Single function to multiple pins; Multiple runtime configurations
msp430f67641A_rtc_01.c              RTC_C, Calendar Mode with Time Event and Alarm Interrupts
msp430f67641A_rtc_02.c              RTC_C, LPM3.5, & alarm
msp430f67641A_sd24b_01.c            SD24_B, Continuous Conversion on a Group of 3 Channels
msp430f67641A_sd24b_02.c            SD24_B, Single Conversion on a Group of 3 Channels
msp430f67641A_sd24b_03.c            SD24_B, Continuous Conversion on a Single Channel
msp430f67641A_sd24b_04.c            SD24_B, Single Conversion on Single Channel Polling IFG
msp430f67641A_sd24b_05.c            SD24_B, Single Conversion on a Single Channel Using ISR
msp430f67641A_sd24b_06.c            SD24_B, Single Conversion on a Single Channel Using ISR with 24 bit Result
msp430f67641A_sd24b_10.c            SD24_B, Continuous Conversion on a Group of 2 Channels
msp430f67641A_sd24b_11.c            SD24_B, Single Conversion on a Group of 2 Channels
msp430f67641A_ta0_02.c              Timer0_A0, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f67641A_ta0_04.c              Timer0_A0, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f67641A_ta0_16.c              Timer0_A0, PWM TA0.1-2, Up Mode, DCO SMCLK
msp430f67641A_ta0_17.c              Timer0_A0, PWM TA0.1-2, Up Mode, 32kHz ACLK
msp430f67641A_ta1_01.c              Timer1_A1, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f67641A_ta1_02.c              Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f67641A_ta1_03.c              Timer1_A1, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f67641A_ta1_04.c              Timer1_A1, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f67641A_ta1_05.c              Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
msp430f67641A_ta1_11.c              Timer1_A1, Toggle P8.4/TA1.0, Up Mode, 32kHz ACLK
msp430f67641A_ta1_13.c              Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, DCO SMCLK
msp430f67641A_ta1_14.c              Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, 32kHz ACLK
msp430f67641A_ta1_16.c              Timer1_A1, PWM TA1.1, Up Mode, DCO SMCLK
msp430f67641A_ta1_17.c              Timer1_A1, PWM TA1.1, Up Mode, 32kHz ACLK
msp430f67641A_ta1_19.c              Timer1_A1, PWM TA1.1, Up/Down Mode, DCO SMCLK
msp430f67641A_ta1_20.c              Timer1_A1, PWM TA1.1, Up/Down Mode, 32kHz ACLK
msp430f67641A_ta2_01.c              Timer2_A2, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f67641A_ta2_03.c              Timer2_A2, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f67641A_ta2_08.c              Timer2_A2, Toggle P1.0;P8.6-7, Cont. Mode ISR, 32kHz ACLK
msp430f67641A_ta2_14.c              Timer2_A2, Toggle P8.6/TA2.0, Up/Down Mode, 32kHz ACLK
msp430f67641A_ta2_19.c              Timer2_A2, PWM TA2.1, Up/Down Mode, DCO SMCLK
msp430f67641A_ucs_1.c               UCS, Software Toggle P1.0 at Default DCO
msp430f67641A_ucs_2.c               UCS, Software Toggle P1.0 with 8MHz DCO
msp430f67641A_ucs_4.c               UCS, Software Toggle P1.0 with 2.45MHz DCO, LFXT1 as Ref
msp430f67641A_ucs_5.c               UCS, VLO sources ACLK. Toggles P1.0
msp430f67641A_ucs_6.c               UCS, XT1 sources ACLK. Toggles P1.0
msp430f67641A_uscia0_spi_09.c       USCI_A0, SPI 3-Wire Master Incremented Data
msp430f67641A_uscia0_spi_10.c       USCI_A0, SPI 3-Wire Slave Data Echo
msp430f67641A_uscia0_uart_03.c      USCI_A0, Ultra-Low Power UART 9600 Echo ISR, 32kHz ACLK
msp430f67641A_uscia0_uart_04.c      USCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
msp430f67641A_uscib0_i2c_06.c       USCI_B0 I2C Slave RX single bytes from MSP430 Master
msp430f67641A_uscib0_i2c_07.c       USCI_B0 I2C Master TX single bytes to MSP430 Slave
msp430f67641A_uscib0_i2c_10.c       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430f67641A_uscib0_i2c_11.c       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430f67641A_wdt_01.c              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430f67641A_wdt_02.c              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430f67641A_wdt_04.c              WDT+, Failsafe Clock, WDT mode, DCO SMCLK
msp430f67641A_wdt_05.c              WDT+, Reset on Invalid Address fetch, Toggle P1.0
msp430f67641A_wdt_06.c              WDT+, Failsafe Clock, 32kHz ACLK

