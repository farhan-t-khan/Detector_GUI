#include "stdafx.h"
#include "SerialGPS.h"


CSerialGPS::CSerialGPS()
{
   m_sComPort = _T("");
   iBytesWritten = 0;
   iBytesRead = 0;
}


CSerialGPS::~CSerialGPS()
{
}

CSerialGPS& CSerialGPS::GetInstance(void)
{
   
   static CSerialGPS st_SerialGPSOnlyInstance;
   return st_SerialGPSOnlyInstance;
}

HANDLE CSerialGPS::OpenSerialGPS(CString Port)
{
   // TODO: Add your implementation code here.
   m_hCom = CreateFile(Port,
      GENERIC_READ | GENERIC_WRITE,
      0, // exclusive access
      NULL, // no security
      OPEN_EXISTING,
      0, // no overlapped I/O
      0); // null template 
   
   if (m_hCom == INVALID_HANDLE_VALUE)
   {
      CString sError;
      sError.Format(_T("Invalid port %s"), Port.GetString());
      AfxMessageBox(sError);
   }
   return m_hCom;
}


bool CSerialGPS::SetupSerialGPS(HANDLE hCom, DCB &dcb)
{
   // TODO: Add your implementation code here.
   
   m_bPortReady = SetupComm(hCom, 1024, 1024); // set buffer sizes
   COMMTIMEOUTS commtime;
   GetCommTimeouts(hCom, &commtime);
   commtime.WriteTotalTimeoutMultiplier = 1000;
   commtime.WriteTotalTimeoutConstant = 1000;
   SetCommTimeouts(hCom, &commtime);

   m_bPortReady = GetCommState(hCom, &dcb);

   //dcb.BaudRate = 115200;
   //dcb.ByteSize = 8;
   //dcb.Parity = NOPARITY;
   //dcb.StopBits = ONESTOPBIT;
   //dcb.fAbortOnError = TRUE;

   dcb.BaudRate = CBR_9600;
   dcb.ByteSize = 8;
   dcb.Parity = NOPARITY;
   dcb.StopBits = ONESTOPBIT;
   dcb.fRtsControl = RTS_CONTROL_DISABLE;

   m_bPortReady = SetCommState(hCom, &dcb); 
   return m_bPortReady;
}


bool CSerialGPS::SetupTimeouts(HANDLE hCom, COMMTIMEOUTS &CommTimeouts)
{
   // TODO: Add your implementation code here.
   m_bPortReady = GetCommTimeouts(hCom, &CommTimeouts);

   CommTimeouts.ReadIntervalTimeout = 1;
   CommTimeouts.ReadTotalTimeoutConstant = 1;
   CommTimeouts.ReadTotalTimeoutMultiplier = 1;
   CommTimeouts.WriteTotalTimeoutConstant = 1;
   CommTimeouts.WriteTotalTimeoutMultiplier = 1;

   m_bPortReady = SetCommTimeouts(hCom, &CommTimeouts);
   return m_bPortReady;
}



