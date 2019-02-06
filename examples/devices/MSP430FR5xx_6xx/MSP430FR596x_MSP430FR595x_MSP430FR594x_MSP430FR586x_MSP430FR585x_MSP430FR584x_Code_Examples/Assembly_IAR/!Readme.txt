File Name                                           Description
----------------------------------------------------------------------------------------
msp430fr59xx_1.s43                                  Blink LED
msp430fr59xx_active_01.s43                          Active mode test
msp430fr59xx_active_02.s43                          Active Mode Test with 100% CHR
msp430fr59xx_adc12_01.s43                           ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc
msp430fr59xx_adc12_02.s43                           ADC12, Using the Internal Reference
msp430fr59xx_adc12_03.s43                           Sample A2-A3 Differential,Set P1.0 if i/p> 1V
msp430fr59xx_adc12_05.s43                           ADC12, Using an External Reference
msp430fr59xx_adc12_21.s43                           ADC12, Window Comparator, 2.5V ref
msp430fr59xx_adc12_22.s43                           ADC12, Window Comparator, 2.5V ref, 2's Complement
msp430fr59xx_compe_01.s43                           COMPE output Toggle in LPM4; input channel CE1
msp430fr59xx_compe_02.s43                           COMPE output Toggle in LPM4; input channel CE12
msp430fr59xx_compe_04.s43                           COMPE output Toggle in LPM4, Vcompare is compared against the Vcc*1/2
msp430fr59xx_compe_05.s43                           COMPE Hysteresis, COUT Toggle in LPM4; High speed mode
msp430fr59xx_crc.s43                                Calculate CRC and compare
msp430fr59xx_cs_01.s43                              Configure MCLK for 8MHz operation
msp430fr59xx_cs_02.s43                              Configure MCLK for 16MHz operation
msp430fr59xx_cs_03.s43                              Output 32768Hz crystal on XT1 and observe failsafe
msp430fr59xx_cs_04.s43                              ACLK = XT1 = 32768Hz, SMCLK= XT2 = 8MHz, MCLK = DCO
msp430fr59xx_cs_05.s43                              Using LFXT in bypass mode, failsafe operation shown
msp430fr59xx_dma_01.s43                             DMA0, Repeated Block to-from RAM, Software Trigger
msp430fr59xx_euscia0_spi_09.s43                     eUSCI_A0, SPI 3-Wire Master Incremented Data
msp430fr59xx_euscia0_spi_10.s43                     eUSCI_A0, SPI 3-Wire Slave Data Echo
msp430fr59xx_euscia0_spi_11.s43                     eUSCI_A0, SPI 4-Wire Master Incremented Data
msp430fr59xx_euscia0_spi_12.s43                     eUSCI_A0, SPI 4-Wire Slave Data Echo
msp430fr59xx_euscia0_uart_01.s43                    eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
msp430fr59xx_euscia0_uart_02.s43                    eUSCI_A0 UART echo at 9600 baud using BRCLK = 32768Hz
msp430fr59xx_euscia0_uart_03.s43                    USCI_A0 External Loopback test @ 115200 baud
msp430fr59xx_euscia0_uart_04.s43                    eUSCI_A0 External Loopback test
msp430fr59xx_euscib0_i2c_10.s43                     eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430fr59xx_euscib0_i2c_11.s43                     eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430fr59xx_euscib0_i2c_master_multislave.s43      eUSCI_B0 I2C Master TX bytes to Multiple Slaves
msp430fr59xx_euscib0_i2c_multislave.s43             eUSCI_B0 I2C 4 Hardware I2C slaves
msp430fr59xx_framwrite.s43                          Long word writes to FRAM
msp430fr59xx_lpm0.s43                               Enters LPM0 with ACLK = LFXT, MCLK = 1MHz
msp430fr59xx_lpm3-5_01.s43                          RTC_B, LPM3.5, & alarm
msp430fr59xx_lpm3_02.s43                            Enters LPM3 with ACLK = VLO
msp430fr59xx_lpm3_03.s43                            Enters LPM3 with ACLK = LFXT1
msp430fr59xx_lpm4-5_01.s43                          Entering and waking up from LPM3.5 via P1.1 interrupt
msp430fr59xx_lpm4.s43                               Enters LPM4
msp430fr59xx_mpu_01.s43                             MPU write protection violation - interrupt
msp430fr59xx_mpu_02.s43                             MPU Write protection violation - PUC
msp430fr59xx_mpy_01.s43                             16x16 Unsigned Multiply
msp430fr59xx_mpy_10.s43                             32x32 Unsigned Multiply
msp430fr59xx_mpy_11.s43                             32x32 Unsigned Multiply Accumulate
msp430fr59xx_p1_01.s43                              Software Poll P1.4, Set P1.0 if P1.4 = 1
msp430fr59xx_p1_03.s43                              Port Interrupt Service on P1.4 from LPM4
msp430fr59xx_ta0_01.s43                             Timer0_A3, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK
msp430fr59xx_ta0_02.s43                             Timer0_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430fr59xx_ta0_04.s43                             Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr59xx_ta0_16.s43                             Timer0_A3, PWM TA0.1-2, Up Mode, DCO SMCLK
msp430fr59xx_ta0_17.s43                             Timer0_A3, PWM TA0.1-2, Up Mode, 32KHz ACLK
msp430fr59xx_ta0_capture.s43                        Timer0_A3 Capture of VLO Period using DCO SMCLK
msp430fr59xx_ta1_05.s43                             Timer1_A3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK
msp430fr59xx_ta1_06.s43                             Timer1_A3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr59xx_ta1_16.s43                             Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430fr59xx_ta1_17.s43                             Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK
msp430fr59xx_tb0_01.s43                             TimerB, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK
msp430fr59xx_tb0_02.s43                             TimerB, Toggle P1.0, CCR0 UP Mode ISR, DCO SMCLK
msp430fr59xx_tb0_03.s43                             TimerB, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430fr59xx_tb0_04.s43                             TimerB, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr59xx_tb0_06.s43                             TimerB, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr59xx_tb0_16.s43                             TimerB, PWM TB0.1-2, Up Mode, DCO SMCLK
msp430fr59xx_wdt_01.s43                             WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430fr59xx_wdt_02.s43                             WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430fr59xx_wdt_04.s43                             WDT+ Failsafe Clock, WDT mode, DCO SMCLK
msp430fr59xx_wdt_05.s43                             Reset on Invalid Address fetch, Toggle P1.0

