#include "grlib.h"

//*****************************************************************************
//
//! \addtogroup circle_api
//! @{
//
//*****************************************************************************

//*****************************************************************************
//
//! Draws a circle.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the center of the circle.
//! \param y is the Y coordinate of the center of the circle.
//! \param radius is the radius of the circle.
//!
//! This function draws a circle, utilizing the Bresenham circle drawing
//! algorithm.  The extent of the circle is from \e x - \e radius to \e x +
//! \e radius and \e y - \e radius to \e y + \e radius, inclusive.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_drawCircle(const Graphics_Context *context, int32_t x, int32_t y,
		int32_t  radius)
{
    int32_t  a, b, d, x1, y1;

    //
    // Check the arguments.
    //
    assert(context);

    //
    // Initialize the variables that control the Bresenham circle drawing
    // algorithm.
    //
    a = 0;
    b = radius;
    d = 3 - (2 * radius);

    //
    // Loop until the A delta is greater than the B delta, meaning that the
    // entire circle has been drawn.
    //
    while(a <= b)
    {
        //
        // Determine the row when subtracting the A delta.
        //
        y1 = y - a;

        //
        // See if this row is within the clipping region.
        //
        if((y1 >= context->clipRegion.yMin) &&
           (y1 <= context->clipRegion.yMax))
        {
            //
            // Determine the column when subtracting the B delta.
            //
            x1 = x - b;

            //
            // If this column is within the clipping region, then draw a pixel
            // at that position.
            //
            if((x1 >= context->clipRegion.xMin) &&
               (x1 <= context->clipRegion.xMax))
            {
                Graphics_drawPixel(context, x1, y1);
            }

            //
            // Determine the column when adding the B delta.
            //
            x1 = x + b;

            //
            // If this column is within the clipping region, then draw a pixel
            // at that position.
            //
            if((x1 >= context->clipRegion.xMin) &&
               (x1 <= context->clipRegion.xMax))
            {
                Graphics_drawPixel(context, x1, y1);
            }
        }

        //
        // Determine the row when adding the A delta.
        //
        y1 = y + a;

        //
        // See if this row is within the clipping region, and the A delta is
        // not zero (otherwise, it will be the same row as when the A delta was
        // subtracted).
        //
        if((y1 >= context->clipRegion.yMin) &&
           (y1 <= context->clipRegion.yMax) &&
           (a != 0))
        {
            //
            // Determine the column when subtracting the B delta.
            //
            x1 = x - b;

            //
            // If this column is within the clipping region, then draw a pixel
            // at that position.
            //
            if((x1 >= context->clipRegion.xMin) &&
               (x1 <= context->clipRegion.xMax))
            {
                Graphics_drawPixel(context, x1, y1);
            }

            //
            // Determine the column when adding the B delta.
            //
            x1 = x + b;

            //
            // If this column is within the clipping region, then draw a pixel
            // at that position.
            //
            if((x1 >= context->clipRegion.xMin) &&
               (x1 <= context->clipRegion.xMax))
            {
                Graphics_drawPixel(context, x1, y1);
            }
        }

        //
        // Only draw the complementary pixels if the A and B deltas are
        // different (otherwise, they describe the same set of pixels).
        //
        if(a != b)
        {
            //
            // Determine the row when subtracting the B delta.
            //
            y1 = y - b;

            //
            // See if this row is within the clipping region.
            //
            if((y1 >= context->clipRegion.yMin) &&
               (y1 <= context->clipRegion.yMax))
            {
                //
                // Determine the column when subtracting the a delta.
                //
                x1 = x - a;

                //
                // If this column is within the clipping region, then draw a
                // pixel at that position.
                //
                if((x1 >= context->clipRegion.xMin) &&
                   (x1 <= context->clipRegion.xMax))
                {
                    Graphics_drawPixel(context, x1, y1);
                }

                //
                // Only draw the mirrored pixel if the A delta is non-zero
                // (otherwise, it will be the same pixel).
                //
                if(a != 0)
                {
                    //
                    // Determine the column when adding the A delta.
                    //
                    x1 = x + a;

                    //
                    // If this column is within the clipping region, then draw
                    // a pixel at that position.
                    //
                    if((x1 >= context->clipRegion.xMin) &&
                       (x1 <= context->clipRegion.xMax))
                    {
                        Graphics_drawPixel(context, x1, y1);
                    }
                }
            }

            //
            // Determine the row when adding the B delta.
            //
            y1 = y + b;

            //
            // See if this row is within the clipping region.
            //
            if((y1 >= context->clipRegion.yMin) &&
               (y1 <= context->clipRegion.yMax))
            {
                //
                // Determine the column when subtracting the A delta.
                //
                x1 = x - a;

                //
                // If this column is within the clipping region, then draw a
                // pixel at that position.
                //
                if((x1 >= context->clipRegion.xMin) &&
                   (x1 <= context->clipRegion.xMax))
                {
                    Graphics_drawPixel(context, x1, y1);
                }

                //
                // Only draw the mirrored pixel if the A delta is non-zero
                // (otherwise, it will be the same pixel).
                //
                if(a != 0)
                {
                    //
                    // Determine the column when adding the A delta.
                    //
                    x1 = x + a;

                    //
                    // If this column is within the clipping region, then draw
                    // a pixel at that position.
                    //
                    if((x1 >= context->clipRegion.xMin) &&
                       (x1 <= context->clipRegion.xMax))
                    {
                        Graphics_drawPixel(context, x1, y1);
                    }
                }
            }
        }

        //
        // See if the error term is negative.
        //
        if(d < 0)
        {
            //
            // Since the error term is negative, adjust it based on a move in
            // only the A delta.
            //
            d += (4 * a) + 6;
        }
        else
        {
            //
            // Since the error term is non-negative, adjust it based on a move
            // in both the A and B deltas.
            //
            d += (4 * (a - b)) + 10;

            //
            // Decrement the B delta.
            //
            b -= 1;
        }

        //
        // Increment the A delta.
        //
        a++;
    }
}

