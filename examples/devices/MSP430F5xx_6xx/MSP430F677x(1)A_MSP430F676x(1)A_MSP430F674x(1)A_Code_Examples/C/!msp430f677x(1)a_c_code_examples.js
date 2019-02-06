var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430F677x(1)A",
		"Filter" : "MSP430F677(\\d|\\d1)A"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430f677xA_adc10_01.c",                            "Description" : "ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc"},
	{"SourcePreview" : "msp430f677xA_adc10_02.c",                            "Description" : "ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V"},
	{"SourcePreview" : "msp430f677xA_adc10_03.c",                            "Description" : "ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C"},
	{"SourcePreview" : "msp430f677xA_adc10_04.c",                            "Description" : "ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc"},
	{"SourcePreview" : "msp430f677xA_adc10_05.c",                            "Description" : "ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V"},
	{"SourcePreview" : "msp430f677xA_adc10_06.c",                            "Description" : "ADC10, Internal Reference toggle"},
	{"SourcePreview" : "msp430f677xA_adc10_07.c",                            "Description" : "ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_10.c",                            "Description" : "ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_11.c",                            "Description" : "ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V"},
	{"SourcePreview" : "msp430f677xA_adc10_12.c",                            "Description" : "ADC10, Sample A5, 1.5V Ref, TA0.1 Trig, Ultra-Low Power"},
	{"SourcePreview" : "msp430f677xA_adc10_13.c",                            "Description" : "ADC10, DMA Sample A1 32x, 2.5V Ref, TA3 Trig, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_14.c",                            "Description" : "ADC10, DMA Sample A1-0 32x, AVcc, Repeat Seq, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_15.c",                            "Description" : "ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_21.c",                            "Description" : "ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO"},
	{"SourcePreview" : "msp430f677xA_adc10_22.c",                            "Description" : "ADC10, Sample A3, AVcc Ref, SD24_B Trig, Set P1.0 if A0 > 0.5*AVcc"},
	{"SourcePreview" : "msp430f677xA_aux_01.c",                              "Description" : "AUX, Software-Controlled Switching, AUXVCC1"},
	{"SourcePreview" : "msp430f677xA_aux_02.c",                              "Description" : "AUX, Software-Controlled Switching, AUXVCC2"},
	{"SourcePreview" : "msp430f677xA_aux_03.c",                              "Description" : "AUX, Hardware-Controlled Switching, AUXVCC1"},
	{"SourcePreview" : "msp430f677xA_aux_04.c",                              "Description" : "AUX, Hardware-Controlled Switching, AUXVCC2"},
	{"SourcePreview" : "msp430f677xA_dma_01.c",                              "Description" : "DMA0, Repeated Block to-from RAM, Software Trigger"},
	{"SourcePreview" : "msp430f677xA_dma_04.c",                              "Description" : "DMA0, Single Transfer Using ADC10 Triggered by TimerA"},
	{"SourcePreview" : "msp430f677xA_flashwrite_01.c",                       "Description" : "Flash In-System Programming, Single-Byte Copy SegC to SegD"},
	{"SourcePreview" : "msp430f677xA_flashwrite_02.c",                       "Description" : "Flash In-System Programming, Long-Word Write at 0x1800"},
	{"SourcePreview" : "msp430f677xA_lcdc_01.c",                             "Description" : "LCD_C, Display a Single Character"},
	{"SourcePreview" : "msp430f677xA_lcdc_02.c",                             "Description" : "LCD_C, Alternates Between Two Strings"},
	{"SourcePreview" : "msp430f677xA_lcdc_03.c",                             "Description" : "LCD_C, Display 6 digit numbers"},
	{"SourcePreview" : "msp430f677xA_lpm3_01.c",                             "Description" : "Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS disabled"},
	{"SourcePreview" : "msp430f677xA_lpm3_02.c",                             "Description" : "Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS default state"},
	{"SourcePreview" : "msp430f677xA_lpm3_03.c",                             "Description" : "Enters LPM3 with ACLK =VLO;  LFXT1, REF0 disabled, SVS default state"},
	{"SourcePreview" : "msp430f677xA_lpm4.c",                                "Description" : "Enters LPM4;  LFXT1, REF0 disabled, SVS disabled"},
	{"SourcePreview" : "msp430f677xA_of_lfxt1_nmi.c",                        "Description" : "LFXT1 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430f677xA_p1_01.c",                               "Description" : "Software Poll P1.4, Set P1.0 if P1.4 = 1"},
	{"SourcePreview" : "msp430f677xA_p1_02.c",                               "Description" : "Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled"},
	{"SourcePreview" : "msp430f677xA_p1_05.c",                               "Description" : "Write a byte to Port 1"},
	{"SourcePreview" : "msp430f677xA_portmap_01.c",                          "Description" : "Port Mapping, Port2; Single runtime configuration"},
	{"SourcePreview" : "msp430f677xA_portmap_02.c",                          "Description" : "Port Mapping, Single function to multiple pins; Single runtime configuration."},
	{"SourcePreview" : "msp430f677xA_portmap_03.c",                          "Description" : "Port Mapping, Port3; Single function to multiple pins; Multiple runtime configurations"},
	{"SourcePreview" : "msp430f677xA_rtc_01.c",                              "Description" : "RTC_C, Calendar Mode with Time Event and Alarm Interrupts"},
	{"SourcePreview" : "msp430f677xA_rtc_02.c",                              "Description" : "RTC_C, LPM3.5, & alarm"},
	{"SourcePreview" : "msp430f677xA_sd24b_01.c",                            "Description" : "SD24_B, Continuous Conversion on a Group of 3 Channels"},
	{"SourcePreview" : "msp430f677xA_sd24b_02.c",                            "Description" : "SD24_B, Single Conversion on a Group of 3 Channels"},
	{"SourcePreview" : "msp430f677xA_sd24b_03.c",                            "Description" : "SD24_B, Continuous Conversion on a Single Channel"},
	{"SourcePreview" : "msp430f677xA_sd24b_04.c",                            "Description" : "SD24_B, Single Conversion on Single Channel Polling IFG"},
	{"SourcePreview" : "msp430f677xA_sd24b_05.c",                            "Description" : "SD24_B, Single Conversion on a Single Channel Using ISR"},
	{"SourcePreview" : "msp430f677xA_sd24b_06.c",                            "Description" : "SD24_B, Single Conversion on a Single Channel Using ISR with 24 bit Result"},
	{"SourcePreview" : "msp430f677xA_sd24b_10.c",                            "Description" : "SD24_B, Continuous Conversion on a Group of 2 Channels"},
	{"SourcePreview" : "msp430f677xA_sd24b_11.c",                            "Description" : "SD24_B, Single Conversion on a Group of 2 Channels"},
	{"SourcePreview" : "msp430f677xA_ta0_02.c",                              "Description" : "Timer0_A0, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta0_04.c",                              "Description" : "Timer0_A0, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta0_16.c",                              "Description" : "Timer0_A0, PWM TA0.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta0_17.c",                              "Description" : "Timer0_A0, PWM TA0.1-2, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_01.c",                              "Description" : "Timer1_A1, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_02.c",                              "Description" : "Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_03.c",                              "Description" : "Timer1_A1, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_04.c",                              "Description" : "Timer1_A1, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_05.c",                              "Description" : "Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_11.c",                              "Description" : "Timer1_A1, Toggle P8.4/TA1.0, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_13.c",                              "Description" : "Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_14.c",                              "Description" : "Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_16.c",                              "Description" : "Timer1_A1, PWM TA1.1, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_17.c",                              "Description" : "Timer1_A1, PWM TA1.1, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta1_19.c",                              "Description" : "Timer1_A1, PWM TA1.1, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta1_20.c",                              "Description" : "Timer1_A1, PWM TA1.1, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta2_01.c",                              "Description" : "Timer2_A2, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta2_03.c",                              "Description" : "Timer2_A2, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ta2_08.c",                              "Description" : "Timer2_A2, Toggle P1.0;P8.6-7, Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta2_14.c",                              "Description" : "Timer2_A2, Toggle P8.6/TA2.0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_ta2_19.c",                              "Description" : "Timer2_A2, PWM TA2.1, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_ucs_1.c",                               "Description" : "UCS, Software Toggle P1.0 at Default DCO"},
	{"SourcePreview" : "msp430f677xA_ucs_2.c",                               "Description" : "UCS, Software Toggle P1.0 with 8MHz DCO"},
	{"SourcePreview" : "msp430f677xA_ucs_4.c",                               "Description" : "UCS, Software Toggle P1.0 with 2.45MHz DCO, LFXT1 as Ref"},
	{"SourcePreview" : "msp430f677xA_ucs_5.c",                               "Description" : "UCS, VLO sources ACLK. Toggles P1.0"},
	{"SourcePreview" : "msp430f677xA_ucs_6.c",                               "Description" : "UCS, XT1 sources ACLK. Toggles P1.0"},
	{"SourcePreview" : "msp430f677xA_uscia0_spi_09.c",                       "Description" : "USCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430f677xA_uscia0_spi_10.c",                       "Description" : "USCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430f677xA_uscia0_uart_03.c",                      "Description" : "USCI_A0, Ultra-Low Power UART 9600 Echo ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_uscib0_i2c_06.c",                       "Description" : "USCI_B0 I2C Slave RX single bytes from MSP430 Master"},
	{"SourcePreview" : "msp430f677xA_uscib0_i2c_07.c",                       "Description" : "USCI_B0 I2C Master TX single bytes to MSP430 Slave"},
	{"SourcePreview" : "msp430f677xA_uscib0_i2c_10.c",                       "Description" : "USCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430f677xA_uscib0_i2c_11.c",                       "Description" : "USCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "msp430f677xA_usci_i2c_standard_master.c",            "Description" : "USCI_B0, I2C Master multiple byte TX/RX"},
	{"SourcePreview" : "msp430f677xA_usci_i2c_standard_slave.c",             "Description" : "USCI_B0, I2C Slave multiple byte TX/RX"},
	{"SourcePreview" : "msp430f677xA_usci_spi_standard_master.c",            "Description" : "USCI_A0, SPI 3-Wire Master multiple byte RX/TX"},
	{"SourcePreview" : "msp430f677xA_usci_spi_standard_slave.c",             "Description" : "USCI_A0, SPI 3-Wire Slave multiple byte RX/TX"},
	{"SourcePreview" : "msp430f677xA_usci_uart_standard_transceiver.c",      "Description" : "USCI_A0, UART Echo received character"},
	{"SourcePreview" : "msp430f677xA_wdt_01.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_wdt_02.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430f677xA_wdt_04.c",                              "Description" : "WDT+, Failsafe Clock, WDT mode, DCO SMCLK"},
	{"SourcePreview" : "msp430f677xA_wdt_05.c",                              "Description" : "WDT+, Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430f677xA_wdt_06.c",                              "Description" : "WDT+, Failsafe Clock, 32kHz ACLK"}
];

