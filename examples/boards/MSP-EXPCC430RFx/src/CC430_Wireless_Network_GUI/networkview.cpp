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
#include "networkview.h"
#include "edge.h"
#include "node.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QIcon>
#include <QSettings>

#include <math.h>


NetworkView::NetworkView(QWidget *pParent)
    : timerId(0)
{
    pParentWindow = pParent;

    pScene = new QGraphicsScene(this);
    pScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    pScene->setSceneRect(-420, -320, 820, 620);
    setScene(pScene);

    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

	pxmTiLogo.load("images\\TI_Red_Platform_Bar.png");
	pxmTiLogo3.load("images\\MSP430_Logo.png");
	pxmTiLogo4.load("images\\CC430_logo.png");

    // Add the Access Point to the Node data base.
    // This must be the first record in the data base
    pNodeDb = new NodeDb;

    NodeDb::DATA *pData = new NodeDb::DATA;
    pData->addr = 0x0000;
    pData->type = Node::ACCESS_POINT;
    pData->parent = -1;
    pData->temp = 32.0;
    pData->voltage = 0;
	pData->strength = 0;
	pData->re = 0;
	pData->t_time_on = 0;
	//xpData->onoff = 0;
	pData->prevStrength = 0;
    pData->xpos = 0;
    pData->ypos = 0;
	pData->fadeNumber=255;
	pData->fadeTime=0;
	pData->newNode=0;
	pData->deletedNode=0;

    pData->status = NodeDb::DS_NORMAL;
    pNodeDb->addData(pData);

    addNode(0);
	

}

void NetworkView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
     //x   scaleView(1.2);
        break;
    case Qt::Key_Minus:
    //x   scaleView(1 / 1.2);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}


void NetworkView::wheelEvent(QWheelEvent *)
{
   //x scaleView(pow((double)2, -event->delta() / 240.0));
}


void NetworkView::drawBackground(QPainter *painter, const QRectF &rect)
{
	QFont grfFont("Arial", 8, QFont::Bold);
	QString qsTemp2;
	//QPainterPath path;
	QRectF sceneRect = this->sceneRect();
    
    // Add fading from upper left to lower right corner
    QLinearGradient gradient(rect.x(),rect.y(),-rect.x(),-rect.y());
    gradient.setColorAt(0, Qt::black);
	gradient.setColorAt(1, Qt::white);
    painter->fillRect(rect.x(),rect.y(),rect.width(),rect.height(), gradient);
    
    // Draw the logo graphics
	painter->drawPixmap(rect.x() + rect.width()- 820, rect.y() + rect.height() - 70, pxmTiLogo); // TI logo
	painter->drawPixmap(rect.x() + rect.width()- 202, rect.y() + rect.height() -145, pxmTiLogo2);// Cymbet logo
	painter->drawPixmap(rect.x() + rect.width()- 820, rect.y() + rect.height() -635, pxmTiLogo3);// TI chip
	painter->drawPixmap(rect.x() + rect.width()- 800, rect.y() + rect.height() -125, pxmTiLogo4);// SEH logo
}

void NetworkView::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 1.3)
        return;

    scale(scaleFactor, scaleFactor);
}


//-----------------------------------------------------------------------------
/** \brief Start check alive timer
 *
 *  Start timer that will be used to check if node is still alive.
 */
//-----------------------------------------------------------------------------
void NetworkView::startCheckAliveTimer()
{
    QSettings settings;

    // The check on timeout will be in milli seconds
    aliveTimerInterval = settings.value("NodeTimeout", 20).toInt() * 1000;

    if (timerId)
    {
        killTimer(timerId);
    }

    // Start timer with fixed interval
    timerId = startTimer(CHECK_ALIVE_INTERVAL);
}

