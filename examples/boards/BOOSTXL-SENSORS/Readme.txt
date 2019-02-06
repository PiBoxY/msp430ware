Readme file for BOOSTXL-Sensors_GUI+ET: ( BOOSTXL-Sensors+ET)
Copyright 2015. Texas Instruments Incorporated. All rights reserved.

This application has been configured to work with the MSP432P401R device

BOOSTXL-SENSORS User's Guide: http://www.ti.com/lit/pdf/slau666
For help, please see http://processors.wiki.ti.com/index.php/Main_Page


Version 0.2 Changes: Updated labels. Changed graph ranges

Versions 0.3-0.5 changes: Implemented EnergyTrace, Gesture Recognition, Light Recognition. Improved Look and feel, improved FW order of operations, removed unnecessary buttons and objects. Added gif hints and extpected current consumption hint. 

Version 1.0 Changes: Changed average current labels, added ms label to Duration label. Added Help menu. Corrected Splash screen. Improved BME280 refresh rate for lower power consumption.

Version 1.1: Fixed Gestures not toggling in firmware.

Version 1.2: Changed look and feel to comply with IMS Styling.

Version 1.3: Fixed Tri_Axis_Graph data phasing, fixed Tri_Axis_Graph value labels not populating, changed  default refresh rates back to correct value. Made accelerometer Magnitude value visable again. Added flag to prevent multiple EnergyTrace requests during calculation. Removed backup files from installer. Removed invalid databinding. Changed reporting method to only send updated sensor data. Improved sensor accuracy and offset values in firmware. 

Version 1.3.1: Fixed non functioning dropdown sample rate boxes. Fixed JSON collisions, though need to investigate if this has an impact on Gesture Recognition.

Version 1.3.2 Fixed bad JSON parses, removed (commented) logging. Removed (commented) EnergyTrace error logging generated while the CSV is being built. 
Known Issues: Very occaisionally, the Magnetometer will initalize in standby, if this happens, toggle the IMU off and on. 
If the MCU is not reset before GUI start up, the last GUI sessions settings will be maintained. Simply unplug or reset the LaunchPad, or change the sample rates and ON/OFF toggles to the desired settings. 

