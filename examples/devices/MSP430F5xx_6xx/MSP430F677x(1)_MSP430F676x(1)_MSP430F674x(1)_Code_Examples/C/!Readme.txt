File Name                                         Description
----------------------------------------------------------------------------------------
MSP430F677x_ADC10_01.c                            ADC10, Sample A0, AVcc Ref, Set P1.0 if A0 > 0.5*AVcc
MSP430F677x_ADC10_02.c                            ADC10, Sample A0, 1.5V Shared Ref, Set P1.0 if A0 > 0.5V
MSP430F677x_ADC10_03.c                            ADC10, Sample A10 Temp, TA0.1 Trigger, Set P1.0 if Temp++ ~2C
MSP430F677x_ADC10_04.c                            ADC10, Sample A1, Signed, Set P1.0 if A1 > 0.5*AVcc
MSP430F677x_ADC10_05.c                            ADC10, Sample A11, Lo_Batt, Clear P1.0 if AVcc < 2.3V
MSP430F677x_ADC10_06.c                            ADC10, Internal Reference toggle
MSP430F677x_ADC10_07.c                            ADC10, DMA Sample A1 64x, AVcc, Repeat Single, DCO
MSP430F677x_ADC10_10.c                            ADC10, DMA Sample A2-0, 8-bit Res, Single Sequence, DCO
MSP430F677x_ADC10_11.c                            ADC10, Sample A0, 1.5V Ref, TA0.1 Trig, Set P1.0 if A0>0.5V
MSP430F677x_ADC10_12.c                            ADC10, Sample A5, 1.5V Ref, TA0.1 Trig, Ultra-Low Power
MSP430F677x_ADC10_13.c                            ADC10, DMA Sample A1 32x, 2.5V Ref, TA3 Trig, DCO
MSP430F677x_ADC10_14.c                            ADC10, DMA Sample A1-0 32x, AVcc, Repeat Seq, DCO
MSP430F677x_ADC10_15.c                            ADC10, DMA Sample A11 32x to Flash, Int Ref, DCO
MSP430F677x_ADC10_21.c                            ADC10, Window Comparator, 2.5V ref, Timer trigger, DCO
MSP430F677x_ADC10_22.c                            ADC10, Sample A3, AVcc Ref, SD24_B Trig, Set P1.0 if A0 > 0.5*AVcc
MSP430F677x_AUX_01.c                              AUX, Software-Controlled Switching, AUXVCC1
MSP430F677x_AUX_02.c                              AUX, Software-Controlled Switching, AUXVCC2
MSP430F677x_AUX_03.c                              AUX, Hardware-Controlled Switching, AUXVCC1
MSP430F677x_AUX_04.c                              AUX, Hardware-Controlled Switching, AUXVCC2
MSP430F677x_DMA_01.c                              DMA0, Repeated Block to-from RAM, Software Trigger
MSP430F677x_DMA_04.c                              DMA0, Single Transfer Using ADC10 Triggered by TimerA
MSP430F677x_FLASHWRITE_01.c                       Flash In-System Programming, Single-Byte Copy SegC to SegD
MSP430F677x_FLASHWRITE_02.c                       Flash In-System Programming, Long-Word Write at 0x1800
MSP430F677x_LCDC_01.c                             LCD_C, Display a Single Character
MSP430F677x_LCDC_02.c                             LCD_C, Alternates Between Two Strings
MSP430F677x_LCDC_03.c                             LCD_C, Display 6 digit numbers
MSP430F677x_LPM3_01.c                             Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS disabled
MSP430F677x_LPM3_02.c                             Enters LPM3 with ACLK = LFXT1, REF0 disabled, SVS default state
MSP430F677x_LPM3_03.c                             Enters LPM3 with ACLK =VLO;  LFXT1, REF0 disabled, SVS default state
MSP430F677x_LPM4.c                                Enters LPM4;  LFXT1, REF0 disabled, SVS disabled
MSP430F677x_OF_LFXT1_NMI.c                        LFXT1 Oscillator Fault Detection
MSP430F677x_P1_01.c                               Software Poll P1.4, Set P1.0 if P1.4 = 1
MSP430F677x_P1_02.c                               Software Port Interrupt Service on P1.4 from LPM4 with Internal Pull-up Resistance Enabled
MSP430F677x_P1_05.c                               Write a byte to Port 1
MSP430F677x_PORTMAP_01.c                          Port Mapping, Port2; Single runtime configuration
MSP430F677x_PORTMAP_02.c                          Port Mapping, Single function to multiple pins; Single runtime configuration.
MSP430F677x_PORTMAP_03.c                          Port Mapping, Port3; Single function to multiple pins; Multiple runtime configurations
MSP430F677x_RTC_01.c                              RTC_C, Calendar Mode with Time Event and Alarm Interrupts
MSP430F677x_RTC_02.c                              RTC_C, LPM3.5, & alarm
MSP430F677x_SD24B_01.c                            SD24_B, Continuous Conversion on a Group of 3 Channels
MSP430F677x_SD24B_02.c                            SD24_B, Single Conversion on a Group of 3 Channels
MSP430F677x_SD24B_03.c                            SD24_B, Continuous Conversion on a Single Channel
MSP430F677x_SD24B_04.c                            SD24_B, Single Conversion on Single Channel Polling IFG
MSP430F677x_SD24B_05.c                            SD24_B, Single Conversion on a Single Channel Using ISR
MSP430F677x_SD24B_06.c                            SD24_B, Single Conversion on a Single Channel Using ISR with 24 bit Result
MSP430F677x_SD24B_10.c                            SD24_B, Continuous Conversion on a Group of 2 Channels
MSP430F677x_SD24B_11.c                            SD24_B, Single Conversion on a Group of 2 Channels
MSP430F677x_TA0_02.c                              Timer0_A0, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F677x_TA0_04.c                              Timer0_A0, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F677x_TA0_16.c                              Timer0_A0, PWM TA0.1-2, Up Mode, DCO SMCLK
MSP430F677x_TA0_17.c                              Timer0_A0, PWM TA0.1-2, Up Mode, 32kHz ACLK
MSP430F677x_TA1_01.c                              Timer1_A1, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F677x_TA1_02.c                              Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
MSP430F677x_TA1_03.c                              Timer1_A1, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F677x_TA1_04.c                              Timer1_A1, Toggle P1.0, Overflow ISR, 32kHz ACLK
MSP430F677x_TA1_05.c                              Timer1_A1, Toggle P1.0, CCR0 Up Mode ISR, 32kHz ACLK
MSP430F677x_TA1_11.c                              Timer1_A1, Toggle P8.4/TA1.0, Up Mode, 32kHz ACLK
MSP430F677x_TA1_13.c                              Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, DCO SMCLK
MSP430F677x_TA1_14.c                              Timer1_A1, Toggle P8.4/TA1.0, Up/Down Mode, 32kHz ACLK
MSP430F677x_TA1_16.c                              Timer1_A1, PWM TA1.1, Up Mode, DCO SMCLK
MSP430F677x_TA1_17.c                              Timer1_A1, PWM TA1.1, Up Mode, 32kHz ACLK
MSP430F677x_TA1_19.c                              Timer1_A1, PWM TA1.1, Up/Down Mode, DCO SMCLK
MSP430F677x_TA1_20.c                              Timer1_A1, PWM TA1.1, Up/Down Mode, 32kHz ACLK
MSP430F677x_TA2_01.c                              Timer2_A2, Toggle P1.0, CCR0 Cont. Mode ISR, DCO SMCLK
MSP430F677x_TA2_03.c                              Timer2_A2, Toggle P1.0, Overflow ISR, DCO SMCLK
MSP430F677x_TA2_08.c                              Timer2_A2, Toggle P1.0;P8.6-7, Cont. Mode ISR, 32kHz ACLK
MSP430F677x_TA2_14.c                              Timer2_A2, Toggle P8.6/TA2.0, Up/Down Mode, 32kHz ACLK
MSP430F677x_TA2_19.c                              Timer2_A2, PWM TA2.1, Up/Down Mode, DCO SMCLK
MSP430F677x_UCS_1.c                               UCS, Software Toggle P1.0 at Default DCO
MSP430F677x_UCS_2.c                               UCS, Software Toggle P1.0 with 8MHz DCO
MSP430F677x_UCS_4.c                               UCS, Software Toggle P1.0 with 2.45MHz DCO, LFXT1 as Ref
MSP430F677x_UCS_5.c                               UCS, VLO sources ACLK. Toggles P1.0
MSP430F677x_UCS_6.c                               UCS, XT1 sources ACLK. Toggles P1.0
MSP430F677x_USCIA0_SPI_09.c                       USCI_A0, SPI 3-Wire Master Incremented Data
MSP430F677x_USCIA0_SPI_10.c                       USCI_A0, SPI 3-Wire Slave Data Echo
MSP430F677x_USCIA0_UART_03.c                      USCI_A0, Ultra-Low Power UART 9600 Echo ISR, 32kHz ACLK
MSP430F677x_USCIB0_I2C_06.c                       USCI_B0 I2C Slave RX single bytes from MSP430 Master
MSP430F677x_USCIB0_I2C_07.c                       USCI_B0 I2C Master TX single bytes to MSP430 Slave
MSP430F677x_USCIB0_I2C_10.c                       USCI_B0 I2C Master RX multiple bytes from MSP430 Slave
MSP430F677x_USCIB0_I2C_11.c                       USCI_B0 I2C Slave TX multiple bytes to MSP430 Master
MSP430F677x_USCI_I2C_STANDARD_MASTER.c            USCI_B0, I2C Master multiple byte TX/RX
MSP430F677x_USCI_I2C_STANDARD_SLAVE.c             USCI_B0, I2C Slave multiple byte TX/RX
MSP430F677x_USCI_SPI_STANDARD_MASTER.c            USCI_A0, SPI 3-Wire Master multiple byte RX/TX
MSP430F677x_USCI_SPI_STANDARD_SLAVE.c             USCI_A0, SPI 3-Wire Slave multiple byte RX/TX
MSP430F677x_USCI_UART_STANDARD_TRANSCEIVER.c      USCI_A0, UART Echo received character
MSP430F677x_WDT_01.c                              WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
MSP430F677x_WDT_02.c                              WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
MSP430F677x_WDT_04.c                              WDT+, Failsafe Clock, WDT mode, DCO SMCLK
MSP430F677x_WDT_05.c                              WDT+, Reset on Invalid Address fetch, Toggle P1.0
MSP430F677x_WDT_06.c                              WDT+, Failsafe Clock, 32kHz ACLK

