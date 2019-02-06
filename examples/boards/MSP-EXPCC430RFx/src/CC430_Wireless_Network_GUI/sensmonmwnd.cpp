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
/*
 *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

#include "sensmonmwnd.h"
#include "qmdiarea.h"
#include "iohandler.h"
#include "graphwindow.h"
#include <Dbt.h>
#include <QT>
#include <QtGui>
#include <QStyle>
#include <QMotifStyle>
#include <QPlastiqueStyle>
#include <QCDEStyle>
#include <QCleanlooksStyle>
#include <QMdiArea>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <QApplication>


//static pointer to main window. Needed in order to post event
// to main window from static event filter function
static QObject *pMainWindow;
///////////////////////////// Defines / Locals ////////////////////////////////

#define APP_VERSION "1.0"

SensMonMWnd::SensMonMWnd()
{
    // Information used to store data in windows registery.
    QCoreApplication::setOrganizationName("Texas Instruments");
    QCoreApplication::setOrganizationDomain("ti.com");
    QCoreApplication::setApplicationName("CC430 Wireless Network Monitor");
// mdi code
	//setCentralWidget(mdiArea);
	//connect(mdiArea, SIGNAL(subWindowActivated(QMdiWindow*)),
	//	this,SLOT(consoleAction()));
// mdi end

	setFixedSize(824,695);
	setWindowFlags(windowFlags()^Qt::WindowMaximizeButtonHint);

	setFont(QFont("Arial", 10, QFont::Normal));

	QApplication::setStyle(new QPlastiqueStyle);

    networkView = new NetworkView(this);
    setCentralWidget(networkView);

	helpShortcut = new QShortcut(QKeySequence(tr("F1")), this);
	connect(helpShortcut, SIGNAL(activated()), this, SLOT(showHelp()));

	esc = new QShortcut(QKeySequence(tr("ESC")), this);
	connect(esc, SIGNAL(activated()), this, SLOT(escapeToggle()));

	altEnter = new QShortcut(QKeySequence(tr("ALT+RETURN")),this);
	connect(altEnter, SIGNAL(activated()), this, SLOT(toggleFullscreen()));
	setWindowIcon(QIcon("TI_Bug_Icon_Red.ico"));		

    QString title;
	{
		title = "CC430 Wireless Network Monitor v";
	}

    title += APP_VERSION;
    setWindowTitle(title);

    //Create IO Handler 
    ioHandler = new IoHandler(this);

	//Create Graph Window and connect events
    graph = new GraphWindow(this);
	connect(networkView, SIGNAL(nodeAdded(int)), graph, SLOT(addNode(int)));
	connect(networkView, SIGNAL(nodeRemoved(int)), graph, SLOT(removeNode(int)));
	connect(ioHandler, SIGNAL(storeData(const QString &)), graph, SLOT(graphData(const QString &)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    /* Get the Dispatcher instace */
    m_EventDispatcher = QAbstractEventDispatcher::instance(0);
    m_EventDispatcher->setEventFilter((QAbstractEventDispatcher::EventFilter)SensMonMWnd::wmEventFilter);

    pMainWindow = this;

    installEventFilter(this);

    bDevChanged = false;
    bComPortOpen = false;

    // Populate Combobox for COM port.
    if (comPortComboBox.SetComPortList(ioHandler->getComPortList()))
    {
        applDeviceConnected = true;
        // If applicable device found, we can start reading from the port.
        startCapture();

    }
    else
    {
        applDeviceConnected = false;
    }

    comPortComboBox.setToolTip("Select applicable COM port.");

    // Create instance to Node Database.
    pNodeDb = new NodeDb;

    bDeleteNodes = false;

}

void SensMonMWnd::escapeToggle()
{
	if(isFullScreen())
		toggleFullscreen();
}

void SensMonMWnd::toggleFullscreen()
{
	if(!isFullScreen())
	{
		setWindowState(Qt::WindowFullScreen);
	}
	else
	{
		setWindowState(Qt::WindowNoState);
	}
}


/** \brief Show help file
 *
 */
void SensMonMWnd::showHelp()
{
	//ShellExecute(0, TEXT("open"),TEXT("SensorMonitorHelp.CHM"), NULL, NULL, SW_SHOWNORMAL);
}

/** \brief Show settings dialog
 *
 */
