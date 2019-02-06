
#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_



//*****************************************************************************
//
//! This structure defines the characteristics of a RadioButton
//
//*****************************************************************************
typedef struct Graphics_RadioButton
{
	uint16_t xPosition;			/*!< x coordinate for upper left corner of button */
	uint16_t yPosition;         /*!< y coordinate for upper left corner of button */
	bool selected;				/*!< draws button with selected colors enabled */
	uint8_t gap;				/*!< draws checbox with checkbox selected in pixels*/
	uint32_t textColor;			/*!< Text color */
	uint8_t numbOfChar;			/*!< Number of characters in checkbox message */
	uint32_t selectedColor;		/*!< Selected radio color */
	uint32_t notSelectedColor;	/*!< Not Selected radio color */
	const Graphics_Font *font;	/*!< Font for text to be displayed in button */
	int8_t *text;               /*!< Text to be displayed in button */
} Graphics_RadioButton;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton);
extern bool Graphics_isRadioButtonSelected(
        const Graphics_RadioButton *radioButton,uint16_t x, uint16_t y);
extern void Graphics_drawSelectedRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton);
extern void Graphics_drawReleasedRadioButton(const Graphics_Context *context,
		const Graphics_RadioButton *radioButton);

#endif /* RADIOBUTTON_H_ */
