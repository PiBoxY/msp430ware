File Name                                         Description
----------------------------------------------------------------------------------------
msp430f665x_1.c                                   Software Toggle P1.0
msp430f665x_adc_01.c                              ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc
msp430f665x_adc_02.c                              ADC12, Using the Internal Reference
msp430f665x_adc_05.c                              ADC12, Using an External Reference
msp430f665x_adc_06.c                              ADC12, Repeated Sequence of Conversions
msp430f665x_adc_07.c                              ADC12, Repeated Single Channel Conversions
msp430f665x_adc_08.c                              ADC12, Using A8 and A9 Ext Channels for Conversion
msp430f665x_adc_09.c                              ADC12, Sequence of Conversions (non-repeated)
msp430f665x_adc_10.c                              ADC12, Sample A10 Temp and Convert to oC and oF
msp430f665x_compB_01.c                            COMPB output Toggle in LPM4; Vcompare is compared against the internal 2.0V reference
msp430f665x_compB_02.c                            COMPB Toggle from LPM4; input channel CB1; Vcompare is compared against the internal reference 2.0V
msp430f665x_compB_03.c                            COMPB interrupt capability; Vcompare is compared against internal 1.5V reference
msp430f665x_compB_04.c                            COMPB Toggle from LPM4; CompB in ultra low power mode Vcompare is compared against the Vcc*1/2
msp430f665x_compB_05.c                            COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
msp430f665x_crc16_01.c                            CRC16, Compare CRC output with software-based algorithm
msp430f665x_crc16_02.c                            CRC16, fed by DMA, compare w/ software algorithm
msp430f665x_dac12_1.c                             DAC12_0, Output 1.5V on DAC0
msp430f665x_dac12_2.c                             DAC12_1, Output 0.75V on DAC1
msp430f665x_dac12_3.c                             DAC12_0, Output Voltage Ramp on DAC0
msp430f665x_dma_01.c                              DMA0, Repeated Block to-from RAM, Software Trigger
msp430f665x_dma_02.c                              DMA0, Repeated Block UCA1UART 9600, TACCR2, ACLK
msp430f665x_flashwrite_01.c                       Single-Byte Flash In-System Programming, Copy SegC to SegD
msp430f665x_flashwrite_02.c                       Flash In-System Programming w/ Long-Word write at 0x1800
msp430f665x_lpm0_01.c                             Enters LPM0 with ACLK = LFXT1, REF0 disabled, VUSB LDO and SLDO disabled, SVS disabled
msp430f665x_LPM35_RTC.c                           RTC_B, LPM3.5, & alarm
msp430f665x_lpm3_01.c                             Enters LPM3 with ACLK = LFXT1, REF0 disabled, VUSB LDO and SLDO disabled, SVS disabled
msp430f665x_lpm3_02.c                             Enters LPM3 with ACLK =REFO; LFXT1 disabled, VUSB LDO and SLDO disabled
msp430f665x_lpm3_03.c                             Enters LPM3 with ACLK =VLO; REFO, LFXT1 disabled, VUSB LDO and SLDO disabled
msp430f665x_lpm4.c                                Enters LPM4
msp430f665x_LPM45_BAKMEM.c                        Enters LPM4.5, tests backup memory
msp430f665x_mid_01.c                              Enabling MID protection for the entire main memory 
msp430f665x_mid_02.c                              Enabling MID protection for one MID block
msp430f665x_mid_03.c                              Error injection when MID protection is enabled
msp430f665x_PortMap_01.c                          Port Mapping Port2; Single runtime configuration
msp430f665x_PortMap_02.c                          Port Mapping single function to multiple pins; Single runtime configuration.
msp430f665x_PortMap_03.c                          Port Map single function to multiple pins; Multiple runtime configurations
msp430f665x_RTC_01.c                              RTC_B in real time clock mode
msp430f665x_ta0_02.c                              Timer0_A5, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f665x_ta0_04.c                              Timer0_A5, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f665x_ta0_16.c                              Timer0_A5, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430f665x_ta0_17.c                              Timer0_A5, PWM TA1.1-2, Up Mode, 32kHz ACLK
msp430f665x_ta1_01.c                              Timer1_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f665x_ta1_02.c                              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f665x_ta1_03.c                              Timer1_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f665x_ta1_04.c                              Timer1_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f665x_ta1_05.c                              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
msp430f665x_ta1_11.c                              Timer1_A3, Toggle P3.1/TA1.0, Up Mode, 32kHz ACLK
msp430f665x_ta1_13.c                              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, DCO SMCLK
msp430f665x_ta1_14.c                              Timer1_A3, Toggle P3.1/TA1.0, Up/Down Mode, 32kHz ACLK
msp430f665x_ta1_16.c                              Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430f665x_ta1_17.c                              Timer1_A3, PWM TA1.1-2, Up Mode, 32kHz ACLK
msp430f665x_ta1_19.c                              Timer1_A3, PWM TA1.1-2, Up/Down Mode, DCO SMCLK
msp430f665x_ta1_20.c                              Timer1_A3, PWM TA1.1-2, Up/Down Mode, 32kHz ACLK
msp430f665x_ta2_01.c                              Timer2_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f665x_ta2_03.c                              Timer2_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f665x_ta2_08.c                              Timer2_A3, Toggle P1.0;P2.3-5, Cont. Mode ISR, 32kHz ACLK
msp430f665x_ta2_14.c                              Timer2_A3, Toggle P3.5/TA2.0, Up/Down Mode, 32kHz ACLK
msp430f665x_ta2_19.c                              Timer2_A3, PWM TA2.1-2, Up/Down Mode, DCO SMCLK
msp430f665x_tb_01.c                               Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f665x_tb_02.c                               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f665x_tb_03.c                               Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f665x_tb_04.c                               Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f665x_tb_05.c                               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
msp430f665x_tb_10.c                               Timer_B, PWM TB1-6, Up Mode, DCO SMCLK
msp430f665x_ucs_03.c                              Software Toggle P1.1 with 12MHz DCO
msp430f665x_ucs_04.c                              FLL+, Runs Internal DCO at 2.45MHz
msp430f665x_ucs_05.c                              VLO sources ACLK
msp430f665x_ucs_06.c                              XT1 sources ACLK
msp430f665x_ucs_07.c                              FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
msp430f665x_ucs_08.c                              XT2 sources SMCLK
msp430f665x_uscia0_spi_09.c                       eUSCI_A0, SPI 3-Wire Master Incremented Data
msp430f665x_uscia0_spi_10.c                       eUSCI_A0, SPI 3-Wire Slave Data Echo
msp430f665x_uscia0_uart_03.c                      eUSCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430f665x_uscia0_uart_04.c                      eUSCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
msp430f665x_uscib0_i2c_06.c                       eUSCI_B0 I2C Slave RX single bytes from MSP430 Master
msp430f665x_uscib0_i2c_07.c                       eUSCI_B0 I2C Master TX single bytes to MSP430 Slave
msp430f665x_uscib0_i2c_08.c                       eUSCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430f665x_uscib0_i2c_09.c                       eUSCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430f665x_uscib0_i2c_10.c                       eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430f665x_uscib0_i2c_11.c                       eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430f665x_usci_i2c_standard_master.c            eUSCI_B0, I2C Master multiple byte TX/RX
msp430f665x_usci_i2c_standard_slave.c             eUSCI_B0, I2C Slave multiple byte TX/RX
msp430f665x_usci_spi_standard_master.c            eUSCI_A0, SPI 3-Wire Master multiple byte RX/TX
msp430f665x_usci_spi_standard_slave.c             eUSCI_A0, SPI 3-Wire Slave multiple byte RX/TX
msp430f665x_usci_uart_standard_transceiver.c      eUSCI_A0, UART Echo received character
msp430f665x_wdt_01.c                              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430f665x_wdt_02.c                              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430f665x_wdt_04.c                              WDT+ Failsafe Clock, WDT mode, DCO SMCLK
msp430f665x_wdt_05.c                              Reset on Invalid Address fetch, Toggle P1.0
msp430f665x_wdt_06.c                              WDT+ Failsafe Clock, 32kHz ACLK

