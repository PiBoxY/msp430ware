File Name                                          Description
----------------------------------------------------------------------------------------
msp430x471x7_1.c                                   Software Toggle P5.1
msp430x471x7_bt_01.c                               Basic Timer, Toggle P5.1 Inside ISR, 32kHz ACLK
msp430x471x7_bt_02.c                               Basic Timer, Toggle P5.1 Inside ISR, DCO SMCLK
msp430x471x7_clks_02.c                             FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
msp430x471x7_compA_01.c                            Comparator A, Poll input CA0, result in P5.1
msp430x471x7_compA_02.c                            Comparator A, Poll input CA0, CA exchange, result in P5.1
msp430x471x7_compA_04.c                            Comparator A, Poll input CA0, result in P5.1
msp430x471x7_compA_05.c                            Comparator A, Poll input CA0, interrupt triggered
msp430x471x7_flashwrite_01.c                       Flash In-System Programming, Copy SegC to SegD
msp430x471x7_flashwrite_03.c                       Flash In-System Programming w/ EEI, Copy SegC to SegD
msp430x471x7_flashwrite_04.c                       Flash In-System Programming w/ EEI, Copy SegD to A/B/C
msp430x471x7_fll_01.c                              FLL+, Runs Internal DCO at 2.45MHz
msp430x471x7_fll_02.c                              FLL+, Runs Internal DCO at 16MHz
msp430x471x7_hfxt2.c                               FLL+, MCLK Configured to Operate from XT2 HF XTAL
msp430x471x7_lcda_04.c                             LCD_A Put "0123456" on SBLCDA4 LCD
msp430x471x7_lcda_05.c                             LCD_A Put "0123456" on SBLCDA4 LCD with charge pump
msp430x471x7_lpm3.c                                FLL+, LPM3 Using Basic Timer ISR, 32kHz ACLK
msp430x471x7_OF_LFXT1_nmi.c                        LFXT1 Oscillator Fault Detection
msp430x471x7_OF_XT2.c                              XT2 Oscillator Fault Detection
msp430x471x7_P1_01.c                               Software Poll P2.7, Set P5.1 if P2.7 = 1
msp430x471x7_P1_02.c                               Software Port Interrupt on P2.7 from LPM4
msp430x471x7_P1_05.c                               Write a byte to Port 1
msp430x471x7_P7_05.c                               Write a byte to Port 7
msp430x471x7_P8_05.c                               Write a byte to Port 8
msp430x471x7_PA_05.c                               Write a Word to Port A (Port7+Port8)
msp430x471x7_PB_05.c                               Write a Word to Port B (Port9+Port10)
msp430x471x7_rtc_01.c                              Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK
msp430x471x7_rtc_02.c                              Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK and send Time via UART
msp430x471x7_sd16_01.c                             SD16, Continuous Conversion on a Group of 7 Channels
msp430x471x7_sd16_02.c                             SD16, Single Conversion on a Group of 7 Channels
msp430x471x7_sd16_03.c                             SD16, Continuous Conversion on a Single Channel
msp430x471x7_sd16_04.c                             SD16, Single Conversion on Single Channel Polling IFG
msp430x471x7_sd16_05.c                             SD16, Single Conversion on a Single Channel Using ISR
msp430x471x7_sd16_06.c                             SD16, Using the Integrated Temperature Sensor
msp430x471x7_svs_01.c                              SVS, POR @ 2.5V Vcc
msp430x471x7_svs_03.c                              SVS, Toggle port 5.1 on Vcc < 2.8V
msp430x471x7_ta_01.c                               Timer_A, Toggle P5.1, CCR0 Cont. Mode ISR, DCO SMCLK
msp430x471x7_ta_02.c                               Timer_A, Toggle P5.1, CCR0 Up Mode ISR, DCO SMCLK
msp430x471x7_ta_03.c                               Timer_A, Toggle P5.1, Overflow ISR, DCO SMCLK
msp430x471x7_ta_04.c                               Timer_A, Toggle P5.1, Overflow ISR, 32kHz ACLK
msp430x471x7_ta_05.c                               Timer_A, Toggle P5.1, CCR0 Up Mode ISR, 32kHz ACLK
msp430x471x7_ta_16.c                               Timer_A, PWM TA1-2 Up Mode, DCO SMCLK
msp430x471x7_ta_17.c                               Timer_A, PWM TA1-2 Up Mode, 32kHz ACLK
msp430x471x7_tb_01.c                               Timer_B, Toggle P5.1, CCR0 Cont. Mode ISR, DCO SMCLK
msp430x471x7_tb_02.c                               Timer_B, Toggle P5.1, CCR0 Up Mode ISR, DCO SMCLK
msp430x471x7_tb_03.c                               Timer_B, Toggle P5.1, Overflow ISR, DCO SMCLK
msp430x471x7_tb_04.c                               Timer_B, Toggle P5.1, Overflow ISR, 32kHz ACLK
msp430x471x7_tb_05.c                               Timer_B, Toggle P5.1, CCR0 Up Mode ISR, 32kHz ACLK
msp430x471x7_tb_10.c                               Timer_B, PWM TB1-2 Up Mode, DCO SMCLK
msp430x471x7_tb_11.c                               Timer_B, PWM TB1-2 Up Mode, 32kHz ACLK
msp430x471x7_uscia0_duplex_9600.c                  USCI_A0, UART 9600 Full-Duplex Transceiver, 32K ACLK
msp430x471x7_uscia0_irda_01.c                      USCI_A0 IrDA External Loopback Test, 4MHz SMCLK
msp430x471x7_uscia0_spi_09.c                       USCI_A0, SPI 3-Wire Master Incremented Data
msp430x471x7_uscia0_spi_10.c                       USCI_A0, SPI 3-Wire Slave Data Echo
msp430x471x7_uscia0_uart_115k.c                    USCI_A0, 115200 UART Echo ISR, DCO SMCLK
msp430x471x7_uscia0_uart_115k_lpm.c                USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM3
msp430x471x7_uscia0_uart_9600.c                    USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430x471x7_uscib0_i2c_08.c                       USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430x471x7_uscib0_i2c_09.c                       USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430x471x7_uscib0_i2c_10.c                       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430x471x7_uscib0_i2c_11.c                       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430x471x7_uscib0_spi_01.c                       USCI_B0, SPI Interface to TLC549 8-Bit ADC
msp430x471x7_usci_i2c_standard_master.c            USCI_B0, I2C Master multiple byte TX/RX
msp430x471x7_usci_i2c_standard_slave.c             USCI_B0, I2C Slave multiple byte TX/RX
msp430x471x7_usci_spi_standard_master.c            USCI_A0, SPI 3-Wire Master multiple byte RX/TX
msp430x471x7_usci_spi_standard_slave.c             USCI_A0, SPI 3-Wire Slave multiple byte RX/TX
msp430x471x7_usci_uart_standard_transceiver.c      USCI_A0, UART Echo received character
msp430x471x7_wdt_01.c                              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430x471x7_wdt_02.c                              WDT, Toggle P5.1, Interval Overflow ISR, 32kHz ACLK
msp430x471x7_wdt_04.c                              WDT+ Failsafe Clock, DCO SMCLK
msp430x471x7_wdt_05.c                              Reset on Invalid Address fetch, Toggle P5.1
msp430x471x7_wdt_06.c                              WDT+ Failsafe Clock, 32kHz ACLK

