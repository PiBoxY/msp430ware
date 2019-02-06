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

#include "comportthread.h"


/** \brief Constructor
 *
 */
ComPortThread::ComPortThread()
{
    stopped = false;
    m_hComDev = NULL;
    bInitiated = false;
}

/** \brief Destructor
 *
 */
ComPortThread::~ComPortThread()
{
    // Close COM Port if open.
 
    if (m_hComDev) Close();
}

/** \brief Initialize COM port
 *
 *  \param[in] qsPortName
 *         Name of the port to open. COMx
 *  \param[in] baudeRate
 *         Baude Rate.
 *  \param[in] maxDataLength
 *         Max packet length handled by the thread. Packet with more bytes will be seen
 *         as an invalid packet. 
 */
bool ComPortThread::Init(const QString &qsPortName, unsigned int baudeRate,  int maxDataLength)
{
    // Set maximum expected packet length.
    m_maxDataLength = maxDataLength;

    // Reset buffer
    memset(&uartBuffer, 0x00, sizeof(UART_BUFFER));
	uartBuffer.inPos = 0;
	uartBuffer.outPos = 0;
	uartBuffer.bufferedCount = 0;

 
    QString portName;
    portName = "\\\\.\\" + qsPortName;
    // Open port, get COM-port handle
    m_hComDev = CreateFile(portName.utf16(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0/*FILE_ATTRIBUTE_NORMAL*/, NULL ); // exclusive access, no security attrs, overlapped I/O

	if (m_hComDev == (HANDLE) -1) 
	{
        m_error = GetLastError();
		return (false); // The COM port is not available
	} 
	else 
	{

		// Setup device buffers (bytes: in, out)
		SetupComm(m_hComDev,(DWORD)2048, (DWORD)2048);

		// Purge any data in both buffers
		PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	    // fill timeout structure
		COMMTIMEOUTS timeout;
	    GetCommTimeouts(m_hComDev, &timeout);
		// Read timeouts
		timeout.ReadIntervalTimeout         = 100;  // 100ms between incoming chars.
		timeout.ReadTotalTimeoutConstant    = 1500; // Each packet sends an update each second
		timeout.ReadTotalTimeoutMultiplier  = 0;
		// Write timeouts
		timeout.WriteTotalTimeoutConstant   = 500;
        timeout.WriteTotalTimeoutMultiplier = 0;  

		SetCommTimeouts(m_hComDev, &timeout);

	}

	// Create a DCB (The DCB structure defines the control setting for a serial communications device).
	m_dcb.DCBlength = sizeof( DCB ) ;
	GetCommState(m_hComDev, &m_dcb ) ;

	m_dcb.BaudRate = baudeRate;				    // current baud rate
	m_dcb.fBinary = TRUE;						// binary mode, no EOF check
	m_dcb.fParity = TRUE;						// enable parity checking
	m_dcb.fOutxCtsFlow = FALSE;					// CTS output flow control
	m_dcb.fOutxDsrFlow = FALSE;					// DSR output flow control
	m_dcb.fDtrControl = DTR_CONTROL_DISABLE;	// DTR flow control type
	m_dcb.fDsrSensitivity = FALSE;				// DSR sensitivity
	m_dcb.fTXContinueOnXoff = FALSE;			// XOFF continues Tx
	m_dcb.fOutX = FALSE;						// XON/XOFF out flow control
	m_dcb.fInX = FALSE;							// XON/XOFF in flow control
	m_dcb.fErrorChar = FALSE;					// enable error replacement
	m_dcb.fNull = FALSE;						// enable null stripping
	m_dcb.fRtsControl = RTS_CONTROL_DISABLE;	// RTS flow control
	m_dcb.fAbortOnError = FALSE;				// abort on error
	m_dcb.XonLim = 1;							// transmit XON threshold
	m_dcb.XoffLim = 1;							// transmit XOFF threshold
	m_dcb.ByteSize = 8;							// number of bits/byte, 4-8
	m_dcb.Parity = NOPARITY;					// 0-4=no,odd,even,mark,space
	m_dcb.StopBits = ONESTOPBIT;				// 0,1,2 = 1, 1.5, 2

	// Activate the new control settings.
	if ( SetCommState(m_hComDev, &m_dcb) ) 
	{
        bInitiated = TRUE;
		return TRUE;
	} 
	else 
	{
		CloseHandle(m_hComDev); // illegal baud rate
		return FALSE;
	}
}

//-----------------------------------------------------------------------------
/** \brief	Run the thread
 *
 *	Start a loop to read data from COM port until stopped has been set true.
 *
 */
//-----------------------------------------------------------------------------
void ComPortThread::run()
{
	unsigned char pPacketBuffer[PACKET_LENGTH];

    while (!stopped)
    {
        int nLength;

		if (GetUartData(&nLength, pPacketBuffer))
		{
			if (nLength > 0)
			{
                QByteArray *packet = new QByteArray((char *)pPacketBuffer, nLength);
                //Save the packet on the Queue
                mutex.lock();
                packets.enqueue(packet);
                mutex.unlock();

                emit packetReady();
			}
		}
    }

    // Reset the stopped variable
    stopped = false;
    // Close the COM port
    Close();
}

//-----------------------------------------------------------------------------
/** \brief	Stop the thread
 *
 *	Set stopped variable true in order to stop the loop in the run function.
 *
 */
//-----------------------------------------------------------------------------
void ComPortThread::Stop()
{
    stopped = true;
}

//-----------------------------------------------------------------------------
/** \brief	Close COM-port
 *
 *	Close the active COM-port
 *
 *  \return
 *      \b bool. TRUE if successfull, else FALSE.
 */
//-----------------------------------------------------------------------------
bool ComPortThread::Close() 
{

	// Variables
	DWORD    error;
	DWORD    errorFlags;
	COMSTAT	 ComStat;

    bInitiated = false;

	// Empty in- and output buffers.
	ClearCommError(m_hComDev, &errorFlags, &ComStat);

	if (!PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR)) 
	{
		// Check for errors (ERROR_INVALID_HANDLE will cause CloseHandle to fail)
		error = GetLastError();
		if (error == ERROR_INVALID_HANDLE) 
		{
			return FALSE;
		}
	}

	// Shut it down
	CloseHandle(m_hComDev);
	m_hComDev = NULL;

	return TRUE;
} // end of CloseCOMM

