#ifndef IOHANDLER_H
#define IOHANDLER_H
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
#include "comport.h"

#include <QStringList>
#include <QMessageBox>
#include <QMainWindow>

class ConWindow;
/** \brief Handler of all communication with connected device.
 *
 * This class provides methods to give a list of connected devices that are applicable
 * for this application and the methods to communicate with the selected device. 
 *
 * This class is implemented with platform independent functionality but for the actual access
 * to the underlaying hardware the ComPort class is implemented with all the platform dependent functionality.
 *
 *
 */
class IoHandler : public QObject
{
    Q_OBJECT

public:
    enum{IO_ERROR, IO_SUCCESS};

	IoHandler(QObject *pParent = NULL);
    virtual IoHandler::~IoHandler();
    int RefreshComPort();
    QStringList &getComPortList();

    int openComPort(const QString &portName);
    int closeComPort();
	QString conTemp;
	void Data_available(const QString data);
	void IoHandler::start_con(void);
	// ComPort *comPort;
signals:
    void dataReceived();
    void unknownMessage();
    void emptyPacket();
	void updateConsole(const QString &);
	void textChanged(const QString data);
	void storeData(const QString &);
private slots:
    void handlePacket();
  
protected:
    ComPort *comPort;
    QMainWindow *pMyParent;
	ConWindow *conWindow;

    bool m_bComPortListOk;
    QStringList m_lstComPort;

    /// Mutex to protect the Node database to make it thread safe.
    QMutex mutex;
    /// Pointer to the node data base
    NodeDb *pNodeDb;
	
    void handleData(QByteArray *pPacket);
private:
	QByteArray* id_node;
	QByteArray* id_temperature;
    QByteArray* id_voltage;
	QByteArray* id_strength;
	QByteArray* id_EOP;
	QByteArray* id_time;
	//QByteArray* id_onoff;
	 QTime timeStamp2;


};

#endif

