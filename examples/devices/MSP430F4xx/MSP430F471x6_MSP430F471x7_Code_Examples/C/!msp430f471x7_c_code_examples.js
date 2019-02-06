var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430F471x7",
		"Filter" : "MSP430F471\\d7"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430x471x7_1.c",                                   "Description" : "Software Toggle P5.1"},
	{"SourcePreview" : "msp430x471x7_bt_01.c",                               "Description" : "Basic Timer, Toggle P5.1 Inside ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_bt_02.c",                               "Description" : "Basic Timer, Toggle P5.1 Inside ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_clks_02.c",                             "Description" : "FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO"},
	{"SourcePreview" : "msp430x471x7_compA_01.c",                            "Description" : "Comparator A, Poll input CA0, result in P5.1"},
	{"SourcePreview" : "msp430x471x7_compA_02.c",                            "Description" : "Comparator A, Poll input CA0, CA exchange, result in P5.1"},
	{"SourcePreview" : "msp430x471x7_compA_04.c",                            "Description" : "Comparator A, Poll input CA0, result in P5.1"},
	{"SourcePreview" : "msp430x471x7_compA_05.c",                            "Description" : "Comparator A, Poll input CA0, interrupt triggered"},
	{"SourcePreview" : "msp430x471x7_flashwrite_01.c",                       "Description" : "Flash In-System Programming, Copy SegC to SegD"},
	{"SourcePreview" : "msp430x471x7_flashwrite_03.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegC to SegD"},
	{"SourcePreview" : "msp430x471x7_flashwrite_04.c",                       "Description" : "Flash In-System Programming w/ EEI, Copy SegD to A/B/C"},
	{"SourcePreview" : "msp430x471x7_fll_01.c",                              "Description" : "FLL+, Runs Internal DCO at 2.45MHz"},
	{"SourcePreview" : "msp430x471x7_fll_02.c",                              "Description" : "FLL+, Runs Internal DCO at 16MHz"},
	{"SourcePreview" : "msp430x471x7_hfxt2.c",                               "Description" : "FLL+, MCLK Configured to Operate from XT2 HF XTAL"},
	{"SourcePreview" : "msp430x471x7_lcda_04.c",                             "Description" : "LCD_A Put \"0123456\" on SBLCDA4 LCD"},
	{"SourcePreview" : "msp430x471x7_lcda_05.c",                             "Description" : "LCD_A Put \"0123456\" on SBLCDA4 LCD with charge pump"},
	{"SourcePreview" : "msp430x471x7_lpm3.c",                                "Description" : "FLL+, LPM3 Using Basic Timer ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_OF_LFXT1_nmi.c",                        "Description" : "LFXT1 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430x471x7_OF_XT2.c",                              "Description" : "XT2 Oscillator Fault Detection"},
	{"SourcePreview" : "msp430x471x7_P1_01.c",                               "Description" : "Software Poll P2.7, Set P5.1 if P2.7 = 1"},
	{"SourcePreview" : "msp430x471x7_P1_02.c",                               "Description" : "Software Port Interrupt on P2.7 from LPM4"},
	{"SourcePreview" : "msp430x471x7_P1_05.c",                               "Description" : "Write a byte to Port 1"},
	{"SourcePreview" : "msp430x471x7_P7_05.c",                               "Description" : "Write a byte to Port 7"},
	{"SourcePreview" : "msp430x471x7_P8_05.c",                               "Description" : "Write a byte to Port 8"},
	{"SourcePreview" : "msp430x471x7_PA_05.c",                               "Description" : "Write a Word to Port A (Port7+Port8)"},
	{"SourcePreview" : "msp430x471x7_PB_05.c",                               "Description" : "Write a Word to Port B (Port9+Port10)"},
	{"SourcePreview" : "msp430x471x7_rtc_01.c",                              "Description" : "Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_rtc_02.c",                              "Description" : "Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK and send Time via UART"},
	{"SourcePreview" : "msp430x471x7_sd16_01.c",                             "Description" : "SD16, Continuous Conversion on a Group of 7 Channels"},
	{"SourcePreview" : "msp430x471x7_sd16_02.c",                             "Description" : "SD16, Single Conversion on a Group of 7 Channels"},
	{"SourcePreview" : "msp430x471x7_sd16_03.c",                             "Description" : "SD16, Continuous Conversion on a Single Channel"},
	{"SourcePreview" : "msp430x471x7_sd16_04.c",                             "Description" : "SD16, Single Conversion on Single Channel Polling IFG"},
	{"SourcePreview" : "msp430x471x7_sd16_05.c",                             "Description" : "SD16, Single Conversion on a Single Channel Using ISR"},
	{"SourcePreview" : "msp430x471x7_sd16_06.c",                             "Description" : "SD16, Using the Integrated Temperature Sensor"},
	{"SourcePreview" : "msp430x471x7_svs_01.c",                              "Description" : "SVS, POR @ 2.5V Vcc"},
	{"SourcePreview" : "msp430x471x7_svs_03.c",                              "Description" : "SVS, Toggle port 5.1 on Vcc < 2.8V"},
	{"SourcePreview" : "msp430x471x7_ta_01.c",                               "Description" : "Timer_A, Toggle P5.1, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_ta_02.c",                               "Description" : "Timer_A, Toggle P5.1, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_ta_03.c",                               "Description" : "Timer_A, Toggle P5.1, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_ta_04.c",                               "Description" : "Timer_A, Toggle P5.1, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_ta_05.c",                               "Description" : "Timer_A, Toggle P5.1, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_ta_16.c",                               "Description" : "Timer_A, PWM TA1-2 Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_ta_17.c",                               "Description" : "Timer_A, PWM TA1-2 Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_tb_01.c",                               "Description" : "Timer_B, Toggle P5.1, CCR0 Cont. Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_tb_02.c",                               "Description" : "Timer_B, Toggle P5.1, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_tb_03.c",                               "Description" : "Timer_B, Toggle P5.1, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_tb_04.c",                               "Description" : "Timer_B, Toggle P5.1, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_tb_05.c",                               "Description" : "Timer_B, Toggle P5.1, CCR0 Up Mode ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_tb_10.c",                               "Description" : "Timer_B, PWM TB1-2 Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_tb_11.c",                               "Description" : "Timer_B, PWM TB1-2 Up Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_uscia0_duplex_9600.c",                  "Description" : "USCI_A0, UART 9600 Full-Duplex Transceiver, 32K ACLK"},
	{"SourcePreview" : "msp430x471x7_uscia0_irda_01.c",                      "Description" : "USCI_A0 IrDA External Loopback Test, 4MHz SMCLK"},
	{"SourcePreview" : "msp430x471x7_uscia0_spi_09.c",                       "Description" : "USCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430x471x7_uscia0_spi_10.c",                       "Description" : "USCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430x471x7_uscia0_uart_115k.c",                    "Description" : "USCI_A0, 115200 UART Echo ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_uscia0_uart_115k_lpm.c",                "Description" : "USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM3"},
	{"SourcePreview" : "msp430x471x7_uscia0_uart_9600.c",                    "Description" : "USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_uscib0_i2c_08.c",                       "Description" : "USCI_B0 I2C Master TX multiple bytes to MSP430 Slave"},
	{"SourcePreview" : "msp430x471x7_uscib0_i2c_09.c",                       "Description" : "USCI_B0 I2C Slave RX multiple bytes from MSP430 Master"},
	{"SourcePreview" : "msp430x471x7_uscib0_i2c_10.c",                       "Description" : "USCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430x471x7_uscib0_i2c_11.c",                       "Description" : "USCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "msp430x471x7_uscib0_spi_01.c",                       "Description" : "USCI_B0, SPI Interface to TLC549 8-Bit ADC"},
	{"SourcePreview" : "msp430x471x7_usci_i2c_standard_master.c",            "Description" : "USCI_B0, I2C Master multiple byte TX/RX"},
	{"SourcePreview" : "msp430x471x7_usci_i2c_standard_slave.c",             "Description" : "USCI_B0, I2C Slave multiple byte TX/RX"},
	{"SourcePreview" : "msp430x471x7_usci_spi_standard_master.c",            "Description" : "USCI_A0, SPI 3-Wire Master multiple byte RX/TX"},
	{"SourcePreview" : "msp430x471x7_usci_spi_standard_slave.c",             "Description" : "USCI_A0, SPI 3-Wire Slave multiple byte RX/TX"},
	{"SourcePreview" : "msp430x471x7_usci_uart_standard_transceiver.c",      "Description" : "USCI_A0, UART Echo received character"},
	{"SourcePreview" : "msp430x471x7_wdt_01.c",                              "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_wdt_02.c",                              "Description" : "WDT, Toggle P5.1, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430x471x7_wdt_04.c",                              "Description" : "WDT+ Failsafe Clock, DCO SMCLK"},
	{"SourcePreview" : "msp430x471x7_wdt_05.c",                              "Description" : "Reset on Invalid Address fetch, Toggle P5.1"},
	{"SourcePreview" : "msp430x471x7_wdt_06.c",                              "Description" : "WDT+ Failsafe Clock, 32kHz ACLK"}
];
