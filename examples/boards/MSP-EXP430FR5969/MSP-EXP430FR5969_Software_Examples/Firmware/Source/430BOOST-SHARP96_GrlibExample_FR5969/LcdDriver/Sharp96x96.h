//*****************************************************************************
//
// Sharp96x96.h - Prototypes for the Sharp96x96 LCD display driver
//
//*****************************************************************************


#ifndef __SHARPLCD_H__
#define __SHARPLCD_H__

#include "grlib.h"

//*****************************************************************************
//
// User Configuration for the LCD Driver
//
//*****************************************************************************

// SYSTEM_CLOCK_SPEED (in Hz) allows to properly closeout SPI communication
#define SYSTEM_CLOCK_SPEED      12000000


// LCD Screen Dimensions
#define LCD_VERTICAL_MAX                   96
#define LCD_HORIZONTAL_MAX                 96


// Define LCD Screen Orientation Here
#define LANDSCAPE

//Maximum Colors in an image color palette
#define MAX_PALETTE_COLORS  2

//*****************************************************************************
//
// Macros for the Display Driver
//
//*****************************************************************************
#define SHARP_BLACK							0x00
#define SHARP_WHITE							0xFF
#define SHARP_SEND_TOGGLE_VCOM_COMMAND		0x01
#define SHARP_SKIP_TOGGLE_VCOM_COMMAND 		0x00
#define SHARP_LCD_TRAILER_BYTE				0x00
#define SHARP_VCOM_TOGGLE_BIT 		   		0x40
#define SHARP_LCD_CMD_CHANGE_VCOM			0x00
#define SHARP_LCD_CMD_CLEAR_SCREEN			0x20
#define SHARP_LCD_CMD_WRITE_LINE			0x80

//*****************************************************************************
//
// Prototypes for the globals exported by this driver.
//
//*****************************************************************************
extern const Graphics_Display g_sharp96x96LCD;

extern void Sharp96x96_SendToggleVCOMCommand(void);
extern void Sharp96x96_initDisplay(void);
extern void Sharp96x96_disable(void);
extern void Sharp96x96_enable(void);
#endif // __SHARPLCD_H__
