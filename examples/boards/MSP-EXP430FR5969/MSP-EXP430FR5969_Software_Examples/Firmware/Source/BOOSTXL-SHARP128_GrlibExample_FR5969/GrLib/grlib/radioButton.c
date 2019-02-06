#include <stdint.h>
#include <stdbool.h>
#include "grlib.h"
#include "radioButton.h"

//*****************************************************************************
//
//! \addtogroup radioButton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton is a pointer to the structure containing the extents of 
//! the RadioButton.
//!
//! This function draws a RadioButton  . The RadioButton   will contain the 
//! image passed in the RadioButton struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton)
{
	uint8_t outerRadius, innerRadius;
	uint16_t textXPos, textYPos;
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_setForegroundColor(&g_sContext, radioButton->textColor);

	// Define outer circle position and radius
	outerRadius = (((radioButton->font)->height) >> 1 );

	Graphics_drawCircle(&g_sContext,
			radioButton->xPosition + outerRadius,
			radioButton->yPosition + outerRadius,
			(uint32_t) outerRadius);

	if(radioButton->selected){
		Graphics_setForegroundColor(&g_sContext, radioButton->selectedColor);
		innerRadius = outerRadius - 2;
		Graphics_fillCircle(&g_sContext,
				radioButton->xPosition + outerRadius,
				radioButton->yPosition + outerRadius,
				(uint32_t) innerRadius);
	}

	textXPos =  (radioButton->xPosition) + (2*outerRadius) +(radioButton->gap);
	textYPos =  (radioButton->yPosition);

	Graphics_setForegroundColor(&g_sContext, radioButton->textColor);
	Graphics_setFont(&g_sContext,radioButton->font);


	Graphics_drawString(&g_sContext,
				radioButton->text,
				AUTO_STRING_LENGTH,
				textXPos,
				textYPos,
				TRANSPARENT_TEXT);



	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Determines if x and y coordinates are contained in RadioButton.
//!
//! \param radioButton   is a pointer to the structure containing the extents of 
//! the RadioButton.
//! \param x x-coordinate to be determined if is inside RadioButton   
//! \param y y-coordinate to be determined if is inside RadioButton   
//!
//! This function determines if x and y coordinates are contains inside 
//! RadioButton.
//!
//! \return true if x and y coordinates are inside RadioButton, false if not
//
//*****************************************************************************

bool Graphics_isRadioButtonSelected(const Graphics_RadioButton *radioButton,
		uint16_t x, uint16_t y)
{
	uint16_t stringSize = radioButton->numbOfChar * (radioButton->font)->maxWidth;
	uint8_t radio = (((radioButton->font)->height) >> 1 );


	return ((((x) >= radioButton->xPosition) &&
			((x) <= radioButton->xPosition + stringSize + (2*radio) + radioButton->gap) &&
            ((y)) >= radioButton->yPosition) &&
			((y) <= radioButton->yPosition  + (2*radio)) ? true : false);

}

//*****************************************************************************
//
//! Draws a selected RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton   is a pointer to the structure containing the extents 
//! of the RadioButton.
//!
//! This function draws a RadioButton using the selected parameters. 
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawSelectedRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton)
{
	uint8_t outerRadius,innerRadius;
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	// Define outer circle position and radius
	outerRadius = (((radioButton->font)->height) >> 1 );
	innerRadius = outerRadius - 2;

	Graphics_setForegroundColor(&g_sContext, radioButton->selectedColor);
	Graphics_fillCircle(&g_sContext,
			radioButton->xPosition + outerRadius,
			radioButton->yPosition + outerRadius,
			(uint32_t) innerRadius);



	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released RadioButton.
//!
//! \param context is a pointer to the drawing context to use.
//! \param radioButton   is a pointer to the structure containing the extents of 
//! the RadioButton.
//!
//! This function draws a RadioButton using the released parameters. 
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawReleasedRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton)
{
	uint8_t outerRadius,innerRadius;
	uint32_t origFrgnd = g_sContext.foreground;

	// Define outer circle position and radius
	outerRadius = (((radioButton->font)->height) >> 1);
	innerRadius = outerRadius - 2;

	Graphics_setForegroundColor(&g_sContext,radioButton->notSelectedColor);

	Graphics_fillCircle(&g_sContext,
			radioButton->xPosition + outerRadius,
			radioButton->yPosition + outerRadius,
			(uint32_t) innerRadius);

	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************


