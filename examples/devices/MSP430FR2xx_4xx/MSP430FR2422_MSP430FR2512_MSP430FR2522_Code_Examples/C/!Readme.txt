File Name                                Description
----------------------------------------------------------------------------------------
msp430fr2422_1.c                         Toggle P1.0 using software
msp430fr2422_adc10_01.c                  ADC, Sample A1, AVcc Ref, Set LED if A1 > 0.5*AVcc
msp430fr2422_adc10_02.c                  ADC, Sample A1, internal 1.5V Ref, Set P1.0 if A1 > 0.5V
msp430fr2422_adc10_05.c                  ADC, Lo_Batt, Set P1.0 if AVcc < 2.50V
msp430fr2422_adc10_10.c                  ADC, Sample A2/A1/A0, internal 1.5V Ref.
msp430fr2422_adc10_11.c                  ADC, Sample A1, internal 1.5V Ref, TA0.1 Trig, Set P1.0 if A1>0.5V
msp430fr2422_adc10_16.c                  ADC, Sample A12 Temp and Convert to oC and oF
msp430fr2422_adc10_21.c                  ADC, Window Comparator, 1.5V ref, Timer trigger, DCO
msp430fr2422_CRC.c                       CRC16, Compare CRC output with software-based algorithm
msp430fr2422_CS_01.c                     Configure MCLK for 8MHz sourced from DCO.
msp430fr2422_CS_02.c                     Configure MCLK for 8MHz and XT1 sourcing ACLK and FLLREF
msp430fr2422_CS_03.c                     Configure MCLK for 16MHz operation, and REFO sourcing FLLREF and ACLK
msp430fr2422_CS_04.c                     Output 32768Hz crystal on XT1 and observe failsafe
msp430fr2422_CS_05.c                     Configure MCLK for 16MHz operation, and XT1 sourcing FLLREF and ACLK
msp430fr2422_euscia0_spi_09.c            eUSCI_A0, SPI 3-Wire Master Incremented Data
msp430fr2422_euscia0_spi_10.c            eUSCI_A0, SPI 3-Wire Slave Data Echo
msp430fr2422_euscia0_spi_11.c            eUSCI_A0, SPI 4-Wire Master Incremented Data
msp430fr2422_euscia0_spi_12.c            eUSCI_A0, SPI 4-Wire Slave Data Echo
msp430fr2422_euscia0_uart_01.c           eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
msp430fr2422_euscia0_uart_03.c           USCI_A0 External Loopback test @ 115200 baud
msp430fr2422_euscia0_uart_06.c           Remapped eUSCI_A0 UART echo at 9600 baud using BRCLK = 8MHz
msp430fr2422_euscib0_i2c_10.c            eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430fr2422_euscib0_i2c_11.c            eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430fr2422_euscib0_i2c_12.c            eUSCI_B0 I2C Master TX bytes to Multiple Slaves
msp430fr2422_euscib0_i2c_13.c            eUSCI_B0 I2C 4 Hardware I2C slaves
msp430fr2422_euscib0_i2c_15.c            Remapped eUSCI_B0 I2C Master RX multiple bytes from MSP430 Slave
msp430fr2422_euscib0_i2c_16.c            Remapped eUSCI_B0 I2C Slave TX multiple bytes to MSP430 Master
msp430fr2422_framwrite.c                 Long word writes to FRAM
msp430fr2422_framwrite_persistent.c      FRAM write, use #pragma PERSISTENT to initialize variable in FRAM
msp430fr2422_lpm3_01.c                   Enters LPM3 with ACLK = XT1CLK = 32768Hz.
msp430fr2422_lpm3_5_01.c                 LPM3.5, device enter LPM3.5 and toggles P1.0 with RTC interrupt handling every 1s
msp430fr2422_lpm4_5_01.c                 LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.2
msp430fr2422_lpm4_5_02.c                 LPM4.5, Device enters LPM4.5 waiting for a port interrupt on P1.2 with SVS disabled
msp430fr2422_P1_01.c                     Software Poll P1.3, Set P1.0 if P1.3 = 1
msp430fr2422_P1_03.c                     Software Port Interrupt Service on P1.3 from LPM3
msp430fr2422_RTC_01.c                    RTC, toggle P1.0 every 1s
msp430fr2422_RTC_02.c                    RTC, device enter LPM3.5 and toggle P1.0 in RTC interrupt handling every 1s
msp430fr2422_ta0_01.c                    Timer0_A3, Toggle P1.0, CCR0 Count Mode ISR, DCO SMCLK
msp430fr2422_ta0_02.c                    Timer0_A3, Toggle P1.0, CCR0 Up Mode ISR, DCO SMCLK
msp430fr2422_ta0_04.c                    Timer0_A3, Toggle P1.0, Overflow ISR, 32kHz ACLK
msp430fr2422_ta0_16.c                    Timer0_A3, PWM TA0.1-2, Up Mode, DCO SMCLK
msp430fr2422_ta0_17.c                    Timer0_A3, PWM TA0.1-2, Up Mode, 32KHz ACLK
msp430fr2422_ta0_20.c                    Timer0_A3, PWM TA0.1-2, Up/Down Mode, 32kHz ACLK
msp430fr2422_ta0_22.c                    Timer_A, Ultra-Low Pwr Pulse Accumulator
msp430fr2422_ta0_capture.c               Timer0_A3 Capture of ACLK
msp430fr2422_ta1_05.c                    Timer1_A3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK
msp430fr2422_ta1_06.c                    Timer1_A3, Toggle P1.0, CCR0 UP Mode ISR, 32KHz ACLK
msp430fr2422_ta1_16.c                    Timer1_A3, PWM TA1.1-2, Up Mode, DCO SMCLK
msp430fr2422_ta1_17.c                    Timer1_A3, PWM TA1.1-2, Up Mode, 32KHz ACLK
msp430fr2422_WDT_01.c                    WDT, Toggle P1.0, Interval Overflow ISR, DCO SMCLK
msp430fr2422_WDT_02.c                    WDT, Toggle P1.0, Interval Overflow ISR, 32kHz ACLK
msp430fr2422_WDT_04.c                    WDT,  Failsafe Clock, WDT mode, DCO SMCLK
msp430fr2422_WDT_05.c                    WDT,  Reset on Invalid Address fetch, Toggle P1.0
msp430fr2422_WDT_06.c                    WDT,  Failsafe Clock, WDT mode, 32kHz ACLK