void SensMonMWnd::showSettings()
{
    SettingsWindow settingsWindow;
    QSettings settings;

    bool celsius = settings.value("TempInCelsius", true).toBool();
    if(settingsWindow.exec() == SettingsWindow::BTN_OK)
    {
        networkView->startCheckAliveTimer();
        if (celsius != settings.value("TempInCelsius", true).toBool())
        {
            networkView->changeTempUnit();
        }
    }
}


void SensMonMWnd::showConsole()
{
    //ConWindow *ConWin=new ConWindow;
    //ConWin->show();
	ioHandler->start_con();
}
void SensMonMWnd::closeConsole()
{
	//console->close();
	//thread.setConsoleClose();
}
void SensMonMWnd::updateConsole(const QString &data)
{

	//consoledialog.txtConsole->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	//textEdit->insertPlainText(data);
	QString tcon;
	tcon=data;
	
}

/** \brief Create Actions
 *
 */
void SensMonMWnd::createActions()
{
    captureAction = new QAction(tr("&Capture"), this);
    captureAction->setIcon(QIcon(appPath+"images\\Play.png"));
    captureAction->setShortcut(tr("F2"));
    captureAction->setToolTip(tr("Start capturing data from COM port. (F2)"));
    captureAction->setDisabled(FALSE);
    connect(captureAction, SIGNAL(triggered()), this, SLOT(onStartCapture()));

    pauseAction  = new QAction(tr("&Pause"), this);
    pauseAction->setIcon(QIcon(appPath+"images\\Pause.png"));
    pauseAction->setShortcut(tr("F3"));
    pauseAction->setToolTip(tr("Pause capturing data from the COM port. (F3)"));
    pauseAction->setDisabled(TRUE);
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(onPauseCapture()));

    stopAction = new QAction(tr("&Stop"), this);
    stopAction->setIcon(QIcon(appPath+"images\\stop.png"));
    stopAction->setShortcut(tr("F4"));
    stopAction->setToolTip(tr("Stop capturing data from COM port. (F4)"));
    stopAction->setDisabled(TRUE);
    connect(stopAction, SIGNAL(triggered()), this, SLOT(onStopCapture()));

/*  refreshAction = new QAction(tr("&Refresh"), this);
    refreshAction->setIcon(QIcon("images\\ButtonRefresh.png"));
    refreshAction->setShortcut(tr("F5"));
    refreshAction->setToolTip(tr("Refresh list of serial ports (F5)"));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshComPort()));
*/
	consoleAction = new QAction(tr("&Console"), this);
    consoleAction->setIcon(QIcon(appPath+"images\\Console.png"));
    consoleAction->setShortcut(tr("F8"));
    consoleAction->setToolTip(tr("Show Console (F8)"));
    connect(consoleAction, SIGNAL(triggered()), this, SLOT(showConsole()));

	graphAction = new QAction(tr("&Graph"), this);
	graphAction->setIcon(QIcon(appPath+"images\\graph.png"));
	graphAction->setShortcut(tr("F9"));
	graphAction->setToolTip(tr("Graph node data (F9)"));
	connect(graphAction, SIGNAL(triggered()), this, SLOT(showGraph()));

	helpAction = new QAction(tr("&Help"), this);
	helpAction->setIcon(QIcon(appPath+"images\\pdf.png"));
	helpAction->setShortcut(tr("F10"));
	helpAction->setToolTip(tr("Open User Guide (F10)"));
	connect(helpAction, SIGNAL(triggered()), this, SLOT(showHelpInfo()));	

    settingsAction = new QAction(tr("&Settings"), this);
    settingsAction->setIcon(QIcon(appPath+"images\\Config.png"));
    settingsAction->setShortcut(tr("F11"));
    settingsAction->setToolTip(tr("Configuration (F11)"));
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettings()));
	
	aboutAction = new QAction(tr("&About"), this);
	aboutAction->setIcon(QIcon(appPath+"images\\Readme.png"));
	aboutAction->setShortcut(tr("F12"));
	aboutAction->setToolTip(tr("About information (F12)"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));


}

void SensMonMWnd::createMenus()
{
    // For the moment we don't have any menu items.
}

