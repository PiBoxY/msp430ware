ImageHit v1.00.01

A command line image converter to create C data structures that can be
efficiently processed by HD66753-based LCDs

Program by Andreas Dannenberg
(C)2011 Texas Instruments, Inc.

********************************************************************************
Revision History
********************************************************************************
Ver 1.00.01 - May 26th, 2011
- Updated project settings to only require JREv5 to fix compatibility issue
  with the current CCSv4 JRE which is being used. No code changes.
    
Ver 1.00.00 - April 7th, 2011
- Initial Production Release

--------------------------------------------------------------------------------

Folder Contents:

ImageHit---+---.settings/               Eclipse settings folder
           +---.classpath               Eclipse settings file
           +---.project                 Eclipse project file
           +---ImageHit.bat             Convenience script to use the tool
           +---ImageHit.class           Compiled Java code (the executable)
           +---ImageHit.java            Project source code
           +---readme.txt               This file
           
--------------------------------------------------------------------------------

All you really need is the ImageHit.bat file and the ImageHit.class file. The
BAT file is provided as a convenience method to start the converter tool. Note
that the resulting output image will be 2-bit grayscale with a maximum
resolution of 138x110 pixels. Larger source images can be used however they
will get cropped accordingly.

Usage: ImageHit [-v] file
  file   Input image file in either JPEG, PNG, BMP, or GIF format.
  -v     Cause ImageHit to be verbose, showing various image stats
         and rendering the actual monochrome image to the console.

By default the output will go to the console window. If you want to create a
file you need to re-direct the output using the standard redirection operator.
For example, let’s say you have an image file called myImage.bmp, and you want
to create a C file named output.c containing a C-structure with your image
data, your command line call would look like this:

> imagehit myImage.bmp > output.c

You can also "render" how ImageHit interprets your file directly to the console
window by using the "verbose" option to double-check if the result is what you
like. Some ASCII-art magic is done to represent the 4 different levels of gray.

> imagehit –v myImage.bmp
