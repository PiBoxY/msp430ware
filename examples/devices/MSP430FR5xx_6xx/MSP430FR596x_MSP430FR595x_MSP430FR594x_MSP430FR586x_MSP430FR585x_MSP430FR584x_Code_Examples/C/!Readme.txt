File Name                                           Description
----------------------------------------------------------------------------------------
msp430fr59xx_1.c                                    Blink LED
msp430fr59xx_active_01.c                            Active mode test
msp430fr59xx_active_02.c                            Active Mode Test with 100% CHR
msp430fr59xx_adc12_01.c                             ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc
msp430fr59xx_adc12_02.c                             ADC12, Using the Internal Reference
msp430fr59xx_adc12_03.c                             Sample A2-A3 Differential,Set P1.0 if i/p> 1V
msp430fr59xx_adc12_05.c                             ADC12, Using an External Reference
msp430fr59xx_adc12_10.c                             ADC12, Sample A10 Temp and Convert to oC and oF
msp430fr59xx_adc12_11.c                             ADC12, Sample A2, T0A1 Trig, Ultra-Low Pwr
msp430fr59xx_adc12_21.c                             ADC12, Window Comparator, 2.5V ref
msp430fr59xx_adc12_22.c                             ADC12, Window Comparator, 2.5V ref, 2's Complement
msp430fr59xx_compe_01.c                             COMPE output Toggle in LPM4; input channel CE1
msp430fr59xx_compe_02.c                             COMPE output Toggle in LPM4; input channel CE12
msp430fr59xx_compe_04.c                             COMPE output Toggle in LPM4, Vcompare is compared against the Vcc*1/2
msp430fr59xx_compe_05.c                             COMPE Hysteresis, COUT Toggle in LPM4; High speed mode
msp430fr59xx_crc.c                                  Calculate CRC and compare
msp430fr59xx_cs_01.c                                Configure MCLK for 8MHz operation
msp430fr59xx_cs_02.c                                Configure MCLK for 16MHz operation
msp430fr59xx_cs_03.c                                Output 32768Hz crystal on XT1 and observe failsafe
msp430fr59xx_cs_04.c                                ACLK = XT1 = 32768Hz, SMCLK= XT2 = 8MHz, MCLK = DCO
msp430fr59xx_cs_05.c                                Using LFXT in bypass mode, failsafe operation shown
msp430fr59xx_dma_01.c                               DMA0, Repeated Block to-from RAM, Software Trigger
msp430fr59xx_euscia0_spi_09.c                       eUSCI_A0, SPI 3-Wire Master Incremented Data
msp430fr59xx_euscia0_spi_10.c                       eUSCI_A0, SPI 3-Wire Slave Data Echo
msp430fr59xx_euscia0_spi_11.c                       eUSCI_A0, SPI 4-Wire Master Incremented Data
msp430fr59xx_euscia0_spi_12.c                       eUSCI_A0, SPI 4-Wire Slave Data Echo
msp430fr59xx_euscia0_uart_01.c                      eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
msp430fr59xx_euscia0_uart_02.c                      eUSCI_A0 UART echo at 9600 baud using BRCLK = 32768Hz
msp430fr59xx_euscia0_uart_03.c                      USCI_A0 External Loopback test @ 115200 baud
msp430fr59xx_euscia0_uart_04.c                      eUSCI_A0 External Loopback test
msp430fr59xx_euscib0_i2c_10.c                       eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430fr59xx_euscib0_i2c_11.c                       eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430fr59xx_euscib0_i2c_master_multislave.c        eUSCI_B0 I2C Master TX bytes to Multiple Slaves
msp430fr59xx_euscib0_i2c_multislave.c               eUSCI_B0 I2C 4 Hardware I2C slaves
msp430fr59xx_eusci_i2c_standard_master.c            eUSCI_B0, I2C Master multiple byte TX/RX
msp430fr59xx_eusci_i2c_standard_slave.c             eUSCI_B0, I2C Slave multiple byte TX/RX
msp430fr59xx_eusci_spi_standard_master.c            eUSCI_A0, SPI 3-Wire Master multiple byte RX/TX
msp430fr59xx_eusci_spi_standard_slave.c             eUSCI_A0, SPI 3-Wire Slave multiple byte RX/TX
msp430fr59xx_eusci_uart_standard_transceiver.c      eUSCI_A0, UART Echo received character
msp430fr59xx_framwrite.c                            Long word writes to FRAM
msp430fr59xx_lpm0.c                                 Enters LPM0 with ACLK = LFXT, MCLK = 1MHz
msp430fr59xx_lpm3-5_01.c                            RTC_B, LPM3.5, & alarm
msp430fr59xx_lpm3-5_02.c                            RTC_B in LPM3.5 using counter mode with 2 second wake-up interval
msp430fr59xx_lpm3_02.c                              Enters LPM3 with ACLK = VLO
msp430fr59xx_lpm3_03.c                              Enters LPM3 with ACLK = LFXT1
msp430fr59xx_lpm4-5_01.c                            Entering and waking up from LPM3.5 via P1.1 interrupt
msp430fr59xx_lpm4-5_02.c                            Entering and waking up from LPM3.5 via P1.1 interrupt with SVS disabled
msp430fr59xx_lpm4.c                                 Enters LPM4
msp430fr59xx_mpu_01.c                               MPU write protection violation - interrupt
msp430fr59xx_mpu_02.c                               MPU Write protection violation - PUC
msp430fr59xx_of_lfxt1_nmi.c                         LFXT Oscillator Fault Detection
msp430fr59xx_p1_01.c                                Software Poll P1.4, Set P1.0 if P1.4 = 1
msp430fr59xx_p1_03.c                                Port Interrupt Service on P1.4 from LPM4
msp430fr59xx_rtc_02.c                               RTC in real time clock mode in LPM3
msp430fr59xx_ta0_01.c                               Timer0_A3, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK
msp430fr59xx_ta0_02.c                               Timer0_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430fr59xx_ta0_04.c                               Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr59xx_ta0_16.c                               Timer0_A3, PWM TA0.1-2, Up Mode, DCO SMCLK
msp430fr59xx_ta0_17.c                               Timer0_A3, PWM TA0.1-2, Up Mode, 32KHz ACLK
msp430fr59xx_ta0_capture.c                          Timer0_A3 Capture of VLO Period using DCO SMCLK
msp430fr59xx_ta1_05.c                               Timer1_A3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK
msp430fr59xx_ta1_06.c                               Timer1_A3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr59xx_ta1_16.c                               Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430fr59xx_ta1_17.c                               Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK
msp430fr59xx_tb0_01.c                               TimerB, Toggle P1.0, CCR0 Cont Mode ISR, DCO SMCLK
msp430fr59xx_tb0_02.c                               TimerB, Toggle P1.0, CCR0 UP Mode ISR, DCO SMCLK
msp430fr59xx_tb0_03.c                               TimerB, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430fr59xx_tb0_04.c                               TimerB, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr59xx_tb0_06.c                               TimerB, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr59xx_tb0_16.c                               TimerB, PWM TB0.1-2, Up Mode, DCO SMCLK
msp430fr59xx_wdt_01.c                               WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430fr59xx_wdt_02.c                               WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430fr59xx_wdt_04.c                               WDT+ Failsafe Clock, WDT mode, DCO SMCLK
msp430fr59xx_wdt_05.c                               Reset on Invalid Address fetch, Toggle P1.0