void SensMonMWnd::showAbout()
{
	QString aboutString;
	QImage tiLogo("images\\ti_logo_red.png");
	QMessageBox mb;

	mb.setWindowTitle(tr("About CC430 Wireless Network Monitor"));

	aboutString = QMessageBox::tr("<h3>CC430 Wireless Network Monitor v" APP_VERSION "</h3>"
								  "<p>This software is developed by Texas Instruments. "
								  "<a href=\"http://www.ti.com/msp430\">www.ti.com/msp430</a>"
								  "<p>Copyright © 2011, Texas Instruments, Inc.<br>All rights reserved.</p>"
								  );

	QPixmap pm = QPixmap::fromImage(tiLogo);
	if (!pm.isNull()) mb.setIconPixmap(pm);
	mb.setText(aboutString);
	mb.addButton(QMessageBox::Ok);
	mb.exec();
}

void SensMonMWnd::showGraph()
{
	getNodeList(m_nodeList);
	//graph->setNodesList(m_nodeList);
	graph->show();
}

void SensMonMWnd::createToolBars()
{
    captureToolBar = addToolBar(tr("&Capture"));
    captureToolBar->addAction(captureAction);
    captureToolBar->addAction(pauseAction);
    captureToolBar->addAction(stopAction);

    comPortToolBar = addToolBar(tr("&ComPort"));
    comPortToolBar->addWidget(&comPortComboBox);
    //comPortToolBar->addAction(refreshAction);

    helpToolBar = addToolBar(tr("&Help"));
    helpToolBar->addAction(consoleAction);
	helpToolBar->addAction(graphAction);  // ??? this really belongs in a separate toolbar!
	helpToolBar->addAction(helpAction);
	helpToolBar->addAction(settingsAction);
	helpToolBar->addAction(aboutAction);
}

void SensMonMWnd::createStatusBar()
{
    connectStatus = new QLabel("Disconnected          ");
    connectStatus->setAlignment(Qt::AlignCenter);
    connectStatus->setMinimumSize(connectStatus->sizeHint());
    statusBar()->addWidget(connectStatus);
}

