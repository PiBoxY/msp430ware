
#ifndef NODE_H
#define NODE_H
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

#include <QGraphicsItem>
#include <QList>
#include <QTime>
#include <QFont>
#include <QPixmap>
#include <QBrush>

#include <qlcdnumber.h>

#include "edge.h"



class Edge;
class NetworkView;
class QGraphicsSceneMouseEvent;
class NodeDb;



class Node : public QGraphicsItem, public QObject
{
public:
    typedef enum{ACCESS_POINT, END_DEVICE}NODE_TYPE;
    typedef struct
    {
        NODE_TYPE	type;
        int			addr;
        int			xPos;
        int			yPos;
		int			strength;
        float		temp;
        float		voltage;
		int			re;
		
		int			t_time_on;
		int			fadeNumber;
		int			fadeTime;
		int			newNode;
		int			deletedNode;
		
    }NODE_DATA;

		QBrush fill[40];
    Node(NetworkView *networkView);
	NODE_DATA nodeData;
    void addEdge(Edge *edge);
    QList<Edge *> edges() const;
	
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    bool advance();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setNodeData(int index);
    void updateData(int index);
    int getElapsedTime();
    void removeEdge(int childAddr = 0);
    int getAddr() { return nodeData.addr; }
    float getTemp() { return nodeData.temp; }
    int getType() { return nodeData.type; }
    void setTempUnit();
    void setColor(const NODE_TYPE type);
	int Node::find_active_node();
	void Node::current_updateData(int index);
	int timerId () const; 

	/*struct colors{
		int alpha;
		int r;
		int g;
		int b;
		int newnodeflag;
		int delnodeflag;
	};
	struct colors node_colors[100];*/
	

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void timerEvent(QTimerEvent *);
	
   // NODE_DATA nodeData;
   
private:
    QList<Edge *> edgeList;
    QPointF newPos;
    NetworkView *graph;
	QPixmap pixmap,alpha_pix,back_pix,pixmap2,pix;

    QColor nodeColor;
    QColor nodeColorDark;

	QFont modeFont;
	QFont addrFont;
	QFont tempFont;
	QFont battFont;
    QFont timeFont;
	QFont end_deviceFont;
	QFont	keyFont;

    QTime timeStamp;
	QDate dateStamp;

    NodeDb *pNodeDb;

    // Flag used to highlight node.
    bool updated;
    int updateTimerId,fade_speed,updateTimerId2,updateTimerId3;

    bool celsius;
	qreal arrowSize;

};

#endif
