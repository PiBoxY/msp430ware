
#ifndef IMAGEBUTTON_H_
#define IMAGEBUTTON_H_

//*****************************************************************************
//
//! This structure defines the characteristics of a ImageButton
//
//*****************************************************************************
typedef struct Graphics_ImageButton
{
	uint16_t xPosition;		/*!< x coordinate for upper left corner of button */
	uint16_t yPosition;		/*!< y coordinate for upper left corner of button */
	uint8_t borderWidth;	/*!< border width in pixels */
	bool selected;			/*!< draws button with selected colors enabled */
	uint16_t imageHeight;	/*!< image height in pixels */
	uint16_t imageWidth;	/*!< image width in pixels */
	uint32_t borderColor;	/*!< Border color, border is drawn if borderWidth >0 */
	uint32_t selectedColor;	/*!< Background button color when selected */
	Graphics_Image *image;	/*!< Pointer to the image */
} Graphics_ImageButton;

//*****************************************************************************
//globals
//*****************************************************************************
extern Graphics_Context g_sContext;

//*****************************************************************************
// the function prototypes
//*****************************************************************************
extern void Graphics_drawImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton);
extern bool Graphics_isImageButtonSelected(
        const Graphics_ImageButton *imageButton,uint16_t x, uint16_t y);
extern void Graphics_drawSelectedImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton);
extern void Graphics_drawReleasedImageButton(const Graphics_Context *context,
		const Graphics_ImageButton *imageButton);

#endif /* IMAGEBUTTON_H_ */
