var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430F23x0",
		"Filter" : "MSP430F23\\d0"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430x23x0_1.c",                                   "Description" : "Software Toggle P1.0"},
	{"SourcePreview" : "msp430x23x0_1_vlo.c",                               "Description" : "Software Toggle P1.0, MCLK = VLO/8"},
	{"SourcePreview" : "msp430x23x0_ca_01.c",                               "Description" : "Comp_A+, Output Reference Voltages on P2.4"},
	{"SourcePreview" : "msp430x23x0_ca_02.c",                               "Description" : "Comp_A+, Detect Threshold, Set P1.0 if P2.4 > 0.25*Vcc"},
	{"SourcePreview" : "msp430x23x0_ca_03.c",                               "Description" : "Comp_A+, Simple 2.2V Low Battery Detect"},
	{"SourcePreview" : "msp430x23x0_clks.c",                                "Description" : "Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10"},
	{"SourcePreview" : "msp430x23x0_dco_flashcal.c",                        "Description" : "DCO Calibration Constants Programmer"},
	{"SourcePreview" : "msp430x23x0_flashwrite_01.c",                       "Description" : "Flash In-System Programming, Copy SegC to SegD"},
	{"SourcePreview" : "msp430x23x0_flashwrite_03.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegC to SegD"},
	{"SourcePreview" : "msp430x23x0_flashwrite_04.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegD to A/B/C"},
	{"SourcePreview" : "msp430x23x0_fll_01.c",                              "Description" : "Basic Clock, Implement Auto RSEL SW FLL"},
	{"SourcePreview" : "msp430x23x0_fll_02.c",                              "Description" : "Basic Clock, Implement Cont. SW FLL with Auto RSEL"},
	{"SourcePreview" : "msp430x23x0_hfxtal.c",                              "Description" : "Basic Clock, MCLK Sourced from HF XTAL"},
	{"SourcePreview" : "msp430x23x0_hfxtal_nmi.c",                          "Description" : "Basic Clock, LFXT1/MCLK Sourced from HF XTAL, NMI"},
	{"SourcePreview" : "msp430x23x0_lpm3.c",                                "Description" : "Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_lpm3_vlo.c",                            "Description" : "Basic Clock, LPM3 Using WDT ISR, VLO ACLK"},
	{"SourcePreview" : "msp430x23x0_nmi.c",                                 "Description" : "Basic Clock, Configure RST/NMI as NMI"},
	{"SourcePreview" : "msp430x23x0_p1_01.c",                               "Description" : "Software Poll P1.3, Set P1.0 if P1.3 = 1"},
	{"SourcePreview" : "msp430x23x0_p1_02.c",                               "Description" : "Software Port Interrupt Service on P1.3 from LPM4"},
	{"SourcePreview" : "msp430x23x0_p1_03.c",                               "Description" : "Poll P1 With Software with Internal Pull-up"},
	{"SourcePreview" : "msp430x23x0_p1_04.c",                               "Description" : "P1 Interrupt from LPM4 with Internal Pull-up"},
	{"SourcePreview" : "msp430x23x0_rosc.c",                                "Description" : "DCOCLK Biased with External Resistor Rosc"},
	{"SourcePreview" : "msp430x23x0_ta_01.c",                               "Description" : "Timer_A, Toggle P1.0, TACCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_02.c",                               "Description" : "Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_03.c",                               "Description" : "Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_04.c",                               "Description" : "Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_05.c",                               "Description" : "Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_06.c",                               "Description" : "Timer_A, Toggle P1.0, TACCR1 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_07.c",                               "Description" : "Timer_A, Toggle P1.0-3, Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_08.c",                               "Description" : "Timer_A, Toggle P1.0-3, Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_09.c",                               "Description" : "Timer_A, Toggle P1.0-3, Cont. Mode ISR, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_10.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_11.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_12.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_13.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_14.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_15.c",                               "Description" : "Timer_A, Toggle P1.1/TA0, Up/Down Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_16.c",                               "Description" : "Timer_A, PWM TA1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_17.c",                               "Description" : "Timer_A, PWM TA1-2, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_18.c",                               "Description" : "Timer_A, PWM TA1-2, Up Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_19.c",                               "Description" : "Timer_A, PWM TA1-2, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_ta_20.c",                               "Description" : "Timer_A, PWM TA1-2, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_21.c",                               "Description" : "Timer_A, PWM TA1-2, Up/Down Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_ta_22.c",                               "Description" : "Timer_A, Ultra-Low Pwr Pulse Accumulator"},
	{"SourcePreview" : "msp430x23x0_tb_01.c",                               "Description" : "Timer_B, Toggle P1.0, TBCCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_02.c",                               "Description" : "Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_03.c",                               "Description" : "Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_04.c",                               "Description" : "Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_05.c",                               "Description" : "Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_06.c",                               "Description" : "Timer_B, Toggle P1.0, TBCCR1 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_07.c",                               "Description" : "Timer_B, Toggle P4.0-2, Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_08.c",                               "Description" : "Timer_B, Toggle P4.0-2, Cont. Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_09.c",                               "Description" : "Timer_B, Toggle P4.0-2, Cont. Mode ISR, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_10.c",                               "Description" : "Timer_B, PWM TB1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_11.c",                               "Description" : "Timer_B, PWM TB1-2, Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_12.c",                               "Description" : "Timer_B, PWM TB1-2, Up Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_13.c",                               "Description" : "Timer_B, PWM TB1-2, Up/Down Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_tb_14.c",                               "Description" : "Timer_B, PWM TB1-2, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_tb_15.c",                               "Description" : "Timer_B, PWM TB1-2, Up/Down Mode, HF XTAL ACLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_irda_01.c",                      "Description" : "USCI_A0 IrDA External Loopback Test, 8MHz SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_irda_02.c",                      "Description" : "USCI_A0 IrDA Monitor, 8MHz SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_irda_03.c",                      "Description" : "USCI_A0 IrDA Physical Layer Comm, 8MHz SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_spi_01.c",                       "Description" : "USCI_A0, SPI Interface to HC164 Shift Register"},
	{"SourcePreview" : "msp430x23x0_uscia0_spi_02.c",                       "Description" : "USCI_A0, SPI Interface to HC165 Shift Register"},
	{"SourcePreview" : "msp430x23x0_uscia0_spi_03.c",                       "Description" : "USCI_A0, SPI Interface to HC165/164 Shift Registers"},
	{"SourcePreview" : "msp430x23x0_uscia0_spi_09.c",                       "Description" : "USCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430x23x0_uscia0_spi_10.c",                       "Description" : "USCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_01_115k.c",                 "Description" : "USCI_A0, 115200 UART Echo ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_01_115k_lpm.c",             "Description" : "USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM4"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_01_19200.c",                "Description" : "USCI_A0, 19200 UART Echo ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_01_19200_2.c",              "Description" : "USCI_A0, UART 19200 Echo ISR, XT2 HF XTAL SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_01_9600.c",                 "Description" : "USCI_A0, 9600 UART Echo ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_05_9600.c",                 "Description" : "USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_06_9600.c",                 "Description" : "USCI_A0, Ultra-Low Pwr UART 9600 String, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_07_9600.c",                 "Description" : "USCI_A0, Ultra-Low Pwr UART 9600 RX/TX, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_uscia0_uart_08_9600.c",                 "Description" : "USCI_A0, UART 9600 Full-Duplex Transceiver, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_01.c",                       "Description" : "USCI_B0 I2C Master to TMP100, Set P5.1 if Temp > 28C"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_02.c",                       "Description" : "USCI_B0 I2C Master Interface to PCF8574, Read/Write"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_03.c",                       "Description" : "USCI_B0 I2C Master Interface to DAC8571, Write"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_04.c",                       "Description" : "USCI_B0 I2C Master RX single bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_05.c",                       "Description" : "USCI_B0 I2C Slave TX single bytes to MSP430 Master"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_06.c",                       "Description" : "USCI_B0 I2C Master TX single bytes to MSP430 Slave"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_07.c",                       "Description" : "USCI_B0 I2C Slave RX single bytes from MSP430 Master"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_08.c",                       "Description" : "USCI_B0 I2C Master TX multiple bytes to MSP430 Slave"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_09.c",                       "Description" : "USCI_B0 I2C Slave RX multiple bytes from MSP430 Master"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_10.c",                       "Description" : "USCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430x23x0_uscib0_i2c_11.c",                       "Description" : "USCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "msp430x23x0_uscib0_spi_01.c",                       "Description" : "USCI_B0, SPI Interface to TLC549 8-Bit ADC"},
	{"SourcePreview" : "msp430x23x0_uscib0_spi_02.c",                       "Description" : "USCI_B0, SPI Interface to TLV1549 10-Bit ADC"},
	{"SourcePreview" : "msp430x23x0_usci_i2c_standard_master.c",            "Description" : "USCI_B0, I2C Master multiple byte TX/RX"},
	{"SourcePreview" : "msp430x23x0_usci_i2c_standard_slave.c",             "Description" : "USCI_B0, I2C Slave multiple byte TX/RX"},
	{"SourcePreview" : "msp430x23x0_usci_spi_standard_master.c",            "Description" : "USCI_A0, SPI 3-Wire Master multiple byte RX/TX"},
	{"SourcePreview" : "msp430x23x0_usci_spi_standard_slave.c",             "Description" : "USCI_A0, SPI 3-Wire Slave multiple byte RX/TX"},
	{"SourcePreview" : "msp430x23x0_usci_uart_standard_transceiver.c",      "Description" : "USCI_A0, UART Echo received character"},
	{"SourcePreview" : "msp430x23x0_wdt_01.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_wdt_02.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x23x0_wdt_04.c",                              "Description" : "WDT+ Failsafe Clock, DCO SMCLK"},
	{"SourcePreview" : "msp430x23x0_wdt_05.c",                              "Description" : "Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430x23x0_wdt_06.c",                              "Description" : "WDT+ Failsafe Clock, 32kHz ACLK"}
];

