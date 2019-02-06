var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430FR215x",
		"Filter" : "MSP430FR215\\d"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430fr235x_1.c",                         "Description" : "Toggle P1.0 using software"},
	{"SourcePreview" : "msp430fr235x_adc12_01.c",                  "Description" : "ADC, Sample A1, AVcc Ref, Set LED if A1 > 0.5*AVcc"},
	{"SourcePreview" : "msp430fr235x_adc12_02.c",                  "Description" : "ADC, Sample A1, internal 1.5V Ref, Set P1.0 if A1 > 0.5V"},
	{"SourcePreview" : "msp430fr235x_adc12_05.c",                  "Description" : "ADC, Lo_Batt, Set P1.0 if AVcc < 2.50V"},
	{"SourcePreview" : "msp430fr235x_adc12_10.c",                  "Description" : "ADC, Sample A2/A1/A0, internal 2V Ref."},
	{"SourcePreview" : "msp430fr235x_adc12_11.c",                  "Description" : "ADC, Sample A1, internal 1.5V Ref, TB1.1 Trig, Set P1.0 if A1>0.5V"},
	{"SourcePreview" : "msp430fr235x_adc12_16.c",                  "Description" : "ADC, Sample A12 Temp and Convert to oC and oF"},
	{"SourcePreview" : "msp430fr235x_adc12_21.c",                  "Description" : "ADC, Window Comparator, 2.5V ref, Timer trigger, DCO"},
	{"SourcePreview" : "msp430fr235x_CRC.c",                       "Description" : "CRC16, Compare CRC output with software-based algorithm"},
	{"SourcePreview" : "msp430fr235x_CS_01.c",                     "Description" : "Configure MCLK for 8MHz sourced from DCO."},
	{"SourcePreview" : "msp430fr235x_CS_02.c",                     "Description" : "Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF"},
	{"SourcePreview" : "msp430fr235x_CS_03.c",                     "Description" : "Configure MCLK for 16MHz operation, and REFO sourcing FLLREF and ACLK"},
	{"SourcePreview" : "msp430fr235x_CS_04.c",                     "Description" : "Output 32768Hz crystal on XT1 and observe failsafe"},
	{"SourcePreview" : "msp430fr235x_CS_05.c",                     "Description" : "Configure MCLK for 16MHz operation, and XT1 sourcing FLLREF and ACLK"},
	{"SourcePreview" : "msp430fr235x_CS_06.c",                     "Description" : "Configure MCLK for 4MHz and XT1HF sourcing MCLK"},
	{"SourcePreview" : "msp430fr235x_CS_07.c",                     "Description" : "Configure MCLK for 1MHz operation, and REFO sourcing FLLREF and ACLK. Use DCOFTRIM register to lock FLL."},
	{"SourcePreview" : "msp430fr235x_CS_08.c",                     "Description" : "Configure MCLK for 1MHz operation, low power REFO(LPREFO) sourcing FLLREF and ACLK. Use DCOFTRIM register to lock FLL."},
	{"SourcePreview" : "msp430fr235x_CS_09.c",                     "Description" : "Configure MCLK for 24MHz operation, and REFO sourcing FLLREF and ACLK."},
	{"SourcePreview" : "msp430fr235x_CS_10.c",                     "Description" : "Configure MCLK for 24MHz operation, and XT1 sourcing FLLREF and ACLK."},
	{"SourcePreview" : "msp430fr235x_eCOMP_01.c",                  "Description" : "eCOMP Toggle from LPM3; eCOMP in ultra low power mode, Vcompare is compared against internal VREF"},
	{"SourcePreview" : "msp430fr235x_eCOMP_02.c",                  "Description" : "eCOMP Toggle from LPM3; eCOMP in ultra low power mode, Vcompare is compared against Vcc*1/2."},
	{"SourcePreview" : "msp430fr235x_eCOMP_03.c",                  "Description" : "eCOMP output toggle, wake up from LPM3; Vcompare is compared against 1V."},
	{"SourcePreview" : "msp430fr235x_eCOMP_04.c",                  "Description" : "eCOMP Hysteresis, COUT Toggle in LPM4; low speed mode"},
	{"SourcePreview" : "msp430fr235x_eCOMP_05.c",                  "Description" : "eCOMP output connected to Timer_B capture, detect pulse width of eCOMP output."},
	{"SourcePreview" : "msp430fr235x_eCOMP_06.c",                  "Description" : "eCOMP Toggle from LPM3; eCOMP in ultra low power mode, compare two channels(P1.0, P1.1) input voltage."},
	{"SourcePreview" : "msp430fr235x_euscia0_spi_09.c",            "Description" : "eUSCI_A0, SPI 3-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430fr235x_euscia0_spi_10.c",            "Description" : "eUSCI_A0, SPI 3-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430fr235x_euscia0_spi_11.c",            "Description" : "eUSCI_A0, SPI 4-Wire Master Incremented Data"},
	{"SourcePreview" : "msp430fr235x_euscia0_spi_12.c",            "Description" : "eUSCI_A0, SPI 4-Wire Slave Data Echo"},
	{"SourcePreview" : "msp430fr235x_euscia0_uart_01.c",           "Description" : "eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz"},
	{"SourcePreview" : "msp430fr235x_euscia0_uart_03.c",           "Description" : "eUSCI_A0 External Loopback test @ 115200 baud"},
	{"SourcePreview" : "msp430fr235x_euscia0_uart_05.c",           "Description" : "eUSCI_A0 UART echo at 4800 baud using BRCLK = 32768Hz"},
	{"SourcePreview" : "msp430fr235x_euscib0_i2c_10.c",            "Description" : "eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave"},
	{"SourcePreview" : "msp430fr235x_euscib0_i2c_11.c",            "Description" : "eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master"},
	{"SourcePreview" : "msp430fr235x_euscib0_i2c_15.c",            "Description" : "eUSCI_B0 I2C Master TX bytes to Multiple Slaves"},
	{"SourcePreview" : "msp430fr235x_euscib0_i2c_16.c",            "Description" : "eUSCI_B0 I2C 4 Hardware I2C slaves"},
	{"SourcePreview" : "msp430fr235x_framwrite.c",                 "Description" : "Long word writes to FRAM"},
	{"SourcePreview" : "msp430fr235x_framwrite_persistent.c",      "Description" : "FRAM write, use #pragma PERSISTENT to initialize variable in info FRAM"},
	{"SourcePreview" : "msp430fr235x_ICC_01.c",                    "Description" : "ICC, Nested interrupt with software configured priority"},
	{"SourcePreview" : "msp430fr235x_LPM3_01.c",                   "Description" : "Enters LPM3 with ACLK = XT1CLK = 32768Hz"},
	{"SourcePreview" : "msp430fr235x_LPM3_03.c",                   "Description" : "Enters LPM3 with ACLK = VLO"},
	{"SourcePreview" : "msp430fr235x_lpm3_5_01.c",                 "Description" : "LPM3.5, device enter LPM3.5 and toggles P1.0 with RTC interrupt handling every 1s"},
	{"SourcePreview" : "msp430fr235x_lpm4_5_01.c",                 "Description" : "LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.3"},
	{"SourcePreview" : "msp430fr235x_lpm4_5_02.c",                 "Description" : "LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.3 with SVS disabled"},
	{"SourcePreview" : "msp430fr235x_P1_01.c",                     "Description" : "Software Poll P1.3, Set P1.0 if P1.3 = 1"},
	{"SourcePreview" : "msp430fr235x_P1_03.c",                     "Description" : "Software Port Interrupt Service on P1.3 from LPM3"},
	{"SourcePreview" : "msp430fr235x_pinosc_01.c",                 "Description" : "Capacitive Touch, Pin Oscillator Method, 1 button"},
	{"SourcePreview" : "msp430fr235x_pinosc_02.c",                 "Description" : "Capacitive Touch, Pin Oscillator Method, 4-buttons"},
	{"SourcePreview" : "msp430fr235x_RTC_01.c",                    "Description" : "RTC, toggle P1.0 every 1s"},
	{"SourcePreview" : "msp430fr235x_RTC_02.c",                    "Description" : "RTC, device enter LPM3.5 and toggle P1.0 in RTC interrupt handling every 1s"},
	{"SourcePreview" : "msp430fr235x_RTC_03.c",                    "Description" : "RTC, using ACLK as clock source, toggle P1.0 every 1s"},
	{"SourcePreview" : "msp430fr235x_tb0_01.c",                    "Description" : "Timer0_B3, Toggle P1.0, CCR0 Count Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_tb0_02.c",                    "Description" : "Timer0_B3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_tb0_04.c",                    "Description" : "Timer0_B3, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb0_16.c",                    "Description" : "Timer0_B3, PWM TB0.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_tb0_17.c",                    "Description" : "Timer0_B3, PWM TB0.1-2, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb0_20.c",                    "Description" : "Timer0_B3, PWM TB0.1-2, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb0_22.c",                    "Description" : "Timer0_B3, Ultra-Low Power Pulse Accumulator"},
	{"SourcePreview" : "msp430fr235x_tb0_capture.c",               "Description" : "Timer0_B3 Capture of ACLK"},
	{"SourcePreview" : "msp430fr235x_tb0_highimpedance.c",         "Description" : "Timer0_B3, Output high-impedance state"},
	{"SourcePreview" : "msp430fr235x_tb0_shadow_register.c",       "Description" : "Timer0_B3, PWM TB0.1-2, Up/Down Mode, 32kHz ACLK. Showcase double-buffered TBxCCRn feature."},
	{"SourcePreview" : "msp430fr235x_tb1_05.c",                    "Description" : "Timer1_B3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb1_06.c",                    "Description" : "Timer1_B3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb1_16.c",                    "Description" : "Timer1_B3, PWM TB1.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_tb1_17.c",                    "Description" : "Timer1_B3, PWM TB1.1-2, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr235x_tb3_01.c",                    "Description" : "Timer3_B7, PWM TB3.1-4, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr235x_WDT_01.c",                    "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_WDT_02.c",                    "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr235x_WDT_04.c",                    "Description" : "WDT,  Failsafe Clock, WDT mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr235x_WDT_05.c",                    "Description" : "WDT,  Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430fr235x_WDT_06.c",                    "Description" : "WDT,  Failsafe Clock, WDT mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr235x_WDT_07.c",                    "Description" : "WDT, WDT mode, use VLO as clock source"}
];

