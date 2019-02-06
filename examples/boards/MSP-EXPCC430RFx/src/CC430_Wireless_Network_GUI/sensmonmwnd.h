
#ifndef SENSMONMWND_H
#define SENSMONMWND_H
 /****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

/* Copyright © 2011, Texas Instruments Incorporated
** June 1, 2011 Modified by Texas Instruments …
*/

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QTextEdit>
#include <QComboBox>
#include <QAbstractEventDispatcher>

#include "iohandler.h"
#include "comportcombo.h"
#include "networkview.h"
#include "settingswindow.h"

#include "graphwindow.h"

class NetworkView;
class DeviceChange;
class IoHandler;
class GraphWindow;

QT_BEGIN_NAMESPACE
class QGroupBox;
QT_END_NAMESPACE

/** \brief The Main Window for the Sensor Monitor
 *
 *
 */
class SensMonMWnd : public QMainWindow
{
    Q_OBJECT

public:
    enum{TIMER_DEV_CHANGE = 2000};
	
    NetworkView *networkView;

    SensMonMWnd();
    static bool wmEventFilter(void *message);
    void setAppPath(char *path);
	//QString IoHandler:conTemp;
	QString conTemp;
	
public slots:
	void showHelp();
	void showConsole();
	//void updateConsole(QString data);
	void closeConsole();


protected:
    void deviceChange();

    /// Pointer to the node data base
    NodeDb *pNodeDb;
    /// The application path is stored on startup.
    QString appPath;
	
private slots:
	void toggleFullscreen();
	void escapeToggle();
	void showAbout();
	void showGraph();
    void showSettings();
	
    void removeAllNodes();
    void onStartCapture();
    void startCapture(bool devChangeEvent = false);
    void onPauseCapture();
    void onStopCapture();
    void stopCapture(bool devChangeEvent = false);
    void refreshComPort();
	void getNodeList(QStringList &pNodeList);
    void dataReceived(/*int index*/);
    void timeOutDevChange();
    void showHelpInfo();
	void updateConsole(const QString &);

private:
    void createActions();
	void createMenus();
    void createToolBars();
    void createStatusBar();

    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
	void setConsoleOpen();
	void setConsoleClose();

	 //QCheckBox *checkBox1;

    //TextWindow *aboutWindow;
	//QMenu *menu;

    // Toolbar for start and stop
    QToolBar *captureToolBar;
    // Toolbar for the combobox.
    QToolBar *comPortToolBar;
    // Toolbar for the help functions.
    QToolBar *helpToolBar;
    // Put the combobox with COM ports on the toolbar.
    ComPortCombo comPortComboBox;
    /// Flag to indicate if applicable device connected
    bool applDeviceConnected;
    /// Flag to indicate that device change event has occured.
    bool bDevChanged;
    /// Flag to indicate if COM port is open or not
    bool bComPortOpen;
    /// Flag used to control if nodes should be deleted or not after pause or stop.
    bool bDeleteNodes;
    /// Event handler
    QAbstractEventDispatcher *m_EventDispatcher;

	QShortcut *helpShortcut;
	QShortcut *esc;
	QShortcut *altEnter;

    QAction *fullscreen;
    QAction *stopAction;
    QAction *captureAction;
    QAction *pauseAction;
    QAction *refreshAction;
    QAction *settingsAction;
    QAction *helpAction;
    QAction *aboutAction;
	QAction *graphAction;
	QAction *consoleAction;

    QLabel *connectStatus;

    IoHandler *ioHandler;
	GraphWindow *graph;
	QStringList m_nodeList;

	SettingsWindow *settingsWindow;
	 

	
};

/** \brief Device Change Event Class
 *
 *  This class is needed to define a user event
 *  that will be used to fetch the WM_DEVICECHANGE message
 *  when a USB device has been connected/disconnected.
 *
 */
class DeviceChangeEvent : public QEvent
{
public :
    enum{DeviceChanged = 1050};
    DeviceChangeEvent(): QEvent((QEvent::Type)DeviceChanged) {}

};

/*
//#######################################################
class ConWindow : public QMainWindow
{
    Q_OBJECT

public:
    ConWindow();
    ConWindow(const QString &fileName);
	
	QString conTemp;
	
	int hub_toggle;
	int selectDeselectToggle;
	int node;

	char node1;
	char node2;
	char node3;
	char node4;
	char node5;
	char node6;
	char node7;
	char node8;

protected:
    void closeEvent(QCloseEvent *event);
	QMainWindow *tMyParent;
public slots:



private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
	void storeData(const QString &);
	void dontShowHub();
	void clearWindow();
	void deselectallnodes();
	void testCheck(bool check );
	//void updateConsole();
private:
    void init();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);
    ConWindow *findConWindow(const QString &fileName);
	void ConWindow::nodeActions();

	QGroupBox *createNonExclusiveGroup();
	
    QTextEdit *textEdit;
    QString curFile;
    bool isUntitled;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
	QMenu	*checkMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *closeAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *togglehubAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
	QAction *clearConsole;
	QAction	*checkNode;
	QAction	*node1Act;
	QAction	*node2Act;
	QAction	*node3Act;
	QAction	*node4Act;
	QAction	*node5Act;
	QAction	*node6Act;
	QAction	*node7Act;
	QAction	*node8Act;
	QAction *deselectAllAct;

	QFont modeFont;
};
*/
#endif