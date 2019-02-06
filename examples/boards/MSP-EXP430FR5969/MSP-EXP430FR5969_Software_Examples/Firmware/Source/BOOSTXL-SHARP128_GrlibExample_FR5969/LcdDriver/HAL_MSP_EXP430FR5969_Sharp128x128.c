//*****************************************************************************
//
// HAL_MSP-EXP430FR5969_Sharp128x128.c
//
//*****************************************************************************
//
//! \addtogroup display_api
//! @{
//
//*****************************************************************************

#include "grlib.h"
#include "HAL_MSP_EXP430FR5969_Sharp128x128.h"
#include "driverlib.h"



//*****************************************************************************
//
//! Initializes the display driver.
//!
//! This function initializes the Sharp128x128 display. This function
//! configures the GPIO pins used to control the LCD display when the basic
//! GPIO interface is in use. On exit, the LCD has been reset and is ready to
//! receive command and data writes.
//!
//! \return None.
//
//*****************************************************************************
void HAL_LCD_initDisplay(void)
{
    // Configure LCD_SCLK_PIN for SPI_CLK mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SPI_CLK_PORT,
                LCD_SPI_CLK_PIN , LCD_SPI_CLK_PIN_FUNCTION);

    // Configure LCD_MOSI_PIN for SPI_MOSI mode
    GPIO_setAsPeripheralModuleFunctionOutputPin(LCD_SPI_SI_PORT,
    		LCD_SPI_SI_PIN , LCD_SPI_SI_PIN_FUNCTION);

    // Provide power to LCD
	GPIO_setAsOutputPin(LCD_POWER_PORT,
	        LCD_POWER_PIN);
	
	GPIO_setOutputHighOnPin(LCD_POWER_PORT,
            LCD_POWER_PIN);
				
	// Turn on DISP
	GPIO_setAsOutputPin(LCD_DISP_PORT,
	        LCD_DISP_PIN);
	
	GPIO_setOutputHighOnPin(LCD_DISP_PORT,
            LCD_DISP_PIN);		
	
			
	// Configure LCD_SPI_CS_PIN as output pin
    GPIO_setAsOutputPin(LCD_SPI_CS_PORT,
    		LCD_SPI_CS_PIN);
			
	HAL_LCD_clearCS();

	EUSCI_B_SPI_initMasterParam spiMasterParams=
	{
		EUSCI_B_SPI_CLOCKSOURCE_SMCLK,
		8000000,
		1000000,
		EUSCI_B_SPI_MSB_FIRST,
		EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT,
		EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
		EUSCI_B_SPI_3PIN
	};

    EUSCI_B_SPI_initMaster(LCD_EUSCI_BASE,
                    &spiMasterParams);

    EUSCI_B_SPI_enable(LCD_EUSCI_BASE);

}



//*****************************************************************************
//
// Writes command or data to the LCD Driver
//
// \param ucCmdData is the 8 or 16 bit command to send to the LCD driver
// Uses the SET_LCD_DATA macro
//
// \return None
//
//*****************************************************************************
void HAL_LCD_writeCommandOrData(uint16_t command)
{


	while (!EUSCI_B_SPI_getInterruptStatus(LCD_EUSCI_BASE,
	                    EUSCI_B_SPI_TRANSMIT_INTERRUPT));
	EUSCI_B_SPI_transmitData(LCD_EUSCI_BASE,command);


}

//*****************************************************************************
//
// Clears CS line
//
// This macro allows to clear the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
void HAL_LCD_clearCS(void){
	GPIO_setOutputLowOnPin(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN);
}

//*****************************************************************************
//
// Set CS line
//
// This macro allows to set the Chip Select (CS) line
//
// \return None
//
//*****************************************************************************
void HAL_LCD_setCS(void){
	GPIO_setOutputHighOnPin(LCD_SPI_CS_PORT, LCD_SPI_CS_PIN);
}

//*****************************************************************************
//
// Waits until the SPI communication with the LCD is finished a command to
// the LCD Driver
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_waitUntilLcdWriteFinish(void)
{
	while  (EUSCI_B_SPI_isBusy(LCD_EUSCI_BASE));
}

//*****************************************************************************
//
// Disables Shapr128x128 LCD
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_disableDisplay(void)
{
	GPIO_setOutputLowOnPin(LCD_DISP_PORT, LCD_DISP_PIN);
	GPIO_setOutputLowOnPin(LCD_POWER_PORT, LCD_POWER_PIN);
}

//*****************************************************************************
//
// Enables Shapr128x128 LCD
//
// \param None
//
// \return None
//*****************************************************************************
void HAL_LCD_enableDisplay(void)
{
	GPIO_setOutputHighOnPin(LCD_POWER_PORT, LCD_POWER_PIN);
	GPIO_setOutputHighOnPin(LCD_DISP_PORT, LCD_DISP_PIN);

}


//*****************************************************************************
//
// Prepare to write memory
//
// This macro unlocks flash memory controller and
// sets access right on flash controller
//
// \return None
//
//*****************************************************************************
void HAL_LCD_prepareMemoryWrite()
{
}


//*****************************************************************************
//
// Finish memory writing
//
// This macro removes access rights on flash controller and
// locks flash memory controller.
//
// \return None
//
//*****************************************************************************
void HAL_LCD_finishMemoryWrite()
{
	__no_operation();
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
