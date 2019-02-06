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

#include <QTextStream>
#include <math.h>
#include <stdlib.h>

#include "nodedb.h"


// The data base is defined static in order to be the 
// same for all instances of the NodeDb class.
//static QQueue<NodeDb::DATA *> nodeData;
static QList<NodeDb::DATA *> nodeData;

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

NodeDb::NodeDb()
{
}

NodeDb::~NodeDb()
{

}

//-----------------------------------------------------------------------------
/** \brief Add Node data
 *
 *  Check if node already exist based on the source address.
 *  If existing: 
 *        Update temperature and voltage information and set status = updated
 *        or moved.
 *  If not existing:
 *        Insert the node in the correct place in the nodeData array. Update
 *        all all End Device postions
 *
 *  \param[in] pData
 *        Pointer to data.
 *
 *  \return int
 *        List index of updated/new item
 */
//-----------------------------------------------------------------------------
int NodeDb::addData(DATA *pData)
{
    int i, numberOfNodes, radius;

    // The Coordinator should be added first
	if (pData->addr == 0x0000){
		
		if(nodeData.isEmpty()){
			nodeData.insert(0, pData);
			return 0;
		}
	}
    // Search to see if existing, and update node data if it is
    if (!nodeData.isEmpty()){

        for (i = 0; i < nodeData.size(); i++) {

            if (nodeData[i]->addr == pData->addr) {

                DB_WRITE("Update address " << pData->addr);
                nodeData[i]->temp = pData->temp;
                nodeData[i]->voltage = pData->voltage;
				nodeData[i]->re = pData->re;
				nodeData[i]->t_time_on = pData->t_time_on;
				//nodeData[i]->fadeNumber = pData->fadeNumber;
				//nodeData[i]->fadeTime = pData->fadeTime;
				//nodeData[i]->newNode = pData->newNode;
				//nodeData[i]->deletedNode = pData->deletedNode;
				
				// Check if the strength signal has been changed
        		if(abs(pData->strength - nodeData[i]->prevStrength)) {
					radius = calculateRadius(pData->strength);
                    nodeData[i]->xpos = radius*cos(nodeData[i]->angle);
					nodeData[i]->ypos = radius*sin(nodeData[i]->angle);
					nodeData[i]->prevStrength = pData->strength;
					
					nodeData[i]->status =DS_MOVE;
		
		
				}
				// If the strength signal is the same as before, just indicate update
				else {
                    nodeData[i]->status = DS_UPDATED;
				}

                return i;
            }
        }
    }

    // Getting here means that we have to add a new End Device connected
    DB_WRITE("Address = " << pData->addr);

    pData->parent = 0x0000;
    pData->type = Node::END_DEVICE;
    pData->status = DS_NEW;

    // Save the current number of nodes
	numberOfNodes = nodeData.size();

	// Insert the node before any existing node with a lower address
	for (i = 1; i < numberOfNodes; i++) {
		if(pData->addr < nodeData[i]->addr) {
			nodeData.insert(i, pData);
			nodeData[i]->prevStrength = pData->strength;
            break;
		}
	}
	// Check if the node has been inserted, if not, append at the end
	if(nodeData.size() == numberOfNodes) {
        nodeData.append(pData);
		nodeData[nodeData.size()-1]->prevStrength = pData->strength;
	}
	// Update all Endpoint Devices postions
    updateEndpointPos();

    return i;
}

//-----------------------------------------------------------------------------
/** \brief Get Node Data
 *
 *  \param[in] index
 *        Index of required list item
 *  \param[out] pData
 *        Pointer to data.
 *  \return bool
 *        true if item found, else false
 */
//-----------------------------------------------------------------------------
bool NodeDb::getData(int index, DATA **pData)
{
    if (index >= nodeData.size()) return false;

    if (nodeData.isEmpty()) return false;

	
	*pData = nodeData[index];

    return true;
}

//-----------------------------------------------------------------------------
/** \brief Delete data for one node
 *
 *  \param[in] index
 *         Index of node to be deleted.
 *
 *  \return bool
 *         true if node could be deleted, else false.
 */
