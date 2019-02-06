#ifndef COMPORTTHREAD_H
#define COMPORTTHREAD_H
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

#include <QThread>
#include <QQueue>
#include <QMutex>

/// Start Of Packet
#define CPT_SOP '\r' // 0x0D carriage return

/** \brief Read COM port Thread
 *
 * This class is will provide methods to read data from the COM port
 * in its own thread.
 * Calling to the run() function will start the thread and stop() will end it.
 *
 */
class ComPortThread : public QThread
{
    Q_OBJECT

public:
    enum{PACKET_LENGTH = 62, UART_BUFFER_SIZE = 4096};//xx was 56 57

	ComPortThread();
    virtual ~ComPortThread();
    bool Init(const QString &qsPortName, unsigned int baudeRate = 9600,  int maxDataLength = 255);
    void Stop();
    bool initiated() { return bInitiated; }
    int WriteCOMMBytes(void * pData, int length);
    int getPacket(QByteArray **pPacket);

signals:
    void packetReady(void);


protected:
	/// Local data buffer (FIFO, trimmed upon uart access)
    typedef struct {
        unsigned char pData[2 * UART_BUFFER_SIZE];    ///< Data buffer
        int inPos;                                    ///< Head position
        int outPos;                                   ///< Tail position
        int bufferedCount;                            ///< The number of bytes currently in the buffer
    } UART_BUFFER;

	/// Local data buffer
    UART_BUFFER uartBuffer;

	/// Handle to the COM-port device.
	HANDLE m_hComDev;
    /// Variable to save error number returned by CreateFile()
    DWORD m_error;
	/// The DCB structure defines the control setting for a serial communications device
	DCB m_dcb; 
    /// The maximum length returned by \c GetUartData
    int m_maxDataLength;
    /// Variable to tell if COM port has been initiated.
    bool bInitiated;
    /// Variable used to stop the loop in the run() function.
    volatile bool stopped;
    /// packet FIFO queue where all the packets will be stored.
    QQueue<QByteArray *> packets;
    /// Mutex to protect the packet Queue to make it thread safe.
    QMutex mutex;

    void run();
    bool Close();
    bool GetUartData(int *pLength, BYTE *pData);
    bool PopUartBuffer(int *pLength, BYTE *pData);
    bool SearchForSop(void);
    void TrimUartBuffer();
    int ReadCOMMBytes(void *pData, int length);
};
#endif