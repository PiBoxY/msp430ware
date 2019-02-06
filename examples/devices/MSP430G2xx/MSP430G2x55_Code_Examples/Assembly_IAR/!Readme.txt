File Name                                    Description
----------------------------------------------------------------------------------------
msp430g2x55_1.s43                            Software Toggle P1.0
msp430g2x55_adc10_01.s43                     ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
msp430g2x55_adc10_02.s43                     ADC10, Sample A0, 1.5V Ref, Set P1.0 if A0 > 0.2V
msp430g2x55_adc10_03.s43                     ADC10, Sample A10 Temp, Set P1.0 if Temp ++ ~2C
msp430g2x55_adc10_04.s43                     ADC10, Sample A0, Signed, Set P1.0 if A0 > 0.5*AVcc
msp430g2x55_adc10_05.s43                     ADC10, Sample A11, Lo_Batt, Set P1.0 if AVcc < 2.3V
msp430g2x55_adc10_06.s43                     ADC10, Output Internal Vref on P2.4 & ADCCLK on P1.0
msp430g2x55_adc10_07.s43                     ADC10, DTC Sample A0 64x, AVcc, Repeat Single, DCO
msp430g2x55_adc10_08.s43                     ADC10, DTC Sample A0 64x, 1.5V, Repeat Single, DCO
msp430g2x55_adc10_09.s43                     ADC10, DTC Sample A10 64x, 1.5V, Repeat Single, DCO
msp430g2x55_adc10_10.s43                     ADC10, DTC Sample A2-0, AVcc, Single Sequence, DCO
msp430g2x55_adc10_11.s43                     ADC10, Sample A0, 1.5V, TA1 Trig, Set P1.0 if > 0.5V
msp430g2x55_adc10_12.s43                     ADC10, Sample A7, 1.5V, TA1 Trig, Ultra-Low Pwr
msp430g2x55_adc10_13.s43                     ADC10, DTC Sample A1 32x, AVcc, TA0 Trig, DCO
msp430g2x55_adc10_14.s43                     ADC10, DTC Sample A1-0 32x, AVcc, Repeat Seq, DCO
msp430g2x55_adc10_15.s43                     ADC10, DTC Sample A10 32x to Flash, Int Ref, DCO
msp430g2x55_adc10_16.s43                     ADC10, DTC Sample A0 -> TA1, AVcc, DCO
msp430g2x55_adc10_17.s43                     ADC10, DTC Sample A0 -> TA1, AVcc, HF XTAL
msp430g2x55_adc10_18.s43                     ADC10, DTC Sample A1/0 -> TA1/2, 2.5V, HF XTAL
msp430g2x55_adc10_19.s43                     ADC10, DTC Sample A0 64x, AVcc, HF XTAL
msp430g2x55_adc10_20.s43                     ADC10, DTC Sample A0 2-Blk Cont. Mode, AVcc, HF XTAL
msp430g2x55_adc10_temp.s43                   ADC10, Sample A10 Temp and Convert to oC and oF
msp430g2x55_ca_01.s43                        Comp_A, Output Reference Voltages on P4.1
msp430g2x55_ca_02.s43                        Comp_A, Detect Threshold, Set P1.0 if P1.1 > 0.25*Vcc
msp430g2x55_ca_03.s43                        Comp_A, Simple 2.2V Low Battery Detect
msp430g2x55_clks.s43                         Basic Clock, Output Buffered SMCLK, ACLK and MCLK/10
msp430g2x55_dco_flashcal.s43                 DCO Calibration Constants Programmer
msp430g2x55_flashwrite_01.s43                Flash In-System Programming, Copy SegC to SegD
msp430g2x55_flashwrite_03.s43                Flash In-System Programming w/ EEI, Copy SegC to SegD
msp430g2x55_flashwrite_04.s43                Flash In-System Programming w/ EEI, Copy SegD to A/B/C
msp430g2x55_fll_01.s43                       Basic Clock, Implement Auto RSEL SW FLL
msp430g2x55_fll_02.s43                       Basic Clock, Implement Cont. SW FLL with Auto RSEL
msp430g2x55_hfxtal.s43                       Basic Clock, MCLK Sourced from HF XTAL
msp430g2x55_hfxtal_nmi.s43                   Basic Clock, LFXT1/MCLK Sourced from HF XTAL, NMI
msp430g2x55_lpm3.s43                         Basic Clock, LPM3 Using WDT ISR, 32kHz ACLK
msp430g2x55_lpm3_vlo.s43                     Basic Clock, LPM3 Using WDT ISR, VLO ACLK
msp430g2x55_nmi.s43                          Configure RST/NMI as NMI
msp430g2x55_p1_01.s43                        Software Poll P1.3, Set P1.0 if P1.3 = 1
msp430g2x55_p1_02.s43                        Software Port Interrupt Service on P1.3 from LPM4
msp430g2x55_p1_03.s43                        Poll P1 With Software with Internal Pull-up
msp430g2x55_p1_04.s43                        P1 Interrupt from LPM4 with Internal Pull-up
msp430g2x55_rosc.s43                         DCOCLK Biased with External Resistor Rosc
msp430g2x55_ta_01.s43                        Timer_A, Toggle P1.0, TACCR0 Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_02.s43                        Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, DCO SMCLK
msp430g2x55_ta_03.s43                        Timer_A, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430g2x55_ta_04.s43                        Timer_A, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430g2x55_ta_05.s43                        Timer_A, Toggle P1.0, TACCR0 Up Mode ISR, 32kHz ACLK
msp430g2x55_ta_06.s43                        Timer_A, Toggle P1.0, TACCR1 Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_07.s43                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, DCO SMCLK
msp430g2x55_ta_08.s43                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, 32kHz ACLK
msp430g2x55_ta_09.s43                        Timer_A, Toggle P1.0-3, Cont. Mode ISR, HF XTAL ACLK
msp430g2x55_ta_10.s43                        Timer_A, Toggle P1.1/TA0, Up Mode, DCO SMCLK
msp430g2x55_ta_11.s43                        Timer_A, Toggle P1.1/TA0, Up Mode, 32kHz ACLK
msp430g2x55_ta_12.s43                        Timer_A, Toggle P1.1/TA0, Up Mode, HF XTAL ACLK
msp430g2x55_ta_13.s43                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, DCO SMCLK
msp430g2x55_ta_14.s43                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, 32kHz ACLK
msp430g2x55_ta_15.s43                        Timer_A, Toggle P1.1/TA0, Up/Down Mode, HF XTAL ACLK
msp430g2x55_ta_16.s43                        Timer_A, PWM TA1-2, Up Mode, DCO SMCLK
msp430g2x55_ta_17.s43                        Timer_A, PWM TA1-2, Up Mode, 32kHz ACLK
msp430g2x55_ta_18.s43                        Timer_A, PWM TA1-2, Up Mode, HF XTAL ACLK
msp430g2x55_ta_19.s43                        Timer_A, PWM TA1-2, Up/Down Mode, DCO SMCLK
msp430g2x55_ta_20.s43                        Timer_A, PWM TA1-2, Up/Down Mode, 32kHz ACLK
msp430g2x55_ta_21.s43                        Timer_A, PWM TA1-2, Up/Down Mode, HF XTAL ACLK
msp430g2x55_ta_22.s43                        Timer_A, Ultra-Low Pwr Pulse Accumulator
msp430g2x55_tb_01.s43                        Timer_B, Toggle P1.0, TBCCR0 Cont. Mode ISR, DCO SMCLK
msp430g2x55_tb_02.s43                        Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, DCO SMCLK
msp430g2x55_tb_03.s43                        Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
msp430g2x55_tb_04.s43                        Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430g2x55_tb_05.s43                        Timer_B, Toggle P1.0, TBCCR0 Up Mode ISR, 32kHz ACLK
msp430g2x55_tb_06.s43                        Timer_B, Toggle P1.0, TBCCR1 Cont. Mode ISR, DCO SMCLK
msp430g2x55_tb_07.s43                        Timer_B, Toggle P4.0-2, Cont. Mode ISR, DCO SMCLK
msp430g2x55_tb_08.s43                        Timer_B, Toggle P4.0-2, Cont. Mode ISR, 32kHz ACLK
msp430g2x55_tb_09.s43                        Timer_B, Toggle P4.0-2, Cont. Mode ISR, HF XTAL ACLK
msp430g2x55_tb_10.s43                        Timer_B, PWM TB1-2, Up Mode, DCO SMCLK
msp430g2x55_tb_11.s43                        Timer_B, PWM TB1-2, Up Mode, 32kHz ACLK
msp430g2x55_tb_12.s43                        Timer_B, PWM TB1-2, Up Mode, HF XTAL ACLK
msp430g2x55_tb_13.s43                        Timer_B, PWM TB1-2, Up/Down Mode, DCO SMCLK
msp430g2x55_tb_14.s43                        Timer_B, PWM TB1-2, Up/Down Mode, 32kHz ACLK
msp430g2x55_tb_15.s43                        Timer_B, PWM TB1-2, Up/Down Mode, HF XTAL ACLK
msp430g2x55_uscia0_irda_01.s43               USCI_A0 IrDA External Loopback Test, 8MHz SMCLK
msp430g2x55_uscia0_irda_02.s43               USCI_A0 IrDA Monitor, 8MHz SMCLK
msp430g2x55_uscia0_irda_03.s43               USCI_A0 IrDA Physical Layer Comm, 8MHz SMCLK
msp430g2x55_uscia0_spi_09.s43                USCI_A0, SPI 3-Wire Master Incremented Data
msp430g2x55_uscia0_spi_10.s43                USCI_A0, SPI 3-Wire Slave Data Echo
msp430g2x55_uscia0_uart_01_115k.s43          USCI_A0, 115200 UART Echo ISR, DCO SMCLK4
msp430g2x55_uscia0_uart_01_115k_lpm.s43      USCI_A0, 115200 UART Echo ISR, DCO SMCLK, LPM4
msp430g2x55_uscia0_uart_01_19200.s43         USCI_A0, 19200 UART Echo ISR, DCO SMCLK
msp430g2x55_uscia0_uart_01_19200_2.s43       USCI_A0, UART 19200 Echo ISR, HF XTAL SMCLK
msp430g2x55_uscia0_uart_01_9600.s43          USCI_A0, 9600 UART Echo ISR, DCO SMCLK
msp430g2x55_uscia0_uart_05_9600.s43          USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
msp430g2x55_uscia0_uart_06_9600.s43          USCI_A0, Ultra-Low Pwr UART 9600 String, 32kHz ACLK
msp430g2x55_uscia0_uart_07_9600.s43          USCI_A0, Ultra-Low Pwr UART 9600 RX/TX, 32kHz ACLK
msp430g2x55_uscia0_uart_08_9600.s43          USCI_A0, UART 9600 Full-Duplex Transceiver, 32kHz ACLK
msp430g2x55_uscib0_i2c_04.s43                USCI_B0 I2C Master RX single bytes from MSP430 Slave
msp430g2x55_uscib0_i2c_05.s43                USCI_B0 I2C Slave TX single bytes to MSP430 Master
msp430g2x55_uscib0_i2c_06.s43                USCI_B0 I2C Master TX single bytes to MSP430 Slave
msp430g2x55_uscib0_i2c_07.s43                USCI_B0 I2C Slave RX single bytes from MSP430 Master
msp430g2x55_uscib0_i2c_08.s43                USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
msp430g2x55_uscib0_i2c_09.s43                USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
msp430g2x55_uscib0_i2c_10.s43                USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430g2x55_uscib0_i2c_11.s43                USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
                                             
                                             
                                             
                                             
                                             

