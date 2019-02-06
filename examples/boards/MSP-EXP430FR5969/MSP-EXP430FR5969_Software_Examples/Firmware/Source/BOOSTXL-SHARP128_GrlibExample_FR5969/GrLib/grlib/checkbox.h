
#ifndef CHECKBOX_H_
#define CHECKBOX_H_

//*****************************************************************************
// typedefs
//*****************************************************************************


//! \brief This structure defines the characteristics of a Checkbox
//!
typedef struct Graphics_CheckBox
{
	uint16_t xPosition;        /*!< x coordinate for upper left corner of button */
	uint16_t yPosition;        /*!< y coordinate for upper left corner of button */
	bool selected;             /*!< draws checbox with checkbox selected */
	uint8_t gap;			   /*!< draws checbox with checkbox selected in pixels*/
	uint32_t textColor;		   /*!< Text color */
	uint32_t backgroundColor;  /*!<	Color for background of checbox */
	uint32_t selectedColor;	   /*!< Selected check color */
	uint8_t numbOfChar;		   /*!< Number of characters in checkbox message */
	const Graphics_Font *font; /*!< Font for text to be displayed in button */
	int8_t *text;			   /*!< Text to be displayed in button */
} Graphics_CheckBox;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox);
extern bool Graphics_isCheckBoxSelected(
        const Graphics_CheckBox *checkBox,uint16_t x, uint16_t y);
extern void Graphics_drawSelectedCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox);
extern void Graphics_drawReleasedCheckBox(const Graphics_Context *context,
		const Graphics_CheckBox *checkBox);

#endif /* CHECKBOX_H_ */
