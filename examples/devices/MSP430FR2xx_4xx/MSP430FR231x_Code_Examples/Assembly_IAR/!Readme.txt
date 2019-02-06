File Name                             Description
----------------------------------------------------------------------------------------
msp430fr231x_1.s43                    Toggle P1.0 using software
msp430fr231x_adc10_01.s43             ADC, Sample A1, AVcc Ref, Set LED if A1 > 0.5*AVcc
msp430fr231x_adc10_02.s43             ADC, Sample A1, internal 1.5V Ref, Set P1.0 if A1 > 0.5V
msp430fr231x_adc10_10.s43             ADC, Sample A2/A1/A0, internal 1.5V Ref.
msp430fr231x_adc10_11.s43             ADC, Sample A1, internal 1.5V Ref, tb0.1 Trig, Set P1.0 if A1>0.5V
msp430fr231x_adc10_21.s43             ADC, Window Comparator, 1.5V ref, Timer trigger, DCO
msp430fr231x_CRC.s43                  CRC16, Compare CRC output with software-based algorithm
msp430fr231x_CS_01.s43                Configure MCLK for 8MHz sourced from DCO.
msp430fr231x_CS_02.s43                Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF
msp430fr231x_CS_03.s43                Configure MCLK for 16MHz operation, and REFO sourcing FLLREF and ACLK
msp430fr231x_CS_05.s43                Configure MCLK for 16MHz operation, and XT1 sourcing FLLREF and ACLK
msp430fr231x_CS_06.s43                Configure MCLK for 4MHz and XT1HF sourcing MCLK
msp430fr231x_CS_07.s43                Configure MCLK for 1MHz and use DCO software trim to lock FLL
msp430fr231x_eCOMP_01.s43             eCOMP output Toggle in LPM3; Vcompare is compared against internal reference
msp430fr231x_euscia0_uart_01.s43      eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
msp430fr231x_euscia0_uart_03.s43      eUSCI_A0 External Loopback test @ 115200 baud
msp430fr231x_euscia0_uart_05.s43      eUSCI_A0 UART echo at 4800 baud using ACLK=32.768KHz
msp430fr231x_framwrite.s43            Long word writes to FRAM
msp430fr231x_lpm3_01.s43              Enters LPM3 with ACLK = XT1CLK = 32768Hz.
msp430fr231x_P1_01.s43                Software Poll P1.3, Set P1.0 if P1.3 = 1
msp430fr231x_P1_03.s43                Software Port Interrupt Service on P1.3 from LPM3
msp430fr231x_SAC_L1_01.s43            SAC0, General-Purpose Mode
msp430fr231x_tb0_01.s43               Timer0_B3, Toggle P1.0, CCR0 Count Mode ISR, DCO SMCLK
msp430fr231x_tb0_02.s43               Timer0_B3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430fr231x_tb0_04.s43               Timer0_B3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr231x_tb0_16.s43               Timer0_B3, PWM tb0.1-2, Up Mode, DCO SMCLK
msp430fr231x_tb0_17.s43               Timer0_B3, PWM tb0.1-2, Up Mode, 32KHz ACLK
msp430fr231x_tb0_20.s43               Timer0_B3, PWM tb0.1-2, Up/Down Mode, 32kHz ACLK
msp430fr231x_tb0_22.s43               Timer0_B3, Ultra-Low Pwr Pulse Accumulator
msp430fr231x_tb0_capture.s43          Timer0_B3 Capture of ACLK
msp430fr231x_tb1_05.s43               Timer1_B3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK
msp430fr231x_tb1_06.s43               Timer1_B3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr231x_tb1_16.s43               Timer1_B3, PWM tb1.1-2, Up Mode, DCO SMCLK
msp430fr231x_tb1_17.s43               Timer1_B3, PWM tb1.1-2, Up Mode, 32KHz ACLK
msp430fr231x_TRI_01.s43               TRI0, General-Purpose Mode
msp430fr231x_uscia0_spi_09.s43        eUSCI_A0, SPI 3-Wire Master Incremented Data
msp430fr231x_uscia0_spi_10.s43        eUSCI_A0, SPI 3-Wire Slave Data Echo
msp430fr231x_uscia0_spi_11.s43        eUSCI_A0, SPI 4-Wire Master Incremented Data
msp430fr231x_uscia0_spi_12.s43        eUSCI_A0, SPI 4-Wire Slave Data Echo
msp430fr231x_WDT_01.s43               WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430fr231x_WDT_02.s43               WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430fr231x_WDT_04.s43               WDT,  Failsafe Clock, WDT mode, DCO SMCLK
msp430fr231x_WDT_05.s43               WDT,  Reset on Invalid Address fetch, Toggle P1.0
msp430fr231x_WDT_06.s43               WDT,  Failsafe Clock, WDT mode, 32kHz ACLK

