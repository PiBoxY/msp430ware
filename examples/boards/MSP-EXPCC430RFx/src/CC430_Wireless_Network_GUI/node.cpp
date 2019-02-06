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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QTimer>
#include <QTextStream>
#include <QSettings>
#include <QPixmapCache>
#include <QPixmap>
#include <QtGui>
#include <QPainterPath>
#include <QT>
#include <qdatetime.h> 



#include "edge.h"
#include "node.h"
#include "networkview.h"
extern int rebyte;
#define apcount	6
int  cnt_rf;
int i,ap_counter; 
int used_nodes[10];
int index_number=0;
int index_number_save=0;

struct colors{
		int alpha;
		int r;
		int g;
		int b;
		int newnodeflag;
		int delnodeflag;
	};
	struct colors node_colors[100];


#define extra_bit_used		0				// 1=extra bit used 0= normal use change for different info
//char swoff["OFF];

Node::Node(NetworkView *networkView)
    : graph(networkView),
    modeFont("Arial", 13, QFont::Bold),
    addrFont("Arial", 13, QFont::Bold),
    tempFont("Arial", 9, QFont::Bold),
    battFont("Arial", 13, QFont::Bold),
    timeFont("Arial", 10, QFont::Bold),
	keyFont("Arial", 8, QFont::Bold),
	end_deviceFont("Arial", 11, QFont::Bold),
	updateTimerId(0),
	updateTimerId2(0)
{
     setZValue(1);

    // Create instance for Node database
    pNodeDb = new NodeDb;

    updated = false;

    // Set time stamp for the first time and start counting
    // elapsed time.
    timeStamp.start();

    QSettings settings;
    celsius = settings.value("TempInCelsius", true).toBool(); 
	//node_colors[1].alpha = 255;
	node_colors[0].alpha = 255;
	fade_speed=30;
	
	
    updateTimerId2 = startTimer(50);//x200 600
    
	
}

void Node::addEdge(Edge *edge)
{
    DB_OPEN
    DB_WRITE("NODE addEdge: nodeData.addr " << hex << nodeData.addr << " source = " << hex << edge->sourceNode()->getAddr());
    edgeList << edge;
    edge->adjust();
}

QList<Edge *> Node::edges() const
{
    return edgeList;
}

void Node::removeEdge(int childAddr)
{
    DB_OPEN
    DB_WRITE("NODE removeEdge: nodeData.addr " << hex << nodeData.addr << " childAddr = " << hex << childAddr);
    if (childAddr == 0)
    {
        // If childAddr = 0 all the edges should be removed from the list
        while (edgeList.size() > 0)
        {
            edgeList.removeLast();
        }
    }
    else
    {
        // Remove edge with childAddr
        for (int i = 0; i < edgeList.size(); i++)
        {
            if ((edgeList[i]->sourceNode()) && (childAddr == edgeList[i]->sourceNode()->getAddr()))
            {
                DB_WRITE("NODE removeEdge: removeAt() index = " << i);
                edgeList.removeAt(i);
            }
        }
    }
}

