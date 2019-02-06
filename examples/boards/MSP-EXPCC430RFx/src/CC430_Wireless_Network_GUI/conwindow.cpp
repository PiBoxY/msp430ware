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

 
#include <QApplication>
#include <QtGui>
#include <QTextEdit>
#include "qmdiarea.h"

#include <QT>

#include <QStyle>
#include <QMotifStyle>
#include <QPlastiqueStyle>
#include <QCDEStyle>
#include <QCleanlooksStyle>
#include <QMdiArea>
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include <qfont.h> 

#include "sensmonmwnd.h"
#include "iohandler.h"
#include "conwindow.h"

int window_open;

ConWindow::ConWindow()
{
	init();
    setCurrentFile("");
}

ConWindow::ConWindow(const QString &fileName)
{
    init();
    loadFile(fileName);
}

void ConWindow::closeEvent(QCloseEvent *)
{
	/* Prompt for Save when closing
	if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }*/
	
	window_open=0;
}

void ConWindow::newFile()
{
    ConWindow *other = new ConWindow;
    other->move(x() + 40, y() + 40);
    other->show();
}

void ConWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        ConWindow *existing = findConWindow(fileName);
        if (existing) {
            existing->show();
            existing->raise();
            existing->activateWindow();
            return;
        }

        if (isUntitled && textEdit->document()->isEmpty()
                && !isWindowModified()) {
            loadFile(fileName);
        } else {
            ConWindow *other = new ConWindow(fileName);
            if (other->isUntitled) {
                delete other;
                return;
            }
            other->move(x() + 40, y() + 40);
            other->show();
        }
    }
}

bool ConWindow::save()
{
    if (isUntitled) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool ConWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
                                                    curFile);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void ConWindow::about()
{
   QMessageBox::about(this, tr("Console"),
            tr("The Console program shows the data stream  "
               "from the Access Point and the End Devices and allows you to save it to file."));
}

void ConWindow::dontShowHub()
{
	if (hub_toggle==1)
	{
		hub_toggle=0;
	}
	else
	{
		hub_toggle=1;
	}
}
void ConWindow::clearWindow()
{
	textEdit->clear();
}

void ConWindow::deselectallnodes()
{
	if (selectDeselectToggle==1)
	{
		selectDeselectToggle=0;
		deselectAllAct->setText("Select All");
	}
	else
	{
		selectDeselectToggle=1;
		deselectAllAct->setText("Deselect All");
	}

	if (selectDeselectToggle==1)
	{
		node1Act->setChecked(true);
		node1=1;
	}
	else
	{
		node1Act->setChecked(false);

		node1=0;
	}

	if (selectDeselectToggle==1)
	{
		node2=1;
		node2Act->setChecked(true);
	}
	else
	{
		node2=0;
		node2Act->setChecked(false);
	}
	
	if (selectDeselectToggle==1)
	{
		node3=1;
		node3Act->setChecked(true);
	}
	else
	{
		node3=0;
		node3Act->setChecked(false);

	}

	if (selectDeselectToggle==1)
	{
		node4=1;
		node4Act->setChecked(true);
	}
	else
	{
		node4=0;
		node4Act->setChecked(false);

	}
	if (selectDeselectToggle==1)
	{
		node5=1;
		node5Act->setChecked(true);
	}
	else
	{
		node5=0;
		node5Act->setChecked(false);

	}
	if (selectDeselectToggle==1)
	{
		node6=1;
		node6Act->setChecked(true);
	}
	else
	{
		node6=0;
		node6Act->setChecked(false);

	}
	if (selectDeselectToggle==1)
	{
		node7=1;
		node7Act->setChecked(true);
	}
	else
	{
		node7=0;
		node7Act->setChecked(false);

	}
	if (selectDeselectToggle==1)
	{
		node8=1;
		node8Act->setChecked(true);
	}
	else
	{
		node8=0;
		node8Act->setChecked(false);
	}
}


void ConWindow::testCheck(bool check)
{
	int i ;

	i=check;
	if (node1Act->isChecked())
	{
		node1=1;
	}
	else
	{
		node1=0;
	}

	if (node2Act->isChecked())
	{
		node2=1;
	}
	else
	{
		node2=0;
	}
	
	if (node3Act->isChecked())
	{
		node3=1;
	}
	else
	{
		node3=0;
	}

	if (node4Act->isChecked())
	{
		node4=1;
	}
	else
	{
		node4=0;
	}
	if (node5Act->isChecked())
	{
		node5=1;
	}
	else
	{
		node5=0;
	}
	if (node6Act->isChecked())
	{
		node6=1;
	}
	else
	{
		node6=0;
	}
	if (node7Act->isChecked())
	{
		node7=1;
	}
	else
	{
		node7=0;
	}
	if (node8Act->isChecked())
	{
		node8=1;
	}
	else
	{
		node8=0;
	}
}