//-----------------------------------------------------------------------------
/** \brief Get UART Data
 *
 * Check Local UART buffer to see if there is already a packet available.
 * If no packet available, read data from the COM port.
 *
 *  \param[out] pLength 
 *         Length of current data packet.
 *  \param[out] pData
 *         Pointer to data buffer.
 *
 *  \return
 *         \b bool. TRUE if data packet found in UART buffer, else FALSE
 */
//-----------------------------------------------------------------------------
bool ComPortThread::GetUartData(int *pLength, BYTE *pData) 
{
	// first try to pop from uart buffer
    if (PopUartBuffer(pLength, pData)) {
        return true;
    } 
	else {
		// buffer empty
        TrimUartBuffer();

        DWORD count = m_maxDataLength;
		count = ReadCOMMBytes(&uartBuffer.pData[uartBuffer.inPos], count);
		uartBuffer.inPos += count;
		uartBuffer.bufferedCount += count;

		// try once more to pop
		if (PopUartBuffer(pLength, pData)){
			return true;
		}
		else{
			return false;
		}
	}
}

//-----------------------------------------------------------------------------
/** \brief	Pops a single packet from the local data buffer
 * 
 * Search for Start Of Packet(SOP) in Local UART buffer.
 * If SOP found, check if the complete packet is given in the buffer.
 * 
 * 
 *  \param[out] pLength 
 *         Length of current data packet.
 *  \param[out] pData
 *         Pointer to data buffer.
 * 
 * \return
 *		\b bool. TRUE when the returned data is valid, otherwise FALSE.
 */
//-----------------------------------------------------------------------------
bool ComPortThread::PopUartBuffer(int *pLength, BYTE *pData) 
{

    // Search for Start Of Packet
	if( SearchForSop() ){

		*pLength = PACKET_LENGTH;

		// Make sure a whole packet has been received
		if(uartBuffer.outPos + PACKET_LENGTH < uartBuffer.inPos) {  

				// Copy all data from SOP byte
				memcpy(pData, uartBuffer.pData + uartBuffer.outPos, *pLength); 
	
				uartBuffer.outPos += PACKET_LENGTH; 
				uartBuffer.bufferedCount -= PACKET_LENGTH;
				return true;
		}
		else {
            // Continue with the first byte after invalid SOP.
            uartBuffer.outPos++;
		}
	}

	return false;
} 

