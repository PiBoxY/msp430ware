#include <stdint.h>
#include <stdbool.h>
#include "grlib.h"
#include "imageButton.h"

//*****************************************************************************
//
//! \addtogroup imageButton_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton . The ImageButton  will contain the image
//! passed in the ImageButton  struct.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle borderRect ={
			imageButton->xPosition,
			imageButton->yPosition,
			imageButton->xPosition + imageButton->imageWidth + (2 * imageButton->borderWidth),
			imageButton->yPosition + imageButton->imageHeight + (2 * imageButton->borderWidth),
	};


	if(imageButton->selected){
		Graphics_setForegroundColor(&g_sContext,imageButton->selectedColor);
	}else{
		Graphics_setForegroundColor(&g_sContext,imageButton->borderColor);
	}
	Graphics_fillRectangle(&g_sContext,&borderRect);

	Graphics_drawImage(&g_sContext,imageButton->image,
			imageButton->xPosition + imageButton->borderWidth,
			imageButton->yPosition  + imageButton->borderWidth);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Determines if x and y coordinates are contained in ImageButton  .
//!
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//! \param x x-coordinate to be determined if is inside ImageButton  
//! \param y y-coordinate to be determined if is inside ImageButton  
//!
//! This function determines if x and y coordinates are contains inside ImageButton 
//!
//! \return true if x and y coordinates are inside ImageButton, false if not
//
//*****************************************************************************

bool Graphics_isImageButtonSelected(const Graphics_ImageButton *imageButton,
		uint16_t x, uint16_t y)
{
	return ((((x) >= imageButton->xPosition) &&
			((x) <= imageButton->xPosition + imageButton->imageWidth + imageButton->borderWidth) &&
            ((y)) >= imageButton->yPosition) &&
			((y) <= imageButton->yPosition  + imageButton->imageHeight + imageButton->borderWidth) ?
            		true : false);

}


//*****************************************************************************
//
//! Draws a selected ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton  using the selected parameters. 
//!
//! \return None.
//
//*****************************************************************************

void Graphics_drawSelectedImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle borderRect ={
			imageButton->xPosition,
			imageButton->yPosition,
			imageButton->xPosition + imageButton->imageWidth + (2 * imageButton->borderWidth),
			imageButton->yPosition + imageButton->imageHeight + (2 * imageButton->borderWidth),
	};

	Graphics_setForegroundColor(&g_sContext,imageButton->selectedColor);

	Graphics_fillRectangle(&g_sContext,&borderRect);

	Graphics_drawImage(&g_sContext,imageButton->image,
			imageButton->xPosition + imageButton->borderWidth,
			imageButton->yPosition  + imageButton->borderWidth);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
//! Draws a released ImageButton .
//!
//! \param context is a pointer to the drawing context to use.
//! \param imageButton  is a pointer to the structure containing the extents of the
//! ImageButton .
//!
//! This function draws a ImageButton  using the released parameters. 
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawReleasedImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton)
{
	uint32_t origFrgnd = g_sContext.foreground;
	Graphics_Font *origFont = (Graphics_Font *) g_sContext.font;

	Graphics_Rectangle borderRect ={
			imageButton->xPosition,
			imageButton->yPosition,
			imageButton->xPosition + imageButton->imageWidth + (2 * imageButton->borderWidth),
			imageButton->yPosition + imageButton->imageHeight + (2 * imageButton->borderWidth),
	};

	Graphics_setForegroundColor(&g_sContext,imageButton->borderColor);

	Graphics_fillRectangle(&g_sContext,&borderRect);

	Graphics_drawImage(&g_sContext,imageButton->image,
			imageButton->xPosition + imageButton->borderWidth,
			imageButton->yPosition  + imageButton->borderWidth);

	Graphics_setFont(&g_sContext,origFont);
	Graphics_setForegroundColor(&g_sContext,origFrgnd);
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************

