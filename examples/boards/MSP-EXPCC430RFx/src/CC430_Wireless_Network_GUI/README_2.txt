This will guide you to setup the environment to build the eZ430-RF2500 Sensor-SEH Monitor application.
The application is distributed under the conditions of the BSD license agreement. 
See the LICENSE.txt for further details. 

Prerequisites:
  -	PC with Windows XP.
  - A fast PC or a lot of time, preferably both.

Download and building of VC++ 2008 Express and Qt libraries:

1.  Download Visual C++ 2008 Express Edition (VC++) from microsofts website:
    http://www.microsoft.com/express/download/#webInstall

2.  Download Qt’s open source version 4.4.0 from Trolltech’s web site:
    ftp://ftp.trolltech.com/qt/source/qt-all-opensource-src-4.4.0.zip 
		Latest Version: http://trolltech.com/downloads/opensource/appdev/windows-cpp 

3.  Install VC++.

4.  Unzip Qt sources to "c:\Qt\4.4.0"

5.  Go to [Control Panel]->[System]->[Advanced] and click on Environment Variables
    Add: Variable name = "PATH"       and Variable value = "c:\Qt\4.4.0\bin" to User variables
    Add: Variable name = "QMAKESPEC"  and Variable value = "win32-msvc2008"  to System variables
    Add: Variable name = "QTDIR"      and Variable value = "c:\Qt\4.4.0"     to System variables

6.  Open Visual Studio 2008 Command Promt from your start menu.
    Change directory to where Qt was unzipped: "cd c:\Qt\4.4.0"

7.  Type: "configure", and select "y" when asked. This will take 30-60min.

8.  Type: "nmake". This will take 1-4 hours.
	>> nmake failed compiling xrefwrap.  
	   Fix: Remove the "#include <xmath.h>" line from the file MathExtras.h which is in c:\qt\4.4.0\src\3rdparty\webkit\JavaScriptCore\wtf\MathExtras.h

9.  Open VC++ 2008 IDE, select [Tools]->[Option]->[Projects and Soulutions]->[VC++ Directories]
    and add following settings:
    --> [Executable files]: add "c:\Qt\4.4.0" and "c:\Qt\4.4.0\Bin"
    --> [Include files]:    add "c:\Qt\4.4.0" and "c:\Qt\4.4.0\Include"
    --> [Library files]:    add "c:\Qt\4.4.0" and "c:\Qt\4.4.0\Lib"

>>> The following steps enable QWT, the graphics extension for the plotting widgets:
10. Download QWT v5.1.1: http://sourceforge.net/projects/qwt 

11. Extract QWT to C:\Qt\qwt-5.1.1

12. Open qwtconfig.pri (found in C:\Qt\qwt-5.1.1)
	a) Update the INSTALLBASE path.  					(INSTALLBASE = C:/Qt/Qwt-5.1.1)
	b) Comment out QwtDesigner at the bottom of file 	(#CONFIG     += QwtDesigner)
	c) Uncomment QwtExamples at the bottom of the file 	(CONFIG     += QwtExamples)

13. Open examples.pri (found in C:\Qt\qwt-5.1.1\examples)
	a) Change msvc2005 to msvc2008 in the line "win32-msvc2005:LIBS += $${QWT_ROOT}/lib/$${QWTLIB}.lib" 

14. Open Visual Studio 2008 Command Prompt to build Qwt.  Examples will be output to the examples\bin directory.
 Type:
	> cd c:\qt\qwt-5.1.1
    > qmake qwt.pro
	> nmake	

Building application:

1. Go to the folder where the application has been unzipped.
2. Copy \images directory to \release\images
3. Find and place the following files in the /release directory (all files are included in the binary release of this program and placed in c:\Program Files\Texas Instruments\eZ430-RF2500-SEH Sensor Monitor\):
	1) Microsoft.VC90.CRT.manifest
	2) msvcp90.dll
	3) msvcr90.dll
	4) QtCore4.dll
	5) QtGui4.dll
	6) qwt5.dll
	
4. Run "qmake sensMon.pro" from the command promt. This will create the required project file for Visual Studio.
5. Open sensMon.vcproj with Visual Studio.
6. If necessary, change configuration from Debug to Release (in top tool bar).
7. Open project settings and change character settings to use unicode characters.
   Project -> sensMon properties -> Configuration Properties -> General -> Character Set = Use Unicode Char. set.    
8. Build -> Build Project [F7]	 

The information in this file is based on the description given on the following web site:
	http://www.qtcentre.org/forum/f-installation-and-deployment-5/t-qt-44-setup-help-with-vc-2008-express-14382.html
    
Additional information can also be found in the INSTALL file that follow the Qt source packet.    
    

