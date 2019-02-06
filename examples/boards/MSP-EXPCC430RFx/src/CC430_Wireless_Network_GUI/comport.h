#ifndef COMPORT_H
#define COMPORT_H
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
#include "windows.h"
#include "setupapi.h"
#include "comportthread.h"
#include "nodedb.h"

#include <QStringList>
#include <QTimer>
#include <QCloseEvent>

class ComPort : public QObject
{
    Q_OBJECT

public:
    enum {CP_ERROR, CP_SUCCESS};

	ComPort(QObject *pParent);
    int Enum(QStringList &portList);
    bool Open(const QString &qsPortName, unsigned int baudeRate = 9600);
    bool Read();
    int write(unsigned char *pBuffer, int length);
    bool Close();
    ComPortThread *getThread(){return &comPortThread; }


signals:
    void dataReceived();
    void emptyPacket();
    void unknownMessage();
	void textChanged(const QString &);
	//void storeData(const QString &);
protected:

    bool IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon);

    /// Comport Thread used to read data.
    ComPortThread comPortThread;

};

#endif
