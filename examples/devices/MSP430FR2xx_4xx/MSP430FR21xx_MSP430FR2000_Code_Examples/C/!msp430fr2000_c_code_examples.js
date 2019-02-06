var jsonInfo = [
	{
		"MCU"    : "MSP430",
		"Device" : "MSP430FR2000",
		"Filter" : "MSP430FR200\\d"
	}
];

var jsonObjC = [
	{"SourcePreview" : "msp430fr211x_1.c",                                    "Description" : "Toggle P1.0 using software"},
	{"SourcePreview" : "msp430fr211x_CS_01.c",                                "Description" : "Configure MCLK for 8MHz sourced from DCO."},
	{"SourcePreview" : "msp430fr211x_CS_02.c",                                "Description" : "Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF"},
	{"SourcePreview" : "msp430fr211x_CS_03.c",                                "Description" : "Configure MCLK for 16MHz operation, and REFO sourcing FLLREF and ACLK"},
	{"SourcePreview" : "msp430fr211x_CS_07.c",                                "Description" : "Configure MCLK for 1MHz operation with DCO software trim"},
	{"SourcePreview" : "msp430fr211x_eCOMP_01.c",                             "Description" : "eCOMP output Toggle in LPM3; Vcompare is compared against internal reference"},
	{"SourcePreview" : "msp430fr211x_eCOMP_02.c",                             "Description" : "eCOMP output Toggle in LPM3, Vcompare is compared against the Vcc*1/2"},
	{"SourcePreview" : "msp430fr211x_eCOMP_03.c",                             "Description" : "eCOMP output Toggle, wake up from LPM3, Vcompare is compared against 1V"},
	{"SourcePreview" : "msp430fr211x_eCOMP_04.c",                             "Description" : "eCOMP Hysteresis, COUT Toggle in LPM4; Low speed and low power mode"},
	{"SourcePreview" : "msp430fr211x_eCOMP_06.c",                             "Description" : "eCOMP to compare two channel input voltage"},
	{"SourcePreview" : "msp430fr211x_euscia0_uart_05.c",                      "Description" : "eUSCI_A0 UART echo at 4800 baud using ACLK=32.768KHz"},
	{"SourcePreview" : "msp430fr211x_LPM3_01.c",                              "Description" : "Enters LPM3 with ACLK = XT1CLK = 32768Hz."},
	{"SourcePreview" : "msp430fr211x_LPM3_02.c",                              "Description" : "LPM3 entry with MCLK 16MHz"},
	{"SourcePreview" : "msp430fr211x_lpm3_5_01.c",                            "Description" : "LPM3.5, device enter LPM3.5 and toggles P1.0 with RTC interrupt handling every 1s"},
	{"SourcePreview" : "msp430fr211x_lpm4_5_01.c",                            "Description" : "LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.3"},
	{"SourcePreview" : "msp430fr211x_lpm4_5_02.c",                            "Description" : "LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.3 with SVS disabled"},
	{"SourcePreview" : "msp430fr211x_P1_01.c",                                "Description" : "Software Poll P1.3, Set P1.0 if P1.3 = 1"},
	{"SourcePreview" : "msp430fr211x_P1_03.c",                                "Description" : "Software Port Interrupt Service on P1.3 from LPM3"},
	{"SourcePreview" : "msp430fr211x_RTC_01.c",                               "Description" : "RTC, toggle P1.0 every 1s"},
	{"SourcePreview" : "msp430fr211x_RTC_02.c",                               "Description" : "RTC, device enter LPM3.5 and toggle P1.0 in RTC interrupt handling every 1s"},
	{"SourcePreview" : "msp430fr211x_RTC_03.c",                               "Description" : "RTC, using ACLK as clock source, toggle P1.0 every 1s"},
	{"SourcePreview" : "msp430fr211x_tb0_01.c",                               "Description" : "Timer0_B3, Toggle P1.0, CCR0 Count Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr211x_tb0_02.c",                               "Description" : "Timer0_B3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr211x_tb0_04.c",                               "Description" : "Timer0_B3, Toggle P1.0, Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr211x_tb0_16.c",                               "Description" : "Timer0_B3, PWM tb0.1-2, Up Mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr211x_tb0_17.c",                               "Description" : "Timer0_B3, PWM tb0.1-2, Up Mode, 32KHz ACLK"},
	{"SourcePreview" : "msp430fr211x_tb0_20.c",                               "Description" : "Timer0_B3, PWM tb0.1-2, Up/Down Mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr211x_tb0_22.c",                               "Description" : "Timer0_B3, Ultra-Low Pwr Pulse Accumulator"},
	{"SourcePreview" : "msp430fr211x_tb0_highimpedance.c",                    "Description" : "Timer0_B3, Output high-impedance state"},
	{"SourcePreview" : "msp430fr211x_tb0_shadow_register.c",                  "Description" : "Timer0_B3, Showcase double-buffered TBxCCRn feature"},
	{"SourcePreview" : "msp430fr211x_WDT_01.c",                               "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK"},
	{"SourcePreview" : "msp430fr211x_WDT_02.c",                               "Description" : "WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr211x_WDT_04.c",                               "Description" : "WDT,  Failsafe Clock, WDT mode, DCO SMCLK"},
	{"SourcePreview" : "msp430fr211x_WDT_05.c",                               "Description" : "WDT,  Reset on Invalid Address fetch, Toggle P1.0"},
	{"SourcePreview" : "msp430fr211x_WDT_06.c",                               "Description" : "WDT,  Failsafe Clock, WDT mode, 32kHz ACLK"},
	{"SourcePreview" : "msp430fr211x_WDT_07.c",                               "Description" : "WDT, WDT mode, use VLO as clock source"}
];

