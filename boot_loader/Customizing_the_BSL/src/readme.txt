BSL for MSP430 - Source code for custom BSLs and release BSL images
===================================================================

This packages contains the following subdirectories:

1. CCS_BSL_Source
   |__CCS_v5_MSP430F543xA_TA_UART
   |__CCS_v5_MSP430G2xx_TA_UART
   |__CCS_v5_MSP430F522x_I2C
   |__CCS_v7_MSP430F543xA_TA_UART
   |__CCS_v7_MSP430G2xx_TA_UART
   |__CCS_v7_MSP430F522x_I2C
   |__Command_Interpreter
   |__Peripheral_Interfaces
   |__API

2. IAR_BSL_Source
   |__IAR_v5_CC430F614x_USCI_UART
   |__IAR_v5_MSP430F522x_I2C
   |__IAR_v5_MSP430F543xA_TA_UART
   |__IAR_v5_MSP430F552x_TA_UART
   |__IAR_v5_MSP430F552x_USB
   |__IAR_v5_MSP430G2xx_TA_UART
   |__IAR_v64_CC430F614x_USCI_UART
   |__IAR_v64_MSP430F522x_I2C
   |__IAR_v64_MSP430F543xA_TA_UART
   |__IAR_v64_MSP430F552x_TA_UART
   |__IAR_v64_MSP430F552x_USB
   |__IAR_v64_MSP430G2xx_TA_UART
   |__Command_Interpreter
   |__Peripheral_Interfaces
   |__API


3. Released_BSL_Images
   |__CC430_Family
   |     contains the image of:
   |        - UART BSL (00.05.04.52)
   |        - UART BSL (00.07.05.53)
   |__MSP430F522x_Family
   |     contains the image of:
   |        - I2C BSL  (00.07.06.94)
   |        - UART BSL (00.07.05.04)
   |        - UART BSL (00.08.08.04)
   |__MSP430F525x_Family
   |     contains the image of UART and I2C BSL.
   |     There are 2 types of UART BSL: 
   |         - DVIO supply domain (00.08.08.53)
   |         - DVCC supply domain (00.08.08.04) 
   |__MSP430F532x_534x_530x_Family
   |     contains the image of:
   |        - UART BSL (00.06.04.04)
   |        - UART BSL (00.07.05.04)
   |__MSP430F552x_550x_Family
   |     contains the image of:
   |        - UART BSL    (00.07.05.04)
   |        - USB BSL     (00.07.88.38)
   |        - USB BSL     (00.08.88.39)
   |        - RAM USB BSL (00.07.88.38)
   |        - RAM USB BSL (00.08.88.39)
   |__MSP430F663x_Family
   |     contains the image of:
   |        - UART BSL    (00.07.05.04)
   |        - UART BSL    (00.07.06.04)
   |        - USB BSL     (00.07.88.38)
   |        - USB BSL     (00.08.88.39)
   |        - RAM USB BSL (00.07.88.38)
   |        - RAM USB BSL (00.08.88.39)
   |__MSP430F665x_Family
   |     contains the image of:
   |        - UART BSL    (00.07.06.04)
   |        - USB BSL     (00.07.86.36)
   |        - USB BSL     (00.08.88.38)
   |        - USB BSL     (00.08.88.39)
   |        - RAM USB BSL (00.07.86.36)
   |        - RAM USB BSL (00.07.88.38)
   |        - RAM USB BSL (00.08.88.39)
   |__MSP430F673x_Family
   |     contains the image of:
   |        - UART BSL    (00.07.05.04)
   |__MSP430F677x_Family
   |     contains the image of:
   |        - UART BSL    (00.07.05.04)
   |__MSP430F5438A_Family
   |     contains the image of:
   |        - UART BSL    (00.05.04.03)
   |        - UART BSL    (00.07.05.04)
   |__MSP430FG662x_Family
   |     contains the image of:
   |        - UART BSL    (00.08.08.04)
   |        - USB BSL     (00.08.88.38)
   |        - USB BSL     (00.08.88.39)
   |        - RAM USB BSL (00.07.88.38)
   |        - RAM USB BSL (00.08.88.39)
   |_______________________________________________________________

   Note: one BSL version might be programmed and used for different
         devices. Please refer to www.ti.com/lit/slau319 to get the
         information regarding the BSL version for specific device.

===================================================================
IDE version
===================================================================
CCS_v5 is provided by CCS v5.5.0.00077
CCS_v7 is provided by CCS v7.4.0.00015
    This version shall be supported by CCS v6, v7, and v8.

IAR_v5 is provided by IAR v5.51.6
IAR_v6 is provided by CCS v6.40.4
    This version shall be supported by IAR v6 and v7

===================================================================


