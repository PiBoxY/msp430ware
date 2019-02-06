File Name                                         Description
----------------------------------------------------------------------------------------
msp430f522x.c                                     Toggle P1.0/LED
msp430f522x_adc10_01.c                            ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
msp430f522x_adc10_02.c                            ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V
msp430f522x_adc10_03.c                            ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C
msp430f522x_adc10_04.c                            ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc
msp430f522x_adc10_05.c                            ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
msp430f522x_adc10_06.c                            ADC10, Internal Reference toggle
msp430f522x_adc10_07.c                            ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
msp430f522x_adc10_10.c                            ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO
msp430f522x_adc10_11.c                            ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V
msp430f522x_adc10_12.c                            ADC10, Sample A5, 1.5V Ref, TA0.0 Trig, Ultra-Low Pwr
msp430f522x_adc10_13.c                            ADC10, DMA Sample A1 32x, 2.5V Ref, TB0 Trig, DCO
msp430f522x_adc10_14.c                            ADC10, DMA Sample A1-0 32x, AVcc, Repeat Seq, DCO
msp430f522x_adc10_15.c                            ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO
msp430f522x_adc10_21.c                            ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
msp430f522x_compb_01.c                            COMPB output Toggle in LPM4; 
msp430f522x_compb_02.c                            COMPB output Toggle from LPM4; input channel CB1; 
msp430f522x_compb_03.c                            COMPB interrupt capability; 
msp430f522x_compb_04.c                            COMPB Toggle from LPM4; Ultra low power mode; 
msp430f522x_compb_05.c                            COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
msp430f522x_compb_06.c                            COMPB and TIMERAx interaction (TA0.1, TA1.1)
msp430f522x_dma_01.c                              DMA0, Repeated Block to-from RAM, Software Trigger
msp430f522x_dma_02.c                              DMA0, Repeated Block UCA1UART 9600, TACCR2, ACLK
msp430f522x_dma_03.c                              SPI TX & RX using DMA0 & DMA1 Single Transfer in Fixed Address Mode
msp430f522x_flashwrite_01.c                       Single-Byte Flash In-System Programming, Copy SegC to SegD
msp430f522x_flashwrite_02.c                       Flash In-System Programming w/ Long-Word write at 0x1800
msp430f522x_lpm3_01.c                             Enters LPM3 with ACLK = LFXT1, REF0 disabled, VUSB LDO and SLDO disabled, SVS disabled
msp430f522x_lpm3_02.c                             Enters LPM3 (ACLK = VLO)
msp430f522x_of_lfxt1_nmi.c                        LFXT1 Oscillator Fault Detection
msp430f522x_p1_01.c                               Software Poll P1.4, Set P1.0 if P1.4 = 1
msp430f522x_p1_02.c                               Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled
msp430f522x_p1_03.c                               Software Port Interrupt Service on P1.4 from LPM4
msp430f522x_pa_05.c                               Write a Word to Port A (Port1+Port2)  
msp430f522x_pb_05.c                               Write a Word to Port B (Port3+Port4)
msp430f522x_portmap_01.c                          Port Mapping Port4; Single runtime configuration
msp430f522x_portmap_02.c                          Port Mapping single function to multiple pins; Single runtime configuration.
msp430f522x_portmap_03.c                          Port Map single function to multiple pins; Multiple runtime configurations
msp430f522x_ta0_02.c                              Timer0_A5, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f522x_ta0_04.c                              Timer0_A5, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f522x_ta0_16.c                              Timer0_A5, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430f522x_ta0_17.c                              Timer0_A5, PWM TA1.1-2, Up Mode, 32kHz ACLK
msp430f522x_ta1_01.c                              Timer1_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK   
msp430f522x_ta1_02.c                              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f522x_ta1_03.c                              Timer1_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f522x_ta1_04.c                              Timer1_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f522x_ta1_05.c                              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
msp430f522x_ta1_11.c                              Timer1_A3, Toggle P1.7/TA1.0, Up Mode, 32kHz ACLK
msp430f522x_ta1_13.c                              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, DCO SMCLK
msp430f522x_ta1_14.c                              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, 32kHz ACLK
msp430f522x_ta1_16.c                              Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430f522x_ta1_17.c                              Timer1_A3, PWM TA1.1-2, Up Mode, 32kHz ACLK
msp430f522x_ta1_19.c                              Timer1_A3, PWM TA1.1-2, Up/Down Mode, DCO SMCLK
msp430f522x_ta1_20.c                              Timer1_A3, PWM TA1.1-2, Up/Down Mode, 32kHz ACLK
msp430f522x_ta2_01.c                              Timer2_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK   
msp430f522x_ta2_03.c                              Timer2_A3, Toggle P1.0, Overflow ISR, DCO SMCLK   
msp430f522x_ta2_08.c                              Timer2_A3, Toggle P1.0;P2.3-5, Cont. Mode ISR, 32kHz ACLK    
msp430f522x_ta2_14.c                              Timer2_A3, Toggle P2.3/TA2.0, Up/Down Mode, 32kHz ACLK   
msp430f522x_ta2_19.c                              Timer2_A3, PWM TA1.1-2, Up/Down Mode, DCO SMCLK  
msp430f522x_tb_01.c                               Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
msp430f522x_tb_02.c                               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430f522x_tb_03.c                               Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430f522x_tb_04.c                               Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430f522x_tb_05.c                               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
msp430f522x_ucs_01.c                              Software Toggle P1.1 at Default DCO 
msp430f522x_ucs_02.c                              Software Toggle P1.1 with 8MHz DCO
msp430f522x_ucs_03.c                              Software Toggle P1.1 with 12MHz DCO
msp430f522x_ucs_04.c                              FLL+, Runs Internal DCO at 2.45MHz
msp430f522x_ucs_05.c                              VLO sources ACLK
msp430f522x_ucs_06.c                              XT1 sources ACLK
msp430f522x_ucs_07.c                              FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
msp430f522x_ucs_08.c                              XT2 sources MCLK & SMCLK
msp430f522x_ucs_10.c                              Software Toggle P1.1 with 25MHz DCO
msp430f522x_uscia0_spi_09.c                       USCI_A0, SPI 3-Wire Master Incremented Data
msp430f522x_uscia0_spi_10.c                       USCI_A0, SPI 3-Wire Slave Data Echo
msp430f522x_uscia0_uart_01.c                      USCI_A0, 115200 UART Echo ISR, DCO SMCLK
msp430f522x_uscia0_uart_02.c                      USCI_A0, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
msp430f522x_uscia0_uart_03.c                      USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430f522x_uscia0_uart_04.c                      USCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
msp430f522x_uscib0_i2c_04.c                       USCI_B0 I2C Master RX single bytes from MSP430 Master
msp430f522x_uscib0_i2c_05.c                       USCI_B0 I2C Slave TX single bytes to MSP430 Slave
msp430f522x_uscib0_i2c_06.c                       USCI_B0 I2C Master TX single bytes to MSP430 Slave
msp430f522x_uscib0_i2c_07.c                       USCI_B0 I2C Slave RX single bytes from MSP430 Master
msp430f522x_uscib0_i2c_08.c                       USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430f522x_uscib0_i2c_09.c                       USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430f522x_uscib0_i2c_10.c                       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430f522x_uscib0_i2c_11.c                       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430f522x_usci_i2c_standard_master.c            USCI_B0, I2C Master multiple byte TX/RX
msp430f522x_usci_i2c_standard_slave.c             USCI_B0, I2C Slave multiple byte TX/RX
msp430f522x_usci_spi_standard_master.c            USCI_A0, SPI 3-Wire Master multiple byte RX/TX
msp430f522x_usci_spi_standard_slave.c             USCI_A0, SPI 3-Wire Slave multiple byte RX/TX
msp430f522x_usci_uart_standard_transceiver.c      USCI_A0, UART Echo received character
msp430f522x_wdt_01.c                              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430f522x_wdt_02.c                              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430f522x_wdt_04.c                              WDT+ Failsafe Clock, WDT mode, DCO SMCLK
msp430f522x_wdt_05.c                              Reset on Invalid Address fetch, Toggle P1.0
msp430f522x_wdt_06.c                               WDT+ Failsafe Clock, 32kHz ACLK

