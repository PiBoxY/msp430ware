File Name                           Description
----------------------------------------------------------------------------------------
MSP430F530x.asm                     Toggle P1.0/LED
MSP430F530x_adc10_01.asm            ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
MSP430F530x_adc10_02.asm            ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V
MSP430F530x_adc10_03.asm            ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C
MSP430F530x_adc10_04.asm            ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc
MSP430F530x_adc10_05.asm            ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
MSP430F530x_adc10_06.asm            ADC10, Internal Reference toggle
MSP430F530x_adc10_07.asm            ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
MSP430F530x_adc10_10.asm            ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO
MSP430F530x_adc10_11.asm            ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V
MSP430F530x_adc10_12.asm            ADC10, Sample A5, 1.5V Ref, TA0.0 Trig, Ultra-Low Pwr
MSP430F530x_adc10_13.asm            ADC10, DMA Sample A1 32x, 2.5V Ref, TB0 Trig, DCO
MSP430F530x_adc10_14.asm            ADC10, DMA Sample A1-0 32x, AVcc, Repeat Seq, DCO
MSP430F530x_adc10_15.asm            ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO
MSP430F530x_adc10_21.asm            ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
MSP430F530x_compB_01.asm            COMPB output Toggle in LPM4; 
MSP430F530x_compB_02.asm            COMPB output Toggle from LPM4; input channel CB1; 
MSP430F530x_compB_03.asm            COMPB interrupt capability; 
MSP430F530x_compB_04.asm            COMPB Toggle from LPM4; Ultra low power mode; 
MSP430F530x_compB_05.asm            COMPB Hysteresis, CBOUT Toggle in LPM4; High speed mode
MSP430F530x_compB_06.asm            COMPB and TIMERAx interaction (TA0.1, TA1.1)
MSP430F530x_dma_01.asm              DMA0, Repeated Block to-from RAM, Software Trigger
MSP430F530x_dma_02.asm              DMA0, Repeated Block UCA1UART 9600, TACCR2, ACLK
MSP430F530x_dma_03.asm              SPI TX & RX using DMA0 & DMA1 Single Transfer in Fixed Address Mode
MSP430F530x_flashwrite_01.asm       Single-Byte Flash In-System Programming, Copy SegC to SegD
MSP430F530x_flashwrite_02.asm       Flash In-System Programming w/ Long-Word write at 0x1800
MSP430F530x_LDO_01.asm              Internal LDO powering device VCC and PortU
MSP430F530x_LDO_02.asm              Internal LDO enabled after device power-up
MSP430F530x_LPM3_01.asm             Enters LPM3 with ACLK = LFXT1, REF0 disabled, VUSB LDO and SLDO disabled, SVS disabled
MSP430F530x_LPM3_02.asm             Enters LPM3 (ACLK = VLO)
MSP430F530x_MPY_01.asm              16x16 Unsigned Multiply
MSP430F530x_MPY_02.asm              8x8 Unsigned Multiply
MSP430F530x_MPY_03.asm              16x16 Signed Multiply
MSP430F530x_MPY_04.asm              8x8 Signed Multiply
MSP430F530x_MPY_05.asm              16x16 Unsigned Multiply Accumulate
MSP430F530x_MPY_06.asm              8x8 Unsigned Multiply Accumulate
MSP430F530x_MPY_07.asm              16x16 Signed Multiply Accumulate
MSP430F530x_MPY_08.asm              8x8 Signed Multiply Accumulate
MSP430F530x_MPY_09.asm              32x32 Unsigned Multiply
MSP430F530x_MPY_10.asm              32x32 Signed Multiply
MSP430F530x_MPY_11.asm              32x32 Signed Multiply Accumalate
MSP430F530x_MPY_12.asm              32x32 Unsigned Multiply Accumalate
MSP430F530x_MPY_13.asm              Saturation mode overflow test
MSP430F530x_MPY_14.asm              Saturation mode underflow test
MSP430F530x_MPY_15.asm              Fractional mode, Q15 multiplication
MSP430F530x_OF_LFXT1_nmi.asm        LFXT1 Oscillator Fault Detection
MSP430F530x_P1_01.asm               Software Poll P1.4, Set P1.0 if P1.4 = 1
MSP430F530x_P1_02.asm               Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled
MSP430F530x_P1_03.asm               Software Port Interrupt Service on P1.4 from LPM4
MSP430F530x_PA_05.asm               Write a Word to Port A (Port1+Port2)
MSP430F530x_PB_05.asm               Write a Word to Port B (Port3+Port4)
MSP430F530x_PortMap_01.asm          Port Mapping Port4; Single runtime configuration
MSP430F530x_PortMap_02.asm          Port Mapping single function to multiple pins; Single runtime configuration.
MSP430F530x_PortMap_03.asm          Port Map single function to multiple pins; Multiple runtime configurations
MSP430F530x_PU_01.asm               Software Toggle PU.0/1
MSP430F530x_PU_02.asm               Software Poll PU.0, Set P1.0 if PU.0 = 1
MSP430F530x_ta0_02.asm              Timer0_A5, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F530x_ta0_04.asm              Timer0_A5, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F530x_ta0_16.asm              Timer0_A5, PWM TA1.1-2, Up Mode, DCO SMCLK
MSP430F530x_ta0_17.asm              Timer0_A5, PWM TA1.1-2, Up Mode, 32kHz ACLK
MSP430F530x_ta1_01.asm              Timer1_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F530x_ta1_02.asm              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F530x_ta1_03.asm              Timer1_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F530x_ta1_04.asm              Timer1_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F530x_ta1_05.asm              Timer1_A3, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
MSP430F530x_ta1_11.asm              Timer1_A3, Toggle P1.7/TA1.0, Up Mode, 32kHz ACLK
MSP430F530x_ta1_13.asm              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, DCO SMCLK
MSP430F530x_ta1_14.asm              Timer1_A3, Toggle P1.7/TA1.0, Up/Down Mode, 32kHz ACLK
MSP430F530x_ta1_16.asm              Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
MSP430F530x_ta1_17.asm              Timer1_A3, PWM TA1.1-2, Up Mode, 32kHz ACLK
MSP430F530x_ta1_19.asm              Timer1_A3, PWM TA1.1-2, Up/Down Mode, DCO SMCLK
MSP430F530x_ta1_20.asm              Timer1_A3, PWM TA1.1-2, Up/Down Mode, 32kHz ACLK
MSP430F530x_ta2_01.asm              Timer2_A3, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F530x_ta2_03.asm              Timer2_A3, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F530x_ta2_08.asm              Timer2_A3, Toggle P1.0;P2.3-5, Cont. Mode ISR, 32kHz ACLK
MSP430F530x_ta2_14.asm              Timer2_A3, Toggle P2.3/TA2.0, Up/Down Mode, 32kHz ACLK
MSP430F530x_ta2_19.asm              Timer2_A3, PWM TA1.1-2, Up/Down Mode, DCO SMCLK
MSP430F530x_tb_01.asm               Timer_B, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F530x_tb_02.asm               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F530x_tb_03.asm               Timer_B, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F530x_tb_04.asm               Timer_B, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F530x_tb_05.asm               Timer_B, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
MSP430F530x_UCS_01.asm              Software Toggle P1.1 at Default DCO 
MSP430F530x_UCS_02.asm              Software Toggle P1.1 with 8MHz DCO
MSP430F530x_UCS_04.asm              Software Toggle P1.1 with 12MHz DCO
MSP430F530x_UCS_05.asm              FLL+, Runs Internal DCO at 2.45MHz
MSP430F530x_UCS_06.asm              VLO sources ACLK
MSP430F530x_UCS_07.asm              XT1 sources ACLK
MSP430F530x_UCS_08.asm              FLL+, Output 32kHz Xtal + HF Xtal + Internal DCO
MSP430F530x_uscia0_spi_09.asm       USCI_A0, SPI 3-Wire Master Incremented Data
MSP430F530x_uscia0_spi_10.asm       USCI_A0, SPI 3-Wire Slave Data Echo
MSP430F530x_uscia0_uart_01.asm      USCI_A0, 115200 UART Echo ISR, DCO SMCLK
MSP430F530x_uscia0_uart_02.asm      USCI_A0, Ultra-Low Pwr UART 2400 Echo ISR, 32kHz ACLK
MSP430F530x_uscia0_uart_03.asm      USCI_A0, Ultra-Low Pwr UART 9600 Echo ISR, 32kHz ACLK
MSP430F530x_uscia0_uart_04.asm      USCI_A0, 9600 UART, SMCLK, LPM0, Echo with over-sampling
MSP430F530x_uscib0_i2c_04.asm       USCI_B0 I2C Master RX single bytes from MSP430 Master
MSP430F530x_uscib0_i2c_05.asm       USCI_B0 I2C Slave TX single bytes to MSP430 Slave
MSP430F530x_uscib0_i2c_06.asm       USCI_B0 I2C Master TX single bytes to MSP430 Slave
MSP430F530x_uscib0_i2c_07.asm       USCI_B0 I2C Slave RX single bytes from MSP430 Master
MSP430F530x_uscib0_i2c_08.asm       USCI_B0 I2C Master TX multiple bytes to MSP430 Slave
MSP430F530x_uscib0_i2c_09.asm       USCI_B0 I2C Slave RX multiple bytes from MSP430 Master
MSP430F530x_uscib0_i2c_10.asm       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
MSP430F530x_uscib0_i2c_11.asm       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
MSP430F530x_WDT_01.asm              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
MSP430F530x_WDT_02.asm              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
MSP430F530x_WDT_04.asm              WDT+ Failsafe Clock, WDT mode, DCO SMCLK
MSP430F530x_WDT_05.asm              Reset on Invalid Address fetch, Toggle P1.0
MSP430F530x_WDT_06.asm              WDT+ Failsafe Clock, 32kHz ACLK