bool Node::advance()
{
    DB_OPEN
    if (newPos == pos())
        return false;

    DB_WRITE("advance x = " << newPos.x() << " y = " << newPos.y());
    setPos(newPos);

    foreach (Edge *edge, edgeList) 
    {
        edge->update();
    }

    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-50 - adjust, -50 - adjust,
                  110 + adjust, 110 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-50, -50, 100, 100);
    return path;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	QPixmap pixmap2(200,100);
	pixmap2.fill(Qt::white);
	QPixmap pix;
	QPainter painter1(&pixmap);
	QPainter painter2(&pixmap2);
	QString id;
	Q_UNUSED(option);

	if (nodeData.addr!=0){
		if(nodeData.fadeNumber>5&&nodeData.deletedNode!=0)
		{
			nodeData.fadeNumber-=fade_speed;
			if (nodeData.fadeNumber<=5){
			
				nodeData.fadeNumber=0;
				nodeData.deletedNode=1;
				index_number=0;
			}
		}

		if(nodeData.fadeNumber<255&&nodeData.newNode!=0)
		{
			nodeData.fadeNumber+=fade_speed;
			if (nodeData.fadeNumber>=255)
			{	
				nodeData.fadeNumber=255;
				nodeData.newNode=0;
				//current_updateData(index_number);
				index_number=0;
			}
		}
	}
    // Draw the grey shadow for a 3d effect.
    painter->setPen(Qt::NoPen);
    painter->setBrush (QColor(0x80, 0x80, 0x80,(nodeData.fadeNumber)));
    painter->drawEllipse(-45, -45, 100, 100);		

    // Set gradient and color of the node
    QRadialGradient gradient(0, 0, 50, -20, -20);// access points

    if (nodeData.addr==0&&updated == true) 
    {
        // Mouse click on node or an update of node data~
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, nodeColor.light(120));
		gradient.setColorAt(.5, nodeColorDark.light(120));
        gradient.setColorAt(0, nodeColorDark.light(120));
        updated = false;
		painter->setBrush(gradient);
    } 
	else if (nodeData.addr==0&&updated == false)
	{
		gradient.setColorAt(0, nodeColor.light(150));
        gradient.setColorAt(1, nodeColorDark);
		painter->setBrush(gradient);
    }
	// end devices
	if (nodeData.addr!=0&&updated == true) 
	{
		// Mouse click on node or an update of node data~
		gradient.setCenter(3, 3);
		gradient.setFocalPoint(3, 3);
		gradient.setColorAt(1, nodeColor);
		gradient.setColorAt(.5, nodeColor);
		gradient.setColorAt(0, nodeColor);
		updated = false;
		painter->setBrush(gradient);
	} 
	else if (nodeData.addr!=0&&updated == false)
	{
		gradient.setColorAt(0, nodeColor);
		gradient.setColorAt(.95, nodeColorDark);
		gradient.setColorAt(1, nodeColor);
		painter->setBrush(gradient);			
	 }
	 
	 // painter->setBrush(gradient);
	 // Draw black circle around the node
	 painter->setPen(QColor(0, 0, 0,(nodeData.fadeNumber)));
	 // Draw the node
	 painter->drawEllipse(-50, -50, 100, 100);
	 
	 //x new text
     // White pen for text
	 if (nodeData.addr==0)
	 {
		 painter->setPen(QPen(Qt::white, 1));
	 }
	 else
	 {
		 if(nodeData.re<100)
		 {
			 painter->setPen(QColor(0, 0, 0,(nodeData.fadeNumber)));
		 }
		 else
		 {
			 painter->setPen(QColor(0xff, 0xff, 0xff,(nodeData.fadeNumber)));
		 }
	 }
	 QString qsTemp;
	 QString qsTemp2;
	 
	 // Set text in the node
	 // Write the temperature
	if(celsius) {
		qsTemp.sprintf("%.1f°C", ((nodeData.temp-32.0)/1.8));
	}
	else {
        qsTemp.sprintf("%.1f°F", nodeData.temp);	
	}
	 if (nodeData.addr==0)					//node 0 
	{
	//xpainter->setFont(tempFont);
			painter->setFont(addrFont);
			painter->drawText(-50,-45,100,40,Qt::AlignCenter , qsTemp,0);
	 }
	 else 
	 {
		 painter->setFont(timeFont);
		 painter->drawText(-50,-48,100,40,Qt::AlignCenter , qsTemp,0);
	 }

	if (nodeData.addr==0)					//node 0 
	{
		// Write node number
		//qsTemp.sprintf("Node: %d", nodeData.addr);
		qsTemp.sprintf("Access Point");
		//xpainter->setFont(addrFont);
		painter->setFont(timeFont);
		painter->drawText(-50,-15,100,20,Qt::AlignCenter , qsTemp,0);
	}
	 else
	 {
		// Write node number
		qsTemp.sprintf("End Device %d", nodeData.addr);
		//xpainter->setFont(addrFont);
		painter->setFont(timeFont);
		painter->drawText(-50,-22,100,20,Qt::AlignCenter , qsTemp,0);
	 }

