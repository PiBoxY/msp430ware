File Name                          Description
----------------------------------------------------------------------------------------
msp430g2210_ca_02.asm              Comp_A, Detect Threshold, Set P1.5 if P1.2/CA2 > 0.25*Vcc
msp430g2210_ca_03.asm              Comp_A, Simple 2.2V Low Battery Detect
msp430g2210_ca_04.asm              Comp_A, Detect Threshold, Set P1.2 if P1.5/CA5 > 0.25*Vcc
msp430g2230_adc10_01.asm           ADC10, Sample A5, AVcc Ref, Set P1.2 if > 0.5*Avcc
msp430g2230_adc10_02.asm           ADC10, Sample A5, 1.5V Ref, Set P1.2 if > 0.2V
msp430g2230_adc10_03.asm           ADC10, Sample A10 Temp, Set P1.2 if Temp ++ ~2C
msp430g2230_adc10_04.asm           ADC10, Sample A5, Signed, Set P1.2 if > 0.5*Avcc
msp430g2230_adc10_05.asm           ADC10, Sample A11, Lo_Batt, Set P1.2 if AVCC<2.3V
msp430g2230_adc10_07.asm           ADC10, DTC Sample A5 32x, AVcc, Repeat Single, DCO
msp430g2230_adc10_08.asm           ADC10, DTC Sample A5 32x, 1.5V, Repeat Single, DCO
msp430g2230_adc10_09.asm           ADC10, DTC Sample A10 32x, 1.5V, Repeat Single, DCO
msp430g2230_adc10_10.asm           ADC10, DTC Sample A7-5, AVcc, Single Sequence, DCO
msp430g2230_adc10_11.asm           ADC10, Sample A5, 1.5V, TA1 Trig, Set P1.2 if > 0.5V
msp430g2230_adc10_12.asm           ADC10, Sample A7, 1.5V, TA1 Trig, Ultra-Low Pwr
msp430g2230_adc10_13.asm           ADC10, DTC Sample A5 32x, AVcc, TA0 Trig, DCO
msp430g2230_adc10_16.asm           ADC10, DTC Sample A5 -> TA1, AVcc, DCO
msp430g2230_adc10_temp.asm         ADC10, Sample A10 Temp and Convert to oC and oF
msp430g2230_usi_01.asm             USICNT Used as a One-Shot Timer Function, DCO SMCLK
msp430g2230_usi_02.asm             SPI 3-wire Master Incremented Data
msp430g2230_usi_03.asm             SPI 3-Wire Slave Data Echo
msp430g2230_usi_06.asm             I2C Master Receiver, single byte
msp430g2230_usi_07.asm             I2C Master Transmitter, single byte
msp430g2230_usi_08.asm             I2C Slave Receiver, single byte
msp430g2230_usi_09.asm             I2C Slave Transmitter, single byte
msp430g2230_usi_12.asm             I2C Master Transmitter / Reciever, multiple bytes
msp430g2230_usi_15.asm             I2C Slave Receiver / Slave Transmitter, multiple bytes
msp430g2230_usi_16.asm             I2C Master Transmitter / Reciever, Repeated Start
msp430g22x0_1.asm                  Software Toggle P1.2
msp430g22x0_1_vlo.asm              Software Toggle P1.2, MCLK = VLO/8
msp430g22x0_clks.asm               Basic Clock, Output Buffered MCLK/10
msp430g22x0_dco_calib.asm          Basic Clock, Output Buffered clocks with preloaded DCO calibration constants for BCSCTL1 and DCOCTL.
msp430g22x0_flashwrite_01.asm      Flash In-System Programming, Copy SegC to SegD
msp430g22x0_lpm3_vlo.asm           Basic Clock, LPM3 Using WDT ISR, VLO ACLK
msp430g22x0_nmi.asm                Basic Clock, Configure RST/NMI as NMI
msp430g22x0_P1_01.asm              Software Poll P1.5, Set P1.2 if P1.5 = 1
msp430g22x0_P1_02.asm              Software Port Interrupt Service on P1.5 from LPM4
msp430g22x0_P1_03.asm              Poll P1 With Software with Internal Pull-up
msp430g22x0_P1_04.asm              P1 Interrupt from LPM4 with Internal Pull-up
msp430g22x0_ta_01.asm              Timer_A, Toggle P1.2, TA0CCR0 Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_02.asm              Timer_A, Toggle P1.2, TA0CCR0 Up Mode ISR, DCO SMCLK
msp430g22x0_ta_03.asm              Timer_A, Toggle P1.2, Overflow ISR, DCO SMCLK
msp430g22x0_ta_06.asm              Timer_A, Toggle P1.2, TA0CCR1 Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_07.asm              Timer_A, Toggle P1.5-6, Cont. Mode ISR, DCO SMCLK
msp430g22x0_ta_10.asm              Timer_A, Toggle P1.5/TA0, Up Mode, DCO SMCLK
msp430g22x0_ta_13.asm              Timer_A, Toggle P1.5/TA0_0, Up/Down Mode, DCO SMCLK
msp430g22x0_ta_16.asm              Timer_A, PWM TA0_1-2, Up Mode, DCO SMCLK
msp430g22x0_ta_19.asm              Timer_A, PWM TA0_1, Up/Down Mode, DCO SMCLK
msp430g22x0_wdt_01.asm             WDT, Toggle P1.2, Interval Overflow ISR, DCO SMCLK
msp430g22x0_wdt_04.asm             WDT+ Failsafe Clock, DCO SMCLK
msp430g22x0_wdt_05.asm             Reset on Invalid Address fetch, Toggle P1.2
msp430g22x0_wdt_07.asm             WDT+ periodic reset

