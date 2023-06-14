#pragma once
class CSerialGPS
{
   CSerialGPS();
   CSerialGPS(CSerialGPS const&);
   void operator=(CSerialGPS const&);

public:
   static CSerialGPS& GetInstance(void);
   ~CSerialGPS();
   bool m_bPortReady;
   HANDLE m_hCom;
   CString m_sComPort;
   DCB m_dcb;
   COMMTIMEOUTS m_CommTimeouts;
   BOOL bWriteRC;
   BOOL bReadRC;
   DWORD iBytesWritten;
   DWORD iBytesRead;
   char sBuffer[1024];
   HANDLE OpenSerialGPS(CString Port);
   bool SetupSerialGPS(HANDLE hCom, DCB &dcb);
   bool SetupTimeouts(HANDLE hCom, COMMTIMEOUTS &CommTimeouts);
};