// use the extra bit for on or off	
	if (extra_bit_used	==1)
	{
		if (nodeData.addr>0&&nodeData.re>100)
		{
			// Write number of rf transmissions
			cnt_rf=nodeData.t_time_on;
			if (cnt_rf>=1000)
			{
				cnt_rf=(cnt_rf-1000);
				qsTemp.sprintf("TxCt: %d ON", cnt_rf);
			}
			else
			{
				qsTemp.sprintf("TxCt: %d OFF", cnt_rf);
			}
			painter->setFont(timeFont);
			painter->drawText(-50,-7,100,20,Qt::AlignCenter , qsTemp,0);
		}
		else if(nodeData.addr>0)
		{
			// Write number of rf transmissions
			cnt_rf=nodeData.t_time_on;
			if (cnt_rf>=1000)
			{
				cnt_rf=(cnt_rf-1000);
				qsTemp.sprintf("ON");
			}
			else
			{
				qsTemp.sprintf("OFF");
			}
			painter->setFont(timeFont);
			painter->drawText(-50,-7,100,20,Qt::AlignCenter , qsTemp,0);
		}
	}
	else
	{
		if (nodeData.addr>0&&nodeData.re>100)
		{
			// Write number of rf transmissions
			cnt_rf=nodeData.t_time_on;
			if (cnt_rf>=1000)
			{
				cnt_rf=(cnt_rf-1000);
			}
					
			qsTemp.sprintf("TxCt: %d", cnt_rf);
			//xpainter->setFont(addrFont);
			painter->setFont(timeFont);
			painter->drawText(-50,-7,100,20,Qt::AlignCenter , qsTemp,0);//x10-	
		}
	}

	// Write RE
	if (nodeData.re>100)
	{
		rebyte=nodeData.re-100;
	}
	else
	{
		rebyte=nodeData.re;
	}

	if (rebyte>4&&nodeData.addr>0)
	{
		qsTemp.sprintf("%d Sec", rebyte);//rebyte
		painter->setFont(timeFont);
		painter->drawText(-50,+8,100,20,Qt::AlignCenter , qsTemp,0);
	}
	else if (nodeData.addr>0)
	{
		qsTemp.sprintf("%d Min", rebyte);
		painter->setFont(timeFont);
		painter->drawText(-50,+8,100,20,Qt::AlignCenter , qsTemp,0);
	}
	else if (nodeData.addr==0)
	{
		//qsTemp = timeStamp.toString("hh:mm:ss");
		qsTemp = timeStamp.toString(Qt::LocalDate);
		
		//qsTemp.sprintf("TxCt: %d", cnt_rf);// for Jeff battery test only
		painter->setFont(timeFont);
		painter->drawText(-50,+5,100,20,Qt::AlignCenter , qsTemp,0);
	}

	// Write the battery voltage
	qsTemp.sprintf("%.1fV", nodeData.voltage);
	painter->setFont(battFont);
	painter->drawText(-50,+25,100,20,Qt::AlignCenter , qsTemp,0);
	
		
