var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430FR6x8x",
		"Filter" : "MSP430FR6(9|8)8(7|8|9)(|1)"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430fr69xx_1.c",                                    "Description" : "Blink LED"},
	{"SourcePreview" : "msp430fr69xx_active_01.c",                            "Description" : "Active mode test"},
	{"SourcePreview" : "msp430fr69xx_active_02.c",                            "Description" : "Active Mode Test with 100% CHR"},
	{"SourcePreview" : "msp430fr69xx_adc12_01.c",                             "Description" : "ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc"},
	{"SourcePreview" : "msp430fr69xx_adc12_02.c",                             "Description" : "ADC12, Using the Internal Reference"},
	{"SourcePreview" : "msp430fr69xx_adc12_03.c",                             "Description" : "Sample A2-A3 Differential,Set P1.0 if i/p> 1V"},
	{"SourcePreview" : "msp430fr69xx_adc12_05.c",                             "Description" : "ADC12, Using an External Reference"},
	{"SourcePreview" : "msp430fr69xx_adc12_10.c",                             "Description" : "ADC12, Sample A10 Temp and Convert to oC and oF"},
	{"SourcePreview" : "msp430fr69xx_adc12_21.c",                             "Description" : "ADC12, Window Comparator, 2.5V ref"},
	{"SourcePreview" : "msp430fr69xx_adc12_22.c",                             "Description" : "ADC12, Window Comparator, 2.5V ref, 2's Complement"},
	{"SourcePreview" : "msp430fr69xx_compe_01.c",                             "Description" : "COMPE output Toggle in LPM4; input channel CE1"},
	{"SourcePreview" : "msp430fr69xx_compe_02.c",                             "Description" : "COMPE output Toggle in LPM4; input channel CE12"},
	{"SourcePreview" : "msp430fr69xx_compe_04.c",                             "Description" : "COMPE output Toggle in LPM4, Vcompare is compared against the Vcc*1/2"},
	{"SourcePreview" : "msp430fr69xx_compe_05.c",                             "Description" : "COMPE Hysteresis, COUT Toggle in LPM4; High speed mode"},
	{"SourcePreview" : "msp430fr69xx_crc.c",                                  "Description" : "Calculate CRC and compare"},
	{"SourcePreview" : "msp430fr69xx_cs_01.c",                                "Description" : "Configure MCLK for 8MHz operation"},
	{"SourcePreview" : "msp430fr69xx_cs_02.c",                                "Description" : "Configure MCLK for 16MHz operation"},
	{"SourcePreview" : "msp430fr69xx_cs_03.c",                                "Description" : "Output 32768Hz crystal on XT1 and observe failsafe"},
	{"SourcePreview" : "msp430fr69xx_cs_04.c",                                "Description" : "ACLK = XT1 = 32768Hz, SMCLK= XT2 = 8MHz, MCLK = DCO"},
	{"SourcePreview" : "msp430fr69xx_cs_05.c",                                "Description" : "Using LFXT in bypass mode, failsafe operation shown"},
	{"SourcePreview" : "msp430fr69xx_dma_01.c",                               "Description" : "DMA0, Repeated Block to-from RAM, Software Trigger"},
	{"SourcePreview" : "msp430fr69xx_esi_01.c",                               "Description" : "ESI Module example"},
	{"SourcePreview" : "msp430fr69xx_euscia0_spi_09.c",                       "Description" : "eUSCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430fr69xx_euscia0_spi_10.c",                       "Description" : "eUSCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430fr69xx_euscia0_spi_11.c",                       "Description" : "eUSCI_A0, SPI 4-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430fr69xx_euscia0_spi_12.c",                       "Description" : "eUSCI_A0, SPI 4-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430fr69xx_euscia0_uart_01.c",                      "Description" : "eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz"},
	{"SourcePreview" : "msp430fr69xx_euscia0_uart_02.c",                      "Description" : "eUSCI_A0 UART echo at 9600 baud using BRCLK = 32768Hz"},
	{"SourcePreview" : "msp430fr69xx_euscia0_uart_03.c",                      "Description" : "USCI_A0 External Loopback test @ 115200 baud"},
	{"SourcePreview" : "msp430fr69xx_euscia0_uart_04.c",                      "Description" : "eUSCI_A0 External Loopback test"},
	{"SourcePreview" : "msp430fr69xx_euscib0_i2c_10.c",                       "Description" : "eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430fr69xx_euscib0_i2c_11.c",                       "Description" : "eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "msp430fr69xx_euscib0_i2c_master_multislave.c",        "Description" : "eUSCI_B0 I2C Master TX bytes to Multiple Slaves"},
	{"SourcePreview" : "msp430fr69xx_euscib0_i2c_multislave.c",               "Description" : "eUSCI_B0 I2C 4 Hardware I2C slaves"},
	{"SourcePreview" : "msp430fr69xx_eusci_i2c_standard_master.c",            "Description" : "eUSCI_B0, I2C Master multiple byte TX/RX"},
	{"SourcePreview" : "msp430fr69xx_eusci_i2c_standard_slave.c",             "Description" : "eUSCI_B0, I2C Slave multiple byte TX/RX"},
	{"SourcePreview" : "msp430fr69xx_eusci_spi_standard_master.c",            "Description" : "eUSCI_A0, SPI 3-Wire Master multiple byte RX/TX"},
	{"SourcePreview" : "msp430fr69xx_eusci_spi_standard_slave.c",             "Description" : "eUSCI_A0, SPI 3-Wire Slave multiple byte RX/TX"},
	{"SourcePreview" : "msp430fr69xx_eusci_uart_standard_transceiver.c",      "Description" : "eUSCI_A0, UART Echo received character"},
	{"SourcePreview" : "msp430fr69xx_framwrite.c",                            "Description" : "Long word writes to FRAM"},
	{"SourcePreview" : "msp430fr69xx_lcdc_01.c",                              "Description" : "LCD_C, 4-Mux example, display \"430\" string on the LCD."},
	{"SourcePreview" : "msp430fr69xx_lcdc_02.c",                              "Description" : "LCD_C, 4-Mux example, display \"430\" string on the LCD and blinks the string every 1Hz."},
	{"SourcePreview" : "msp430fr69xx_lcdc_03.c",                              "Description" : "LCD_C, RTC_C, Display real time clock on LCD in LPM3 mode."},
	{"SourcePreview" : "msp430fr69xx_lpm0.c",                                 "Description" : "Enters LPM0 with ACLK = LFXT, MCLK = 1MHz"},
	{"SourcePreview" : "msp430fr69xx_lpm3-5_01.c",                            "Description" : "RTC_C, LPM3.5, & alarm"},
	{"SourcePreview" : "msp430fr69xx_lpm3_02.c",                              "Description" : "Enters LPM3 with ACLK = VLO"},
	{"SourcePreview" : "msp430fr69xx_lpm3_03.c",                              "Description" : "Enters LPM3 with ACLK = LFXT1"},
	{"SourcePreview" : "msp430fr69xx_lpm4-5_01.c",                            "Description" : "Entering and waking up from LPM4.5 via P1.3 interrupt"},
	{"SourcePreview" : "msp430fr69xx_lpm4-5_02.c",                            "Description" : "Entering and waking up from LPM4.5 via P1.3 interrupt with SVS disabled"},
	{"SourcePreview" : "msp430fr69xx_lpm4.c",                                 "Description" : "Enters LPM4"},
	{"SourcePreview" : "msp430fr69xx_mpu_01.c",                               "Description" : "MPU write protection violation - interrupt"},
	{"SourcePreview" : "msp430fr69xx_mpu_02.c",                               "Description" : "MPU Write protection violation - PUC"},
	{"SourcePreview" : "msp430fr69xx_of_lfxt1_nmi.c",                         "Description" : "LFXT Oscillator Fault Detection"},
	{"SourcePreview" : "msp430fr69xx_p1_01.c",                                "Description" : "Software Poll P1.4, Set P1.0 if P1.4 = 1"},
	{"SourcePreview" : "msp430fr69xx_p1_03.c",                                "Description" : "Port Interrupt Service on P1.4 from LPM4"},
	{"SourcePreview" : "msp430fr69xx_rtc_01.c",                               "Description" : "RTC in Counter Mode toggles P1.0 every 1s"},
	{"SourcePreview" : "msp430fr69xx_rtc_02.c",                               "Description" : "RTC in real time clock mode"},
	{"SourcePreview" : "msp430fr69xx_ta0_01.c",                               "Description" : "Timer0_A3, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_ta0_02.c",                               "Description" : "Timer0_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_ta0_04.c",                               "Description" : "Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_ta0_16.c",                               "Description" : "Timer0_A3, PWM TA0.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_ta0_17.c",                               "Description" : "Timer0_A3, PWM TA0.1-2, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_ta0_capture.c",                          "Description" : "Timer0_A3 Capture of VLO Period using DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_ta1_05.c",                               "Description" : "Timer1_A3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_ta1_06.c",                               "Description" : "Timer1_A3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_ta1_16.c",                               "Description" : "Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_ta1_17.c",                               "Description" : "Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_01.c",                               "Description" : "TimerB, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_02.c",                               "Description" : "TimerB, Toggle P1.0, CCR0 UP Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_03.c",                               "Description" : "TimerB, Toggle P1.0, Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_04.c",                               "Description" : "TimerB, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_06.c",                               "Description" : "TimerB, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_tb0_16.c",                               "Description" : "TimerB, PWM TB0.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_wdt_01.c",                               "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_wdt_02.c",                               "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr69xx_wdt_04.c",                               "Description" : "WDT+ Failsafe Clock, WDT mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr69xx_wdt_05.c",                               "Description" : "Reset on Invalid Address fetch, Toggle P1.0"}
];

