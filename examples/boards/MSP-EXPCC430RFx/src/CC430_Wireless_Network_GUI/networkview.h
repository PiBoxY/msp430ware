
#ifndef NETWORKVIEW_H
#define NETWORKVIEW_H
/*
 *
 * Copyright (C) 2009 Texas Instruments Incorporated - http://www.ti.com/ 
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


#include "qgraphicsview.h"
#include "node.h"
#include "nodedb.h"


class NetworkView : public QGraphicsView
{
    Q_OBJECT

public:
    typedef struct
    {
        Node *pNode;
        Edge *pEdge;
    }LIST_DATA;

    enum{CHECK_ALIVE_INTERVAL=1000};

    NetworkView(QWidget *pParent = NULL);

    void addNode(int index);
    void connectNodes(int srce, int dest);
    void updateNode(int index);
    void deleteAll();
    void startCheckAliveTimer();
    void stopCheckAliveTimer();
    void changeTempUnit();

signals:
	void nodeAdded(int index);
	void nodeRemoved(int index);
	

protected:
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);
    void wheelEvent(QWheelEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);
    void deleteNode(int index);

    void scaleView(qreal scaleFactor);

    QGraphicsScene *pScene;

private:
    QWidget *pParentWindow;
    /// Timer ID used for the time between each check to see if a node is alive or not.
    int timerId;
    /// Timer interval used to determine if Node is alive or not.
    int aliveTimerInterval;

    QList<LIST_DATA *> nodeList;
    
    QPixmap pxmTiLogo;
    QPixmap pxmTiLogo2;//x
	QPixmap pxmTiLogo3;
	QPixmap pxmTiLogo4;
    NodeDb *pNodeDb;
};

#endif
