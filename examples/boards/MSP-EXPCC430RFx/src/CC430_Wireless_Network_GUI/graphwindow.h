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

#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
#include <QApplication>
#include <QtGui>
#include <QMainWindow>
#include <qlayout.h>
#include <qlabel.h>
#include <qpainter.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>


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

#include <QString>

#include "nodedb.h"
#include "sensorplot.h"


class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    GraphWindow(QObject *pParent);
	void setNodesList(const QStringList &pList);
	
    
public slots:
	void addNode(int addr);
	void removeNode(int addr);
	void graphData(const QString &);
	void setCurrentNode(int);

protected:
	QMainWindow *pMyParent;

private:
    void init();
	void getNodeName(int addr, QString &name);

	QWidget vBox;
	QVBoxLayout *layout;
	QComboBox *nodes;
    SensorPlot *plot;

	NodeDb::DATA *node;
};

#endif
