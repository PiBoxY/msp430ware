#ifndef NODEDB_H
#define NODEDB_H
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

#include <QQueue>
#include <QMutex>
#include <QFile>

#include "node.h"

#define DB_WRITE(T)
#define DB_OPEN

class NodeDb
{
public:
    typedef enum{DS_NORMAL, DS_NEW, DS_UPDATED, DS_MOVE}DB_STATUS;
    typedef struct
    {
        int              parent;
		int              addr;
        Node::NODE_TYPE  type;
        float            temp;
        float            voltage;
		double           angle;
		int              prevStrength;
        int              strength;
		int              re;
		
		int				 t_time_on;
		int              xpos;
        int              ypos;
		int				fadeNumber;
		int				fadeTime;
		int				newNode;
		int				deletedNode;
        DB_STATUS        status;
    }DATA;

    NodeDb();
    virtual ~NodeDb();
    bool deleteNode(int index);
    bool deleteAll();
    int addData(DATA *pData);
    bool getData(int index, DATA **pData);
    int getIndex(int addr);
	void updateEndpointPos();

protected:
   // void updateEndpointPos();

private:
	int calculateRadius(int strength);

};

#endif
