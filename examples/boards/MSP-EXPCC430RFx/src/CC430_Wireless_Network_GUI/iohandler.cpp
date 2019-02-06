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

#include <QT>
#include <qdatetime.h> 
#include "sensmonmwnd.h"
#include "iohandler.h"
#include "conwindow.h"

int rebyte;
int rebyte2;
int pack_len;
int trigger;

class ConWindow;
extern int window_open;

IoHandler::IoHandler(QObject *pParent)
{
    pMyParent = (QMainWindow *)pParent;

    RefreshComPort();

	//conWindow=new ConWindow;
	//conWindow->show();
    comPort = new ComPort(this);

    connect(this, SIGNAL(dataReceived(/*int index*/)), pParent, SLOT(dataReceived(/*int index*/)));

    // Create instance to Node Database.
    pNodeDb = new NodeDb;

    id_node = new QByteArray("Node:");
	id_temperature= new QByteArray("Temp:");
	id_voltage = new QByteArray("Battery:");
    id_strength = new QByteArray("Strength:");
	id_EOP = new QByteArray("RE:");
}


IoHandler::~IoHandler()
{
}


//-----------------------------------------------------------------------------
/** \brief Open COM Port
 *
 *  Open COM port and start reading. 
 *
 *  \param[in] portName
 *          Name of the COM port. COMx
 *
 */
//-----------------------------------------------------------------------------
int IoHandler::openComPort(const QString &portName)
{
    if (comPort->Open(portName))
    {
        if (comPort->Read())
        {
            return IO_SUCCESS;
        }
        else
        {
            return IO_ERROR;
        }
    }
    else
    {
        return IO_ERROR;
    }
}

//-----------------------------------------------------------------------------
/** \brief Close COM Port
 *
 *  Close COM port 
 *
 */
//-----------------------------------------------------------------------------
int IoHandler::closeComPort()
{
    if (comPort->Close())
    {
        return IO_SUCCESS;
    }
    else
    {
        return IO_ERROR;
    }
}


//-----------------------------------------------------------------------------
/** \brief Refresh list of COM Port
 *
 *
 */
//-----------------------------------------------------------------------------
int IoHandler::RefreshComPort()
{
    // Make sure the list is empty
    while(m_lstComPort.size() > 0) m_lstComPort.removeFirst();
    m_lstComPort.clear();

    if (comPort->Enum(m_lstComPort) == ComPort::CP_SUCCESS)
    {
        m_bComPortListOk = TRUE;
        return IO_SUCCESS;
    }
    else
    {
        m_bComPortListOk = FALSE;
        return IO_ERROR;
    }
}

//-----------------------------------------------------------------------------
/** \brief Get list of available COM ports.
 *
 *
 */
//-----------------------------------------------------------------------------
QStringList &IoHandler::getComPortList()
{
    return m_lstComPort;
}


//-----------------------------------------------------------------------------
/** \brief	Handle packet
 *
 *  Get packet and analyze content.
 *
 *  \return
 *         \b bool. TRUE if COM-port openend successfully, else FALSE
 */
//-----------------------------------------------------------------------------
void IoHandler::handlePacket() 
{
    QByteArray *pPacket;

    if (comPort->getThread()->getPacket(&pPacket) > 0)
    {
        handleData(pPacket);
    }
    else
    {
        emit emptyPacket();
    }
}

//-----------------------------------------------------------------------------
/** \brief	Handle data
 *
 *  Parse packet and store data in Node database.
 *  emit signal that data base has changed.
 *  The data is received on the ingenious format:
 *  \r\nNode:XXXX,Temp:-XX.XF,Battery:X.XV,Strength:XXX%,RE:no 
 *  So a search on keywords and find values method is used
 *  \param[in] pPacket
 *         Pointer to packet buffer
 */