void ConWindow::documentWasModified()
{
    setWindowModified(true);
}

//********************
void ConWindow::init()
{
    setAttribute(Qt::WA_DeleteOnClose);

    isUntitled = true;

    textEdit = new QTextEdit;
    setCentralWidget(textEdit);

    createActions();
	nodeActions();
    createMenus();
	
    createToolBars();
    createStatusBar();
	testCheck(true);
	
    readSettings();
	window_open=1;
	hub_toggle=0;
	selectDeselectToggle=1;
	//node1Act->setChecked(true);
	QFont modeFont("Courier", 9, QFont::Normal);
	textEdit->setFont(modeFont);

    connect(textEdit->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));

	//connect(, SIGNAL(updateConsole(const QString &)),
     //       this, SLOT(updateConsole(const QString &)));
}

void ConWindow::createActions()
{
    //newAct = new QAction(QIcon("new.png"), tr("&New"), this);
    //newAct->setShortcut(tr("Ctrl+N"));
    //newAct->setStatusTip(tr("Create a new file"));
    //connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon("images\\open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon("images\\save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    closeAct = new QAction(tr("&Close"), this);
    closeAct->setShortcut(tr("Ctrl+W"));
    closeAct->setStatusTip(tr("Close this window"));
    connect(closeAct, SIGNAL(triggered()), this, SLOT(close()));

    //exitAct = new QAction(tr("E&xit"), this);
    //exitAct->setShortcut(tr("Ctrl+Q"));
    //exitAct->setStatusTip(tr("Exit the application"));
    //connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //cutAct = new QAction(QIcon("images\\cut.png"), tr("Clear console window"), this);
    //cutAct->setShortcut(tr("Ctrl+X"));
    //cutAct->setStatusTip(tr("Clear the  "
    //                       "console winddow"));
    //connect(cutAct, SIGNAL(triggered()), this, SLOT(clearWindow()));

    copyAct = new QAction(QIcon("images\\copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

	togglehubAct = new QAction(QIcon("images\\accept.png"), tr("&Toggle Access Point data"), this);
	togglehubAct->setShortcut(tr("Ctrl+T"));
	togglehubAct->setStatusTip(tr("Do not show Access Point data on console"));
    connect(togglehubAct, SIGNAL(triggered()), this, SLOT(dontShowHub()));

    //aboutAct = new QAction(tr("&About"), this);
    //aboutAct->setStatusTip(tr("Show the application's About box"));
    //connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    //aboutQtAct = new QAction(tr("About &Qt"), this);
    //aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    //connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(clearWindow()));
	//connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	//clearConsole
	clearConsole = new QAction(QIcon("images\\new.png"), tr("Clear console window"), this);
    clearConsole->setStatusTip(tr("Clear the console window"));
    connect(clearConsole, SIGNAL(triggered()), this, SLOT(clearWindow()));

    //cutAct->setEnabled(true);
    //copyAct->setEnabled(false);
    //connect(textEdit, SIGNAL(copyAvailable(bool)),
    ///        cutAct, SLOT(setEnabled(bool)));
}

void ConWindow::nodeActions()
{
	//check boxes
    node1Act = new QAction(tr("Node 1"), this);
	node1Act->setCheckable(true);
	node1Act->setStatusTip(tr("Check to display"));
	node1Act->setChecked(true);
    connect(node1Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node2Act = new QAction(tr("Node 2"), this);
	node2Act->setCheckable(true);
	node2Act->setStatusTip(tr("Check to display"));
	node2Act->setChecked(true);
    connect(node2Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node3Act = new QAction(tr("Node 3"), this);
	node3Act->setCheckable(true);
	node3Act->setStatusTip(tr("Check to display"));
	node3Act->setChecked(true);
    connect(node3Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node4Act = new QAction(tr("Node 4"), this);
	node4Act->setCheckable(true);
	node4Act->setStatusTip(tr("Check to display"));
	node4Act->setChecked(true);
    connect(node4Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node5Act = new QAction(tr("Node 5"), this);
	node5Act->setCheckable(true);
	node5Act->setStatusTip(tr("Check to display"));
	node5Act->setChecked(true);
    connect(node5Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node6Act = new QAction(tr("Node 6"), this);
	node6Act->setCheckable(true);
	node6Act->setStatusTip(tr("Check to display"));
	node6Act->setChecked(true);
    connect(node6Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node7Act = new QAction(tr("Node 7"), this);
	node7Act->setCheckable(true);
	node7Act->setStatusTip(tr("Check to display"));
	node7Act->setChecked(true);
    connect(node7Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	node8Act = new QAction(tr("Node 8"), this);
	node8Act->setCheckable(true);
	node8Act->setStatusTip(tr("Check to display"));
	node8Act->setChecked(true);
    connect(node8Act, SIGNAL(toggled(bool)), this, SLOT(testCheck(bool)));

	deselectAllAct = new QAction(tr("Deselect All"), this);
	deselectAllAct->setStatusTip(tr("Deselect All Nodes"));
    connect(deselectAllAct, SIGNAL(triggered()), this, SLOT(deselectallnodes()));
}

void ConWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
	//fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAct);
    //fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(togglehubAct);
	editMenu->addAction(clearConsole);

    menuBar()->addSeparator();

    /*
	helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
	menuBar()->addSeparator();
	*/

	 checkMenu = menuBar()->addMenu(tr("&Select Nodes"));
}

void ConWindow::createToolBars()
{
//! [0]
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(openAct);
//! [0]
    fileToolBar->addAction(saveAct);

    editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(clearConsole);
	editToolBar->addAction(copyAct);
	
	checkMenu->addAction(node1Act);
	checkMenu->addAction(node2Act);
	checkMenu->addAction(node3Act);
	checkMenu->addAction(node4Act);
	checkMenu->addAction(node5Act);
	checkMenu->addAction(node6Act);
	checkMenu->addAction(node7Act);
	checkMenu->addAction(node8Act);
	checkMenu->addAction(deselectAllAct);
}

void ConWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void ConWindow::readSettings()
{
    QSettings settings("", "Console");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(700, 500)).toSize();
    move(pos);
    resize(size);
}

void ConWindow::writeSettings()
{
    QSettings settings("", "Console");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool ConWindow::maybeSave()
{
    if (textEdit->document()->isModified()) {
	QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Console Session"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard
		     | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void ConWindow::loadFile(const QString &fileName)
{

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Console Session"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool ConWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Console Session"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

void ConWindow::setCurrentFile(const QString &fileName)
{
    static int sequenceNumber = 1;

    isUntitled = fileName.isEmpty();
    if (isUntitled) {
        curFile = tr("Log.txt").arg(sequenceNumber++);
    } else {
        curFile = QFileInfo(fileName).canonicalFilePath();
    }

    textEdit->document()->setModified(false);
    setWindowModified(false);

    setWindowTitle(tr("%1[*] - %2").arg(strippedName(curFile))
                                       .arg(tr("Console Session")));
}

QString ConWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

ConWindow *ConWindow::findConWindow(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QWidget *widget, qApp->topLevelWidgets()) {
        ConWindow *ConWin = qobject_cast<ConWindow *>(widget);
        if (ConWin && ConWin->curFile == canonicalFilePath)
            return ConWin;
    }
    return 0;
}
/*void ConWindow::updateConsole()
{
	QString tscon; 
	//tscon=data;
	//tscon.append(data.constData);
	//storeData(tscon);
	//textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
	
	//textEdit->insertPlainText(data);
	//textEdit->document()->clear();
}*/
void ConWindow::storeData(const QString &data)
{
	QString tscon; 
	tscon=data.mid(7,4);
	
	if (hub_toggle==0 && (tscon=data.mid(7,4))=="HUB0")
	{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
	}
	else
	{
		//tscon=data.mid(7,4);
		if(node1&&(tscon=data.mid(7,4))=="0001")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node2&&(tscon=data.mid(7,4))=="0002")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node3&&(tscon=data.mid(7,4))=="0003")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node4&&(tscon=data.mid(7,4))=="0004")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node5&&(tscon=data.mid(7,4))=="0005")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node6&&(tscon=data.mid(7,4))=="0006")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node7&&(tscon=data.mid(7,4))=="0007")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
		
		if(node8&&(tscon=data.mid(7,4))=="0008")
		{
			textEdit->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor);
			textEdit->insertPlainText(data);
		}
	}
}
