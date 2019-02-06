
#include <stdint.h>
#include <stdbool.h>
#include "grlib.h"
#include "button.h"

//*****************************************************************************
//
//! \addtogroup button_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button. The button will contain a text string and will
//! be created based on the parameters passed in the button struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawButton(const Graphics_Context *context,
		const Graphics_Button *button)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle borderRect ={
			button->xMin,
			button->yMin,
			button->xMax,
			button->yMax,
	};

	Graphics_Rectangle innerRect ={
			button->xMin + button->borderWidth,
			button->yMin + button->borderWidth,
			button->xMax - button->borderWidth,
			button->yMax - button->borderWidth,
	};


	Graphics_setForegroundColor(&g_sContext,button->borderColor);
	Graphics_fillRectangle(&g_sContext, &borderRect);

	if(button->selected){
		Graphics_setForegroundColor(&g_sContext,button->selectedColor);
	}else{
		Graphics_setForegroundColor(&g_sContext,button->fillColor);
	}

	Graphics_fillRectangle(&g_sContext, &innerRect);

	Graphics_setFont(&g_sContext,button->font);

	if(button->selected){
		Graphics_setForegroundColor(&g_sContext,button->selectedTextColor);
	}else{
		Graphics_setForegroundColor(&g_sContext,button->textColor);
	}
	Graphics_drawString(&g_sContext,
			button->text,
			AUTO_STRING_LENGTH,
			button->textXPos,
			button->textYPos,
			TRANSPARENT_TEXT);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}


//*****************************************************************************
//
//! Determines if x and y coordinates are contained in button .
//!
//! \param button is a pointer to the structure containing the extents of the
//! button.
//! \param x x-coordinate to be determined if is inside button 
//! \param y y-coordinate to be determined if is inside button 
//!
//! This function determines if x and y coordinates are contains inside button
//!
//! \return true if x and y coordinates are inside button, false if not
//
//*****************************************************************************
bool Graphics_isButtonSelected(const Graphics_Button *button,
		uint16_t x, uint16_t y)
{
	return ((((x) >= button->xMin) && ((x) <= button->xMax) &&
            ((y)) >= button->yMin) && ((y) <= button->yMax) ? true : false);

}

//*****************************************************************************
//
//! Draws a selected Button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button using the selected parameters. 
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawSelectedButton(const Graphics_Context *context,
		const Graphics_Button *button)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle innerRect ={
			button->xMin + button->borderWidth,
			button->yMin + button->borderWidth,
			button->xMax - button->borderWidth,
			button->yMax - button->borderWidth,
	};

	Graphics_setForegroundColor(&g_sContext,button->selectedColor);
	Graphics_fillRectangle(&g_sContext, &innerRect);

	Graphics_setFont(&g_sContext,button->font);

	Graphics_setForegroundColor(&g_sContext,button->selectedTextColor);
	Graphics_drawString(&g_sContext,
			button->text,
			AUTO_STRING_LENGTH,
			button->textXPos,
			button->textYPos,
			TRANSPARENT_TEXT);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released Button.
//!
//! \param context is a pointer to the drawing context to use.
//! \param button is a pointer to the structure containing the extents of the
//! button.
//!
//! This function draws a button using the released parameters. 
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawReleasedButton(const Graphics_Context *context,
		const Graphics_Button *button)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle innerRect ={
			button->xMin + button->borderWidth,
			button->yMin + button->borderWidth,
			button->xMax - button->borderWidth,
			button->yMax - button->borderWidth,
	};

	Graphics_setForegroundColor(&g_sContext,button->fillColor);
	Graphics_fillRectangle(&g_sContext, &innerRect);

	Graphics_setFont(&g_sContext,button->font);

	Graphics_setForegroundColor(&g_sContext,button->textColor);
	Graphics_drawString(&g_sContext,
			button->text,
			AUTO_STRING_LENGTH,
			button->textXPos,
			button->textYPos,
			TRANSPARENT_TEXT);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