//-----------------------------------------------------------------------------
bool NodeDb::deleteNode(int index)
{
    if (index > 0)
    {
        delete nodeData[index];
        nodeData.removeAt(index);
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
/** \brief Delete All data
 *
 */
//-----------------------------------------------------------------------------
bool NodeDb::deleteAll()
{
    while (nodeData.size() > 1)
    {
        delete nodeData[1];
        nodeData.removeAt(1);
    }

    return true;
}

//-----------------------------------------------------------------------------
/** \brief Update Endpoints position
 * 
 */
//-----------------------------------------------------------------------------
void NodeDb::updateEndpointPos()
{
	// The angle between all children nodes of the connector
	double angleBetweenNodes;
    int i, nbrOfEndpoints, radius;

	// Number of End Devices are all nodes except the Access Point
    nbrOfEndpoints = nodeData.size()-1;
    angleBetweenNodes = TwoPi/(nbrOfEndpoints);//TwoPi

    // When we know the number of End Devices we have to adjust all
	// End Devices position and angle
    DB_WRITE("updateEndpointPos: Nbr Endpoints = " << nbrOfEndpoints);
    for (i = 1; i < nodeData.size(); i++) {
		// Polar coordinates
		radius = calculateRadius(nodeData[i]->prevStrength);
		nodeData[i]->angle = (i-1)*angleBetweenNodes;
        nodeData[i]->xpos = radius * cos(nodeData[i]->angle);
        nodeData[i]->ypos = radius * sin(nodeData[i]->angle);

		// Update status to moved, so the position is updated
		// Should not be done for new nodes
        if (nodeData[i]->status != DS_NEW) {
            nodeData[i]->status = DS_MOVE;
        }
    }
    

}

//-----------------------------------------------------------------------------
/** \brief Get Index
 * 
 *  Search data base for given address and return index
 *
 *  \param[in] addr
 *        address to search
 */
//-----------------------------------------------------------------------------
int NodeDb::getIndex(int addr)
{
    int index = 0;
    for (int i = 0; i < nodeData.size(); i++)
    {
        if (nodeData[i]->addr == addr)
        {
            index = i;
            break;
        }
    }

    return index;
}

//-----------------------------------------------------------------------------
/** \brief Return the endpoint depending on the strength.
 *
 *  The strength value and the distance from the Access Point have no
 *  proportional property. The strength value depends on the angle between
 *  the antennas, obstacles in the way, and reflections from nearby objects.
 *  The radius have been "calculated" by noting the strength signal
 *  while measuring the length when both Access Point and End Device are 
 *  hanging in free air with no obstacles in between.
 * 
 *  \param[in] strength
 *        Strength of the received signal
 */
//-----------------------------------------------------------------------------
int NodeDb::calculateRadius(int strength){
	
	/*if     (strength > 85) // 0-2cm
		return 130;
	else if(strength > 75) // 2-5cm
		return 140;
	else if(strength > 65) // 5-10 cm
		return 150;
	else if(strength > 60) // 10-15 cm
		return 160;
	else if(strength > 58) // 15-20 cm
		return 170;
	else if(strength > 56) // 20-200cm until next comment
		return 180;
	else if(strength > 54)
		return 190;
	else if(strength > 52)
		return 200;
	else if(strength > 49) 
		return 210;
	else if(strength > 45) // ca 250cm
		return 220;
	else if(strength > 42) // ca 350cm
		return 230;
	else if(strength > 39)  // Everything below is just bad reception on one signal
		return 240;
	else if(strength > 37)
		return 250;
	else
		//return 260;
		return 130;*/

//new data table for version 2.0
	if     (strength > 33) // 0-2cm
		return 130;
	else if(strength > 29) // 2-5cm
		return 140;
	else if(strength > 27) // 5-10 cm
		return 150;
	else if(strength > 25) // 10-15 cm
		return 160;
	else if(strength > 23) // 15-20 cm
		return 170;
	else if(strength > 22) // 20-200cm until next comment
		return 180;
	else if(strength > 21)
		return 190;
	else if(strength > 20)
		return 200;
	else if(strength > 19) 
		return 210;
	else if(strength > 18) // ca 250cm
		return 220;
	else if(strength > 17) // ca 350cm
		return 230;
	else if(strength > 16)  // Everything below is just bad reception on one signal
		return 240;
	else if(strength > 15)
		return 250;
	else
		return 260;
		







}