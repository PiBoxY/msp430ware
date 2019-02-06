#ifndef BUTTON_H_
#define BUTTON_H_

//*****************************************************************************
// typedefs
//*****************************************************************************


//! \brief This structure defines the characteristics of a Button
//!

typedef struct Graphics_Button
{
	uint16_t xMin;	       /*!< x coordinate for upper left corner of button */
	uint16_t xMax;         /*!< x coordinate for lower right corner of button */
	uint16_t yMin;         /*!< y coordinate for upper left corner of button */
	uint16_t yMax;         /*!< y coordinate for lower right corner of button */
	uint8_t borderWidth;   /*!< border width */
	bool selected;	       /*!< draws button with selected colors enabled */
	uint32_t fillColor;    /*!< Background button color when not selected */
	uint32_t borderColor;  /*!< Border color, border is drawn if borderWidth >0 */
	uint32_t selectedColor;/*!< Background button color when selected */
	uint32_t textColor;	   /*!< Text color */
	uint32_t selectedTextColor; /*!< Selected text color */
	uint16_t textXPos;     /*!< x coordinate for text upper left corner */
	uint16_t textYPos;     /*!< y coordinate for text upper left corner */
	int8_t *text;		   /*!< Text to be displayed in button */
	const Graphics_Font *font; /*!< Font for text to be displayed in button */
} Graphics_Button;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawButton(const Graphics_Context *context,
		const Graphics_Button *button);
extern bool Graphics_isButtonSelected(const Graphics_Button *button,
		uint16_t x, uint16_t y);
extern void Graphics_drawSelectedButton(const Graphics_Context *context,
		const Graphics_Button *button);
extern void Graphics_drawReleasedButton(const Graphics_Context *context,
		const Graphics_Button *button);

#endif /* BUTTON_H_ */