//-----------------------------------------------------------------------------
/** \brief On Start capturing packets
 *
 *  Called when user click button on toolbar.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::onStartCapture()
{
    startCapture();
}

//-----------------------------------------------------------------------------
/** \brief Start capturing packets
 *
 *  Call function in IO Handler to start reading from COM port.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::startCapture(bool devChangeEvent)
{
    QString qsComPort;
    qsComPort = comPortComboBox.currentText();
    QString verbose,console_buf;
    if (bDeleteNodes) removeAllNodes();

    int s, e;
    s = qsComPort.indexOf("COM");
    int i = s + 3;
    while (qsComPort[i++].isDigit());
    e = i - 1;

    if (ioHandler->openComPort(qsComPort.mid(s,e - s)) == IoHandler::IO_SUCCESS)
    {
        DB_WRITE("startCapture: Open COM port");
        bComPortOpen = true;
        connectStatus->setText(tr("Connected                 "));
        captureAction->setDisabled(TRUE);
        pauseAction->setDisabled(FALSE);
        stopAction->setDisabled(FALSE);
        networkView->startCheckAliveTimer();
		networkView->changeTempUnit();

        if(devChangeEvent)
        {
            bDevChanged = true;
        }
    }
    else
    {
        DB_WRITE("startCapture: Open COM port failed.");
        QString qsTemp;
        bComPortOpen = false;
        qsTemp = "Not able to open " + qsComPort.mid(s,e - s);
        QMessageBox::warning(this, tr("Open COM Port"), qsTemp, QMessageBox::AcceptRole, QMessageBox::AcceptRole);

        refreshComPort();
    }
}

//-----------------------------------------------------------------------------
/** \brief On Pause capturing 
 *
 *  Called when user click Pause button on toolbar.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::onPauseCapture()
{
    stopCapture();
    bDeleteNodes = false;
}

//-----------------------------------------------------------------------------
/** \brief On Stop capturing 
 *
 *  Called when user click button on toolbar.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::onStopCapture()
{
    stopCapture();
    bDeleteNodes = true;
}

//-----------------------------------------------------------------------------
/** \brief Stop capturing packets
 *
 *  Call function in IO handler to close COM port.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::stopCapture(bool devChangeEvent)
{
    DB_WRITE("stopCapture: devChangeEvent = " << devChangeEvent);

    networkView->stopCheckAliveTimer();

    if (bComPortOpen)
    {
        if (ioHandler->closeComPort() == IoHandler::IO_ERROR)
        {
            QMessageBox::warning(this, tr("Close COM Port"), tr("Not able to close COM port"));
        }
    }

    bComPortOpen = false;

    if (applDeviceConnected)
    {
        connectStatus->setText(tr("Connected and idle.       "));
    }
    else
    {
        connectStatus->setText(tr("Disconnected.             "));
    }

    if (devChangeEvent)
    {
        bDevChanged = true;
    }

    captureAction->setDisabled(FALSE);
    pauseAction->setDisabled(TRUE);
    stopAction->setDisabled(TRUE);
}

//-----------------------------------------------------------------------------
/** \brief Remove All Nodes
 *
 *  Both the Node Data base and the graphical view will be cleared.
 *  The Coordinator node(sink) will not be removed.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::removeAllNodes()
{
    networkView->deleteAll();
    pNodeDb->deleteAll();
}

//-----------------------------------------------------------------------------
/** \brief Refresh COM port
 *
 *  Call function in IO handler to refresh com ports.
 *  get new list of COM ports.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::refreshComPort()
{
    if (ioHandler->RefreshComPort() == IoHandler::IO_SUCCESS)
    {
        comPortComboBox.clear();
        if (comPortComboBox.SetComPortList(ioHandler->getComPortList()))
        {
            DB_WRITE("RefreshComPort: Applicable device found.");
            // Applicable COM port found. change status of connected node
            // to not responding.
            applDeviceConnected = true;
        }
        else
        {
            DB_WRITE("RefreshComPort: No Applicable device found.");
            // No applicable COM port found. Dongle not connected.
            applDeviceConnected = false;
            connectStatus->setText(tr("Disconnected.             "));
        }
    }
    else
    {
        DB_WRITE("RefreshComPort: Not able to refresh COM port.");
        applDeviceConnected = false;
        connectStatus->setText(tr("Disconnected.             "));
        QMessageBox::warning(this, tr("Refresh COM port"), tr("Not able to refresh COM Port")); 
    }

}

//-----------------------------------------------------------------------------
/** \brief Get Node List
 *
 *  Returns the address of current nodes
 *
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::getNodeList(QStringList &pNodeList)
{	

	int i = 0;
    NodeDb::DATA *pData;
    // Get the address of each node and append that to the nodelist
	QString name;
    while (pNodeDb->getData(i, &pData))
    {
		if (pData->addr == 0)
		{
			name.sprintf("%s %d", "Access Point", pData->addr);
		}
		else
		{
			name.sprintf("%s %d", "End Device", pData->addr);
		}

		if (pNodeList.contains(name) == false)
		{
			pNodeList.append(name);
		}
		i++;
    }
}

//-----------------------------------------------------------------------------
/** \brief Data Received
 *
 *  Called when packet with Node information received.
 *  Read Node data base and check for new information.
 *
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::dataReceived()
{
    int i = 0;
	//BYTE buf[256];
	//xDWORD read;
	//xQString verbose,console_buf;
    NodeDb::DATA *pData;
    // Check each node in the data base and check if updates are needed.
    while(pNodeDb->getData(i, &pData))
    {
        // Check if status changed
        DB_WRITE("dataReceived: i = " << i << " addr = " << pData->addr);

        switch (pData->status)
        {
            case NodeDb::DS_NORMAL :
                // Do nothing. Put first in the switch because this will be the most used status.            
                break;
            case NodeDb::DS_UPDATED :
            case NodeDb::DS_MOVE :
                networkView->updateNode(i);
                DB_WRITE("dataReceived: DS_UPDATED addr = " << pData->addr << "xpos" << pData->xpos); 
                pData->status = NodeDb::DS_NORMAL;
				if (pData->addr !=0) //x for test
				{
					pData->status = NodeDb::DS_NORMAL;
				}
				break;
            case NodeDb::DS_NEW :
				//emit addNode(i);
                networkView->addNode(i);
                DB_WRITE("dataReceived: DS_NEW addr = " << pData->addr << "xpos" << pData->xpos); 
                networkView->connectNodes(i, 0); // Add Access Point as parent
                pData->status = NodeDb::DS_NORMAL;
				if (pData->addr !=0) //x for test
				{
					pData->status = NodeDb::DS_NORMAL;

				}				
				break;
            default :
                ;// Do nothing
        }
        i++;
    }
}

//-----------------------------------------------------------------------------
/** \brief	Close event
 *
 *  \param[in] event
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::closeEvent(QCloseEvent *event)
{
    // Make sure that the COM port is closed before ending the application.
    if (event->type() == QEvent::Close)
    {
        ioHandler->closeComPort();
    }
}

//-----------------------------------------------------------------------------
/** \brief	Windows Event Filter
 *
 *   Catch any window message.
 *   vmEventFilter must be made known by the main application loop on beforehand.
 *   See constructor of this class.
 *   Since this is a static function we have to post an event to tell
 *   the main window about it.
 *
 *  \param[in] message
 *        Window message.
 */