//-----------------------------------------------------------------------------
void IoHandler::handleData(QByteArray *pPacket)
{
    NodeDb::DATA *pData = new NodeDb::DATA;

	int position, sizeOfIdentifier,i;
	// Array to save the value which will be converted
    QByteArray temporary("");

	// securety to see if the array received is empty or do not contain end of packet
	if(pPacket->isNull() ||!(pPacket->contains(*id_EOP))){
	    // We do not want to start messing with an emtpy or incomplete message
		return;
	}

	position = pPacket->indexOf(*id_node);
	sizeOfIdentifier = id_node->size();

	if (position == -1) {
	    // Bad things can happen if "Node:" is not found in the message...
		return;
	}
    // Base node
	else if(pPacket->at(position + sizeOfIdentifier + 2) == 'B'){
		pData->addr = 0x0000;
		pData->type = Node::ACCESS_POINT;
	}
	// Other node
	else {
		// The node number is found here, see function description
		temporary.append(pPacket->at(position + sizeOfIdentifier + 2));
		temporary.append(pPacket->at(position + sizeOfIdentifier + 3));
        pData->addr = temporary.toInt();
		temporary.clear();
		pData->type = Node::END_DEVICE;
	}

	pData->status = NodeDb::DS_UPDATED;

	// Find and add the temperature
    position = pPacket->indexOf(*id_temperature);
	if (position == -1) {
	    // Bad things can happen if "Temp:" is not found in the message...
		return;
	}
	sizeOfIdentifier = id_temperature->size();
    temporary.append(pPacket->at(position + sizeOfIdentifier));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 1));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 2));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 3));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 4));
	
	// Trimmed to remove space at start if the temperature is below 100.0F
	temporary.trimmed();
    pData->temp = temporary.toFloat();

	// Cleared so voltage can be added
    temporary.clear();

	// Find and add voltage data
    position = pPacket->indexOf(*id_voltage);
	if (position == -1) {
	    // Bad things can happen if "Battery:" is not found in the message...
		return;
	}
	
	sizeOfIdentifier = id_voltage->size();
    temporary.append(pPacket->at(position + sizeOfIdentifier));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 1));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 2));
    pData->voltage = temporary.toFloat();

    // Cleared so strength can be added
    temporary.clear();

	// Find and add strength data
    position = pPacket->indexOf(*id_strength);
	if (position == -1) {
	    // Bad things can happen if "Strength:" is not found in the message...
		return;
	}
	sizeOfIdentifier = id_strength->size();
    temporary.append(pPacket->at(position + sizeOfIdentifier));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 1));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 2));
    pData->strength = temporary.toInt();

	// Cleared so RE can be added
    temporary.clear();

	// Find the RE data
    position = pPacket->indexOf(*id_EOP);
	if (position == -1) {
	    // Bad things can happen if "RE:" is not found in the message...
		return;
	}

	sizeOfIdentifier = id_EOP->size();
    temporary.append(pPacket->at(position + sizeOfIdentifier));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 1));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 2));
    pData->re = temporary.toInt();

	temporary.clear();
	temporary.append(pPacket->at(position + sizeOfIdentifier +3));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 4));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 5));
	temporary.append(pPacket->at(position + sizeOfIdentifier + 6));
	pData->t_time_on=temporary.toInt();
	
	// time stamp for console information	
	QDate date = QDate::currentDate();
	QString cur_date;
	cur_date.sprintf( "%2d/%2d/%4d", date.month(), date.day(),date.year());	

	QString qsTemp;
	qsTemp = QTime::currentTime().toString();

	pack_len=ComPortThread::PACKET_LENGTH;	

	for (i=0;i<pack_len;i++)
	{
		conTemp.append(pPacket->at(i));
	}

	conTemp.append(", ");
	conTemp.append(cur_date);
	conTemp.append(", ");
	conTemp.append(qsTemp);
	
	emit storeData(conTemp);
	conTemp="";

	// for test
	if (pData->type == Node::END_DEVICE)
	{
		rebyte2=temporary.toInt();

		pData->fadeNumber=0;
		pData->fadeTime=0;
		pData->newNode=1;
		pData->deletedNode=0;
	}
	
	int index;
    index = pNodeDb->addData(pData);

    emit dataReceived();
}

void IoHandler::start_con(void)
{
	if (window_open==0)
	{
		ConWindow *ConWin=new ConWindow;
		ConWin->show();
		connect(this, SIGNAL(storeData(const QString &)), ConWin, SLOT(storeData(const QString &)));
	}
}