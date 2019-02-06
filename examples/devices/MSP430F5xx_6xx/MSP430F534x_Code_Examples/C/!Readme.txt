File Name                         Description
----------------------------------------------------------------------------------------
MSP430F534x_1.c                   Toggle P1.0/LED
MSP430F534x_adc_01.c              ADC12, Sample A0, Set P1.0 if A0 > 0.5*AVcc
MSP430F534x_adc_02.c              ADC12, Using the Internal Reference
MSP430F534x_adc_05.c              ADC12, Using an External Reference
MSP430F534x_adc_06.c              ADC12, Repeated Sequence of Conversions
MSP430F534x_adc_07.c              ADC12, Repeated Single Channel Conversions
MSP430F534x_adc_08.c              ADC12, Using A8 and A9 Ext Channels for Conversion
MSP430F534x_adc_09.c              ADC12, Sequence of Conversions (non-repeated)
MSP430F534x_adc_10.c              ADC12, Sample A10 Temp and Convert to oC and oF
MSP430F534x_compB_01.c            COMPB output Toggle in LPM4; 
MSP430F534x_compB_02.c            COMPB output Toggle from LPM4; input channel CB1; 
MSP430F534x_compB_03.c            COMPB interrupt capability; 
MSP430F534x_compB_04.c            COMPB Toggle from LPM4; Ultra low power mode; 
MSP430F534x_compB_05.c            COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
MSP430F534x_compB_06.c            COMPB and TIMERAx interaction (TA0.1, TA1.1)
MSP430F534x_flashwrite_01.c       Single-Byte Flash In-System Programming, Copy SegC to SegD
MSP430F534x_flashwrite_02.c       Flash In-System Programming w/ Long-Word write at 0x1800
MSP430F534x_flashwrite_03.c       Bank Erase from a Block while Executing Code from Another Block
MSP430F534x_LPM3_01.c             Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS disabled
MSP430F534x_LPM3_02.c             Enters LPM3 (ACLK = VLO), SVS disabled
MSP430F534x_OF_LFXT1_nmi.c        LFXT1 Oscillator Fault Detection
MSP430F534x_P1_01.c               Software Poll P1.4, Set P1.0 if P1.4 = 1
MSP430F534x_P1_02.c               Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled
MSP430F534x_P1_03.c               Software Port Interrupt Service on P1.4 from LPM4
MSP430F534x_PA_05.c               Write a Word to Port A (Port1+Port2.7) 
MSP430F534x_PB_05.c               Write a Word to Port B (P3.0-P3.4+P4.x)
MSP430F534x_PortMap_01.c          Port Mapping Port4; Single runtime configuration
MSP430F534x_PortMap_02.c          Port Mapping single function to multiple pins; Single runtime configuration
MSP430F534x_PortMap_03.c          Port Map single function to multiple pins; Multiple runtime configurations
MSP430F534x_refctl_adc_02.c       Using REFCTL0 control register to configure internal reference
MSP430F534x_ta0_02.c              Timer0_A5, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F534x_ta0_04.c              Timer0_A5, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F534x_ta0_16.c              Timer0_A5, PWM TA0.1-2, Up Mode, DCO SMCLK
MSP430F534x_ta0_17.c              Timer0_A5, PWM TA0.1-2, Up Mode, 32kHz ACLK
MSP430F534x_ta1_01.c              Timer1_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F534x_ta1_02.c              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F534x_ta1_03.c              Timer1_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F534x_ta1_04.c              Timer1_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F534x_ta1_05.c              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
MSP430F534x_ta1_11.c              Timer1_A3, Toggle P1.7/TA1.0, Up Mode, 32kHz ACLK
MSP430F534x_ta1_13.c              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, DCO SMCLK
MSP430F534x_ta1_14.c              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, 32kHz ACLK
MSP430F534x_ta2_01.c              Timer2_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F534x_ta2_03.c              Timer2_A3, Toggle P1.0, Overflow ISR, DCO SMCLK   
MSP430F534x_tb_01.c               Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F534x_tb_02.c               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F534x_tb_03.c               Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F534x_tb_04.c               Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F534x_tb_05.c               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
MSP430F534x_tb_10.c               Timer_B, PWM TB1-6, Up Mode, DCO SMCLK
MSP430F534x_UCS_02.c              Software Toggle P1.1 with 8MHz DCO
MSP430F534x_UCS_03.c              Software Toggle P1.1 with 12MHz DCO
MSP430F534x_UCS_04.c              FLL+, Runs Internal DCO at 2.45MHz
MSP430F534x_UCS_05.c              VLO sources ACLK
MSP430F534x_UCS_06.c              XT1 sources ACLK
MSP430F534x_UCS_10.c              Software Toggle P1.1 with 25MHz DCO
MSP430F534x_uscia0_spi_09.c       USCI_A0, SPI 3-Wire Master Incremented Data
MSP430F534x_uscia0_spi_10.c       USCI_A0, SPI 3-Wire Slave Data Echo
MSP430F534x_uscia0_uart_01.c      USCI_A0, 115200 UART Echo ISR, DCO SMCLK
MSP430F534x_uscia0_uart_02.c      USCI_A0, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
MSP430F534x_uscia0_uart_03.c      USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
MSP430F534x_uscia0_uart_04.c      USCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
MSP430F534x_uscib0_i2c_04.c       USCI_B0 I2C Master RX single bytes from MSP430 Master
MSP430F534x_uscib0_i2c_05.c       USCI_B0 I2C Slave TX single bytes to MSP430 Slave
MSP430F534x_uscib0_i2c_06.c       USCI_B0 I2C Master TX single bytes to MSP430 Slave
MSP430F534x_uscib0_i2c_07.c       USCI_B0 I2C Slave RX single bytes from MSP430 Master
MSP430F534x_uscib0_i2c_08.c       USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
MSP430F534x_uscib0_i2c_09.c       USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
MSP430F534x_uscib0_i2c_10.c       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
MSP430F534x_uscib0_i2c_11.c       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
MSP430F534x_WDT_01.c              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
MSP430F534x_WDT_02.c              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
MSP430F534x_WDT_04.c              WDT+ Failsafe Clock, WDT mode, DCO SMCLK
MSP430F534x_WDT_05.c              Reset on Invalid Address fetch, Toggle P1.0
MSP430F534x_WDT_06.c              WDT+ Failsafe Clock, 32kHz ACLK