//*****************************************************************************
//
//! Draws a filled circle.
//!
//! \param context is a pointer to the drawing context to use.
//! \param x is the X coordinate of the center of the circle.
//! \param y is the Y coordinate of the center of the circle.
//! \param radius is the radius of the circle.
//!
//! This function draws a filled circle, utilizing the Bresenham circle drawing
//! algorithm.  The extent of the circle is from \e x - \e radius to \e x +
//! \e radius and \e y - \e radius to \e y + \e radius, inclusive.
//!
//! \return None.
//
//*****************************************************************************
void Graphics_fillCircle(const Graphics_Context *context, int32_t  x, int32_t  y,
		int32_t  radius)
{
    int32_t  a, b, d, x1, x2, y1;

    //
    // Check the arguments.
    //
    assert(context);

    //
    // Initialize the variables that control the Bresenham circle drawing
    // algorithm.
    //
    a = 0;
    b = radius;
    d = 3 - (2 * radius);

    //
    // Loop until the A delta is greater than the B delta, meaning that the
    // entire circle has been filled.
    //
    while(a <= b)
    {
        //
        // Determine the row when subtracting the A delta.
        //
        y1 = y - a;

        //
        // See if this row is within the clipping region.
        //
        if((y1 >= context->clipRegion.yMin) &&
           (y1 <= context->clipRegion.yMax))
        {
            //
            // Determine the column when subtracting the B delta, and move it
            // to the left edge of the clipping region if it is to the left of
            // the clipping region.
            //
            x1 = x - b;
            if(x1 < context->clipRegion.xMin)
            {
                x1 = context->clipRegion.xMin;
            }

            //
            // Determine the column when adding the B delta, and move it to the
            // right edge of the clipping region if it is to the right of the
            // clipping region.
            //
            x2 = x + b;
            if(x2 > context->clipRegion.xMax)
            {
                x2 = context->clipRegion.xMax;
            }

            //
            // Draw a horizontal line if this portion of the circle is within
            // the clipping region.
            //
            if(x1 <= x2)
            {
                Graphics_drawLineH(context, x1, x2, y1);
            }
        }

        //
        // Determine the row when adding the A delta.
        //
        y1 = y + a;

        //
        // See if this row is within the clipping region, and the A delta is
        // not zero (otherwise, this describes the same row of the circle).
        //
        if((y1 >= context->clipRegion.yMin) &&
           (y1 <= context->clipRegion.yMax) &&
           (a != 0))
        {
            //
            // Determine the column when subtracting the B delta, and move it
            // to the left edge of the clipping region if it is to the left of
            // the clipping region.
            //
            x1 = x - b;
            if(x1 < context->clipRegion.xMin)
            {
                x1 = context->clipRegion.xMin;
            }

            //
            // Determine the column when adding the B delta, and move it to the
            // right edge of the clipping region if it is to the right of the
            // clipping region.
            //
            x2 = x + b;
            if(x2 > context->clipRegion.xMax)
            {
                x2 = context->clipRegion.xMax;
            }

            //
            // Draw a horizontal line if this portion of the circle is within
            // the clipping region.
            //
            if(x1 <= x2)
            {
                Graphics_drawLineH(context, x1, x2, y1);
            }
        }

        //
        // Only draw the complementary lines if the B delta is about to change
        // and the A and B delta are different (otherwise, they describe the
        // same set of pixels).
        //
        if((d >= 0) && (a != b))
        {
            //
            // Determine the row when subtracting the B delta.
            //
            y1 = y - b;

            //
            // See if this row is within the clipping region.
            //
            if((y1 >= context->clipRegion.yMin) &&
               (y1 <= context->clipRegion.yMax))
            {
                //
                // Determine the column when subtracting the A delta, and move
                // it to the left edge of the clipping regino if it is to the
                // left of the clipping region.
                //
                x1 = x - a;
                if(x1 < context->clipRegion.xMin)
                {
                    x1 = context->clipRegion.xMin;
                }

                //
                // Determine the column when adding the A delta, and move it to
                // the right edge of the clipping region if it is to the right
                // of the clipping region.
                //
                x2 = x + a;
                if(x2 > context->clipRegion.xMax)
                {
                    x2 = context->clipRegion.xMax;
                }

                //
                // Draw a horizontal line if this portion of the circle is
                // within the clipping region.
                //
                if(x1 <= x2)
                {
                    Graphics_drawLineH(context, x1, x2, y1);
                }
            }

            //
            // Determine the row when adding the B delta.
            //
            y1 = y + b;

            //
            // See if this row is within the clipping region.
            //
            if((y1 >= context->clipRegion.yMin) &&
               (y1 <= context->clipRegion.yMax))
            {
                //
                // Determine the column when subtracting the A delta, and move
                // it to the left edge of the clipping region if it is to the
                // left of the clipping region.
                //
                x1 = x - a;
                if(x1 < context->clipRegion.xMin)
                {
                    x1 = context->clipRegion.xMin;
                }

                //
                // Determine the column when adding the A delta, and move it to
                // the right edge of the clipping region if it is to the right
                // of the clipping region.
                //
                x2 = x + a;
                if(x2 > context->clipRegion.xMax)
                {
                    x2 = context->clipRegion.xMax;
                }

                //
                // Draw a horizontal line if this portion of the circle is
                // within the clipping region.
                //
                if(x1 <= x2)
                {
                    Graphics_drawLineH(context, x1, x2, y1);
                }
            }
        }

        //
        // See if the error term is negative.
        //
        if(d < 0)
        {
            //
            // Since the error term is negative, adjust it based on a move in
            // only the A delta.
            //
            d += (4 * a) + 6;
        }
        else
        {
            //
            // Since the error term is non-negative, adjust it based on a move
            // in both the A and B deltas.
            //
            d += (4 * (a - b)) + 10;

            //
            // Decrement the B delta.
            //
            b -= 1;
        }

        //
        // Increment the A delta.
        //
        a++;
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
//! @}
//
//*****************************************************************************
