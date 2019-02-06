File Name                                        Description
----------------------------------------------------------------------------------------
msp430x47x_1.c                                   Toggle P4.6 in software
msp430x47x_bt_01.c                               Basic Timer, Toggle P4.6 Inside ISR, DCO SMCLK
msp430x47x_bt_02.c                               Basic Timer, Toggle P4.6 Inside ISR, 32kHz ACLK
msp430x47x_clks_03.c                             FLL+, Output 32kHz XTAL + HF XTAL + Internal DCO
msp430x47x_compA_01.c                            Comparator_A, Poll input CA0, result in P4.6
msp430x47x_compA_02.c                            Comparator_A, Poll input CA0, CA exchange, result in P4.6
msp430x47x_compA_04.c                            Comparator_A, Poll input CA0, result in P4.6
msp430x47x_compA_05.c                            Comparator_A, Poll input CA0, interrupt triggered
msp430x47x_dac0_1.c                              DAC12_0, Output 1V on DAC0
msp430x47x_dac0_2.c                              DAC12_0, Output 2V on DAC1
msp430x47x_dac0_3.c                              DAC12_0, Output Voltage Ramp on DAC0
msp430x47x_dac0_5.c                              DAC12_0, Output Voltage Ramp on DAC0
msp430x47x_flashwrite_01.c                       Flash In-System Programming, Copy SegC to SegD
msp430x47x_flashwrite_03.c                       Flash In-System Programming w/ EEI, Copy SegC to SegD
msp430x47x_flashwrite_04.c                       Flash In-System Programming w/ EEI, Copy SegD to A/B/C
msp430x47x_fll_01.c                              FLL+, Runs Internal DCO at 2.5MHz
msp430x47x_fll_02.c                              FLL+, Runs Internal DCO at 8MHz
msp430x47x_LFxtal_nmi.c                          LFXT1 Oscillator Fault Detection
msp430x47x_lpm3.c                                FLL+, LPM3 Using Basic Timer ISR, 32kHz ACLK
msp430x47x_oa_02.c                               OA0,Comparator in General-Purpose Mode
msp430x47x_oa_03.c                               OA0,General-Purpose Mode
msp430x47x_oa_06.c                               OA0,Unity-Gain Buffer Mode
msp430x47x_oa_11.c                               OA1,Unity-Gain Buffer Mode
msp430x47x_sd16a_03.c                            SD16_A, Continuous Conversion on a Single Channel
msp430x47x_sd16a_04.c                            SD16_A, Single Conversion on Single Channel Polling IFG
msp430x47x_sd16a_05.c                            SD16_A, Single Conversion on a Single Channel Using ISR
msp430x47x_sd16a_07.c                            SD16, Single Conversion on a Single Channel Using ISR
msp430x47x_sd16a_08.c                            SD16_A, Single Conversion on a Channel using buffered input
msp430x47x_sd16a_09.c                            SD16_A, Single Conversion on a Single Channel Using ISR
msp430x47x_sd16a_10.c                            SD16_A, Single Conversion on a Single Channel Using ISR 1024 Extended Oversampling Rate
msp430x47x_sd16a_11.c                            SD16_A, Single Conversion on a Single Channel Using ISR ACLK input to SD16_A
msp430x47x_sd16a_12.c                            SD16_A, Single Conversion on a Single Channel Using ISR SMCLK input is divided by 32
msp430x47x_svs_01.c                              SVS, POR @ 2.5V Vcc
msp430x47x_svs_03.c                              SVM, Toggle port 4.6 on Vcc < 2.8V
msp430x47x_ta_01.c                               Timer_A, Toggle P4.6, CCR0 Cont. Mode ISR, DCO SMCLK
msp430x47x_ta_02.c                               Timer_A, Toggle P4.6, CCR0 Up Mode ISR, DCO SMCLK
msp430x47x_ta_03.c                               Timer_A, Toggle P4.6, Overflow ISR, DCO SMCLK
msp430x47x_ta_04.c                               Timer_A, Toggle P4.6, Overflow ISR, 32kHz ACLK
msp430x47x_ta_05.c                               Timer_A, Toggle P4.6, CCR0 Up Mode ISR, 32kHz ACLK
msp430x47x_ta_16.c                               Timer_A, Timer_A, PWM TA1-2 Up Mode, DCO SMCLK
msp430x47x_ta_17.c                               Timer_A, PWM TA1-2, Up Mode, 32kHz ACLK
msp430x47x_tb_01.c                               Timer_B, Toggle P4.6, CCR0 Cont. Mode ISR, DCO SMCLK
msp430x47x_tb_02.c                               Timer_B, Toggle P4.6, CCR0 Up Mode ISR, DCO SMCLK
msp430x47x_tb_03.c                               Timer_B, Toggle P4.6, Overflow ISR, DCO SMCLK
msp430x47x_tb_04.c                               Timer_B, Toggle P4.6, Overflow ISR, 32kHz ACLK
msp430x47x_tb_05.c                               Timer_B, Toggle P4.6, CCR0 Up Mode ISR, 32kHz ACLK
msp430x47x_tb_10.c                               Timer_B, PWM TB1 -2 Up Mode, DCO SMCLK
msp430x47x_tb_11.c                               Timer_B, PWM TB1-2 Up Mode, 32kHz ACLK
msp430x47x_uscia0_duplex_9600.c                  USCI_A0, UART 9600 Full-Duplex Transceiver, 32K ACLK
msp430x47x_uscia0_irda_01.c                      USCI_A0 IrDA External Loopback Test, 4MHz SMCLK
msp430x47x_uscia0_spi_09.c                       USCI_A0, SPI 3-Wire Master Incremented Data
msp430x47x_uscia0_spi_10.c                       USCI_A0, SPI 3-Wire Slave Data Echo
msp430x47x_uscia0_uart_115k.c                    USCI_A0, 115200 UART Echo ISR, DCO SMCLK
msp430x47x_uscia0_uart_115k_lpm.c                USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM3
msp430x47x_uscia0_uart_9600.c                    USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430x47x_uscib0_i2c_02.c                       USCI_B0 I2C Master Interface to PCF8574, Read/Write
msp430x47x_uscib0_i2c_08.c                       USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430x47x_uscib0_i2c_09.c                       USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430x47x_uscib0_i2c_10.c                       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430x47x_uscib0_i2c_11.c                       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430x47x_usci_i2c_standard_master.c            USCI_B0, I2C Master multiple byte TX/RX
msp430x47x_usci_i2c_standard_slave.c             USCI_B0, I2C Slave multiple byte TX/RX
msp430x47x_usci_spi_standard_master.c            USCI_A0, SPI 3-Wire Master multiple byte RX/TX
msp430x47x_usci_spi_standard_slave.c             USCI_A0, SPI 3-Wire Slave multiple byte RX/TX
msp430x47x_usci_uart_standard_transceiver.c      USCI_A0, UART Echo received character
msp430x47x_wdt_01.c                              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430x47x_wdt_02.c                              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK

