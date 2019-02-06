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

#define _WIN32_DCOM
#include <iostream>
using namespace std;
#include <wbemidl.h>
#pragma comment(lib, "WbemUuid.Lib")

#include <comdef.h>

#include <string.h>
#include <tchar.h>

#include <QMessageBox.h>

#include "comport.h"

// Definition of the functions in setupapi.dll
typedef HKEY (__stdcall SETUPDIOPENDEVREGKEY)(HDEVINFO, PSP_DEVINFO_DATA, DWORD, DWORD, DWORD, REGSAM);
typedef BOOL (__stdcall SETUPDIDESTROYDEVICEINFOLIST)(HDEVINFO);
typedef BOOL (__stdcall SETUPDIENUMDEVICEINFO)(HDEVINFO, DWORD, PSP_DEVINFO_DATA);
typedef HDEVINFO (__stdcall SETUPDIGETCLASSDEVS)(LPGUID, LPCTSTR, HWND, DWORD);
typedef BOOL (__stdcall SETUPDIGETDEVICEREGISTRYPROPERTY)(HDEVINFO, PSP_DEVINFO_DATA, DWORD, PDWORD, PBYTE, DWORD, PDWORD);

DEFINE_GUID(GUID_DEVINTERFACE_COMPORT, 0x86E0D1E0L, 0x8089,
   0x11D0, 0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73);

/** \brief Constructor
 *
 */
ComPort::ComPort(QObject *pParent)
{

    //Initialize COM (Required by WMI)
	CoInitialize(NULL);

	HRESULT hr = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT/*PKT_INTEGRITY*/, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, NULL);
	if (FAILED(hr) )
	{
	    CoUninitialize();
	}

    connect(&comPortThread, SIGNAL(packetReady()), pParent, SLOT(handlePacket()));


}

//-----------------------------------------------------------------------------
/** \brief Enumeration of COM port
 *
 * The enumeration of the serial COM port is executed with two methods:
 * 1. With the use of the setupapi.dll library.
 * 2. With the use of the Window Management Instrumentation (WMI)
 */
//-----------------------------------------------------------------------------

int ComPort::Enum(QStringList &portList)
{
    // In order to find all serial ports, two methods will be used.
    // First method is to use the setupapi library.

    // Load the setupapi library
    HINSTANCE hSetupAPI = LoadLibrary(TEXT("SETUPAPI.DLL"));

    if (hSetupAPI == NULL) return CP_ERROR;

    // With the handler of the library we can get pointers to the required functions.
    SETUPDIOPENDEVREGKEY *pSetupDiOpenDevRegKey = reinterpret_cast<SETUPDIOPENDEVREGKEY*>(GetProcAddress(hSetupAPI, "SetupDiOpenDevRegKey"));

#ifdef _UNICODE
    SETUPDIGETCLASSDEVS *pSetupDiGetClassDevs = reinterpret_cast<SETUPDIGETCLASSDEVS*>(GetProcAddress(hSetupAPI, "SetupDiGetClassDevsW"));
    SETUPDIGETDEVICEREGISTRYPROPERTY *pSetupDiGetDeviceRegistryProperty = reinterpret_cast<SETUPDIGETDEVICEREGISTRYPROPERTY*>(GetProcAddress(hSetupAPI, "SetupDiGetDeviceRegistryPropertyW"));
#else
    SETUPDIGETCLASSDEVS *pSetupDiGetClassDevs = reinterpret_cast<SETUPDIGETCLASSDEVS*>(GetProcAddress(hSetupAPI, "SetupDiGetClassDevsA"));
    SETUPDIGETDEVICEREGISTRYPROPERTY *pSetupDiGetDeviceRegistryProperty = reinterpret_cast<SETUPDIGETDEVICEREGISTRYPROPERTY*>(GetProcAddress(hSetupAPI, "SetupDiGetDeviceRegistryPropertyA"));
#endif

    SETUPDIDESTROYDEVICEINFOLIST *pSetupDiDestroyDeviceInfoList = reinterpret_cast<SETUPDIDESTROYDEVICEINFOLIST*>(GetProcAddress(hSetupAPI, "SetupDiDestroyDeviceInfoList"));
    SETUPDIENUMDEVICEINFO *pSetupDiEnumDeviceInfo = reinterpret_cast<SETUPDIENUMDEVICEINFO*>(GetProcAddress(hSetupAPI, "SetupDiEnumDeviceInfo"));

    // Check if all functions are available.
    if ((pSetupDiOpenDevRegKey == NULL) || 
        (pSetupDiGetClassDevs == NULL) ||
        (pSetupDiGetDeviceRegistryProperty == NULL) || 
        (pSetupDiDestroyDeviceInfoList == NULL) || 
        (pSetupDiEnumDeviceInfo == NULL))
    {
        FreeLibrary(hSetupAPI);

        return CP_ERROR;
    }

    //Create a "device information set" which is required to enumerate all the ports
    //SetupDiGetClassDevs returns a device information set that contains all devices of a specified class
    GUID guid = GUID_DEVINTERFACE_COMPORT;
    HDEVINFO hDevInfoSet = pSetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
    if (hDevInfoSet == INVALID_HANDLE_VALUE)
    {
        FreeLibrary(hSetupAPI);

        return CP_ERROR;
    }

    //Walk through the list of device information
    BOOL bMoreItems = TRUE;
    int nIndex = 0;
    SP_DEVINFO_DATA devInfo;
    while (bMoreItems)
    {
        //Enumerate the current device
        // SetupDiEnumDeviceInfo returns a context structure for a device information element of a device information set
        devInfo.cbSize = sizeof(SP_DEVINFO_DATA);
        bMoreItems = pSetupDiEnumDeviceInfo(hDevInfoSet, nIndex, &devInfo);
        if (bMoreItems)
        {
            //Opens a registry storage key for device-specific configuration information and returns a handle to the key
            HKEY hDeviceKey = pSetupDiOpenDevRegKey(hDevInfoSet, &devInfo, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_QUERY_VALUE);
            if (hDeviceKey)
            {
                //Read in the name of the port
                  TCHAR pPortName[255];
				
				DWORD dwSize = sizeof(pPortName);
                DWORD dwType = 0;
                // Retrieves the type and data for the specified value name associated with an open registry key.
                if ((RegQueryValueEx(hDeviceKey, TEXT("PortName"), NULL, &dwType, reinterpret_cast<LPBYTE>(pPortName), &dwSize) == ERROR_SUCCESS) && (dwType == REG_SZ))
                {
                    //If it looks like "COMX" then
                    //add it to the array which will be returned 
                    size_t nLen = _tcslen(pPortName);
                    if (nLen > 3)
                    {
                        if ((_tcsnicmp(pPortName, TEXT("COM"), 3) == 0) && IsNumeric(&pPortName[3], FALSE))
                        {
                            QString strPort;

                            TCHAR pDevDescr[256];
							
                            DWORD dwSize = sizeof(pDevDescr);
                            DWORD dwType = 0;
                            // Retrieves the specified plug and play device descriptor
                            if (!pSetupDiGetDeviceRegistryProperty(hDevInfoSet, &devInfo, SPDRP_DEVICEDESC, &dwType, reinterpret_cast<PBYTE>(pDevDescr), dwSize, &dwSize) && (dwType == REG_SZ))
                            {
                                pDevDescr[0] = _T('\0');
                            }

                            strPort = QString::fromStdWString(pDevDescr);
                            strPort.append(" (" + QString::fromStdWString(pPortName) + ")");
                            
                            portList << strPort;
                        }
                    }
                }

                //Close the key now that we are finished with it
                RegCloseKey(hDeviceKey);
            }

        }

        ++nIndex;
    }

    //Free up the "device information set" now that we are finished with it
    pSetupDiDestroyDeviceInfoList(hDevInfoSet);

    //Unload the setup dll
    FreeLibrary(hSetupAPI);

    // The second method is to use Windows Management Instrumentation(WMI).
    // A standard technologie for accessing management information in an enterprise environment.
    // See http://msdn2.microsoft.com/en-us/library/aa389762(VS.85).aspx

    // Initialize the COM library for use by the calling thread.
    HRESULT hr;
    /*
    hr = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

    if (FAILED(hr))
    {
        return CP_ERROR;
    }

    // Set the general COM security level
    hr = CoInitializeSecurity( NULL,
                               -1,
                               NULL,
                               NULL,
                               RPC_C_AUTHN_LEVEL_DEFAULT,
                               RPC_C_IMP_LEVEL_IMPERSONATE,
                               NULL,
                               EOAC_NONE,
                               NULL);
    if (FAILED(hr))
    {
        CoUninitialize();
        return CP_ERROR;
    }
*/
    // Create a connection to the WMI namespace.

    // Initialize the IWbemLocator
    IWbemLocator* pLoc = 0;
    hr = CoCreateInstance(CLSID_WbemLocator,
                          0, 
                          CLSCTX_INPROC_SERVER, 
                          IID_IWbemLocator, 
                          reinterpret_cast<void**>(&pLoc));

    if (FAILED(hr))
    {
        CoUninitialize();
        return CP_ERROR;
    }

    // Connect to WMI
    IWbemServices *pSvc = 0;
    hr = pLoc->ConnectServer(_bstr_t("\\\\.\\root\\cimv2"), 
                             NULL, 
                             NULL, 
                             NULL, 0, NULL, NULL, &pSvc);

    if (FAILED(hr))
    {
        pLoc->Release();
        CoUninitialize();
        return CP_ERROR;
    }

    //Set the security level
    hr = CoSetProxyBlanket(pSvc, 
                             RPC_C_AUTHN_WINNT,
                             RPC_C_AUTHZ_NONE,
                             NULL,
                             RPC_C_AUTHN_LEVEL_CALL,
                             RPC_C_IMP_LEVEL_IMPERSONATE,
                             NULL,
                             EOAC_NONE);
    if (FAILED(hr))
    {
        pSvc->Release();
        pLoc->Release();
        return CP_ERROR;
    }

    // Get a pointer to the enumerator class for serial ports
    IEnumWbemClassObject* pClsObj = NULL;
    hr = pSvc->CreateInstanceEnum(_bstr_t("Win32_SerialPort"), 
                                  WBEM_FLAG_RETURN_WBEM_COMPLETE, 
                                  NULL, &pClsObj);

    if (FAILED(hr))
    {
        pSvc->Release();
        pLoc->Release();
        return CP_ERROR;
    }


    while (pClsObj)
    {

        ULONG uReturned;
        IWbemClassObject *apObj[256];
        hr = pClsObj->Next(WBEM_INFINITE, 10, apObj, &uReturned);

        // Break out of loop if zero returned
        if (uReturned == 0) break;

        // Check all the objects returned in the array
        for (ULONG n=0; n<uReturned; n++)
        {
            VARIANT vProperty1;
            VariantInit(&vProperty1);
            HRESULT hrGet = apObj[n]->Get(L"DeviceID", 0, &vProperty1, NULL, NULL);
            if (SUCCEEDED(hrGet) && (vProperty1.vt == VT_BSTR) && (wcslen(vProperty1.bstrVal) > 3))
            {
                QString qsPort = QString::fromStdWString(vProperty1.bstrVal);
 
                //If it looks like "COMX" then check if we have to add it to the list
                if (qsPort.contains("COM") && qsPort[3].isDigit()) 
                {
                    // Check if Port already exist in port list
                    bool portExist = FALSE;
                    for (int i = 0; i < portList.size(); i++)
                    {
                        if (portList[i].contains(qsPort))
                        {
                            portExist = TRUE;
                            break;
                        }
                    }

                    //Get the Port descriptor name and add to the list if not existing
                    if (!portExist)
                    {
                        VARIANT vProperty2;
                        VariantInit(&vProperty2);
                        hrGet = apObj[n]->Get(L"Name", 0, &vProperty2, NULL, NULL);
                        if (SUCCEEDED(hrGet) && (vProperty2.vt == VT_BSTR))
                        {
                            QString qsPortDesc;
                            qsPortDesc = QString::fromStdWString(vProperty2.bstrVal);

                            // check if COMx is part of the descriptor
                            if (!qsPortDesc.contains("COM"))
                            {
                                // If not, we prepend the descriptor to the COMx name
                                qsPort.prepend(qsPortDesc + " (");
                                qsPort += ")";
                            }
                            else
                            {
                                qsPort = qsPortDesc;
                            }
                        }

                        // Add port name to the list
                        portList << qsPort;
                      
                        //Free the variant now that we are finished with it
                        VariantClear(&vProperty2);
                    }
                }
            }

            //Free the variant now that we are finished with it
            VariantClear(&vProperty1);

            //Release the interfaces returned
            apObj[n]->Release();
        }
    }

    //Release the class object
    pClsObj->Release();

    //Release the services interface    
    pSvc->Release();

    //Release the locator interface
    //CoUninitialize();
    pLoc->Release();

    return CP_SUCCESS;
}

bool ComPort::IsNumeric(LPCTSTR pszString, BOOL bIgnoreColon)
{
  size_t nLen = _tcslen(pszString);
  if (nLen == 0)
    return FALSE;

  //Assume the best
  BOOL bNumeric = TRUE;

  for (size_t i=0; i<nLen && bNumeric; i++)
  {
    bNumeric = (_istdigit(pszString[i]) != 0);
    if (bIgnoreColon && (pszString[i] == _T(':')))
      bNumeric = TRUE;
  }

  return bNumeric;
}

//-----------------------------------------------------------------------------
/** \brief	Open COM-port
 *
 *		Opens and configures a COM-port.
 *		Note: Some baud rates may be available on one machine and not on others
 *		because of differences in the serial port hardware used on the two machines.
 *
 *  \return
 *         \b bool. TRUE if COM-port openend successfully, else FALSE
 */
//-----------------------------------------------------------------------------
bool ComPort::Open(const QString &qsPortName, unsigned int baudeRate) 
{
    // Create instance of the COM port thread.

    if (comPortThread.Init(qsPortName, baudeRate))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
/** \brief	Read COM-port
 *
 *		Start reading from COM port by calling start of COM port thread.
 *
 *  \return
 *         \b bool. TRUE if COM-port openend successfully, else FALSE
 */
//-----------------------------------------------------------------------------
bool ComPort::Read() 
{
    if (comPortThread.initiated())
    {
        comPortThread.start();
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
/** \brief	Write COM-port
 *
 *  \param[in] pBuffer
 *        Ponter to byte buffer.   
 *  \param[in] length
 *        Length of buffer to write
 */
//-----------------------------------------------------------------------------
int ComPort::write(unsigned char *pBuffer, int length)
{
    return comPortThread.WriteCOMMBytes(pBuffer, length);
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
bool ComPort::Close() 
{
    comPortThread.Stop();
    if (comPortThread.initiated())
    {
        comPortThread.Stop();
        comPortThread.wait();
        return true;
    }
    else
    {
        return false;
    }
}

