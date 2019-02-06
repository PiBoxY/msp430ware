File Name                                    Description
----------------------------------------------------------------------------------------
msp430g2x55_1.asm                            Software Toggle P1.0
msp430g2x55_adc10_01.asm                     ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
msp430g2x55_adc10_02.asm                     ADC10, Sample A0, 1.5V Ref, Set P1.0 if A0 > 0.2V
msp430g2x55_adc10_03.asm                     ADC10, Sample A10 Temp, Set P1.0 if Temp ++ ~2C
msp430g2x55_adc10_04.asm                     ADC10, Sample A0, Signed, Set P1.0 if A0 > 0.5*AVcc
msp430g2x55_adc10_05.asm                     ADC10, Sample A11, Lo_Batt, Set P1.0 if AVcc < 2.3V
msp430g2x55_adc10_06.asm                     ADC10, Output Internal Vref on P2.4 & ADCCLK on P1.0
msp430g2x55_adc10_07.asm                     ADC10, DTC Sample A0 64x, AVcc, Repeat Single, DCO
msp430g2x55_adc10_08.asm                     ADC10, DTC Sample A0 64x, 1.5V, Repeat Single, DCO
msp430g2x55_adc10_09.asm                     ADC10, DTC Sample A10 64x, 1.5V, Repeat Single, DCO
msp430g2x55_adc10_10.asm                     ADC10, DTC Sample A2-0, AVcc, Single Sequence, DCO
msp430g2x55_adc10_11.asm                     ADC10, Sample A0, 1.5V, TA1 Trig, Set P1.0 if > 0.5V
msp430g2x55_adc10_12.asm                     ADC10, Sample A7, 1.5V, TA1 Trig, Ultra-Low Pwr
msp430g2x55_adc10_13.asm                     ADC10, DTC Sample A1 32x, AVcc, TA0 Trig, DCO
msp430g2x55_adc10_14.asm                     ADC10, DTC Sample A1-0 32x, AVcc, Repeat Seq, DCO
msp430g2x55_adc10_15.asm                     ADC10, DTC Sample A10 32x to Flash, Int Ref, DCO
msp430g2x55_adc10_16.asm                     ADC10, DTC Sample A0 -> TA1, AVcc, DCO
msp430g2x55_adc10_17.asm                     ADC10, DTC Sample A0 -> TA1, AVcc, HF XTAL
msp430g2x55_adc10_18.asm                     ADC10, DTC Sample A1/0 -> TA1/2, 2.5V, HF XTAL
msp430g2x55_adc10_19.asm                     ADC10, DTC Sample A0 64x, AVcc, HF XTAL
msp430g2x55_adc10_20.asm                     ADC10, DTC Sample A0 2-Blk Cont. Mode, AVcc, HF XTAL
msp430g2x55_adc10_temp.asm                   ADC10, Sample A10 Temp and Convert to oC and oF
msp430g2x55_ca_01.asm                        Comp_A, Output Reference Voltages on P4.1
msp430g2x55_ca_02.asm                        Comp_A, Detect Threshold, Set P1.0 if P1.1 > 0.25*Vcc
msp430g2x55_ca_03.asm                        Comp_A, Simple 2.2V Low Battery Detect
msp430g2x55_clks.asm                         Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10
msp430g2x55_dco_flashcal.asm                 DCO Calibration Constants Programmer
msp430g2x55_flashwrite_01.asm                Flash In-System Programming, Copy SegC to SegD
msp430g2x55_flashwrite_03.asm                Flash In-System Programming w/ EEI, Copy SegC to SegD
msp430g2x55_flashwrite_04.asm                Flash In-System Programming w/ EEI, Copy SegD to A/B/C
msp430g2x55_fll_01.asm                       Basic Clock, Implement Auto RSEL SW FLL
msp430g2x55_fll_02.asm                       Basic Clock, Implement Cont. SW FLL with Auto RSEL
msp430g2x55_hfxtal.asm                       Basic Clock, MCLK Sourced from HF XTAL
msp430g2x55_hfxtal_nmi.asm                   Basic Clock, LFXT1/MCLK Sourced from HF XTAL, NMI
msp430g2x55_lpm3.asm                         Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK
msp430g2x55_lpm3_vlo.asm                     Basic Clock, LPM3 Using WDT ISR, VLO ACLK
msp430g2x55_nmi.asm                          Configure RST/NMI as NMI
msp430g2x55_p1_01.asm                        Software Poll P1.3, Set P1.0 if P1.3 = 1
msp430g2x55_p1_02.asm                        Software Port Interrupt Service on P1.3 from LPM4
msp430g2x55_p1_03.asm                        Poll P1 With Software with Internal Pull-up
msp430g2x55_p1_04.asm                        P1 Interrupt from LPM4 with Internal Pull-up
msp430g2x55_rosc.asm                         DCOCLK Biased with External Resistor Rosc
msp430g2x55_ta_01.asm                        Timer_A, Toggle P1.0, TACCR0 Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_02.asm                        Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, DCO SMCLK
msp430g2x55_ta_03.asm                        Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430g2x55_ta_04.asm                        Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430g2x55_ta_05.asm                        Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, 32kHz ACLK
msp430g2x55_ta_06.asm                        Timer_A, Toggle P1.0, TACCR1 Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_07.asm                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_08.asm                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, 32kHz ACLK
msp430g2x55_ta_09.asm                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, HF XTAL ACLK
msp430g2x55_ta_10.asm                        Timer_A, Toggle P1.1/TA0, Up Mode, DCO SMCLK
msp430g2x55_ta_11.asm                        Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK
msp430g2x55_ta_12.asm                        Timer_A, Toggle P1.1/TA0, Up Mode, HF XTAL ACLK
msp430g2x55_ta_13.asm                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK
msp430g2x55_ta_14.asm                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK
msp430g2x55_ta_15.asm                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, HF XTAL ACLK
msp430g2x55_ta_16.asm                        Timer_A, PWM TA1-2, Up Mode, DCO SMCLK
msp430g2x55_ta_17.asm                        Timer_A, PWM TA1-2, Up Mode, 32kHz ACLK
msp430g2x55_ta_18.asm                        Timer_A, PWM TA1-2, Up Mode, HF XTAL ACLK
msp430g2x55_ta_19.asm                        Timer_A, PWM TA1-2, Up/Down Mode, DCO SMCLK
msp430g2x55_ta_20.asm                        Timer_A, PWM TA1-2, Up/Down Mode, 32kHz ACLK
msp430g2x55_ta_21.asm                        Timer_A, PWM TA1-2, Up/Down Mode, HF XTAL ACLK
msp430g2x55_ta_22.asm                        Timer_A, Ultra-Low Pwr Pulse Accumulator
msp430g2x55_tb_01.asm                        Timer_B, Toggle P1.0, TBCCR0 Cont. Mode ISR, DCO SMCLK
                                             Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, DCO SMCLK
                                             Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430g2x55_tb_04.asm                        Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430g2x55_tb_05.asm                        Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, 32kHz ACLK
