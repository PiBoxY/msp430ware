#include <stdint.h>
#include <stdbool.h>
#include "grlib.h"
#include "checkbox.h"

//*****************************************************************************
//
//! \addtogroup checkbox_api
//! @{
//
//*****************************************************************************


//*****************************************************************************
//
//! Draws a checkbox.
//!
//! \param context is a pointer to the drawing context to use.
//! \param checkBox is a pointer to the structure containing the extents of the
//! checkbox.
//!
//! This function draws a checkbox. The checkbox will be created based on the
//! parameters passed in the checkbox struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox)
{
	uint16_t lenght, textXPos, textYPos;
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	// Define outer circle position and radius
	lenght = ((checkBox->font)->height);

	Graphics_Rectangle selRec = {
			checkBox->xPosition,
			checkBox->yPosition,
			checkBox->xPosition + lenght,
			checkBox->yPosition + lenght
	};

	Graphics_setForegroundColor(&g_sContext, checkBox->backgroundColor);
	Graphics_fillRectangle(&g_sContext,&selRec);

	Graphics_setForegroundColor(&g_sContext, checkBox->textColor);
	Graphics_drawRectangle(&g_sContext,&selRec);

	if(checkBox->selected){
		Graphics_setForegroundColor(&g_sContext, checkBox->selectedColor);
		Graphics_drawLine(&g_sContext,
			checkBox->xPosition,
			checkBox->yPosition,
			checkBox->xPosition + lenght,
			checkBox->yPosition + lenght);


		Graphics_drawLine(&g_sContext,
					checkBox->xPosition,
					checkBox->yPosition + lenght,
					checkBox->xPosition + lenght,
					checkBox->yPosition);
	}

	textXPos =  (checkBox->xPosition) + (lenght) +(checkBox->gap);
	textYPos =  (checkBox->yPosition);

	Graphics_setForegroundColor(&g_sContext, checkBox->textColor);
	Graphics_setFont(&g_sContext,checkBox->font);

	Graphics_drawString(&g_sContext,
			checkBox->text,
			AUTO_STRING_LENGTH,
			textXPos,
			textYPos,
			TRANSPARENT_TEXT);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}


//*****************************************************************************
//
//! Determines if x and y coordinates are contained in the checkbox.
//!
//! \param checkBox is a pointer to the structure containing the extents of the
//! checkbox.
//! \param x x-coordinate to be determined if is inside button 
//! \param y y-coordinate to be determined if is inside button .
//!
//! This function determines if x and y coordinates are contains inside checkbox 
//! struct.
//!
//! \return true if x and y coordinates are inside checkbox, false if not
//
//*****************************************************************************

bool Graphics_isCheckBoxSelected(const Graphics_CheckBox *checkBox,
		uint16_t x, uint16_t y)
{
	uint16_t stringSize = checkBox->numbOfChar * (checkBox->font)->maxWidth;
	uint8_t length = ((checkBox->font)->height);


	return ((((x) >= checkBox->xPosition) &&
			((x) <= checkBox->xPosition + stringSize + (2*length) + checkBox->gap) &&
            ((y)) >= checkBox->yPosition) &&
			((y) <= checkBox->yPosition  + (2*length)) ? true : false);

}

//*****************************************************************************
//
//! Draws a selected Checkbox.
//!
//! \param context is a pointer to the drawing context to use.
//! \param checkBox is a pointer to the structure containing the extents of the
//! checkBox.
//!
//! This function draws a selected checkbox using the  selected parameters.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawSelectedCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox)
{
	uint8_t lenght;
	uint32_t origFrgnd = g_sContext.foreground;

	// Define outer circle position and radius
	lenght = ((checkBox->font)->height);

	Graphics_setForegroundColor(&g_sContext, checkBox->selectedColor);

	Graphics_drawLine(&g_sContext,
			checkBox->xPosition,
			checkBox->yPosition,
			checkBox->xPosition + lenght,
			checkBox->yPosition + lenght);


	Graphics_drawLine(&g_sContext,
			checkBox->xPosition,
			checkBox->yPosition + lenght,
			checkBox->xPosition + lenght,
			checkBox->yPosition);

	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released Checkbox.
//!
//! \param context is a pointer to the drawing context to use.
//! \param checkBox is a pointer to the structure containing the extents of the
//! checkBox.
//!
//! This function draws a released checkbox using the  selected parameters.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawReleasedCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox)
{
	uint32_t origFrgnd = g_sContext.foreground;
	uint16_t lenght;

	// Define outer circle position and radius
	lenght = ((checkBox->font)->height);

	Graphics_Rectangle selRec = {
			checkBox->xPosition,
			checkBox->yPosition,
			checkBox->xPosition + lenght,
			checkBox->yPosition + lenght
	};

	Graphics_setForegroundColor(&g_sContext, checkBox->backgroundColor);
	Graphics_fillRectangle(&g_sContext,&selRec);

	Graphics_setForegroundColor(&g_sContext, checkBox->textColor);
	Graphics_drawRectangle(&g_sContext,&selRec);

	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}
//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
