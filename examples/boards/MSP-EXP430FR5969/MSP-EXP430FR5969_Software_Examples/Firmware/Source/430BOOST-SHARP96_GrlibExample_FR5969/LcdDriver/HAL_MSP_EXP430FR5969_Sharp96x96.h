//*****************************************************************************
//
// HAL_MSP-EXP430FR5969_Sharp96x96.h - Prototypes for the Sharp96x96
//                  LCD display driver. There is no output from Sharp96x96 LCD
//
//                   MSP430FR5969                 LCD Sharp96x96
//                -----------------             -----------------
//               |     P2.2/UCB0CLK|---------> |SPI_CLK  EXT_MODE|--GND
//            /|\|                 |           |            	 |
//             | |    P1.6/UCB0SIMO|---------> |SPI_SI   EXTCOMIN|--GND
//             --|RST              |           |              	 |
//               |             P2.4|---------> |SPI_CS           |
//               |             	   |		   |         	     |
//               |             P4.3|---------> |DISP             |
//               |                 |		   |                 |
//               |             P4.2|-----*---> |VDD              |
//               |                 |      `--> |VDDA             |
//               |                 |            -----------------
//                -----------------
//*****************************************************************************


#ifndef __HAL_MSP_EXP430FR5969_SHARPLCD_H__
#define __HAL_MSP_EXP430FR5969_SHARPLCD_H__


//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// Ports from MSP430 connected to LCD
#define LCD_SPI_SI_PORT                     GPIO_PORT_P1
#define LCD_SPI_CLK_PORT                    GPIO_PORT_P2
#define LCD_DISP_PORT                       GPIO_PORT_P4
#define LCD_POWER_PORT                      GPIO_PORT_P4
#define LCD_SPI_CS_PORT	                    GPIO_PORT_P2


// Pins from MSP430 connected to LCD
#define LCD_SPI_SI_PIN                      GPIO_PIN6
#define LCD_SPI_SI_PIN_FUNCTION             GPIO_SECONDARY_MODULE_FUNCTION
#define LCD_SPI_CLK_PIN                     GPIO_PIN2
#define LCD_SPI_CLK_PIN_FUNCTION            GPIO_SECONDARY_MODULE_FUNCTION
#define LCD_DISP_PIN                        GPIO_PIN3
#define LCD_POWER_PIN                       GPIO_PIN2
#define LCD_SPI_CS_PIN                      GPIO_PIN4


// Definition of USCI base address to be used for SPI communication
#define LCD_EUSCI_BASE		      EUSCI_B0_BASE


// Non-volatile Memory used to store DisplayBuffer
#define NON_VOLATILE_MEMORY_BUFFER


//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern void HAL_LCD_initDisplay(void);
extern void HAL_LCD_writeCommandOrData(uint16_t command);
extern void HAL_LCD_clearCS(void);
extern void HAL_LCD_setCS(void);
extern void HAL_LCD_prepareMemoryWrite(void);
extern void HAL_LCD_finishMemoryWrite(void);
extern void HAL_LCD_waitUntilLcdWriteFinish(void);
extern void HAL_LCD_disableDisplay(void);
extern void HAL_LCD_enableDisplay(void);


#endif // __HAL_MSP_EXP430FR5969_SHARPLCD_H__
