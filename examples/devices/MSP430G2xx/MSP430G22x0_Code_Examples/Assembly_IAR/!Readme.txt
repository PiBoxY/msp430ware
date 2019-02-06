File Name                          Description
----------------------------------------------------------------------------------------
msp430g2210_ca_02.s43              Comp_A, Detect Threshold, Set P1.5 if P1.2/CA2 > 0.25*Vcc
msp430g2210_ca_03.s43              Comp_A, Simple 2.2V Low Battery Detect
msp430g2210_ca_04.s43              Comp_A, Detect Threshold, Set P1.2 if P1.5/CA5 > 0.25*Vcc
msp430g2230_adc10_01.s43           ADC10, Sample A5, AVcc Ref, Set P1.2 if > 0.5*Avcc
msp430g2230_adc10_02.s43           ADC10, Sample A5, 1.5V Ref, Set P1.2 if > 0.2V
msp430g2230_adc10_03.s43           ADC10, Sample A10 Temp, Set P1.2 if Temp ++ ~2C
msp430g2230_adc10_04.s43           ADC10, Sample A5, Signed, Set P1.2 if > 0.5*Avcc
msp430g2230_adc10_05.s43           ADC10, Sample A11, Lo_Batt, Set P1.2 if AVCC<2.3V
msp430g2230_adc10_07.s43           ADC10, DTC Sample A5 32x, AVcc, Repeat Single, DCO
msp430g2230_adc10_08.s43           ADC10, DTC Sample A5 32x, 1.5V, Repeat Single, DCO
msp430g2230_adc10_09.s43           ADC10, DTC Sample A10 32x, 1.5V, Repeat Single, DCO
msp430g2230_adc10_10.s43           ADC10, DTC Sample A7-5, AVcc, Single Sequence, DCO
msp430g2230_adc10_11.s43           ADC10, Sample A5, 1.5V, TA1 Trig, Set P1.2 if > 0.5V
msp430g2230_adc10_12.s43           ADC10, Sample A7, 1.5V, TA1 Trig, Ultra-Low Pwr
msp430g2230_adc10_13.s43           ADC10, DTC Sample A5 32x, AVcc, TA0 Trig, DCO
msp430g2230_adc10_16.s43           ADC10, DTC Sample A5 -> TA1, AVcc, DCO
msp430g2230_adc10_temp.s43         ADC10, Sample A10 Temp and Convert to oC and oF
msp430g2230_usi_01.s43             USICNT Used as a One-Shot Timer Function, DCO SMCLK
msp430g2230_usi_02.s43             SPI 3-wire Master Incremented Data
msp430g2230_usi_03.s43             SPI 3-Wire Slave Data Echo
msp430g2230_usi_06.s43             I2C Master Receiver, single byte
msp430g2230_usi_07.s43             I2C Master Transmitter, single byte
msp430g2230_usi_08.s43             I2C Slave Receiver, single byte
msp430g2230_usi_09.s43             I2C Slave Transmitter, single byte
msp430g2230_usi_12.s43             I2C Master Transmitter / Reciever, multiple bytes
msp430g2230_usi_15.s43             I2C Slave Receiver / Slave Transmitter, multiple bytes
msp430g2230_usi_16.s43             I2C Master Transmitter / Reciever, Repeated Start
msp430g22x0_1.s43                  Software Toggle P1.2
msp430g22x0_1_vlo.s43              Software Toggle P1.2, MCLK = VLO/8
msp430g22x0_clks.s43               Basic Clock, Output Buffered MCLK/10
msp430g22x0_dco_calib.s43          Basic Clock, Output Buffered clocks with preloaded DCO calibration constants for BCSCTL1 and DCOCTL.
msp430g22x0_flashwrite_01.s43      Flash In-System Programming, Copy SegC to SegD
msp430g22x0_lpm3_vlo.s43           Basic Clock, LPM3 Using WDT ISR, VLO ACLK
msp430g22x0_nmi.s43                Basic Clock, Configure RST/NMI as NMI
msp430g22x0_P1_01.s43              Software Poll P1.5, Set P1.2 if P1.5 = 1
msp430g22x0_P1_02.s43              Software Port Interrupt Service on P1.5 from LPM4
msp430g22x0_P1_03.s43              Poll P1 With Software with Internal Pull-up
msp430g22x0_P1_04.s43              P1 Interrupt from LPM4 with Internal Pull-up
msp430g22x0_ta_01.s43              Timer_A, Toggle P1.2, TA0CCR0 Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_02.s43              Timer_A, Toggle P1.2, TA0CCR0 Up Mode ISR, DCO SMCLK
msp430g22x0_ta_03.s43              Timer_A, Toggle P1.2, Overflow ISR, DCO SMCLK
msp430g22x0_ta_06.s43              Timer_A, Toggle P1.2, TA0CCR1 Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_07.s43              Timer_A, Toggle P1.5-6, Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_10.s43              Timer_A, Toggle P1.5/TA0, Up Mode, DCO SMCLK
msp430g22x0_ta_13.s43              Timer_A, Toggle P1.5/TA0_0, Up/Down Mode, DCO SMCLK
msp430g22x0_ta_16.s43              Timer_A, PWM TA0_1-2, Up Mode, DCO SMCLK
msp430g22x0_ta_19.s43              Timer_A, PWM TA0_1, Up/Down Mode, DCO SMCLK
msp430g22x0_wdt_01.s43             WDT, Toggle P1.2, Interval Overflow ISR, DCO SMCLK
msp430g22x0_wdt_04.s43             WDT+ Failsafe Clock, DCO SMCLK
msp430g22x0_wdt_05.s43             Reset on Invalid Address fetch, Toggle P1.2
msp430g22x0_wdt_07.s43             WDT+ periodic reset