//-----------------------------------------------------------------------------
bool SensMonMWnd::wmEventFilter(void *message)
{
    MSG *msg;

    msg = (MSG*)message;
    if (msg->message == WM_DEVICECHANGE && msg->wParam == DBT_DEVNODES_CHANGED) 
    {
        DeviceChangeEvent *deviceChangeEvent = new DeviceChangeEvent();
        QCoreApplication::postEvent(pMainWindow, deviceChangeEvent);
    }
    return false;
}

 //-----------------------------------------------------------------------------
/** \brief	Event Filter
 *
 *   Catch events that need special handling by the main window.
 *
 *  \param[in] obj
 *           The watched object.
 *  \param[in] ev
 *           Event.
 */
//-----------------------------------------------------------------------------
bool SensMonMWnd::eventFilter(QObject *obj, QEvent *event)
 {
    if (obj == this)
    {
        // bDevChanged will by default be false. If this event occur and either
        // COM port is opened or closed, bDevChanged will be set true to avoid
        // repeated calls to open/close COM port. Plug/unplug of USB device seems 
        // to create several VW_DEVICECHANGE events. To make sure that the bDevChanged
        // is reset to default value even if open/close should not be successful, there
        // is a timer set and on timeout the value will be set false.
        if (!bDevChanged && event->type() == DeviceChangeEvent::DeviceChanged)
        {
            QTimer::singleShot(TIMER_DEV_CHANGE, pMainWindow, SLOT(timeOutDevChange()));
            deviceChange();
            return true;
        }
    }

     // If event not already handled, we have to pass it to the parent class.
     return QMainWindow::eventFilter(obj, event);
 }
//-----------------------------------------------------------------------------
/** \brief	Device Change event
 *
 *   Called when device changes has occured. E.G. USB device plugged/removed.
 *
 *  \param[in] ev
 *           Event.
 */
//-----------------------------------------------------------------------------

void SensMonMWnd::deviceChange()
{
	refreshComPort();

    if (applDeviceConnected)
    {
        if (captureAction->isEnabled())
        {
            // COM port was not open
            DB_WRITE("Device change, appl. device: Start Capturing data");
            startCapture(true);
        }
        else
        {
            // COM port was open
            DB_WRITE("Device change, appl. device: Stop Capturing data");
        }
    }
    else
    {
        if (!captureAction->isEnabled())
        {
            // Applicable device not connected and COM port was open
            // this means that the device was removed.
            DB_WRITE("Device change, no appl. device: !captureAction->isEnabled()");
            stopCapture(true);
        }
        else
        {
            // Appl. device not connected and COM port closed.
            DB_WRITE("Device change, no appl. device: captureAction->isEnabled()");
        }
    } 
}

//-----------------------------------------------------------------------------
/** \brief	Called when timeout of Device Change event timer expired.
 *
 *  The timer and the flag is used to avoid handling of the same event
 *  more than ones. It seems that when the USB dongle is plugged/unplugged, 
 *  several WM_DEVICECHANGED messages are generated by the system.
 *
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::timeOutDevChange()
{
    DB_WRITE("Timeout dev change.");
    bDevChanged = false;
}

//-----------------------------------------------------------------------------
/** \brief	Set Application path
 *
 *  Should be called in the main procedure to set current path of the application
 *
 *  \param[in] path
 *        Complete path including the name of the exe file to start application.  
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::setAppPath(char *path)
{
    QString temp = path;
    appPath = temp.left(temp.lastIndexOf('\\'));
}

//-----------------------------------------------------------------------------
/** \brief	Show help information
 *
 *  Open pdf file with application help information.
 */
//-----------------------------------------------------------------------------
void SensMonMWnd::showHelpInfo()
{
    QString temp;
    //temp = appPath + USER_GUIDE_PDF;
	temp = "http://www.ti.com/lit/pdf/slau344";
	ShellExecute(winId(), 0, temp.utf16(), 0, 0, SW_SHOWMAXIMIZED );

}
void SensMonMWnd::setConsoleOpen()
{
	//xQMutexLocker locker(&mutex);
	//xconsoleOpen = true;
}

void SensMonMWnd::setConsoleClose()
{
	//xQMutexLocker locker(&mutex);
	//xconsoleOpen = false;
}