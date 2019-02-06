File Name                                    Description
----------------------------------------------------------------------------------------
msp430x46x_1.c                               Software Toggle P1.0
msp430x46x_adc12_01.c                        ADC12, Sample A0, Set P5.1 if A0 > 0.5*AVcc
msp430x46x_adc12_02.c                        ADC12, Using the Internal Reference
msp430x46x_adc12_03.c                        ADC12, Sample A10 Temp, Set P5.1 if Temp ++ ~2C
msp430x46x_adc12_04.c                        ADC12, Extend Sampling Period with SHT Bits
msp430x46x_adc12_05.c                        ADC12, Using an External Reference
msp430x46x_adc12_06.c                        ADC12, Repeated Sequence of Conversions
msp430x46x_adc12_07.c                        ADC12, Repeated Single Channel Conversions
msp430x46x_adc12_08.c                        ADC12, Using 10 External Channels for Conversion
msp430x46x_adc12_09.c                        ADC12, Sequence of Conversions (non-repeated)
msp430x46x_adc12_10.c                        ADC12, Sample A10 Temp and Convert to oC and oF
msp430x46x_bt_01.c                           Basic Timer, Toggle P5.1 Inside ISR, 32kHz ACLK
msp430x46x_clks_02.c                         FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
msp430x46x_compA_01.c                        Comparator A, Poll input CA0, result in P5.1
msp430x46x_compA_02.c                        Comparator A, Poll input CA0, CA exchange, result in P5.1
msp430x46x_compA_04.c                        Comparator A, Poll input CA0, result in P5.1
msp430x46x_compA_05.c                        Comparator A, Poll input CA0, interrupt triggered
msp430x46x_fll_01.c                          FLL+, Runs Internal DCO at 2.45MHz
msp430x46x_fll_02.c                          FLL+, Runs Internal DCO at 8MHz
msp430x46x_hfxt2.c                           FLL+, MCLK Configured to Operate from XT2 HF XTAL
msp430x46x_lpm3.c                            Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK
msp430x46x_OF_LFXT1_nmi.c                    LFXT1 Oscillator Fault Detection
msp430x46x_OF_XT2.c                          XT2 Oscillator Fault Detection
msp430x46x_P1_01.c                           Software Poll P1.4, Set P5.1 if P1.4 = 1
msp430x46x_P1_02.c                           Software Port Interrupt on P1.4 from LPM4
msp430x46x_P1_05.c                           Write a byte to Port 1
msp430x46x_P7_05.c                           Write a byte to Port 7
msp430x46x_P8_05.c                           Write a byte to Port 8
msp430x46x_PA_05.c                           Write a word to Port A
msp430x46x_PB_05.c                           Write a word to Port B
msp430x46x_rtc_01.c                          Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK
msp430x46x_rtc_02.c                          Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK;and send Time via UART
msp430x46x_rtc_03.c                          Real Time Clock, Toggle P5.1 Inside ISR, 32kHz ACLK;send Time via UART and preserve across RESET
msp430x46x_ta_01.c                           Timer_A, Toggle P5.1, TACCR0 Cont. Mode ISR, DCO SMCLK
msp430x46x_ta_02.c                           Timer_A, Toggle P5.1, TACCR0 Up Mode ISR, DCO SMCLK
msp430x46x_ta_03.c                           Timer_A, Toggle P5.1, Overflow ISR, DCO SMCLK
msp430x46x_ta_04.c                           Timer_A, Toggle P5.1, Overflow ISR, 32kHz ACLK
msp430x46x_ta_05.c                           Timer_A, Toggle P5.1, TACCR0 Up Mode ISR, 32kHz ACLK
msp430x46x_ta_08.c                           Timer_A, Toggle P1.0,P1.2 & P2.0 Cont. Mode ISR, 32kHz ACLK
msp430x46x_ta_11.c                           Timer_A, Toggle P1.0/TA0, Up Mode, 32kHz ACLK
msp430x46x_ta_13.c                           Timer_A, Toggle P1.0/TA0, Up/Down Mode, DCO SMCLK
msp430x46x_ta_17.c                           Timer_A, PWM TA1-2 Up Mode, 32kHz ACLK
msp430x46x_ta_19.c                           Timer_A, PWM TA1-2, Up/Down Mode, 32kHz ACLK
msp430x46x_tb_01.c                           Timer_B, Toggle P5.1, TBCCR0 Cont. Mode ISR, DCO SMCLK
msp430x46x_tb_02.c                           Timer_B, Toggle P5.1, TBCCR0 Up Mode ISR, DCO SMCLK
msp430x46x_tb_04.c                           Timer_B, Toggle P5.1, Overflow ISR, 32kHz ACLK
msp430x46x_tb_05.c                           Timer_B, Toggle P5.1, TBCCR0 Up Mode ISR, 32kHz ACLK
msp430x46x_tb_10.c                           Timer_B, PWM TB1-6, Up Mode, DCO SMCLK
msp430x46x_usart1_19200.c                    USART1, Ultra-Low Pwr UART 19200 Echo ISR, 32kHz ACLK
msp430x46x_usart1_spi0_09.c                  USART1, SPI 3-Wire Master Incremented Data
msp430x46x_usart1_spi0_10.c                  USART1, SPI 3-Wire Slave Data Echo
msp430x46x_usart1_spi1_01.c                  USART1, SPI Interface to TLC549 8-Bit ADC
msp430x46x_usart1_uart_115k.c                USART1, 115200 UART Echo ISR, DCO SMCLK
msp430x46x_usart1_uart_2400.c                USART1, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
msp430x46x_usart1_uart_9600.c                USART1, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430x46x_uscia0b0_uart_spi_01.c            USCI_A0 UART TX single byte and USCI_B0 SPI Master TX single byte to MSP430 Slave
msp430x46x_uscia0b0_uart_spi_02.c            USCI_A0 UART RX single byte and USCI_B0 SPI Slave RX single byte from MSP430 Master
msp430x46x_uscia0_duplex_9600.c              USCI_A0, UART 9600 Full-Duplex Transceiver, 32K ACLK
msp430x46x_uscia0_irda_01.c                  USCI_A0 IrDA External Loopback Test, 4MHz SMCLK
msp430x46x_uscia0_irda_02.c                  USCI_A0 IrDA Monitor, 4MHz SMCLK
msp430x46x_uscia0_spi_09.c                   USCI_A0, SPI 3-Wire Master Incremented Data
msp430x46x_uscia0_spi_10.c                   USCI_A0, SPI 3-Wire Slave Data Echo
msp430x46x_uscia0_uart_115k.c                USCI_A0, 115200 UART Echo ISR, DCO SMCLK
msp430x46x_uscia0_uart_115k_lpm.c            USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM3
msp430x46x_uscia0_uart_9600.c                USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430x46x_uscia0_uart_uscia0b0_i2c_1.c      USCI_A0 UART TX single byte and USCI_B0 I2C Master TX single byte to MSP430 Slave
msp430x46x_uscia0_uart_uscia0b0_i2c_2.c      USCI_A0 UART RX single byte and USCI_B0 I2C Slave RX single byte from MSP430 Master
msp430x46x_uscib0_i2c_02.c                   USCI_B0 I2C Master Interface to PCF8574, Read/Write
msp430x46x_uscib0_i2c_03.c                   USCI_B0 I2C Master Interface to DAC8571, Write
msp430x46x_uscib0_i2c_08.c                   USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430x46x_uscib0_i2c_09.c                   USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430x46x_uscib0_i2c_10.c                   USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430x46x_uscib0_i2c_11.c                   USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430x46x_uscib0_spi_01.c                   USCI_B0, SPI Interface to TLC549 8-Bit ADC

