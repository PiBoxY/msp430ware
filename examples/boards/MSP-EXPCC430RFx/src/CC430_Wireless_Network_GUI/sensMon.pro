TEMPLATE = 	vcapp
CONFIG 	+= 	qt
CONFIG 	+= 	static release
DEFINE 	+= 	UNICODE
		
CONFIG 	+= 	assistant
HEADERS	=   comport.h \
            comportcombo.h \
            comportthread.h \
			conwindow.h \
			edge.h \
			graphwindow.h \
			iohandler.h \
            networkview.h \
            node.h \
            nodedb.h \
			sensmonmwnd.h \
			sensorplot.h \
			settingswindow.h 

SOURCES	= 	comport.cpp \
            comportcombo.cpp \
            comportthread.cpp \
			conwindow.cpp \
			edge.cpp \
			graphwindow.cpp \		
			iohandler.cpp \
			main.cpp \
            networkview.cpp \
            node.cpp \
            nodedb.cpp \
            sensmonmwnd.cpp \
			sensorplot.cpp \
			settingswindow.cpp \

FORMS	= 	settingswindow.ui \
			console.ui
  
RC_FILE	= 	sensMon.rc

RESOURCES =	sensMon.qrc

INCLUDEPATH  = C:\Qt\qwt-5.1.1\src
win32:LIBS 	+= C:\Qt\qwt-5.1.1\lib\qwt5.lib
DEFINES 	+= QWT_DLL