//-----------------------------------------------------------------------------
/** \brief	Search for SOP
 *
 *  Search for the Start Of Packet in local UART buffer starting from 
 *  current outPos. The search will continue up to the inPos or the end of
 *  buffer. If SOP found, the outPos will be the position of the SOP.
 *
 *  \return
 *        \b bool. TRUE if SOP found, else FALSE.
 */
//-----------------------------------------------------------------------------
bool ComPortThread::SearchForSop(void)
{

	// search for first SOP
	while((uartBuffer.outPos < (2 * UART_BUFFER_SIZE)) && (uartBuffer.outPos < uartBuffer.inPos)){
		if (uartBuffer.pData[uartBuffer.outPos] == CPT_SOP ){
			return true;
		}
		uartBuffer.outPos++;
		uartBuffer.bufferedCount--;

	}

	return false;
}

//-----------------------------------------------------------------------------
/** \brief	Trim the local data buffer
 * 
 * Unread data is moved to the start of the buffer, and the pointers are 
 * updated accordingly
 */
//-----------------------------------------------------------------------------
void ComPortThread::TrimUartBuffer() 
{
    memmove(uartBuffer.pData, uartBuffer.pData + uartBuffer.outPos, uartBuffer.bufferedCount);
    uartBuffer.inPos = uartBuffer.bufferedCount;
    uartBuffer.outPos = 0;
} // TrimuartBuffer

//-----------------------------------------------------------------------------
/** \brief	Read COM-port
 *  
 *  Read given number of bytes from the active COM-port using current timeouts.
 *  Mutex is used to get exclusive access to the COM-port. This is to avoid
 *  read and write at the same time.
 *
 *  \param[in] pData
 *         Pointer to data buffer.
 *  \param[in] length
 *         Number of bytes to read
 *
 *  \return
 *         \b int. Number of bytes read
 * 
 */
//-----------------------------------------------------------------------------
int ComPortThread::ReadCOMMBytes(void *pData, int length) 
{

	// Variables
	COMSTAT		ComStat;
	DWORD		dwLength;
	DWORD		dwErrorFlags;

	// Try to read length bytes
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
	ReadFile(m_hComDev, pData, ((DWORD) length), &dwLength, NULL);

	return dwLength;
} // end of ReadCOMMBytes

//-----------------------------------------------------------------------------
/** \brief	Write COM-port
 * 
 *  Write given number of bytes to the active COM-port.
 *  Mutex is used to get exclusive access to the COM-port. This is to avoid
 *  read and write at the same time.
 *
 *  \param[in] pData
 *         Pointer to input data buffer.
 *  \param[in] length
 *         Number of bytes to write.
 *
 *  \return
 *         \b int. Number of bytes actually written.
 */
//-----------------------------------------------------------------------------
int ComPortThread::WriteCOMMBytes(void * pData, int length) 
{

	// Variables
	COMSTAT     ComStat;
	BOOL        fWriteStat;
	DWORD       dwBytesWritten;
	DWORD       dwErrorFlags;

	// Try to write [nBytes] bytes
	ClearCommError(m_hComDev, &dwErrorFlags, &ComStat);
	fWriteStat = WriteFile(m_hComDev, pData, length, &dwBytesWritten, NULL);

	return dwBytesWritten;	
} // end of WriteCOMMBytes

//-----------------------------------------------------------------------------
/** \brief	Get packet
 * 
 *  Read packet from packet queue
 */
//-----------------------------------------------------------------------------
int ComPortThread::getPacket(QByteArray **pPacket)
{
    int length;

    //Read the packet from the Queue
    mutex.lock();

    if (!packets.isEmpty())
    {
        length = packets.at(0)->count();
        *pPacket = packets.dequeue();
    }
    else
    {
        length = 0;
    }

    mutex.unlock();

    return length;
}