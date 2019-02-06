var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430F261x",
		"Filter" : "MSP430F261[6-9]"
	}
];

var jsonObjC = [
	{"SourcePreview" : "MSP430x261x_1.c",                                   "Description" : "Software Toggle P1.0"},
	{"SourcePreview" : "MSP430x261x_1_vlo.c",                               "Description" : "Software Toggle P1.0, MCLK = VLO/8"},
	{"SourcePreview" : "MSP430x261x_adc12_01.c",                            "Description" : "ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc"},
	{"SourcePreview" : "MSP430x261x_adc12_02.c",                            "Description" : "ADC12, Using the Internal Reference"},
	{"SourcePreview" : "MSP430x261x_adc12_03.c",                            "Description" : "ADC12, Sample A10 Temp, Set P1.0 if Temp ++ ~2C"},
	{"SourcePreview" : "MSP430x261x_adc12_04.c",                            "Description" : "ADC12, Extend Sampling Period with SHT Bits"},
	{"SourcePreview" : "MSP430x261x_adc12_05.c",                            "Description" : "ADC12, Using an External Reference"},
	{"SourcePreview" : "MSP430x261x_adc12_06.c",                            "Description" : "ADC12, Repeated Sequence of Conversions"},
	{"SourcePreview" : "MSP430x261x_adc12_07.c",                            "Description" : "ADC12, Repeated Single Channel Conversions"},
	{"SourcePreview" : "MSP430x261x_adc12_08.c",                            "Description" : "ADC12, Using 10 External Channels for Conversion"},
	{"SourcePreview" : "MSP430x261x_adc12_09.c",                            "Description" : "ADC12, Sequence of Conversions (non-repeated)"},
	{"SourcePreview" : "MSP430x261x_adc12_10.c",                            "Description" : "ADC12, Sample A10 Temp and Convert to oC and oF"},
	{"SourcePreview" : "MSP430x261x_adc12_11.c",                            "Description" : "ADC12, Single Channel Extended Sample, TA1 Trigger"},
	{"SourcePreview" : "MSP430x261x_clks.c",                                "Description" : "Basic Clock, Output Buffered SMCLK, ACLK, and MCLK"},
	{"SourcePreview" : "MSP430x261x_compA_01.c",                            "Description" : "Comparator A, Poll input CA0, result in P1.0"},
	{"SourcePreview" : "MSP430x261x_compA_02.c",                            "Description" : "Comparator A, Poll input CA0, CA exchange, result in P1.0"},
	{"SourcePreview" : "MSP430x261x_compA_04.c",                            "Description" : "Comparator A, Poll input CA0, result in P1.0"},
	{"SourcePreview" : "MSP430x261x_compA_05.c",                            "Description" : "Comparator A, Input to CA0, interrupt triggered"},
	{"SourcePreview" : "MSP430x261x_dac12_01.c",                            "Description" : "DAC12_0, Output 1.0V on DAC0"},
	{"SourcePreview" : "MSP430x261x_dac12_02.c",                            "Description" : "DAC12_0, Output 2.0V on DAC1"},
	{"SourcePreview" : "MSP430x261x_dac12_03.c",                            "Description" : "DAC12_0, Output Voltage Ramp on DAC0"},
	{"SourcePreview" : "MSP430x261x_dco_flashcal.c",                        "Description" : "DCO Calibration Constants Programmer"},
	{"SourcePreview" : "MSP430x261x_dma_01.c",                              "Description" : "DMA0, Repeated Burst to-from RAM, Software Trigger"},
	{"SourcePreview" : "MSP430x261x_dma_02.c",                              "Description" : "DMA0, Repeated single transfer to P1OUT, TACCR2 Trigger"},
	{"SourcePreview" : "MSP430x261x_dma_03.c",                              "Description" : "DMA0, Repeated single transfer UCA1UART 9600, TACCR2, ACLK"},
	{"SourcePreview" : "MSP430x261x_dma_04.c",                              "Description" : "DMA0, single transfer Mode UART1 9600, ACLK"},
	{"SourcePreview" : "MSP430x261x_dma_05.c",                              "Description" : "DMA0, Repeated single transfer to DAC0, Sine Output, TACCR1, DCO"},
	{"SourcePreview" : "MSP430x261x_dma_06.c",                              "Description" : "DMA2, Rpt'd single transfer to DAC1, 8-Bit Sine, TBCCR2, DCO"},
	{"SourcePreview" : "MSP430x261x_dma_07.c",                              "Description" : "DMA0/1, Rpt'd single transfer to DAC12_0/1, Sin/Cos, TACCR1, XT2"},
	{"SourcePreview" : "MSP430x261x_dma_09.c",                              "Description" : "DMA0, ADC12 A10 single Block Xfer to RAM, TBCCR1, DCO"},
	{"SourcePreview" : "MSP430x261x_dma_10.c",                              "Description" : "DMA0, ADC12 A10 Block Xfer to Flash, TBCCR1, DCO"},
	{"SourcePreview" : "MSP430x261x_dma_11.c",                              "Description" : "DMA0/1, ADC12 A10 rpt single transfer Xfer to MPY/RAM, TBCCR1, DCO"},
	{"SourcePreview" : "MSP430x261x_dma_12.c",                              "Description" : "DMA0/1, rpt single transfer Mode UART1 9600 Auto RX/TX String, ACLK"},
	{"SourcePreview" : "MSP430x261x_flashwrite_01.c",                       "Description" : "Flash In-System Programming, Copy SegC to SegD"},
	{"SourcePreview" : "MSP430x261x_flashwrite_03.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegC to SegD"},
	{"SourcePreview" : "MSP430x261x_flashwrite_04.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegD to B&C"},
	{"SourcePreview" : "MSP430x261x_fll_01.c",                              "Description" : "Basic Clock, Implement Auto RSEL SW FLL"},
	{"SourcePreview" : "MSP430x261x_fll_02.c",                              "Description" : "Basic Clock, Implement Cont. SW FLL with Auto RSEL"},
	{"SourcePreview" : "MSP430x261x_hfxt2.c",                               "Description" : "Basic Clock, MCLK Configured to Operate from XT2 HF XTAL"},
	{"SourcePreview" : "MSP430x261x_hfxt2_nmi.c",                           "Description" : "Basic Clock, MCLK Sourced from HF XTAL XT2, NMI"},
	{"SourcePreview" : "MSP430x261x_lpm3.c",                                "Description" : "Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_lpm3_vlo.c",                            "Description" : "Basic Clock, LPM3 Using WDT ISR, VLO ACLK"},
	{"SourcePreview" : "MSP430x261x_nmi.c",                                 "Description" : "Configure RST/NMI as NMI"},
	{"SourcePreview" : "MSP430x261x_OF_XT2.c",                              "Description" : "XT2 Oscillator Fault Detection"},
	{"SourcePreview" : "MSP430x261x_P1_01.c",                               "Description" : "Software Poll P1.3, Set P1.0 if P1.3 = 1"},
	{"SourcePreview" : "MSP430x261x_P1_02.c",                               "Description" : "Software Port Interrupt Service on P1.3 from LPM4"},
	{"SourcePreview" : "MSP430x261x_P1_05.c",                               "Description" : "Write a byte to Port 1"},
	{"SourcePreview" : "MSP430x261x_P7_05.c",                               "Description" : "Write a byte to Port 7"},
	{"SourcePreview" : "MSP430x261x_P8_05.c",                               "Description" : "Write a byte to Port 8"},
	{"SourcePreview" : "MSP430x261x_PA_05.c",                               "Description" : "Write a word to Port A"},
	{"SourcePreview" : "MSP430x261x_rosc.c",                                "Description" : "DCOCLK Biased with External Resistor Rosc"},
	{"SourcePreview" : "MSP430x261x_svs_01.c",                              "Description" : "SVS, POR @ 2.5V Vcc"},
	{"SourcePreview" : "MSP430x261x_ta_01.c",                               "Description" : "Timer_A, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_02.c",                               "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_03.c",                               "Description" : "Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_04.c",                               "Description" : "Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_05.c",                               "Description" : "Timer_A, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_08.c",                               "Description" : "Timer_A, Toggle P1.0,P1.2 & P2.0 Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_11.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_13.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_14.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_16.c",                               "Description" : "Timer_A, PWM TA1-2 Up Mode, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_17.c",                               "Description" : "Timer_A, PWM TA1-2, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_ta_19.c",                               "Description" : "Timer_A, PWM TA1-2, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_ta_20.c",                               "Description" : "Timer_A, PWM TA1-2, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_tb_01.c",                               "Description" : "Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_tb_02.c",                               "Description" : "Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_tb_03.c",                               "Description" : "Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_tb_04.c",                               "Description" : "Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_tb_05.c",                               "Description" : "Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_tb_07.c",                               "Description" : "Timer_B, PWM TB1-6, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_uscia0_irda_01.c",                      "Description" : "USCI_A0 IrDA External Loopback Test, 8MHz SMCLK"},
	{"SourcePreview" : "MSP430x261x_uscia0_spi_09.c",                       "Description" : "USCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "MSP430x261x_uscia0_spi_10.c",                       "Description" : "USCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "MSP430x261x_uscia0_uart_01.c",                      "Description" : "UUSCI_A0, 115200 UART Echo ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_uscia0_uart_03.c",                      "Description" : "USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_uscia0_uart_04.c",                      "Description" : "UUSCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling"},
	{"SourcePreview" : "MSP430x261x_uscia1_uart_02.c",                      "Description" : "USCI_A1, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_04.c",                       "Description" : "USCI_B0 I2C Master RX single bytes from MSP430 Slave"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_05.c",                       "Description" : "USCI_B0 I2C Slave TX single bytes to MSP430 Master"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_06.c",                       "Description" : "USCI_B0 I2C Master TX single bytes to MSP430 Slave"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_07.c",                       "Description" : "USCI_B0 I2C Slave RX single bytes from MSP430 Master"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_08.c",                       "Description" : "USCI_B0 I2C Master TX multiple bytes to MSP430 Slave"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_09.c",                       "Description" : "USCI_B0 I2C Slave RX multiple bytes from MSP430 Master"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_10.c",                       "Description" : "USCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_11.c",                       "Description" : "USCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "MSP430x261x_uscib0_i2c_16.c",                       "Description" : "USCI_B0 to USCI_B1 I2C RX and TX single bytes"},
	{"SourcePreview" : "MSP430x261x_uscib0_spi_09.c",                       "Description" : "USCI_B0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "MSP430x261x_uscib0_spi_10.c",                       "Description" : "USCI_B0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "MSP430x261x_usci_i2c_standard_master.c",            "Description" : "USCI_B0, I2C Master multiple byte TX/RX"},
	{"SourcePreview" : "MSP430x261x_usci_i2c_standard_slave.c",             "Description" : "USCI_B0, I2C Slave multiple byte TX/RX"},
	{"SourcePreview" : "MSP430x261x_usci_spi_standard_master.c",            "Description" : "USCI_A0, SPI 3-Wire Master multiple byte RX/TX"},
	{"SourcePreview" : "MSP430x261x_usci_spi_standard_slave.c",             "Description" : "USCI_A0, SPI 3-Wire Slave multiple byte RX/TX"},
	{"SourcePreview" : "MSP430x261x_usci_uart_standard_transceiver.c",      "Description" : "USCI_A0, UART Echo received character"},
	{"SourcePreview" : "MSP430x261x_vlo_capture.c",                         "Description" : "Basic Clock, VLO-Driven Timer with VLO Compensation"},
	{"SourcePreview" : "MSP430x261x_wdt_01.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_wdt_02.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "MSP430x261x_wdt_04.c",                              "Description" : "WDT+ Failsafe Clock, WDT mode, DCO SMCLK"},
	{"SourcePreview" : "MSP430x261x_wdt_05.c",                              "Description" : "Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "MSP430x261x_wdt_06.c",                              "Description" : "WDT+ Failsafe Clock, 32kHz ACLK"}
];