msp430g2x55_tb_06.asm                        Timer_B, Toggle P1.0, TBCCR1 Cont. Mode ISR, DCO SMCLK
                                             Timer_B, Toggle P4.0-2, Cont. Mode ISR, DCO SMCLK
                                             Timer_B, Toggle P4.0-2, Cont. Mode ISR, 32kHz ACLK
msp430g2x55_tb_09.asm                        Timer_B, Toggle P4.0-2, Cont. Mode ISR, HF XTAL ACLK
                                             Timer_B, PWM TB1-2, Up Mode, DCO SMCLK
msp430g2x55_tb_11.asm                        Timer_B, PWM TB1-2, Up Mode, 32kHz ACLK
                                             Timer_B, PWM TB1-2, Up Mode, HF XTAL ACLK
                                             Timer_B, PWM TB1-2, Up/Down Mode, DCO SMCLK
                                             Timer_B, PWM TB1-2, Up/Down Mode, 32kHz ACLK
                                             Timer_B, PWM TB1-2, Up/Down Mode, HF XTAL ACLK
msp430g2x55_uscia0_irda_01.asm               USCI_A0 IrDA External Loopback Test, 8MHz SMCLK
msp430g2x55_uscia0_irda_02.asm               USCI_A0 IrDA Monitor, 8MHz SMCLK
                                             USCI_A0 IrDA Physical Layer Comm, 8MHz SMCLK
msp430g2x55_uscia0_spi_09.asm                USCI_A0, SPI 3-Wire Master Incremented Data
msp430g2x55_uscia0_spi_10.asm                USCI_A0, SPI 3-Wire Slave Data Echo
msp430g2x55_uscia0_uart_01_115k.asm          USCI_A0, 115200 UART Echo ISR, DCO SMCLK4
msp430g2x55_uscia0_uart_01_115k_lpm.asm      USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM4
                                             USCI_A0, 19200 UART Echo ISR, DCO SMCLK
                                             USCI_A0, UART 19200 Echo ISR, HF XTAL SMCLK
msp430g2x55_uscia0_uart_01_9600.asm          USCI_A0, 9600 UART Echo ISR, DCO SMCLK
msp430g2x55_uscia0_uart_05_9600.asm          USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
                                             USCI_A0, Ultra-Low Pwr UART 9600 String, 32kHz ACLK
                                             USCI_A0, Ultra-Low Pwr UART 9600 RX/TX, 32kHz ACLK
msp430g2x55_uscia0_uart_08_9600.asm          USCI_A0, UART 9600 Full-Duplex Transceiver, 32kHz ACLK
                                             USCI_B0 I2C Master RX single bytes from MSP430 Slave
                                             USCI_B0 I2C Slave TX single bytes to MSP430 Master
                                             USCI_B0 I2C Master TX single bytes to MSP430 Slave
                                             USCI_B0 I2C Slave RX single bytes from MSP430 Master
msp430g2x55_uscib0_i2c_08.asm                USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430g2x55_uscib0_i2c_09.asm                USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430g2x55_uscib0_i2c_10.asm                USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430g2x55_uscib0_i2c_11.asm                USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
                                             
                                             
                                             
                                             
                                             