//-----------------------------------------------------------------------------
/** \brief Stop check alive timer
 *
 *  Stop timer that will be used to check if node is still alive.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::stopCheckAliveTimer()
{
    if (timerId)
    {
        killTimer(timerId);
    }
}

//-----------------------------------------------------------------------------
/** \brief Timer event
 *
 *  On timeout of timerId, check timestamp of each node and check if
 *  the alive time has expired.
 *
 *  \param[in] event
 *        Current event. Not used.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

	


    QList<int> deletedNodes;
    int i;

    // Node 0 is the Access Point and should not be checked.
    for (i = 1; i < nodeList.size(); i++)
    {
        // If elapsed time is more than the given interval
        // the node is probably out of service and should
        // be deleted from the view.
		if (nodeList[i]->pNode->getElapsedTime() > aliveTimerInterval) {
			emit nodeRemoved(i);
			deleteNode(i);
		}
    }

}

//-----------------------------------------------------------------------------
/** \brief Add Node to the network
 *
 *  Add node to the network scene
 *
 *  \param[in] nodeData
 *        Structure with data about the node.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::addNode(int index)
{
	
    DB_OPEN
    NodeDb::DATA *pData;
    pNodeDb->getData(index, &pData);

	emit nodeAdded(pData->addr);

    Node *pNode = new Node(this);
    pNode->setNodeData(index);

	// Access point
    if(pData->type == Node::ACCESS_POINT)
    {
        pNode->setToolTip(tr("Access Point\n \
Blinks when data is received.\n \
Data should be received once per second."));
    }
	// End Device
      else {
        pNode->setToolTip(tr("End Device\n \
Blinks each time data is updated.\n \
TxCt is the number of RF transmissions remaining when the\n \
End Device is running on a EnerChip only."));
    }

    pScene->addItem(pNode);

    DB_WRITE("Add node to nodeList: addr = " << pData->addr << "index = " << index);
    LIST_DATA *pListData = new LIST_DATA;
    pListData->pNode = pNode;
    pListData->pEdge = NULL;
    nodeList.insert(index, pListData);	
}

//-----------------------------------------------------------------------------
/** \brief Delete Node
 *
 *  Delete Node for given index. The index is related to the Node data base
 *  and the node list. All child nodes will also be deleted by calling this
 *  functin recursively.
 *
 *  \param[in] index
 *        Index of the Node DB.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::deleteNode(int index)
{
    NodeDb::DATA *pData;
    pNodeDb->getData(index, &pData);
	pData->deletedNode=1;
    Node *pNode = new Node(this);
    pNode->setNodeData(index);

	// Remove node from available nodes on graph
	emit nodeRemoved(pData->addr);

	// Remove Edge from parent node
	nodeList[pNodeDb->getIndex(pData->parent)]->pNode->removeEdge(pData->addr);

	pScene->removeItem(nodeList[index]->pNode);
	// There is no edge for the coordinator, so start with index 0
	pScene->removeItem(nodeList[index]->pEdge);
	delete nodeList[index]->pNode;
	delete nodeList[index]->pEdge;
	nodeList[index]->pNode = NULL;
	nodeList[index]->pEdge = NULL;
	nodeList.removeAt(index);
	pNodeDb->deleteNode(index);

	//pNodeDb->updateEndpointPos();			
}

//-----------------------------------------------------------------------------
/** \brief Delete All Nodes
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::deleteAll()
{
    int i;
 
    // Remove all edges from Coordinator node
    nodeList[0]->pNode->removeEdge(0);

    if (nodeList[0]->pEdge) 
    {
        pScene->removeItem(nodeList[0]->pEdge);
        delete nodeList[0]->pEdge;
    }

    for(i = 1; i < nodeList.size();i++)
    {
		emit nodeRemoved(i);
        pScene->removeItem(nodeList[i]->pNode);
        if (nodeList[i]->pEdge) pScene->removeItem(nodeList[i]->pEdge);
    }

    while(nodeList.size() > 1)
    {
        delete nodeList[1]->pNode;
        if (nodeList[1]->pEdge != NULL) delete nodeList[1]->pEdge;
        nodeList.removeAt(1);
    }
}

//-----------------------------------------------------------------------------
/** \brief Update Node data
 *
 *  \param[in] node
 *        The numerical id of the node.
 *  \param[in] data
 *        Reference to the updated data.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::updateNode(int index)
{
    nodeList[index]->pNode->updateData(index);
}

//-----------------------------------------------------------------------------
/** \brief Connect nodes
 *
 *  Setting a connector between two nodes.
 *
 *  \param[in] srce
 *        The source node.
 *
 *  \param[in] dest
 *        The destination node.
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::connectNodes(int srce, int dest)
{
    Edge *pEdge = new Edge(nodeList[srce]->pNode, nodeList[dest]->pNode);
    pScene->addItem(pEdge);
    nodeList[srce]->pEdge = pEdge;

}

//-----------------------------------------------------------------------------
/** \brief Change Temperature unit
 *
 *
 */
//-----------------------------------------------------------------------------
void NetworkView::changeTempUnit()
{
    for(int i = 0; i < nodeList.size();i++)
    {
        nodeList[i]->pNode->setTempUnit();
    }
}