//####################################################
// Legend
	if (nodeData.addr==0){

		painter->setBrush(Qt::white);
		painter->setPen(QPen(Qt::black, 2));
		painter->drawRect(+265,-215,+125,-95);
		painter->setPen(QPen(Qt::black, 1));

		//Yellow node
		QRadialGradient gradient(280, -268,9, 280, -271);
		gradient.setColorAt(0, QColor(0xFF, 0xFF, 0x00,0xFF));		
		gradient.setColorAt(.95, QColor(0xD0, 0x80, 0x00,0xFF));	
		gradient.setColorAt(1.5, Qt::black);
		painter->setBrush(gradient);
		painter->drawEllipse(+270, -278, 20, 20);

		//Blue node
		QRadialGradient gradient1(+280, -242,9, +280, -245);
		gradient1.setColorAt(0, QColor(0x00, 0xF0, 0xFF, 0xFF));		
		gradient1.setColorAt(.95, Qt::blue);						
		gradient1.setColorAt(1.5, QColor(0x00, 0xF0, 0xFF, 0xFF));	
		painter->setBrush(gradient1);
		painter->drawEllipse(+270, -252, 20, 20);
		
		qsTemp.sprintf("Power Source");
		painter->setPen(QPen(Qt::black, 0));	
		painter->setFont( keyFont);
		painter->drawText(+293, -300,80,80,Qt::AlignLeft, qsTemp,0);

		qsTemp.sprintf("Solar Cell"); 
		painter->setPen(QPen(Qt::black, 0));
		painter->setFont(keyFont);
		painter->drawText(+293, -275,100,75,Qt::AlignLeft, qsTemp,0);

		qsTemp.sprintf("EnerChip Battery");
		painter->setPen(QPen(Qt::black, 0));
		painter->setFont(keyFont);
		painter->drawText(+293, -250,100,75,Qt::AlignLeft, qsTemp,0);	
	}
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    DB_OPEN
    switch (change) {
    case ItemPositionHasChanged:
        DB_WRITE("NODE ItemChange: addr = " << hex << nodeData.addr << " nbr items edgeList = " << edgeList.size());
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

//-----------------------------------------------------------------------------
/** \brief	Set Node Data
 *
 *	Node data has changed. Use index to find corresponding record in Node DB.
 *
 *  \param[in] index
 *        Index to the Node Data Base. 
 *
 */
//-----------------------------------------------------------------------------
void Node::setNodeData(int index)
{
    NodeDb::DATA *pData;
    pNodeDb->getData(index, &pData);

    nodeData.addr = pData->addr;
    nodeData.temp = pData->temp;
    nodeData.voltage = pData->voltage;
    nodeData.type = pData->type;
	nodeData.re = pData->re;
	nodeData.t_time_on = pData->t_time_on;
	nodeData.fadeTime = pData->fadeTime;
	nodeData.newNode = pData->newNode;
	nodeData.deletedNode = pData->deletedNode;
	nodeData.fadeNumber = pData->fadeNumber;

    timeStamp =QTime::currentTime();
    setColor(nodeData.type);
    newPos.setX(pData->xpos);
    newPos.setY(pData->ypos);
    advance();
}

//-----------------------------------------------------------------------------
/** \brief	Set Mode
 *
 *	Set Mode of the node. 
 *
 */
//-----------------------------------------------------------------------------
void Node::setColor(const NODE_TYPE type)
{
    switch(type)
    {
        case ACCESS_POINT :
            nodeColor = Qt::red;
            nodeColorDark = Qt::darkRed;
          
            break;
        case END_DEVICE :
	
			if (nodeData.fadeNumber>255 ||nodeData.fadeNumber<0)
			{
				nodeData.fadeNumber=255;
			}
			if (nodeData.re<100)
			{
				nodeColorDark = QColor(0xD0, 0x80, 0x00,(nodeData.fadeNumber));	
				nodeColor =QColor(0xff, 0xff, 0x00,(nodeData.fadeNumber));
			}
			else
			{
				nodeColor = QColor(0x00, 0xf0, 0xff,(nodeData.fadeNumber));
				nodeColorDark =QColor(0x00, 0x00, 0xff,(nodeData.fadeNumber));
			}
            break;
        default :
            break;
    }
}

//-----------------------------------------------------------------------------
/** \brief	Update Data
 *
 *	Update Node data
 *
 */
//-----------------------------------------------------------------------------
void Node::updateData(int index)
{
	//setNodeData(index);
  
	DB_OPEN
    NodeDb::DATA *pData;
    pNodeDb->getData(index, &pData);

    nodeData.temp= pData->temp;
    nodeData.voltage = pData->voltage;
    nodeData.type = pData->type;
    nodeData.strength = pData->strength;
	nodeData.re = pData->re;
	nodeData.t_time_on = pData->t_time_on;
	
	nodeData.fadeNumber=pData->fadeNumber; 
	nodeData.fadeTime =pData->fadeTime ;
	nodeData.newNode=pData->newNode;
	nodeData.deletedNode=pData->deletedNode;
	
    setColor(nodeData.type);

    if (pData->status == NodeDb::DS_MOVE)
    {
        newPos.setX(pData->xpos);
        newPos.setY(pData->ypos);
        advance();
    }
	
	if (nodeData.addr==0){
		nodeData.fadeNumber = 255;//pData->fadeNumber;
		//ap_counter=apcount;
		updated = true;
		update();
		if (!updateTimerId) 
		{
			updateTimerId = startTimer(200);
		}
	}

	timeStamp.restart();

	if (nodeData.addr!=0)
	{
		updated = true;
		update();
	}
}

//-----------------------------------------------------------------------------
/** \brief	Timer Event handler
 *
 *	Only one timer event is registrated for this class
 *
 */
//-----------------------------------------------------------------------------
void Node::timerEvent(QTimerEvent *e)
{
	//Q_UNUSED(event);

	if (e->timerId() ==updateTimerId)
	{
		killTimer(updateTimerId);
		updateTimerId = 0;
		updated = false;
		update();
	}
	 
	if (e->timerId() ==updateTimerId2)
	{
		if (nodeData.addr!=0 )
		{
			setColor(nodeData.type);
			foreach (Edge *edge, edgeList) 
			{	
				edge->update();
			}
			updated = false;
			update();	
		}			
	}
}



//-----------------------------------------------------------------------------
/** \brief	Get elapsed time 
 *
 *	return elapsed time since previous update of the node.
 *
 */
//-----------------------------------------------------------------------------
int Node::getElapsedTime()
{
    return timeStamp.elapsed();
}

//-----------------------------------------------------------------------------
/** \brief	Set temperature unit
 *
 *	Current temp. unit should be retrieved from the settings
 *
 */
//-----------------------------------------------------------------------------
void Node::setTempUnit()
{
    QSettings settings;
    celsius = settings.value("TempInCelsius", true).toBool();
    update();
}

//-----------------------------------------------------------------------------
int Node::find_active_node()
{
	int i;
	for (i = 1; i < 10; i++)
    {
		if (used_nodes[i]==1)
		{
			return used_nodes[i];
		}
	}
	return false;
}

void Node::current_updateData(int index)
{	
		DB_OPEN
		NodeDb::DATA *pData;
		pNodeDb->getData(index, &pData);

		pData->newNode=nodeData.newNode;
		pData->fadeNumber=nodeData.fadeNumber;
		pData->deletedNode=nodeData.deletedNode;
		//index_number=0;    
}