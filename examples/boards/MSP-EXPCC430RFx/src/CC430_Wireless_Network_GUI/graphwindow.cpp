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
#include "graphwindow.h"


GraphWindow::GraphWindow(QObject *pParent)
{
	pMyParent = (QMainWindow *)pParent;
	init();
}


//********************
void GraphWindow::init()
{
	setWindowTitle("Real-Time Node Data");

	// create node combobox
	nodes = new QComboBox(&vBox);
	nodes->setInsertPolicy(QComboBox::InsertAlphabetically);
	// add the access point, as it's by default in the networkview constructor
	addNode(0); 

	// create graph
	plot = new SensorPlot(&vBox);
	//plot->setTitle("Node Data");
    plot->setMargin(5);

	connect(nodes, SIGNAL(activated(int)), this, SLOT(setCurrentNode(int)));

	layout = new QVBoxLayout(&vBox);
	layout->addWidget(nodes);
	layout->addWidget(plot);
	setCentralWidget(&vBox);
	vBox.setMinimumSize(400,400);
}

void GraphWindow::setNodesList(const QStringList &pList)
{
	nodes->clear();
	nodes->addItems(pList);	
}

void GraphWindow::getNodeName(int addr, QString &name)
{
	if (addr == 0)
		name.sprintf("%s", "Access Point");
	else
		name.sprintf("%s %d", "End Device", addr);
}

void GraphWindow::setCurrentNode(int index)
{
	QString title = "Real-Time Node Data: "; //debug	
	QString name = nodes->itemText(index);
	title = title.append(name);
	setWindowTitle(title);

	int addr = name.right(1).toInt();
	plot->updateCurrentNode(addr);
}

void GraphWindow::addNode(int addr)
{
	QString name;
	getNodeName(addr, name);

	if (nodes->findText(name) == -1)
	{
		for (int i = 0; i<nodes->count(); i++)
		{
			if (nodes->itemText(i) >= name)
			{
				nodes->insertItem(i, name);
				return ;
			}
		}
		nodes->addItem(name);
	}
}

void GraphWindow::removeNode(int addr)
{
	QString name;
	getNodeName(addr, name);

	int itemIndex = nodes->findText(name);
	if (itemIndex > -1)
		nodes->removeItem(itemIndex);
}

void GraphWindow::graphData(const QString &data)
{
	plot->graphData(data);
}