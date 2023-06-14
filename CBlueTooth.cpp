#include "stdafx.h"
#include "CBlueTooth.h"
#include "Detector_guiDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include "mmsystem.h"
#include "windowsx.h"
#include "string.h"
#include <ctime>
#include "SerialGPS.h"
#include "NMEAParser.h"
#include "GPSInfo.h"
#include <string.h>
#include <stdlib.h>
#include "Profiler.h"
#include "CDlgSelectBT.h"
#include "Settings.h"
#include "CDlgSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


TCHAR g_szLabels[][64] = { _T("CH4_Sample"), _T("Tec_Temp"), _T("Tec_Current"), _T("Tec_Error"),
_T("avg_min_2F"), _T("avg_max_2F"), _T("avg_min_1F"), _T("avg_max_1F"),
_T("Battery_Voltage"), _T("Laser_Temperature"), _T("Receiver_temperature"), _T("Laser_Humidity"),
_T("Receiver_Humidity"), _T("avg_minlocation1"), _T("avg_maxlocation1"), _T("avg_minlocation2"),
_T("avg_maxlocation2"), _T("Laser_current") };


CBlueTooth::CBlueTooth()
{
}


CBlueTooth::~CBlueTooth()
{
}

#include "wchar.h"
#include "stdio.h"

#include <Winsock2.h>
#include <wtypes.h>
#include <ws2bth.h>
#pragma comment(lib, "Ws2_32.lib")

unsigned char TableCrc8[256] = {
0x0, 0x5E, 0xBC, 0xE2, 0x61, 0x3F, 0xDD, 0x83, 0xC2, 0x9C, 0x7E, 0x20, 0xA3, 0xFD, 0x1F, 0x41,
0x9D, 0xC3, 0x21, 0x7F, 0xFC, 0xA2, 0x40, 0x1E, 0x5F, 0x1, 0xE3, 0xBD, 0x3E, 0x60, 0x82, 0xDC,
0x23, 0x7D, 0x9F, 0xC1, 0x42, 0x1C, 0xFE, 0xA0, 0xE1, 0xBF, 0x5D, 0x3, 0x80, 0xDE, 0x3C, 0x62,
0xBE, 0xE0, 0x2, 0x5C, 0xDF, 0x81, 0x63, 0x3D, 0x7C, 0x22, 0xC0, 0x9E, 0x1D, 0x43, 0xA1, 0xFF,
0x46, 0x18, 0xFA, 0xA4, 0x27, 0x79, 0x9B, 0xC5, 0x84, 0xDA, 0x38, 0x66, 0xE5, 0xBB, 0x59, 0x7,
0xDB, 0x85, 0x67, 0x39, 0xBA, 0xE4, 0x6, 0x58, 0x19, 0x47, 0xA5, 0xFB, 0x78, 0x26, 0xC4, 0x9A,
0x65, 0x3B, 0xD9, 0x87, 0x4, 0x5A, 0xB8, 0xE6, 0xA7, 0xF9, 0x1B, 0x45, 0xC6, 0x98, 0x7A, 0x24,
0xF8, 0xA6, 0x44, 0x1A, 0x99, 0xC7, 0x25, 0x7B, 0x3A, 0x64, 0x86, 0xD8, 0x5B, 0x5, 0xE7, 0xB9,
0x8C, 0xD2, 0x30, 0x6E, 0xED, 0xB3, 0x51, 0xF, 0x4E, 0x10, 0xF2, 0xAC, 0x2F, 0x71, 0x93, 0xCD,
0x11, 0x4F, 0xAD, 0xF3, 0x70, 0x2E, 0xCC, 0x92, 0xD3, 0x8D, 0x6F, 0x31, 0xB2, 0xEC, 0xE, 0x50,
0xAF, 0xF1, 0x13, 0x4D, 0xCE, 0x90, 0x72, 0x2C, 0x6D, 0x33, 0xD1, 0x8F, 0xC, 0x52, 0xB0, 0xEE,
0x32, 0x6C, 0x8E, 0xD0, 0x53, 0xD, 0xEF, 0xB1, 0xF0, 0xAE, 0x4C, 0x12, 0x91, 0xCF, 0x2D, 0x73,
0xCA, 0x94, 0x76, 0x28, 0xAB, 0xF5, 0x17, 0x49, 0x8, 0x56, 0xB4, 0xEA, 0x69, 0x37, 0xD5, 0x8B,
0x57, 0x9, 0xEB, 0xB5, 0x36, 0x68, 0x8A, 0xD4, 0x95, 0xCB, 0x29, 0x77, 0xF4, 0xAA, 0x48, 0x16,
0xE9, 0xB7, 0x55, 0xB, 0x88, 0xD6, 0x34, 0x6A, 0x2B, 0x75, 0x97, 0xC9, 0x4A, 0x14, 0xF6, 0xA8,
0x74, 0x2A, 0xC8, 0x96, 0x15, 0x4B, 0xA9, 0xF7, 0xB6, 0xE8, 0xA, 0x54, 0xD7, 0x89, 0x6B, 0x35,
};






//
//	This is what the structure looks like but we are sending this out
//	as a stream of bytes since the structure alignment and padding is 
//	different between the micro and tablet
//

#define txsize  50
#define rxsize	txsize - 6
unsigned char	rx1[rxsize];


/*
rx1[0]	=	0xAA;
rx1[1]	=	0xBB;

rx1[2]	=	Type of packet

rx1[3]	=	CH4 Sample
rx1[4]	=	CH4 Sample

rx1[5]	=	Tec Temp
rx1[6]	=	Tec Temp

rx1[7]	=	Tec Current
rx1[8]	=	Tec Current

rx1[9]	=	Tec Error
rx1[10]	=	Tec Error

rx1[11] =     avg_min_2F
rx1[12] =     avg_min_2F
rx1[13] =     avg_max_2F
rx1[14] =     avg_max_2F
rx1[15] =     avg_min_1F
rx1[16] =     avg_min_1F
rx1[17] =     avg_max_1F
rx1[18] =     avg_max_1F

rx1[19] = 	Battery Voltage
rx1[20] = 	Battery Voltage

rx1[21] = 	Laser Temperature
rx1[22] = 	Laser Temperature

rx1[23] = 	Receiver Temperature
rx1[24] = 	Receiver Temperature

rx1[25] = 	Laser Humidity
rx1[26] = 	Laser Humidity

rx1[27] = 	Receiver Humidity
rx1[28] = 	Receiver Humidity

rx1[29] =     avg_minlocation1
rx1[30] =     avg_minlocation1
rx1[31] =     avg_maxlocation1
rx1[32] =     avg_maxlocation1
rx1[33] =     avg_minlocation2
rx1[34] =     avg_minlocation2
rx1[35] =     avg_maxlocation2
rx1[36] =     avg_maxlocation2

rx1[37]	=    Laser_Current
rx1[38]	=    Laser_Current
rx1[39]	=
rx1[40]	=
rx1[41]	=
rx1[42]	=
rx1[43]	=	CRC8

*/


struct		BlueToothList *BT[BTArraySize] = { 0 };
//int		Mode = 0;
int		checksum_errors = 0;
unsigned 	char Crc8;
//int		first_time;
//int		ppmalarmlevel;
int		alarm_active;
int		alarm_muted;

//int		graph_left;
//int		graph_top;
//int		graph_right;
//int		graph_bottom;
//int		graph_width;
//int		graph_height;

//int		graph_ppm;
//int		graph_x;
//int		graph_y;
//int		last_graph_x;
//int		last_graph_y;

//int 		saved_mode;
int		maxvalue;
int		minvalue;
int		maxmincount;

clock_t start, now, GpsStart, GpsNow;

CCriticalSection CBlueTooth::m_st_CS;
CBlueTooth& CBlueTooth::GetInstance(void)
{
   CSingleLock lock(&m_st_CS, TRUE);
   static CBlueTooth st_BlueToothOnlyInstance;
   return st_BlueToothOnlyInstance;
}

void CBlueTooth::CallbackWrapper(CObject* pObj, const CEnumDevInfo *pInfo)
{
   TRACE(_T("%s %d\n"), pInfo->m_strName.GetString(), (DWORD)pInfo->m_nPortNum & DATA_PORT_MASK);
   GetInstance().ProcessComport(pInfo);
}

void CBlueTooth::ProcessComport(const CEnumDevInfo *pInfo)
{
   int x = (DWORD)pInfo->m_nPortNum & DATA_PORT_MASK;
   int y, z;
   m_pMainGui->wPrintf(_T("Scanning port %03d"), x);
   wchar_t port[20];
   unsigned char buffer[2000];
   unsigned char response[2000];
   //unsigned char buffer1[1024];
   DCB dcb;
   unsigned long 	er = 0;
   COMSTAT 	lp;
   unsigned long BytesWritten = 0;
   unsigned long BytesReturned = 0;
   unsigned long Success = 0;
   _stprintf(port, _T("\\\\.\\COM%d"), x);
   HANDLE 	temp_port = 0;

   COMMTIMEOUTS commtime;
   temp_port = CreateFile(&port[0], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
   if (temp_port != INVALID_HANDLE_VALUE)
   {
      GetCommTimeouts(temp_port, &commtime);
      commtime.WriteTotalTimeoutMultiplier = 300;
      commtime.WriteTotalTimeoutConstant = 300;
      SetCommTimeouts(temp_port, &commtime);

      dcb.DCBlength = sizeof(DCB);
      GetCommState(temp_port, &dcb);
      dcb.BaudRate = CBR_115200;
      dcb.ByteSize = 8;
      dcb.Parity = NOPARITY;
      dcb.StopBits = ONESTOPBIT;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      SetCommState(temp_port, &dcb);
      //
      //	Flush any left over bytes in the air
      //
      Sleep(500);
      Success = ClearCommError(temp_port, &er, &lp);
      ASSERT(lp.cbInQue < ARRAYSIZE(buffer));
      ReadFile(temp_port, &buffer[0], lp.cbInQue, &BytesReturned, NULL);
      //
      //	Issue a cmd_system_get_bt_address and see if the com port responds properly
      //
      buffer[0] = 0x20;
      buffer[1] = 0x00;
      buffer[2] = 0x01;
      buffer[3] = 0x03;
      Success = WriteFile(temp_port, &buffer[0], 4, &BytesWritten, NULL);
      Sleep(500);
      Success = ClearCommError(temp_port, &er, &lp);

      if (lp.cbInQue > 0)
      {
         ASSERT(lp.cbInQue < ARRAYSIZE(response));
         Success = ReadFile(temp_port, &response[0], lp.cbInQue, &BytesReturned, NULL);
         if (response[0] == 0x20 && response[1] == 0x06 && response[2] == 0x01 && response[3] == 0x03)
         {
            bluetooth_port = temp_port;
            m_blu = x;
            //sprintf((char *const)&buffer1[0], "%d", x);
            //for (y = 0; y < 20; ++y)
            //   buffer[y] = 0;
            //BT_PORT->SetSel(0, -1);
            //BT_PORT->Clear();
            buffer[0] = response[0];
            buffer[2] = response[1];
            //BT_PORT->ReplaceSel((LPCTSTR)buffer);
         }
         else
         {
            CloseHandle(temp_port);
         }
      }
      else
      {
         CloseHandle(temp_port);
      }
   }

   temp_port = CreateFile(&port[0], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
   if (temp_port != INVALID_HANDLE_VALUE)
   {
      GetCommTimeouts(temp_port, &commtime);
      commtime.WriteTotalTimeoutMultiplier = 300;
      commtime.WriteTotalTimeoutConstant = 300;
      SetCommTimeouts(temp_port, &commtime);

      dcb.DCBlength = sizeof(DCB);
      GetCommState(temp_port, &dcb);
      dcb.BaudRate = CBR_9600;
      dcb.ByteSize = 8;
      dcb.Parity = NOPARITY;
      dcb.StopBits = ONESTOPBIT;
      dcb.fRtsControl = RTS_CONTROL_DISABLE;
      SetCommState(temp_port, &dcb);
      Sleep(1200);
      Success = ClearCommError(temp_port, &er, &lp);
      if (lp.cbInQue > 0)
      {
         ASSERT(lp.cbInQue < ARRAYSIZE(response));
         Success = ReadFile(temp_port, &response[0], lp.cbInQue, &BytesReturned, NULL);
         for (z = 0; z < BytesReturned; ++z)
         {
            if (response[z] == '$' && response[z + 1] == 'G' && response[z + 2] == 'P' && response[z + 3] == 'G' && response[z + 4] == 'S' && response[z + 5] == 'V' ||
               (response[z] == '$' && response[z + 1] == 'G' && response[z + 2] == 'N' && response[z + 3] == 'G' && response[z + 4] == 'G' && response[z + 5] == 'A'))
            {
               gps_port = temp_port;
               m_gps = x;
               for (y = 0; y < 20; ++y)
                  buffer[y] = 0;
               //sprintf((char *const)&buffer1[0], "%d", x);
               //GPS_PORT->SetSel(0, -1);
               //GPS_PORT->Clear();
               buffer[0] = response[0];
               buffer[2] = response[1];
               //GPS_PORT->ReplaceSel((LPCTSTR)buffer);
               break;
            }
         }
      }
      CloseHandle(temp_port);
      gps_port = 0;
   }

}


int CBlueTooth::ScanForBTandGPS()
{
   FILE *stream;
   //stevebooth

   CEnumDevices Enum;
   Enum.EnumSerialPorts(nullptr, CallbackWrapper, TRUE);
   if ((stream = fopen("c:\\BascomTurnerMLDS\\ports.txt", "w")) != NULL)
   {
      char buffer[64] = { 0 };
      sprintf((char *const)&buffer[0], "BLU=%d", m_blu);
      buffer[strlen((const char *)&buffer)] = 0x0a;
      fwrite(&buffer[0], 1, strlen((const char *)&buffer[0]), stream);

      sprintf((char *const)&buffer[0], "GPS=%d", m_gps);
      buffer[strlen((const char *)&buffer)] = 0x0a;
      fwrite(&buffer[0], 1, strlen((const char *)&buffer[0]), stream);
      fclose(stream);
      if (m_blu == 0 || m_gps == 0)
      {
         return 1;
      }
   }
   m_pMainGui->wPrintf(_T("Done BT_GPS Scan"));
   return 0;
}

int CBlueTooth::scan_bluetooth_devices()
{
   int x, y, z, a, c, sent, found, stat, retry;
   int	numbt = 0;
   unsigned char* pbuffer = 0;
   unsigned char buff1[300];
   unsigned char output[50];
   unsigned char A2[100];
   unsigned long 	er;
   COMSTAT 	lp;
   COMMTIMEOUTS *commtime;
   unsigned long BytesWritten = 0;
   unsigned long BytesReturned = 0;
   unsigned long Success = 0;
   DWORD dwError;
   DCB dcb;
   unsigned char bt_array[600];
   FILE *stream;


   Read_Lasers_Txt();
   m_dlgSelectBT.DoModal();
   if (!m_dlgSelectBT.m_bScan && !m_dlgSelectBT.m_sBTName.IsEmpty())
   {
      m_pMainGui->wPrintf(_T("Done No Scan"));
      return 1;
   }
   int nBufferSize = 300;
   pbuffer = new unsigned char[nBufferSize];
   m_pMainGui->wPrintf(_T("Scan for lasers"));

   //
   //	Issue a discovery command for 30 seconds
   //

discover_loop:

   pbuffer[0] = 0x20;
   pbuffer[1] = 0x05;
   pbuffer[2] = 0x02;
   pbuffer[3] = 0x00;
   pbuffer[4] = 0x17;	// This number * 1.28. 30 seconds in this case
   pbuffer[5] = 0x00;
   pbuffer[6] = 0x00;
   pbuffer[7] = 0x00;
   pbuffer[8] = 0x00;
   Success = WriteFile(bluetooth_port, pbuffer, 9, &BytesWritten, NULL);
   stat = 0;
   for (retry = 0; retry < 5; ++retry)
   {
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue >= 6)
      {
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
         if (buff1[0] = 0x20 && buff1[1] == 0x02 && buff1[2] == 0x02 && buff1[3] == 0x00 && buff1[4] == 0x00 && buff1[5] == 0x00)
         {
            m_pMainGui->wPrintf(_T("Discover cmd OK"));
            stat = 1;
            break;
         }
      }
      Sleep(1000);
   }
   if (stat == 0)
   {
      m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
         buff1[0], buff1[1], buff1[2], buff1[3], buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9]);
      m_pMainGui->wPrintf(_T("status=%d bytes=%d"), Success, BytesReturned);
      m_pMainGui->wPrintf(_T("Discover cmd failed")); 
      delete[] pbuffer;
      return 0;
   }
   flush();


   m_pMainGui->wPrintf(_T("for Discovery"));
   m_pMainGui->wPrintf(_T("Wait 32 sec"));
   Sleep(32000);		// Wait 32 seconds till done

   Success = ClearCommError(bluetooth_port, &er, &lp);
   if (lp.cbInQue > 0)
   {
      if (lp.cbInQue >= nBufferSize)
      {
         delete[] pbuffer;
         nBufferSize = lp.cbInQue;
         pbuffer = new unsigned char[nBufferSize];
      }
      Success = ReadFile(bluetooth_port, pbuffer, lp.cbInQue, &BytesReturned, NULL);
      m_pMainGui->wPrintf(_T("status=%d bytes=%d"), Success, BytesReturned);
   }

   for (x = 0; x < 600; ++x)
      bt_array[x] = 0;

   y = 0;
   x = 0;
   numbt = 0;
   for (;;)
   {
      if (x >= lp.cbInQue)
         break;

      if (pbuffer[x] != 0xA0 || pbuffer[x + 1] != 0x0E)
      {
         x = x + (pbuffer[1] + 4);
         continue;
      }


      if (pbuffer[x] == 0xA0)
      {
         m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x"), pbuffer[x + 4], pbuffer[x + 5], pbuffer[x + 6], pbuffer[x + 7], pbuffer[x + 8], pbuffer[x + 9]);

         numbt = numbt + 1;

         bt_array[y + 0] = pbuffer[x + 4];
         bt_array[y + 1] = pbuffer[x + 5];
         bt_array[y + 2] = pbuffer[x + 6];
         bt_array[y + 3] = pbuffer[x + 7];
         bt_array[y + 4] = pbuffer[x + 8];
         bt_array[y + 5] = pbuffer[x + 9];

         x = x + (pbuffer[x + 1] + 4);
         y = y + 6;
      }
   }


   m_pMainGui->wPrintf(_T("# BT devices %02d"), numbt);
   //
   // Now run through the list of BT addresses and get their ascii name
   //
   found = 0;
   y = 0;
   for (x = 0; x < numbt; ++x)
   {
      for (z = 0; z < 100; ++z)
         buff1[z] = 0x20;
      //
      //	cmd_bt_gap_get_remote_name 3.2.1.4
      //
      buff1[0] = 0x20;
      buff1[1] = 0x06;
      buff1[2] = 0x02;
      buff1[3] = 0x02;

      buff1[4] = bt_array[y + 0];
      buff1[5] = bt_array[y + 1];
      buff1[6] = bt_array[y + 2];
      buff1[7] = bt_array[y + 3];
      buff1[8] = bt_array[y + 4];
      buff1[9] = bt_array[y + 5];

      m_pMainGui->wPrintf(_T("Asking %02x %02x %02x %02x %02x %02x"),
         buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9]);
      for (int b = 0; b < 50; ++b)
         output[b] = 0;

      //		sprintf((char *const)&output[0],"%02x%02x%02x%02x%02x%02x",buff1[4],buff1[5],buff1[6],buff1[7],buff1[8],buff1[9]);
      y = y + 6;
      Success = WriteFile(bluetooth_port, &buff1[0], 10, &BytesWritten, NULL);






      m_pMainGui->wPrintf(_T("Wait 10 sec"));
      Sleep(10000);



      //stevebooth





      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue > 0)
      {
         for (z = 0; z < 100; ++z)
            buff1[z] = 0x20;

         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);

         //			wPrintf("Bytes--->%02d %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",lp.cbInQue,
         //				buff1[0],buff1[1],buff1[2],buff1[3],buff1[4],buff1[5],buff1[6],buff1[7],buff1[8],buff1[9],
         //				buff1[10],buff1[11],buff1[12],buff1[13],buff1[14],buff1[15],buff1[16],buff1[17],buff1[18],buff1[19],
         //				buff1[20],buff1[21],buff1[22],buff1[23],buff1[24],buff1[25],buff1[26],buff1[27],buff1[28],buff1[29],
         //				buff1[30],buff1[31],buff1[32],buff1[33],buff1[34],buff1[35],buff1[36],buff1[37],buff1[38],buff1[39],
         //				buff1[40],buff1[41],buff1[42],buff1[43],buff1[44],buff1[45],buff1[46],buff1[47],buff1[48],buff1[49]);


         for (a = 0; a < lp.cbInQue; ++a)
         {
            if (buff1[a] == 'M' && buff1[a + 1] == 'L' && buff1[a + 2] == 'D' && buff1[a + 3] == 'S')
            {
               m_pMainGui->wPrintf(_T("Found a MLDS device"));
               found = 1;

               //					wPrintf("Bytes--->%02d %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",lp.cbInQue,
               //				buff1[0],buff1[1],buff1[2],buff1[3],buff1[4],buff1[5],buff1[6],buff1[7],buff1[8],buff1[9],
               //				buff1[10],buff1[11],buff1[12],buff1[13],buff1[14],buff1[15],buff1[16],buff1[17],buff1[18],buff1[19],
               //				buff1[20],buff1[21],buff1[22],buff1[23],buff1[24],buff1[25],buff1[26],buff1[27],buff1[28],buff1[29],
               //				buff1[30],buff1[31],buff1[32],buff1[33],buff1[34],buff1[35],buff1[36],buff1[37],buff1[38],buff1[39],
               //				buff1[40],buff1[41],buff1[42],buff1[43],buff1[44],buff1[45],buff1[46],buff1[47],buff1[48],buff1[49]);


               //
               //	Now lets walk backwards looking at the protocol until we see 0xa0 xx 0x02 0x02 0x00 0x00
               //	after that starts the MAC address then the name that contains the MLDS which could be in 
               //	any part of the 16 character name of device
               //
               //
               //			variable "a" is on the 'M' at the moment
               //
               for (;;)
               {
                  if (buff1[a] == 0xa0 && buff1[a + 2] == 0x02 && buff1[a + 3] == 0x02 && buff1[a + 4] == 0x00 && buff1[a + 5] == 0x00)
                  {
                     a = a + 6;
                     break;
                  }
                  a = a - 1;
               }

               output[0] = buff1[a + 7];
               output[1] = buff1[a + 8];
               output[2] = buff1[a + 9];
               output[3] = buff1[a + 10];
               output[4] = buff1[a + 11];
               output[5] = buff1[a + 12];
               output[6] = buff1[a + 13];
               output[7] = buff1[a + 14];
               output[8] = buff1[a + 15];
               output[9] = buff1[a + 16];
               output[10] = buff1[a + 17];
               output[11] = buff1[a + 18];
               output[12] = buff1[a + 19];
               output[13] = buff1[a + 20];
               output[14] = buff1[a + 21];
               output[15] = buff1[a + 22];

               sprintf((char *const)&output[16], "%02x%02x%02x%02x%02x%02x", buff1[a], buff1[a + 1], buff1[a + 2], buff1[a + 3], buff1[a + 4], buff1[a + 5]);

               CA2T A2((char*)output);
               CString sA2 = A2;
               
               m_pMainGui->wPrintf(_T("%s"), sA2);
               m_pMainGui->wPrintf(_T("String is "));
               bool bFound = false;
               for (size_t nIndex = 0; nIndex < m_dlgSelectBT.m_vBT.size(); nIndex++)
               {
                  m_dlgSelectBT.m_vBT[nIndex].Remove('\n');
                  if (m_dlgSelectBT.m_vBT[nIndex] == sA2)
                  {
                     bFound = true;
                  }
               }
               if (!bFound)
               {
                  m_dlgSelectBT.m_vBT.push_back(sA2);
                  output[28] = 0x0a;
                  if ((stream = fopen("c:\\BascomTurnerMLDS\\lasers.txt", "a")) != NULL)
                  {
                     fwrite(&output[0], 1, 29, stream);
                     fclose(stream);
                  }
               }




               //for (a = 0; a < 100; ++a)
               //   A2[a] = 0;

               //for (a = 0; a < 28; ++a)
               //   A2[a * 2] = output[a];

               //Serial_Number->AddString((LPCTSTR)A2);


               break;
            }
         }
      }
   }


   m_dlgSelectBT.DoModal();

   if (m_dlgSelectBT.m_bScan)
   {
      goto discover_loop;
   }

   m_pMainGui->wPrintf(_T("Done"));
   delete[] pbuffer;
   return 1;
}

void CBlueTooth::Read_Lasers_Txt()
{
   FILE *str;
   int numbytes, x, y, z, i, a;
   unsigned char buffer[MAX_PATH];
   unsigned long 	er;
   COMSTAT 	lp;
   COMMTIMEOUTS *commtime;
   HANDLE 	temp_port = 0;
   DCB dcb;
   wchar_t port[20];
   unsigned long Success;
   unsigned long BytesWritten;
   unsigned long BytesReturned;

   m_dlgSelectBT.m_vBT.clear();
   if ((str = fopen("c:\\BascomTurnerMLDS\\lasers.txt", "rb")) != NULL)
   {
      for (;;)
      {
         for (x = 0; x < ARRAYSIZE(buffer); ++x)
            buffer[x] = 0;

         for (numbytes = 0; numbytes < ARRAYSIZE(buffer);)
         {
            i = fread(&buffer[numbytes], 1, 1, str);
            if (i <= 0 && buffer[0] == 0)
            {
               fclose(str);
               return;
            }
            else if (i <= 0)
            {
               break;
            }
            if (buffer[numbytes] == '\r')
            {
               buffer[numbytes] = 0;
               if (numbytes > 0 && buffer[numbytes-1] == '\n')
               {
                  buffer[numbytes - 1] = 0;
               }
               break;
            }
            numbytes = numbytes + 1;
         }
         if (numbytes > 0 && buffer[numbytes-1] == '\n')
         {
            buffer[numbytes - 1] = 0;
         }
         CA2T A2((char*)buffer);
         CString sA2 = A2;
         //sA2.Remove(_T('\n'));
         if (!sA2.IsEmpty())
         {
            m_dlgSelectBT.m_vBT.push_back(sA2);
         }
         //Serial_Number->AddString((LPCTSTR)A2);
      }
   }
}

void CBlueTooth::Close_Bluetooth_port()
{
   unsigned char buffer[25];
   unsigned char buff1[1000];
   unsigned long 	er;
   COMSTAT 	lp;
   unsigned long Success;
   unsigned long BytesWritten;
   unsigned long BytesReturned;
   if (bluetooth_port)
   {
      
      //
      //		Turn off BT LED
      //
      buffer[0] = 0x20;
      buffer[1] = 0x05;
      buffer[2] = 0x0c;
      buffer[3] = 0x02;
      buffer[4] = 0x01;
      buffer[5] = 0x00;
      buffer[6] = 0x04;
      buffer[7] = 0x00;
      buffer[8] = 0x00;
      Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
      Sleep(100);
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue > 0)
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
      
      flush();
      Sleep(500);
      CloseHandle(bluetooth_port);
      bluetooth_port = 0;
   }
}

void CBlueTooth::Reopen_Bluetooth_port()
{
   Close_Bluetooth_port();
   FILE *str;
   int numbytes, x, y, z, i;
   int blue, gps;
   unsigned char buffer[25];
   unsigned char buffer1[25];
   unsigned char buff1[1000];
   unsigned long 	er;
   COMSTAT 	lp;
   COMMTIMEOUTS commtime = { 0 };
   HANDLE 	temp_port = 0;
   DCB dcb;
   wchar_t port[20];
   unsigned long Success;
   unsigned long BytesWritten;
   unsigned long BytesReturned;

   _stprintf(port, _T("\\\\.\\COM%d"), m_blu);

   temp_port = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, NULL, NULL);
   if (temp_port != INVALID_HANDLE_VALUE)
   {
      bluetooth_port = temp_port;
      GetCommTimeouts(temp_port, &commtime);
      commtime.WriteTotalTimeoutMultiplier = 1000;
      commtime.WriteTotalTimeoutConstant = 1000;


      commtime.ReadTotalTimeoutConstant = 1000;


      SetCommTimeouts(temp_port, &commtime);

      dcb.DCBlength = sizeof(DCB);
      GetCommState(temp_port, &dcb);
      dcb.BaudRate = CBR_115200;
      dcb.ByteSize = 8;
      dcb.Parity = NOPARITY;
      dcb.StopBits = ONESTOPBIT;
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
      SetCommState(temp_port, &dcb);
   }

   if (bluetooth_port != 0)
   {
      //
      //		Make BT LED output
      //
      buffer[0] = 0x20;
      buffer[1] = 0x05;
      buffer[2] = 0x0c;
      buffer[3] = 0x01;
      buffer[4] = 0x01;
      buffer[5] = 0x0a;
      buffer[6] = 0x00;
      buffer[7] = 0x01;
      buffer[8] = 0x00;
      Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
      Sleep(100);
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue > 0)
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);

      flush();
      Sleep(100);
      //
      //		Turn off BT LED
      //
      buffer[0] = 0x20;
      buffer[1] = 0x05;
      buffer[2] = 0x0c;
      buffer[3] = 0x02;
      buffer[4] = 0x01;
      buffer[5] = 0x00;
      buffer[6] = 0x04;
      buffer[7] = 0x00;
      buffer[8] = 0x00;
      Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
      Sleep(100);
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue > 0)
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
      flush();
   }
}

int CBlueTooth::Connect_To_Bluetooth_Device()
{
   int x, y, z, sent, found, nbytes, retry, stat;
   unsigned char wb[25];
   int	numbt = 0;
   unsigned char buffer[300];
   unsigned char buff1[300];
   unsigned long 	er;
   COMSTAT 	lp;
   COMMTIMEOUTS *commtime;
   unsigned long BytesWritten;
   unsigned long BytesReturned;
   unsigned long Success;
   DWORD dwError;
   DCB dcb;
   unsigned char bt_array[600];


try_again:
   flush();
   m_pMainGui->wPrintf(_T("** Init Bluetooth **"));

   //
   //	Reset 3.17.1.5
   //
   buffer[0] = 0x20;
   buffer[1] = 0x01;
   buffer[2] = 0x01;
   buffer[3] = 0x01;
   buffer[4] = 0x00;
   Success = WriteFile(bluetooth_port, &buffer[0], 5, &BytesWritten, NULL);
   stat = 0;
   //
   //	We need 59 bytes. which contain multiple packets
   //

   for (x = 0; x < 300; ++x)
      buff1[x] = 0;
   Sleep(1000);

   Success = ClearCommError(bluetooth_port, &er, &lp);
   if (lp.cbInQue != 59)
   {
      m_pMainGui->wPrintf(_T("Got %d bytes"), lp.cbInQue);
      m_pMainGui->wPrintf(_T("Did not receive 59 bytes."));
      flush();
      goto try_again;
   }

   Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
   //
   //		evt_system_boot 3.17.2.1
   //		evt_system_initialized 3.17.2.2
   //
   if (buff1[49] == 0xa0 && buff1[50] == 0x06 && buff1[51] == 0x01 && buff1[52] == 0x01)
   {
      m_pMainGui->wPrintf(_T("%02x-%02x-%02x-%02x-%02x-%02x"),
         buff1[53], buff1[54], buff1[55], buff1[56], buff1[57], buff1[58]);
      m_pMainGui->wPrintf(_T("System initialized"));
   }
   else
   {
      m_pMainGui->wPrintf(_T("Reset failed"));
      flush();
      goto try_again;
   }
   //
   //	Set Gap mode 3.2.1.10
   //
   buffer[0] = 0x20;
   buffer[1] = 0x03;
   buffer[2] = 0x02;
   buffer[3] = 0x03;
   buffer[4] = 0x01;
   buffer[5] = 0x01;
   buffer[6] = 0x00;
   Success = WriteFile(bluetooth_port, &buffer[0], 7, &BytesWritten, NULL);
   stat = 0;
   for (retry = 0; retry < 5; ++retry)
   {
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue >= 6)
      {
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
         if (buff1[0] = 0x20 && buff1[1] == 0x02 && buff1[2] == 0x02 && buff1[3] == 0x03 && buff1[4] == 0x00 && buff1[5] == 0x00)
         {
            m_pMainGui->wPrintf(_T("GAP mode set"));
            stat = 1;
            break;
         }
      }
      Sleep(500);
   }
   if (stat == 0)
   {
      m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
         buff1[0], buff1[1], buff1[2], buff1[3], buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9]);
      m_pMainGui->wPrintf(_T("status=%d bytes=%d"), Success, BytesReturned);
      m_pMainGui->wPrintf(_T("GAP failed"));

      flush();
      goto try_again;
   }
   flush();
   //
   //	Set Bondable mode 3.16.1.11
   //
   buffer[0] = 0x20;
   buffer[1] = 0x01;
   buffer[2] = 0x0f;
   buffer[3] = 0x00;
   buffer[4] = 0x01;
   Success = WriteFile(bluetooth_port, &buffer[0], 5, &BytesWritten, NULL);
   stat = 0;
   for (retry = 0; retry < 5; ++retry)
   {
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue >= 6)
      {
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
         if (buff1[0] = 0x20 && buff1[1] == 0x02 && buff1[2] == 0x0f && buff1[3] == 0x00 && buff1[4] == 0x00 && buff1[5] == 0x00)
         {
            m_pMainGui->wPrintf(_T("Bondable mode set"));
            stat = 1;
            break;
         }
      }
      Sleep(500);
   }
   if (stat == 0)
   {
      
      m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
         buff1[0], buff1[1], buff1[2], buff1[3], buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9]);
      m_pMainGui->wPrintf(_T("status=%d bytes=%d"), Success, BytesReturned);
      m_pMainGui->wPrintf(_T("Set Bondable failed"));

      flush();
      goto try_again;
   }
   flush();
   //
   //	Start Server 3.4.1.5
   //
   buffer[0] = 0x20;
   buffer[1] = 0x02;
   buffer[2] = 0x04;
   buffer[3] = 0x01;
   buffer[4] = 0x02;
   buffer[5] = 0x00;
   Success = WriteFile(bluetooth_port, &buffer[0], 6, &BytesWritten, NULL);
   stat = 0;
   for (retry = 0; retry < 5; ++retry)
   {
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue >= 6)
      {
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
         if (buff1[0] = 0x20 && buff1[1] == 0x02 && buff1[2] == 0x04 && buff1[3] == 0x01 && buff1[4] == 0x00 && buff1[5] == 0x00)
         {
            m_pMainGui->wPrintf(_T("Server started"));
            stat = 1;
            break;
         }
      }
      Sleep(500);
   }
   if (stat == 0)
   {
      m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
         buff1[0], buff1[1], buff1[2], buff1[3], buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9]);
      
      m_pMainGui->wPrintf(_T("status=%d bytes=%d"), Success, BytesReturned); 
      m_pMainGui->wPrintf(_T("Start Server failed")); 
      flush();
      goto try_again;
   }
   flush();
   //
   //	cmd_bt_rfcomm_open 3.4.1.2
   //
   buffer[0] = 0x20;
   buffer[1] = 0x0a;
   buffer[2] = 0x04;
   buffer[3] = 0x00;

   buffer[4] = bt_address[0];
   buffer[5] = bt_address[1];
   buffer[6] = bt_address[2];
   buffer[7] = bt_address[3];
   buffer[8] = bt_address[4];
   buffer[9] = bt_address[5];

   buffer[10] = 0x00;
   buffer[11] = 0x02;
   buffer[12] = 0x01;
   buffer[13] = 0x11;

   Success = WriteFile(bluetooth_port, &buffer[0], 14, &BytesWritten, NULL);
   stat = 0;
   for (retry = 0; retry < 10; ++retry)
   {
      Success = ClearCommError(bluetooth_port, &er, &lp);
      if (lp.cbInQue >= 70)
      {
         Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
         for (x = 0; x < lp.cbInQue; ++x)
         {
            if (buff1[x] == 0xa0 && buff1[x + 1] == 0x07 && buff1[x + 2] == 0x04 && buff1[x + 3] == 0x00)
            {
               m_endpoint = buff1[x + 4];
               stat = 1;
               break;
            }
         }
      }
      Sleep(500);
   }
   if (stat == 0)
   {
      

      m_pMainGui->wPrintf(_T("%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
         buff1[0], buff1[1], buff1[2], buff1[3], buff1[4], buff1[5], buff1[6], buff1[7], buff1[8], buff1[9],
         buff1[10], buff1[11], buff1[12], buff1[13], buff1[14], buff1[15], buff1[16], buff1[17], buff1[18], buff1[19],
         buff1[20], buff1[21], buff1[22], buff1[23], buff1[24], buff1[25], buff1[26], buff1[27], buff1[28], buff1[29],
         buff1[30], buff1[31], buff1[32], buff1[33], buff1[34], buff1[35], buff1[36], buff1[37], buff1[38], buff1[39],
         buff1[40], buff1[41], buff1[42], buff1[43], buff1[44], buff1[45], buff1[46], buff1[47], buff1[48], buff1[49]);
      m_pMainGui->wPrintf(_T("Failed to open connection"));
      m_pMainGui->wPrintf(_T("Read %02d bytes"), lp.cbInQue);

      flush();
      goto try_again;
   }
   flush();
   m_pMainGui->wPrintf(_T("endpoint = %02d"), m_endpoint);
   m_pMainGui->wPrintf(_T("Connected:"));
   //
   //	Make BT LED output
   //
   buffer[0] = 0x20;
   buffer[1] = 0x05;
   buffer[2] = 0x0c;
   buffer[3] = 0x01;
   buffer[4] = 0x01;
   buffer[5] = 0x0a;
   buffer[6] = 0x00;
   buffer[7] = 0x01;
   buffer[8] = 0x00;
   Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
   Sleep(100);
   Success = ClearCommError(bluetooth_port, &er, &lp);
   if (lp.cbInQue > 0)
      Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
   flush();
   Sleep(100);
   //
   //	Turn on BT LED
   //
   buffer[0] = 0x20;
   buffer[1] = 0x05;
   buffer[2] = 0x0c;
   buffer[3] = 0x02;
   buffer[4] = 0x01;
   buffer[5] = 0x00;
   buffer[6] = 0x04;
   buffer[7] = 0x00;
   buffer[8] = 0x04;
   Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
   Sleep(100);
   Success = ClearCommError(bluetooth_port, &er, &lp);
   if (lp.cbInQue > 0)
      Success = ReadFile(bluetooth_port, &buff1[0], lp.cbInQue, &BytesReturned, NULL);
   flush();
   Sleep(100);

   //stevebooth

   m_pMainGui->wPrintf(_T("Asking micro mode"));
   flush();
   wb[0] = 'M';
   wb[1] = 'o';
   wb[2] = 'd';
   wb[3] = 'e';
   wb[4] = '?';
   sent = WriteBluetooth(&wb[0], 5, 0);
   //
   //	Wait for response msg to come back
   //
   nbytes = ReadBluetooth((unsigned char *)rx1);

   if (strncmp("Infrared", (const char *)&rx1[3], 8) == 0)
   {
      m_pMainGui->wPrintf(_T("Ans: infrared mode"));
      Running_Mode = 'I';
   }
   else
   {
      m_pMainGui->wPrintf(_T("Ans: loader mode"));
      Running_Mode = 'L';
   }

   return(1);
}
// runs in initdialog to connect bluetooth device on startup
int CBlueTooth::Look_For_Bluetooth_DevicesOLD()
{
   // TODO: Add your implementation code here.
   int steveerror;
   ULONG		ulRetCode;
   WSADATA		WSAData = { 0 };

   INT             iResult = BT_SUCCESS;
   BOOL            bContinueLookup = FALSE, bRemoteDeviceFound = FALSE;
   ULONG           ulFlags = 0, ulPQSSize = sizeof(WSAQUERYSET);
   HANDLE          hLookup = NULL;
   PWSAQUERYSET	pWSAQuerySet = NULL;
   int xx;
//   char rcv_buf[16];
   int x;
   int found;
   //TCHAR IR[] = _T("BASCOMTURNER");
   CString sIR = _T(" BASCOMTURNERMLDS");
   //unsigned char IR[]={"BASCOM-TURNER-IR-42C3"};
   //unsigned char IR[]={"BASCOM-TURNER-8C6F"};

//   HANDLE 		keyboard;
//   DWORD		mode;
//   DWORD 		NumEvents;
   //unsigned char steve[50];

   for (xx = 0; xx < BTArraySize; ++xx)
      BT[xx] = new BlueToothList;// (struct BlueToothList *)calloc(1, sizeof(struct BlueToothList));

   // Initialize WinSock.
   ulRetCode = WSAStartup(MAKEWORD(2, 2), &WSAData);

   pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize);
   if (NULL == pWSAQuerySet)
   {
      iResult = STATUS_NO_MEMORY;
      m_pMainGui->wPrintf(_T("memory for WSAQUERYSET"));
      m_pMainGui->wPrintf(_T("ERROR: Unable to allocate"));
      for (x = 0; x < BTArraySize; ++x)
      {
         //free(BT[x]);
         delete BT[x];
         BT[x] = 0;
      }
      return false;
   }

   //
   // Search for the device with the correct name
   //
   int nBTCounter = 0;
   if (iResult == BT_SUCCESS)
   {
      for (INT iRetryCount = 0; !bRemoteDeviceFound && (iRetryCount < BT_MAX_INQUIRY_RETRY); iRetryCount++)
      {
         if (iRetryCount == 1 && CDlgSelectBT::m_st_nShowDialog == 0)
         {
            sIR = BT[0]->name;
            if (nBTCounter > 1)
            {
               CDlgSelectBT dlg;
               dlg.DoModal();
               sIR = BT[dlg.m_nSelection]->name;
               CDlgSelectBT::m_st_nShowDialog = (CDlgSelectBT::m_st_nShowDialog + 1) % 5;
            }
            nBTCounter = 0;
         }
         //
         // WSALookupService is used for both service search and device inquiry
         // LUP_CONTAINERS is the flag which signals that we're doing a device inquiry.
         //
         ulFlags = LUP_CONTAINERS;

         //
         // Friendly device name (if available) will be returned in lpszServiceInstanceName
         //
         ulFlags |= LUP_RETURN_NAME;

         //
         // BTH_ADDR will be returned in lpcsaBuffer member of WSAQUERYSET
         //
         ulFlags |= LUP_RETURN_ADDR;

         if (0 == iRetryCount)
         {
         }
         else
         {
            //
            // Flush the device cache for all inquiries, except for the first inquiry
            //
            // By setting LUP_FLUSHCACHE flag, we're asking the lookup service to do
            // a fresh lookup instead of pulling the information from device cache.
            //

            ulFlags |= LUP_FLUSHCACHE;

            //
            // Pause for some time before all the inquiries after the first inquiry
            //
            // Remote Name requests will arrive after device inquiry has
            // completed.  Without a window to receive IN_RANGE notifications,
            // we don't have a direct mechanism to determine when remote
            // name requests have completed.
            //
         }


         //
         // Start the lookup service
         //
         iResult = BT_SUCCESS;
         hLookup = 0;
         bContinueLookup = FALSE;
         ZeroMemory(pWSAQuerySet, ulPQSSize);
         pWSAQuerySet->dwNameSpace = NS_BTH;
         pWSAQuerySet->dwSize = sizeof(WSAQUERYSET);

         iResult = WSALookupServiceBegin(pWSAQuerySet, ulFlags, &hLookup);

         //
         // Even if we have an error, we want to continue until we
         // reach the CXN_MAX_INQUIRY_RETRY
         //
         if ((NO_ERROR == iResult) && (NULL != hLookup))
         {
            bContinueLookup = TRUE;
         }
         else if (0 < iRetryCount)
         {
            break;
         }
         while (bContinueLookup)
         {
            if (m_pMainGui->m_st_nTerminaThread) break;
            //
            // Get information about next bluetooth device
            //
            // Note you may pass the same WSAQUERYSET from LookupBegin
            // as long as you don't need to modify any of the pointer
            // members of the structure, etc.
            //
            int nWSAError = WSALookupServiceNext(hLookup, ulFlags, &ulPQSSize, pWSAQuerySet);
            if (NO_ERROR == nWSAError)
            {
               // Compare the name to see if this is the device we are looking for.

               found = 0;

               //for(xx=0;xx<100;++xx)
               //	BT[0]->name[xx] = 0;
               TRACE(_T("Blue tooth %s\n"), pWSAQuerySet->lpszServiceInstanceName);
               if (_tcsstr(pWSAQuerySet->lpszServiceInstanceName, _T("BASCOMTURNERMLDS")))
               {
                  TRACE(_T("Found %s\n"), pWSAQuerySet->lpszServiceInstanceName);
               }
               if (nBTCounter < BTArraySize && _tcsstr(pWSAQuerySet->lpszServiceInstanceName, _T("MLDS")))
               {
                  BT[nBTCounter++]->name = pWSAQuerySet->lpszServiceInstanceName;
                  if (sIR.IsEmpty() && iRetryCount + 1 == BT_MAX_INQUIRY_RETRY)
                  {
                     sIR = pWSAQuerySet->lpszServiceInstanceName;
                  }
                  //_tcscpy_s(BT[nBTCounter++]->name, pWSAQuerySet->lpszServiceInstanceName);
               }
               //for (xx = 0; xx < 100; ++xx)
               //{
               //   if (pWSAQuerySet->lpszServiceInstanceName[xx] == 0)
               //      break;
               //   found = 1;

               //   BT[0]->name[xx] = pWSAQuerySet->lpszServiceInstanceName[xx];
               //}

               found = 1;
               //pMainGui->wPrintf("%s ",&BT[0]->name[0]);

               if (pWSAQuerySet->lpszServiceInstanceName != NULL)
               {
                  //						for(xx=0;xx<18;++xx)
                  if (sIR.CompareNoCase(pWSAQuerySet->lpszServiceInstanceName) != 0)
                  {
                     found = 0;
                     continue;
                  }
                  //for (xx = 0; xx < 12; ++xx)
                  //{
                  //   if (pWSAQuerySet->lpszServiceInstanceName[xx] != IR[xx])
                  //   {
                  //      found = 0;
                  //      break;
                  //   }
                  //}
               }

               if (iRetryCount == 0 || sIR.IsEmpty())
               {
                  if (!BT[0]->name.IsEmpty())
                  {
                     sIR = BT[0]->name;
                  }
                  found = 0;
               }
               if (found == 1)
               {
                  CopyMemory(&m_RemoteBthAddr,
                     (PSOCKADDR_BTH)pWSAQuerySet->lpcsaBuffer->RemoteAddr.lpSockaddr,
                     sizeof(m_RemoteBthAddr));
                  bRemoteDeviceFound = TRUE;
                  bContinueLookup = FALSE;
               }
            }
            else
            {
               iResult = WSAGetLastError();
               if (WSA_E_NO_MORE == iResult)
               {
                  //
                  // No more devices found.  Exit the lookup.
                  //
                  bContinueLookup = FALSE;
               }
               else if (WSAEFAULT == iResult)
               {
                  //
                  // The buffer for QUERYSET was insufficient.
                  // In such case 3rd parameter "ulPQSSize" of function "WSALookupServiceNext()" receives
                  // the required size.  So we can use this parameter to reallocate memory for QUERYSET.
                  //
                  HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
                  pWSAQuerySet = (PWSAQUERYSET)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, ulPQSSize);
                  if (NULL == pWSAQuerySet)
                  {
                     m_pMainGui->wPrintf(_T("memory for WSAQUERYSET"));
                     m_pMainGui->wPrintf(_T("ERROR: Unable to allocate"));
                     iResult = STATUS_NO_MEMORY;
                     bContinueLookup = FALSE;
                  }
               }
               else
               {
                  m_pMainGui->wPrintf(_T("error code %d\n"), iResult);
                  m_pMainGui->wPrintf(_T("WSALookupServiceNext failed"));
                  
                  bContinueLookup = FALSE;
               }
            }
         }

         //
         // End the lookup service
         //
         WSALookupServiceEnd(hLookup);

         if (iResult == STATUS_NO_MEMORY)
            break;
      }
   }

   if (NULL != pWSAQuerySet)
   {
      HeapFree(GetProcessHeap(), 0, pWSAQuerySet);
      pWSAQuerySet = NULL;
   }

   if (!bRemoteDeviceFound)
   {
      iResult = BT_ERROR;
      WSACleanup();
      for (x = 0; x < BTArraySize; ++x)
      {
         //free(BT[x]);
         delete BT[x];
         BT[x] = 0;
      }
      return false;
   }

   bool bResult = ReConnect();

   Sleep(100);

   for (x = 0; x < BTArraySize; ++x)
   {
      //free(BT[x]);
      delete BT[x];
      BT[x] = 0;
   }
   return bResult;
}

bool CBlueTooth::ReConnect(void)
{
   //if (BtSocket != INVALID_SOCKET)
   //{
   //   shutdown(BtSocket, SD_BOTH);
   //   closesocket(BtSocket);
   //   BtSocket = INVALID_SOCKET;
   //}
   //m_RemoteBthAddr.addressFamily = AF_BTH;
   //m_RemoteBthAddr.serviceClassId = RFCOMM_PROTOCOL_UUID;
   //m_RemoteBthAddr.port = 0;
   //int steveerror = 0;
   ////
   //// Open a bluetooth socket using RFCOMM protocol
   ////
   //BtSocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
   //if (INVALID_SOCKET == BtSocket)
   //{
   //   steveerror = WSAGetLastError();
   //   m_pMainGui->wPrintf(_T("WSAError %d "),steveerror);      
   //   WSACleanup();
   //   return false;
   //}

   //// Set timeout on socket
   //int iOptVal = 25;
   //int iOptLen = sizeof(int);
   //iOptVal = 8192;
   //steveerror = setsockopt(BtSocket, SOL_SOCKET, SO_RCVBUF, (char *)&iOptVal, iOptLen);
   //iOptVal = 10;
   //steveerror = setsockopt(BtSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&iOptVal, iOptLen);

   ////
   //// Connect the socket (pSocket) to a given remote socket represented by address (pServerAddr)
   ////
   //if (SOCKET_ERROR == connect(BtSocket, (struct sockaddr *) &m_RemoteBthAddr, sizeof(SOCKADDR_BTH)))
   //{
   //   steveerror = WSAGetLastError();
   //   m_pMainGui->wPrintf(_T("WSAError %d "), steveerror);
   //   WSACleanup();
   //   shutdown(BtSocket, SD_BOTH);
   //   closesocket(BtSocket);
   //   BtSocket = INVALID_SOCKET;
   //   return false;
   //}
   return true;
}


int CBlueTooth::ReadBluetooth(unsigned char *ptr)
{
#ifdef USE_PROFILER
   CProfiler func("CBlueTooth::ReadBluetooth");
#endif
   unsigned long	nbytes;
   int		result, x;
   unsigned long 	numbytes;
   time_t ltime;
   time_t exp_time;
   COMSTAT 	lp;
   unsigned long 	er;
   unsigned char buffer[500];

   time(&ltime);
   exp_time = ltime + 15;

   for (;;)
   {
      ClearCommError(bluetooth_port, &er, &lp);

      time(&ltime);
      if (ltime >= exp_time)
      {
         m_pMainGui->wPrintf(_T("timeout"));
         return(-1);
      }
      if (Mode == Exit_Software)
      {
         return 0;
      }
      if (lp.cbInQue < rxsize + 6)
         continue;

      ReadFile(bluetooth_port, &buffer[0], rxsize + 6, &nbytes, NULL);

      for (x = 0; x < rxsize; ++x)
         ptr[x] = buffer[x + 6];

      //if (ptr[0] != 0xAA || ptr[1] != 0xBB)
      //{
      //   m_pMainGui->wPrintf(_T("Header did not match"));
      //   m_pMainGui->wPrintf(_T("bytes=%d   %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x"),
      //      nbytes, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7], ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15], ptr[16], ptr[17], ptr[18], ptr[19]);
      //}

      return(nbytes);
   }
}


int CBlueTooth::WriteBluetooth(unsigned char *ptr, int nbytes, int dummy)
{
   unsigned char tx1[100];
   int size;
   int x;
   unsigned long numberwritten;
   COMSTAT 	lp;
   unsigned long 	er;
   unsigned char buffer[500];
   unsigned long	numbytes;

   size = nbytes + 6;

   tx1[0] = 0x20;
   tx1[1] = size - 4;;
   tx1[2] = 0x0b;
   tx1[3] = 0x00;
   tx1[4] = m_endpoint;		// address of Bluetooth device. The tablet
   tx1[5] = size - 6;		// number of bytes to send from this point on 

   for (x = 0; x < nbytes; ++x)
      tx1[x + 6] = ptr[x];

   WriteFile(bluetooth_port, &tx1[0], size, &numberwritten, NULL);
   Sleep(50);

   ClearCommError(bluetooth_port, &er, &lp);
   if (lp.cbInQue >= 7)
      ReadFile(bluetooth_port, &buffer[0], 7, &numbytes, NULL);

   return numberwritten;
}

void CBlueTooth::Send_CONFIG_Parameters()
{
   FILE *stream;
   unsigned char buffer[50];
   int i, x, sent;


   if ((stream = fopen("c:\\BascomTurnerMLDS\\CONFIG.txt", "r")) != NULL)
   {
      for (;;)
      {
         for (x = 0; x < 50; ++x)
            buffer[x] = 0;

         for (x = 0; x < 100; ++x)
         {
            i = fread(&buffer[x], 1, 1, stream);
            if (buffer[x] == 10)
            {
               buffer[x] = 0;
               sent = WriteBluetooth(&buffer[0], 50, 0);
               Sleep(500);
               break;
            }
            if (i <= 0)
               break;
         }
         if (i <= 0)
            break;
      }
      fclose(stream);
   }
}


void CBlueTooth::Request_Bump()
{
   // TODO: Add your implementation code here.
   FILE *stream;
   unsigned char buffer[1000];
   unsigned char output[1000];
   char str_temp[5];
  
   int i, x, y, sent, c, length;



   for (x = 0; x < 1000; ++x)
      buffer[x] = 0;

   for (x = 0; x < 1000; ++x)
      output[x] = 0;

   output[0] = 'R';
   output[1] = 'E';
   output[2] = 'Q';
   output[3] = 'B';
   output[4] = ' ';
 

   y = 5;
   x = 0;
   sent = WriteBluetooth(output, y + 1, 0);
   Sleep(300);

}

void CBlueTooth::Request_CalP()
{
   // TODO: Add your implementation code here.
   FILE *stream;
   unsigned char buffer[1000];
   unsigned char output[1000];
   char str_temp[5];
   unsigned int   Tec_Set_Temp;
   int i, x, y, sent, c, length;



   for (x = 0; x < 1000; ++x)
      buffer[x] = 0;

   for (x = 0; x < 1000; ++x)
      output[x] = 0;

   output[0] = 'R';
   output[1] = 'E';
   output[2] = 'Q';
   output[3] = 'C';
   output[4] = ' ';


   y = 5;
   x = 0;
   sent = WriteBluetooth(output, y + 1, 0);
   Sleep(300);

}

void CBlueTooth::flush()
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   unsigned long	nbytes;
   unsigned long 	numbytes;
   COMSTAT 	lp = { 0 };
   unsigned long 	er;
   unsigned char buffer[2000];

   ClearCommError(bluetooth_port, &er, &lp);
   while (lp.cbInQue)
   {
      int nSize = lp.cbInQue;
      if (nSize > sizeof(buffer))
      {
         nSize = sizeof(buffer);
      }
      ReadFile(bluetooth_port, buffer, nSize, &nbytes, NULL);
      lp.cbInQue -= nbytes;
   }
}

TCHAR* TimeandDateStr(TCHAR* szBuff, UINT nLength)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   SYSTEMTIME systemTime = { 0 };
   GetLocalTime(&systemTime);
   _sntprintf(szBuff, nLength, _T("%02d:%02d:%02d.%03d"), systemTime.wHour,
      systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);
   //struct tm* time_now;
   //time_t secs_now;
   //time(&secs_now);
   //time_now = localtime(&secs_now);
   //_tcsftime(szBuff, nLength, _T("%H:%M:%S , %m-%d-%Y"), localtime(&secs_now));
   return szBuff;
}

int CBlueTooth::CalibrateFunc(void)
{

   return 0;
}

void CBlueTooth::SetGPSLED(int nOn)
{
   //flush();
   //
   //	Turn off GPS LED
   //
   //unsigned char buffer[1024];
   //unsigned long Success;
   //unsigned long BytesWritten = 0;
   //unsigned long BytesReturned = 0;
   //unsigned long 	er;
   //COMSTAT 	lp = { 0 };
   //buffer[0] = 0x20;
   //buffer[1] = 0x05;
   //buffer[2] = 0x0c;
   //buffer[3] = 0x02;
   //buffer[4] = 0x01;
   //buffer[5] = 0x00;
   //buffer[6] = 0x10;
   //buffer[7] = 0x00;
   //if (nOn)
   //{
   //   buffer[8] = 0x10;
   //}
   //else
   //{
   //   buffer[8] = 0x00;
   //}
   //Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
   //Sleep(200);
   //Success = ClearCommError(bluetooth_port, &er, &lp);
   //if (lp.cbInQue > 0)
   //{
   //   ASSERT(lp.cbInQue <= ARRAYSIZE(buffer));
   //   Success = ReadFile(bluetooth_port, &buffer[0], lp.cbInQue, &BytesReturned, NULL);
   //   
   //}
   Sleep(500);
   flush();
   if (3 == nOn)
   {
      m_pMainGui->SendMessage(UWM_GPS_BTN_UPDATE, 0, 0);
   }
   else
   {
      m_pMainGui->SendMessage(UWM_GPS_BTN_UPDATE, 1, nOn);
   }
   m_nGSPSLEDState = nOn;
}

void CBlueTooth::FlashGPSLED()
{
   if (Mode == Exit_Software)
   {
      return;
   }
   if ((clock() - m_tGPSLEDTimer) / double(CLOCKS_PER_SEC) > 1.0)
   {
      SetGPSLED(!m_nGSPSLEDState);
      m_tGPSLEDTimer = clock();
   }
}

//void CBlueTooth::InitGPS()
//{
//   return;
//   unsigned char buffer[1024];
//   unsigned long Success;
//   unsigned long BytesWritten = 0;
//   unsigned long BytesReturned = 0;
//   unsigned long 	er;
//   COMSTAT 	lp = { 0 };
//   flush();
//   buffer[0] = 0x20;
//   buffer[1] = 0x05;
//   buffer[2] = 0x0c;
//   buffer[3] = 0x01;
//   buffer[4] = 0x01;
//   buffer[5] = 0x0c;
//   buffer[6] = 0x00;
//   buffer[7] = 0x01;
//   buffer[8] = 0x00;
//   Success = WriteFile(bluetooth_port, &buffer[0], 9, &BytesWritten, NULL);
//   Sleep(100);
//   Success = ClearCommError(bluetooth_port, &er, &lp);
//   if (lp.cbInQue > 0)
//      Success = ReadFile(bluetooth_port, &buffer[0], lp.cbInQue, &BytesReturned, NULL);
//
//   flush();
//   Sleep(100);
//}

int CBlueTooth::Sample_Data_And_Display(HANDLE hCom)
{
#ifdef USE_PROFILER
   CProfiler func(__FUNCDNAME__);
#endif
   // TODO: Add your implementation code here.
   int x, y = 0, nbytes;
   int counter = 0;
   //unsigned char buff1[40];
   //unsigned char buff2[40];
   //unsigned char *ptr;
   CString sLabel;
   char buff1[40];
   char buff2[40];
   char *ptr;
   short sample;
   long lat, lon;
   DOUBLE GpsLat = 0.0, GpsLon = 0.0;
   DOUBLE GpsLat1 = 0.0, GpsLon1 = 0.0;
   DOUBLE GpsLat2 = 0.0, GpsLon2 = 0.0;
   DOUBLE GpsLatI = 0.0, GpsLonI = 0.0;
   float GpsHDOP;
   int GpsNSat;
   short latlon;
   BOOL bReadRC;
   char sGPSBuffer[1024];
   DWORD iBytesRead;
//   BOOL bExtractGPS;
   ULONG er;
   COMSTAT lp = { 0 };
   char szBuffer[40];
   TCHAR szTime[64] = { 0 };
   DWORD dwTimeR = GetTickCount();
   DWORD dwTimeG = GetTickCount();
   DWORD dwTimeG1 = GetTickCount();
   DWORD dwTimeG2 = GetTickCount();

   MyNMEA MyGps;// = new MyNMEA;
   PurgeComm(hCom, PURGE_RXCLEAR);
   int nOldMode = Mode;
   LARGE_INTEGER lastRead;
   LARGE_INTEGER frequency;
   QueryPerformanceFrequency(&frequency);
   QueryPerformanceCounter(&lastRead);
   LARGE_INTEGER newTimer;
   for (;;)
   {
      while (m_pMainGui->PauseBlueToothThread())
      {
         Sleep(1000);
      }
      if (Write_Flashc_Block == Mode)
      {
         //Mode = CBlueTooth::Stop_Sampling;
         Sleep(500);
         write_flashc_block();
         Mode = CBlueTooth::Idle;		
      }
      if (nOldMode == Idle && Mode == Start_Sampling)
      {
         return 0;
      }
      if ((Mode != Start_Sampling && Mode != Idle) || Mode == Exit_Software)
      {
         
         m_pMainGui->BluetoothStop();
         
         return(0);
      }
#ifdef USE_PROFILER
      CProfiler func("Sample_Data_And_Display for loop");
#endif
      QueryPerformanceCounter(&newTimer);
      double dTimeElapseSec = (newTimer.QuadPart - lastRead.QuadPart) / (double)frequency.QuadPart;
      while (dTimeElapseSec < 0.030)
      {
         QueryPerformanceCounter(&newTimer);
         dTimeElapseSec = (newTimer.QuadPart - lastRead.QuadPart) / (double)frequency.QuadPart;
         Sleep(0);
      }
      lastRead = newTimer;
      nbytes = ReadBluetooth((unsigned char *)rx1);
      if (nbytes == -1)
      {
         Sleep(1000);
         for (int nRetries = 0; nRetries < 1 && nbytes == -1; nRetries++)
         {
            nbytes = ReadBluetooth((unsigned char *)rx1);
         }
      }
      
      //if (Mode == Stop_Sampling)
      //   return (0);
      if (nbytes == -1 || nbytes == 0)
         return(-1);
      

      Crc8 = 0;
      for (x = 0; x < rxsize - 1; ++x)
         UpdateCRC8(rx1[x]);

      if (Crc8 != rx1[rxsize - 1])
      {
         ++checksum_errors;
         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         sprintf((char *const)&buff2[0], "%d", checksum_errors);
         //cheksum->SetSel(0, -1);
         //cheksum->Clear();
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         buff1[8] = buff2[4];
         buff1[10] = buff2[5];
         //cheksum->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         flush();
         continue;
      }
      m_st_CS.Lock();
      ptr = (char *)&sample;
      ptr[0] = rx1[3];
      ptr[1] = rx1[4];
      memcpy(m_aData, rx1 + 3, sizeof(m_aData));
      ASSERT(m_aData[0] == sample);
      m_Settings.graph_ppm = sample;
      float fSample = sample / 10.0f;
      if (m_Settings.m_bSubtractBackground && !m_pMainGui->m_bCalibrationInProgress)
      {
         //fSample = g_fFitG * fSample + g_fFitO - static_cast<float>(m_pMainGui->GetBumpTest()) - 1.8;
         fSample = g_fFitG * fSample + g_fFitO - fBgppm;
      }
      else if (!m_pMainGui->m_bCalibrationInProgress)
      {
         fSample = g_fFitG * fSample + g_fFitO;
      }
      if (fSample < 0.0f)
      {
         fSample = 0.0f;
      }
      /*short navg_min_2F, short navg_max_2F,
      short navg_min_1F, short navg_max_1F, short navg_minlocation1,
      short navg_maxlocation1, short navg_minlocation2, short navg_maxlocation2)*/
      m_pMainGui->Plot_ppm(fSample, 
         m_aData[avg_min_2F], m_aData[avg_max_2F],
         m_aData[avg_min_1F], m_aData[avg_max_1F], 
         m_aData[avg_minlocation1], m_aData[avg_maxlocation1],
         m_aData[avg_minlocation2], m_aData[avg_maxlocation2]);
      bool bSkipValue = false;
      if (g_MedFilter.aMedmin1floc != filter::UNDEF &&
         !(abs(m_aData[avg_minlocation1] - g_MedFilter.aMedmin1floc) < filter::THRESHOLD &&
            abs(m_aData[avg_maxlocation1] - g_MedFilter.aMedmax1floc) < filter::THRESHOLD &&
            abs(m_aData[avg_minlocation2] - g_MedFilter.aMedmin2floc) < filter::THRESHOLD))
      {
         bSkipValue = true;
      }
      if (!bSkipValue)
      {
        // m_pMainGui->m_plotGraph.Updateppm((int)std::round(fSample), GpsLon, GpsLat);
         m_pMainGui->m_plotGraph.Updateppm(fSample, GpsLon, GpsLat);

      }

      TimeandDateStr(szTime, ARRAYSIZE(szTime));
      CString sInfo;
      if (fSample >= m_Settings.m_nAlarmLevel && !bSkipValue)
      {
         dwTimeR = GetTickCount();

         GpsLatI = extrapolateGPS(GpsLat2, GpsLat1, dwTimeG2, dwTimeG1, dwTimeR);
         GpsLonI = extrapolateGPS(GpsLon2, GpsLon1, dwTimeG2, dwTimeG1, dwTimeR);
         sInfo.Format(_T("%s,%.6f,%.6f,%.1f"), szTime, GpsLatI, GpsLonI, fSample);
         m_pMainGui->WriteToSurveyFileR(sInfo);
      }
      ClearCommError(hCom, &er, &lp);
      if (lp.cbInQue >= 75) {
         Sleep(10);
         for (x = 0; x < 1024; ++x)
            sGPSBuffer[x] = 0;
         bReadRC = ReadFile(hCom, &sGPSBuffer, 1024, &iBytesRead, NULL);
         if (iBytesRead != 0) {
            m_bGPSDetect = true;
            MyGps.ParseNMEASentence("$GNGGA", (const char *)&sGPSBuffer, (UINT)iBytesRead);
            GpsLat = MyGps.m_GPSInfo.m_latitude;
            GpsLon = MyGps.m_GPSInfo.m_longitude;
            GpsNSat = MyGps.m_GPSInfo.m_satelitesInUse;
            GpsHDOP = static_cast<float>(MyGps.m_GPSInfo.m_signalQuality);
            TimeandDateStr(szTime, ARRAYSIZE(szTime));
            dwTimeG = GetTickCount();
            if (GpsLat == 0.0 || GpsLon == 0.0)
            {
               FlashGPSLED();
            }
            else
            {
               if (m_nGSPSLEDState != 3)
               {
                  SetGPSLED(3);
               }
            }
            sInfo.Format(_T("%s,%.6lf,%.6lf,%02d,%.1lf,%.1f"), szTime, GpsLat, GpsLon,
               GpsNSat, GpsHDOP, fSample);
            if (!bSkipValue)
            {
               m_pMainGui->WriteToSurveyFile(sInfo);
            }
            GpsLat2 = GpsLat1;
            GpsLat1 = GpsLat;
            GpsLon2 = GpsLon1;
            GpsLon1 = GpsLon;
            dwTimeG2 = dwTimeG1;
            dwTimeG1 = dwTimeG;
         }
      }
      else
      {
         if (!m_bGPSDetect)
         {
            SetGPSLED(3);
         }
      }

      if (sample > 10 && sample < 40) m_Settings.BumpTestSucess();
      //counter = counter + 1;
      //if (counter == 30)
      {

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //tec_temp->SetSel(0, -1);
         //tec_temp->Clear();
         ptr = (char *)&latlon;
         ptr[0] = rx1[5];
         ptr[1] = rx1[6];
         sprintf((char *const)&buff2[0], "%d", latlon);
         m_pMainGui->m_nTec_Temperature = latlon;

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //tec_curr->SetSel(0, -1);
         //tec_curr->Clear();
         ptr = (char *)&latlon;
         ptr[0] = rx1[7];
         ptr[1] = rx1[8];
         sprintf((char *const)&buff2[0], "%d", latlon);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         buff1[8] = buff2[4];
         //tec_curr->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         for (x = 0; x < 40; ++x)
            buff2[x] = 0;

         //Latitude->SetSel(0, -1);
         //Latitude->Clear();
         ptr = (char *)&lat;
         //ptr[0] = rx1[11];
         //ptr[1] = rx1[12];
         //ptr[2] = rx1[13];
         //ptr[3] = rx1[14];
         if (GpsLat < 0.0) {
            _gcvt(GpsLat, 9, szBuffer);
         }
         else {
            _gcvt(GpsLat, 8, szBuffer);
         }
         sprintf((char *const)&buff2[0], "%s", szBuffer);
         sLabel.Format(_T("%06lf"), GpsLat);

         x = strlen(buff2) + 2;
         for (;;)
         {
            if (buff2[x] != 0)
               break;
            x = x - 1;
         }
         //buff2[x + 1] = buff2[x];
         //buff2[x] = buff2[x - 1];
         //buff2[x - 1] = buff2[x - 2];
         //buff2[x - 2] = buff2[x - 3];
         //buff2[x - 3] = buff2[x - 4];
         //buff2[x - 4] = buff2[x - 5];
         //buff2[x - 5] = buff2[x - 6];
         //buff2[x - 6] = '.';


         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         buff1[8] = buff2[4];
         buff1[10] = buff2[5];
         buff1[12] = buff2[6];
         buff1[14] = buff2[7];
         buff1[16] = buff2[8];
         buff1[18] = buff2[9];
         buff1[20] = buff2[10];
         buff1[22] = buff2[11];
         //Latitude->ReplaceSel((LPCTSTR)&buff1[0], FALSE);
         //wprintf(szBuffer, GpsLat);

         m_pMainGui->m_plotGraph.UpdateLat((LPCTSTR)sLabel);

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         for (x = 0; x < 40; ++x)
            buff2[x] = 0;
         //Longitude->SetSel(0, -1);
         //Longitude->Clear();
         ptr = (char *)&lon;
         //ptr[0] = rx1[15];
         //ptr[1] = rx1[16];
         //ptr[2] = rx1[17];
         //ptr[3] = rx1[18];
         if (GpsLon < 0.0) {
            _gcvt(GpsLon, 9, szBuffer);
         }
         else {
            _gcvt(GpsLon, 8, szBuffer);
         }
         sprintf((char *const)&buff2[0], "%s", szBuffer);
         sLabel.Format(_T("%06lf"), GpsLon);

         x = 11;
         for (;;)
         {
            if (buff2[x] != 0)
               break;
            x = x - 1;
         }
         //buff2[x + 1] = buff2[x];
         //buff2[x] = buff2[x - 1];
         //buff2[x - 1] = buff2[x - 2];
         //buff2[x - 2] = buff2[x - 3];
         //buff2[x - 3] = buff2[x - 4];
         //buff2[x - 4] = buff2[x - 5];
         //buff2[x - 5] = buff2[x - 6];
         //buff2[x - 6] = '.';



         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         buff1[8] = buff2[4];
         buff1[10] = buff2[5];
         buff1[12] = buff2[6];
         buff1[14] = buff2[7];
         buff1[16] = buff2[8];
         buff1[18] = buff2[9];
         buff1[20] = buff2[10];
         buff1[22] = buff2[11];
         //Longitude->ReplaceSel((LPCTSTR)&buff1[0], FALSE);
         //wprintf(szBuffer, GpsLon);
         //_gcvt(GpsLon, 12, &szBuffer);
         m_pMainGui->m_plotGraph.UpdateLon((LPCTSTR)sLabel);


         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //Battery_Voltage->SetSel(0, -1);
         //Battery_Voltage->Clear();
         ptr = (char *)&sample;
         ptr[0] = rx1[19];
         ptr[1] = rx1[20];
         sprintf((char *const)&buff2[0], "%d", sample);
         m_pMainGui->m_nBattery_Voltage = sample;
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         //Battery_Voltage->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //Laser_Temperature->SetSel(0, -1);
         //Laser_Temperature->Clear();
         ptr = (char *)&sample;
         ptr[0] = rx1[21];
         ptr[1] = rx1[22];
         sprintf((char *const)&buff2[0], "%d", sample);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];

         //Laser_Temperature->ReplaceSel((LPCTSTR)&buff1[0], FALSE);


         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //Receiver_Temperature->SetSel(0, -1);
         //Receiver_Temperature->Clear();
         ptr = (char *)&sample;
         ptr[0] = rx1[23];
         ptr[1] = rx1[24];
         sprintf((char *const)&buff2[0], "%d", sample);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         //Receiver_Temperature->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //Laser_Humidity->SetSel(0, -1);
         //Laser_Humidity->Clear();
         ptr = (char *)&sample;
         ptr[0] = rx1[25];
         ptr[1] = rx1[26];
         sprintf((char *const)&buff2[0], "%d", sample);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         //Laser_Humidity->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         //Receiver_Humidity->SetSel(0, -1);
         //Receiver_Humidity->Clear();
         ptr = (char *)&sample;
         ptr[0] = rx1[27];
         ptr[1] = rx1[28];
         sprintf((char *const)&buff2[0], "%d", sample);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         //Receiver_Humidity->ReplaceSel((LPCTSTR)&buff1[0], FALSE);

         counter = 0;
      }
      if (!m_pMainGui->IsAlarmDisabled())
      {
         now = std::clock();
         if (fSample >= m_Settings.m_nAlarmLevel && !bSkipValue)//ppmalarmlevel)
         {
            start = std::clock();
            if (alarm_active == 0 && alarm_muted != 1) {

               PlaySound(m_Settings.m_sWavFile, NULL, SND_ASYNC | SND_LOOP);
               //PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\alarm.wav"), NULL, SND_ASYNC | SND_LOOP);
            }
            alarm_active = 1;

         }
         else
         {
            if (alarm_active == 1 && (now - start) > 1000) {
               PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
               alarm_active = 0;
            }
         }
      }
      m_st_CS.Unlock();
      //display_graph_data();
   }
   return 0;
}
DOUBLE CBlueTooth::extrapolateGPS(DOUBLE GpsL2, DOUBLE GpsL1,DWORD dwTimeG2, DWORD dwTimeG1, DWORD dwTimeR)
{
   DOUBLE GpsI;
   GpsI = GpsL2 + (GpsL1 - GpsL2) / (dwTimeG1 - dwTimeG2)*(dwTimeR - dwTimeG2);
   return GpsI;
};


void CBlueTooth::read_flashf_block()
{
   // TODO: Add your implementation code here.
   int x, y, z, nbytes, i, sent;
   FILE *stream = 0;
//   unsigned char buffer[100];
   unsigned char wb[20];
//   unsigned char temp[10];
   unsigned long address = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0;
//   unsigned short test[3];

   flush();
   wb[0] = 'R';
   wb[1] = 'e';
   wb[2] = 'a';
   wb[3] = 'd';
   wb[4] = 'f';
   sent = WriteBluetooth(&wb[0], 5, 0);
   flush();

   Sleep(500);

   y = 0;
   for (x = 0; x < 37; ++x)
   {
      nbytes = ReadBluetooth((unsigned char *)rx1);

      Crc8 = 0;
      for (i = 0; i < rxsize - 1; ++i)
         UpdateCRC8(rx1[i]);

      if (Crc8 != rx1[rxsize - 1])
         m_pMainGui->wPrintf(_T("CRC Error"));

      for (z = 2; z < 43; ++z)
      {
         Flashf_Block[y] = rx1[z];
         y = y + 1;
      }
   }
   nbytes = ReadBluetooth((unsigned char *)rx1);

   Crc8 = 0;
   for (i = 0; i < rxsize - 1; ++i)
      UpdateCRC8(rx1[i]);

   if (Crc8 != rx1[rxsize - 1])
      m_pMainGui->wPrintf(_T("CRC Error"));

   for (z = 2; z < 21; ++z)
   {
      Flashf_Block[y] = rx1[z];
      y = y + 1;
   }
   m_pMainGui->wPrintf(_T("Read IFlash Done"));
}
void CBlueTooth::read_flashc_block()
{
   // TODO: Add your implementation code here.
   int x, y, z, nbytes, i, sent;
   FILE *stream = 0;
   //   unsigned char buffer[100];
   unsigned char wb[20];
   //   unsigned char temp[10];
   unsigned long address = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0;
   //   unsigned short test[3];

   flush();
   wb[0] = 'R';
   wb[1] = 'e';
   wb[2] = 'a';
   wb[3] = 'd';
   wb[4] = 'c';
   sent = WriteBluetooth(&wb[0], 5, 0);
   flush();

   Sleep(500);

   y = 0;
   for (x = 0; x < 37; ++x)
   {
      nbytes = ReadBluetooth((unsigned char *)rx1);

      Crc8 = 0;
      for (i = 0; i < rxsize - 1; ++i)
         UpdateCRC8(rx1[i]);

      if (Crc8 != rx1[rxsize - 1])
         m_pMainGui->wPrintf(_T("CRC Error"));

      for (z = 2; z < 43; ++z)
      {
         Flashc_Block[y] = rx1[z];
         y = y + 1;
      }
   }
   nbytes = ReadBluetooth((unsigned char *)rx1);

   Crc8 = 0;
   for (i = 0; i < rxsize - 1; ++i)
      UpdateCRC8(rx1[i]);

   if (Crc8 != rx1[rxsize - 1])
      m_pMainGui->wPrintf(_T("CRC Error"));

   for (z = 2; z < 21; ++z)
   {
      Flashc_Block[y] = rx1[z];
      y = y + 1;
   }
   m_pMainGui->wPrintf(_T("Read CFlash Done"));
}

bool CBlueTooth::IsFlashcBlockEmpty()
{
   bool bResult = true;
   bool bFlash255 = true;
   for (UINT i = 0; i < ARRAYSIZE(Flashc_Block); i++)
   {
      if (Flashc_Block[i] != 255)
      {
         bFlash255 = false;
         break;
      }
   }
   if (!bFlash255 && strlen((char*)Flashc_Block) > 10)
   {
      bResult = false;
   }
   return bResult;
}

bool CBlueTooth::IsFlashfBlockEmpty()
{
   bool bResult = true;
   bool bFlash255 = true;
   for (UINT i = 0; i < ARRAYSIZE(Flashf_Block); i++)
   {
      if (Flashf_Block[i] != 255)
      {
         bFlash255 = false;
         break;
      }
   }
   if (!bFlash255 && strlen((char*)Flashf_Block) > 10)
   {
      bResult = false;
   }
   return bResult;
}


void CBlueTooth::init_flashc_block()
{
 // flash block read earlier 
   // read_flash_block();

   if (!IsFlashcBlockEmpty()&& !m_Settings.m_bResetCFlash)
    {
      process_flashc_block();
      return;
   }


   ::ZeroMemory(Flashc_Block, sizeof(Flashc_Block));

   sprintf((char*)Flashc_Block, "%s;", m_Settings.m_szcalrecord);
   strcpy(m_CalRecords.CalRecord[0], m_Settings.m_szcalrecord);
   m_CalRecords.m_nCount = 1;
   write_flashc_block();
   m_Settings.ResetCFlashFlag();
}
void CBlueTooth::init_flashf_block()
{
   // flash block read earlier 
     // read_flash_block();

   if (!IsFlashfBlockEmpty())
   {
      process_flashf_block();
      return;
   }
   else
   {
      if (AfxMessageBox(_T("No Instrument Record in Flash \n Initialize"), MB_YESNO) == IDNO)
      {
         return;
      }
   }

   ::ZeroMemory(Flashf_Block, sizeof(Flashf_Block));

   sprintf((char*)Flashf_Block, "%s;", m_Settings.m_szHeader);
   strcpy(m_CalRecords.InstrumentRecord, m_Settings.m_szHeader);
   write_flashf_block();
}

void CBlueTooth::write_flashf_block()
{
   //AfxMessageBox(_T("write_flash_block needs review"));
   //return;
   // TODO: Add your implementation code here.
   int x, y, z, nbytes = 0, sent;
   FILE *stream = 0;
   unsigned char buffer[100];
   unsigned char wb[20];
   m_CalRecords.m_WriteflashInProgress = true;
//   unsigned char temp[10];
//   unsigned long address = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0;
//   unsigned short test[3];
   //char empty[1536] = { NULL };
   //y = 0;
   //for (x = 0; x < 1536; ++x)
   //{
   //   Flash_Block[x] = y;
   //   y = y + 1;
   //   if (y == 256) {
   //      Flash_Block[x] = ';';
   //      y = 0;
   //   }
   //      

   //}
   
   m_pMainGui->wPrintf(_T("Writing IFlash"));
   m_CalRecords.WriteToFlashf((char*)Flashf_Block, ARRAYSIZE(Flashf_Block));
   //strncpy((char*) Flash_Block, empty, 1536);
   flush();
   wb[0] = 'W';
   wb[1] = 'r';
   wb[2] = 'i';
   wb[3] = 't';
   wb[4] = 'f';
   sent = WriteBluetooth(&wb[0], 5, 0);
   //ASSERT(sent != SOCKET_ERROR);
   flush();

   Sleep(500);

   y = 0;
   for (x = 0; x < 37; ++x)
   {
      for (z = 0; z < 41; ++z)
      {
         buffer[z] = Flashf_Block[y];
         y = y + 1;
      }
      sent = WriteBluetooth(&buffer[0], 41, 0);
      //ASSERT(sent != SOCKET_ERROR);
      
      Sleep(100);
   }

   Sleep(100);

   for (z = 0; z < 19; ++z)
   {
      buffer[z] = Flashf_Block[y];
      y = y + 1;
   }
   //
   //	Send 41 but really only 19 of that are used on the other end
   //	The other side expects a certain packet size
   //
   sent = WriteBluetooth(&buffer[0], 41, 0);
   //ASSERT(sent != SOCKET_ERROR);
   

   for (int i = 0; i < 70; i++)
   {
      flush();
      Sleep(100);
   }
   
   //Sleep(7000);
   
   m_pMainGui->wPrintf(_T("Write IFlash Done"));
   m_CalRecords.m_WriteflashInProgress = false;
}
void CBlueTooth::write_flashc_block()
{
   //AfxMessageBox(_T("write_flash_block needs review"));
   //return;
   // TODO: Add your implementation code here.
   int x, y, z, nbytes = 0, sent;
   FILE *stream = 0;
   unsigned char buffer[100];
   unsigned char wb[20];
   m_CalRecords.m_WriteflashInProgress = true;
   //   unsigned char temp[10];
   //   unsigned long address = 0, data1 = 0, data2 = 0, data3 = 0, data4 = 0;
   //   unsigned short test[3];
      //char empty[1536] = { NULL };
      //y = 0;
      //for (x = 0; x < 1536; ++x)
      //{
      //   Flash_Block[x] = y;
      //   y = y + 1;
      //   if (y == 256) {
      //      Flash_Block[x] = ';';
      //      y = 0;
      //   }
      //      

      //}

   m_pMainGui->wPrintf(_T("Writing CFlash"));
   m_CalRecords.WriteToFlashc((char*)Flashc_Block, ARRAYSIZE(Flashc_Block));
   //strncpy((char*) Flash_Block, empty, 1536);
   flush();
   wb[0] = 'W';
   wb[1] = 'r';
   wb[2] = 'i';
   wb[3] = 't';
   wb[4] = 'c';
   sent = WriteBluetooth(&wb[0], 5, 0);
   //ASSERT(sent != SOCKET_ERROR);
   flush();

   Sleep(500);

   y = 0;
   for (x = 0; x < 37; ++x)
   {
      for (z = 0; z < 41; ++z)
      {
         buffer[z] = Flashc_Block[y];
         y = y + 1;
      }
      sent = WriteBluetooth(&buffer[0], 41, 0);
      //ASSERT(sent != SOCKET_ERROR);

      Sleep(100);
   }

   Sleep(100);

   for (z = 0; z < 19; ++z)
   {
      buffer[z] = Flashc_Block[y];
      y = y + 1;
   }
   //
   //	Send 41 but really only 19 of that are used on the other end
   //	The other side expects a certain packet size
   //
   sent = WriteBluetooth(&buffer[0], 41, 0);
   //ASSERT(sent != SOCKET_ERROR);


   for (int i = 0; i < 70; i++)
   {
      flush();
      Sleep(100);
   }

   //Sleep(7000);

   m_pMainGui->wPrintf(_T("Write CFlash Done"));
   m_CalRecords.m_WriteflashInProgress = false;
}

void structCalRecord::Insert(char* szNewCal)
{
   if (m_nCount == CAL_MAX_SIZE)
   {
      m_nCount--;
   }
   for (int i = 0; i < m_nCount; i++)
   {
      strncpy(CalRecord[m_nCount - i], CalRecord[m_nCount - 1 - i], STRING_SIZE);
   }
   strncpy(CalRecord[0], szNewCal, STRING_SIZE);
   m_nCount++;
}

void structCalRecord::WriteToFlashf(char* Flash_Block, int nMaxSize)
{
   int nWriteTotal = 0;
   int nLength = 0;
   strcpy(Flash_Block, InstrumentRecord);
   strcat(Flash_Block, ";");
   nWriteTotal += strlen(InstrumentRecord) + 1;
   
}

void structCalRecord::WriteToFlashc(char* Flash_Block, int nMaxSize)
{
   int nWriteTotal = 0;
   int nLength = 0;
   for (int i = 0; i < m_nCount; i++)
   {
      nLength = strlen(CalRecord[i]) + 1;
      if (nLength + nWriteTotal >= nMaxSize)
      {
         break;
      }
      strcpy(Flash_Block + nWriteTotal, CalRecord[i]);
      strcat(Flash_Block, ";");
      nWriteTotal += nLength;
   }
}


void CBlueTooth::process_flashc_block()
{ 
   char szDate[64] = { 0 };
   double ppm_before = 0.0, ppm_after = 0.0;
   float fFit = 0.0;
   int CalDummy = 0;
   int nYear = 2018, nMonth = 1, nDay = 1;

   char * pch = strtok((char *) Flashc_Block, ";");
   //strcpy(m_CalRecords.InstrumentRecord, pch);
   //pch = strtok(NULL, ";");
   if (pch != NULL)
   { 
      if (sscanf(pch, "%d-%d-%d,%d,%lf,%f,%lf", &nMonth, &nDay, &nYear, &CalDummy, &ppm_before, &fFit, &ppm_after) == 7)
      {
         CTime time(nYear, nMonth, nDay, 0, 0, 0);
         CTimeSpan calPeriod(m_Settings.m_nCalDuePeriod, 0, 0, 0);
         m_Settings.m_TimeNextCal = time + calPeriod;
      }
   }
   else
   {
      CTime time(2020, 1, 1, 0, 0, 0);
      CTimeSpan calPeriod(m_Settings.m_nCalDuePeriod, 0, 0, 0);
      m_Settings.m_TimeNextCal = time + calPeriod;
   }
   // INSERT nextcal calculation here
   m_CalRecords.m_nCount = 0;
   while (pch != NULL)
   {
      strcpy(m_CalRecords.CalRecord[m_CalRecords.m_nCount], pch);
      if (m_CalRecords.m_nCount == 0)
      {
         
         if (sscanf(pch, "%[^,],%d,%lf,%f,%lf", szDate, &CalDummy, &ppm_before, &fFit, &ppm_after) == 5)
         {
            g_fFitG = fFit;
         }
         else
         {
            //ASSERT(FALSE);
            g_fFitG = 1.0;
         }
      }
      m_CalRecords.m_nCount++;
      pch = strtok(NULL, ";");
   }
}

void CBlueTooth::process_flashf_block()
{
   char szDate[64] = { 0 };
   double ppm_before = 0.0, ppm_after = 0.0;
   float fFit = 0.0;
   int CalDummy = 0;
   int nYear = 2018, nMonth = 1, nDay = 1;

   char * pch = strtok((char *)Flashf_Block, ";");
   strcpy(m_CalRecords.InstrumentRecord, pch);
   //pch = strtok(NULL, ";");

}


void CBlueTooth::UpdateCRC8(unsigned char c)
{
   // TODO: Add your implementation code here.
   Crc8 = TableCrc8[Crc8 ^ c];
}


int CBlueTooth::download_new_program()
{
   CA2T sFileName(m_szFWFile);
   //for (int i = 0; i < 100; i++)
   //{
   //   m_pMainGui->m_pCurrentProgressDlg->PostMessage(WMU_PLEASEDLG_UPDATE, i, 0);
   //   Sleep(100);
   //}
   //m_pMainGui->m_pCurrentProgressDlg->SendMessage(WMU_PLEASEDLG_COMPLETE, 0, 0);
   //AfxMessageBox(sFileName);
   //Mode = Idle;
   //return 1;

   int x, y, z, nbytes, i, sent;
   unsigned char buffer[100], buff2[50], buff1[50];
   unsigned char wb[20];
   unsigned char temp[10];
   unsigned long address, data1, data2, data3, data4;
   unsigned short test[3];
   int progress, prog, saved_prog;
   int step;
   int numbytes;

   prog = 0;
   saved_prog = 0;
   progress = 0;
   FILE* Loader_Stream = 0;

   m_pMainGui->wPrintf(_T("Download new program"));
   //
   //	Send a Stop command even if we're in the loader code on the micro.
   //???
   m_pMainGui->wPrintf(_T("Sending Halt"));
   flush();
   wb[0] = 'H';
   wb[1] = 'a';
   wb[2] = 'l';
   wb[3] = 't';
   wb[4] = ' ';
   sent = WriteBluetooth(wb, 5, 0);
   flush();

   Sleep(10000);

   //
   //	First ask micro if it's running the infrared program or the loader.
   //	If it's already running the loader then just continue on with the
   //	code. 
   //
   //	If it's running the infrared code we need to send it a command
   //	to put itself into loader mode. After that we need to close the 
   //	Bluetooth connection, wait a bit then wait for a reconnect from
   //	the loader then continue on.
   //
   m_pMainGui->wPrintf(_T("Asking micro mode"));
   flush();
   wb[0] = 'M';
   wb[1] = 'o';
   wb[2] = 'd';
   wb[3] = 'e';
   wb[4] = '?';
   sent = WriteBluetooth(&wb[0], 5, 0);
   //flush();
   //
   //	Wait for response msg to come back
   //
   nbytes = ReadBluetooth((unsigned char *)rx1);

   if (strncmp("Infrared", (const char *)&rx1[3], 8) == 0)
   {
      m_pMainGui->wPrintf(_T("Ans: infrared mode"));

      //
      //	Tell the infrared code to jump into loader mode
      //
      m_pMainGui->wPrintf(_T("Jump to loader"));
      flush();
      Sleep(1000);//???
      wb[0] = 'L';
      wb[1] = 'o';
      wb[2] = 'a';
      wb[3] = 'd';
      wb[4] = ' ';
      sent = WriteBluetooth(&wb[0], 5, 0);
      flush();
      //
      // 	Close the socket
      //
      Close_Bluetooth_port();
      PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
      Sleep(2000);
      PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);



      //
      //	Uncheck connected box
      //
      //btconn->SetSel(0, -1);
      //btconn->Clear();
      //
      //	Now lets sleep a bit and wait for the infrared code to write the new int vectors
      //	then switch to the loader code then we'll re-connect the Bluetooth
      //
      Reopen_Bluetooth_port();
      //read_ports_txt_blue();
      Sleep(10000);


      for (;;)
      {
         x = Connect_To_Bluetooth_Device();
         if (x == 1)
            break;
      }
      m_Settings.m_ConnectionStatus.bBluetooth = true;
      //test[0] = 254;
      //test[1] = 0;
      //test[2] = 0;
      //btconn->SetSel(0, -1);
      //btconn->Clear();
      //btconn->ReplaceSel((LPCTSTR)&test[0], FALSE);

      PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
      Sleep(2000);
      PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
   }
   //
   //	If we received back a Loader message just fall through.
   //	We fall through from above if that code executes also.
   //
   if (strncmp("Loader", (const char *)&rx1[3], 6) == 0)
   {
      m_pMainGui->wPrintf(_T("Ans: loader mode"));
   }

   m_pMainGui->wPrintf(_T("Sending prog"));
   flush();
   wb[0] = 'P';
   wb[1] = 'r';
   wb[2] = 'o';
   wb[3] = 'g';
   wb[4] = ' ';
   sent = WriteBluetooth(&wb[0], 5, 0);
   //flush();
   //
   //	Wait for OK msg to come back
   //
   nbytes = ReadBluetooth((unsigned char *)rx1);

   m_pMainGui->wPrintf(_T("Received OK msg back"));

   for (x = 0; x < 100; ++x)
      buffer[x] = 0;


   convert_microchip_hex_file();
   //Loader_Stream = fopen("c:\\BascomTurnerMLDS\\Program\\Infrared.hex", "r");
   Loader_Stream = fopen(m_szFWFile, "r");
   Preprocess_File(Loader_Stream);		// go figure how many lines of code for % progress
   fclose(Loader_Stream);
   Loader_Stream = fopen(m_szFWFile, "r");// "c:\\BascomTurnerMLDS\\Program\\Infrared.hex", "r");

   step = program_progress / 100;
   step = step + 1;

   m_pMainGui->wPrintf(_T("# of records=%d"), program_progress);
  

   for (;;)
   {
      numbytes = 0;
      for (;;)
      {
         i = fread(&buffer[numbytes], 1, 1, Loader_Stream);
         if (i <= 0)
            break;
         if (buffer[numbytes] == 0x0a)
            break;
         numbytes = numbytes + 1;
      }

      if (i <= 0)
         break;

   retry:


      //
      //		One data word
      //
      if (numbytes == 13)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);
      }
      //
      //		Two data words
      //
      if (numbytes == 20)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);
      }
      //
      //		Three data word
      //???
      if (numbytes == 27)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);

         temp[0] = buffer[21];
         temp[1] = buffer[22];
         temp[2] = buffer[23];
         temp[3] = buffer[24];
         temp[4] = buffer[25];
         temp[5] = buffer[26];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data3);
      }
      //
      //		Four data word
      //
      if (numbytes == 34)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);

         temp[0] = buffer[21];
         temp[1] = buffer[22];
         temp[2] = buffer[23];
         temp[3] = buffer[24];
         temp[4] = buffer[25];
         temp[5] = buffer[26];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data3);

         temp[0] = buffer[28];
         temp[1] = buffer[29];
         temp[2] = buffer[30];
         temp[3] = buffer[31];
         temp[4] = buffer[32];
         temp[5] = buffer[33];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data4);
      }
      //
      //	Skip this range. This is the loaders C-Lib area
      //
      if (address >= 0x3800 && address < 0x7000)
      {
         for (x = 0; x < 100; ++x)
            buffer[x] = 0;
         continue;
      }
      //
      //	Skip this range. This is storage block and loader code all the way till end except for last 4 locations 
      //

      //			if(address >= 0x13800 && address < 0x157f8)
      if (address >= 0x13800)
      {
         for (x = 0; x < 100; ++x)
            buffer[x] = 0;
         continue;
      }

      prog = progress / step;
      prog = prog + 1;			// this is ONLY so we end up on 100% instead of 99% cause at 100%, it's fricken done!
      if (prog != saved_prog)
      {
         saved_prog = prog;

         for (x = 0; x < 40; ++x)
            buff1[x] = 0;
         if (prog > 100)
            prog = 100;
         sprintf((char *const)&buff2[0], "%d%%", prog);
         buff1[0] = buff2[0];
         buff1[2] = buff2[1];
         buff1[4] = buff2[2];
         buff1[6] = buff2[3];
         m_pMainGui->m_pCurrentProgressDlg->PostMessage(WMU_PLEASEDLG_UPDATE, prog, 0);
         //PPM->SetWindowText((LPCTSTR)&buff1[0]);
      }
      progress = progress + 1;

      if (numbytes != 13 && numbytes != 20 && numbytes != 27 && numbytes != 34)
         m_pMainGui->wPrintf(_T("#bytes wrong %02d"), numbytes);


      if (numbytes == 13)
      {

         Crc8 = 0;
         for (x = 0; x < 13; ++x)
            UpdateCRC8(buffer[x]);
         buffer[13] = Crc8;

         sent = WriteBluetooth(&buffer[0], 14, 0);
      }


      if (numbytes == 20)
      {
         Crc8 = 0;

         for (x = 0; x < 20; ++x)
            UpdateCRC8(buffer[x]);
         buffer[20] = Crc8;

         sent = WriteBluetooth(&buffer[0], 21, 0);
      }

      //???
      if (numbytes == 27)
      {


         Crc8 = 0;
         for (x = 0; x < 27; ++x)
            UpdateCRC8(buffer[x]);
         buffer[27] = Crc8;

         sent = WriteBluetooth(&buffer[0], 28, 0);
      }


      if (numbytes == 34)
      {


         Crc8 = 0;
         for (x = 0; x < 34; ++x)
            UpdateCRC8(buffer[x]);
         buffer[34] = Crc8;

         sent = WriteBluetooth(&buffer[0], 35, 0);
      }
      //
      //		Wait for OK msg to come back
      //
      nbytes = ReadBluetooth((unsigned char *)rx1);
      if (rx1[3] == 'N' && rx1[4] == 'O')
      {
         m_pMainGui->wPrintf(_T("Checksum error"));
         goto retry;
      }

      if (nbytes == -1)
      {
         Mode = Idle;	//idle
//
//			If -1 then we timed out so close the socket, clean up and
//			go back to waiting for connection
//
         Close_Bluetooth_port();
         m_pMainGui->wPrintf(L"Connection closed \n");
         //
         //		Uncheck connected box
         //
         //btconn->SetSel(0, -1);
         //btconn->Clear();

         PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
         Sleep(2000);
         PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);
         if (m_pMainGui->m_pCurrentProgressDlg)
         {
            m_pMainGui->m_pCurrentProgressDlg->PostMessage(WMU_PLEASEDLG_COMPLETE, 0, 0);
         }
         return(-1);
      }
      continue;
   }

   m_pMainGui->wPrintf(_T("Send FFFFFF address"));


   for (x = 0; x < 100; ++x)
      buffer[x] = 0;

   buffer[0] = 'F';
   buffer[1] = 'F';
   buffer[2] = 'F';
   buffer[3] = 'F';
   buffer[4] = 'F';
   buffer[5] = 'F';

   Crc8 = 0;
   for (x = 0; x < 13; ++x)
      UpdateCRC8(buffer[x]);
   buffer[13] = Crc8;

   sent = WriteBluetooth(&buffer[0], 14, 0);

   Sleep(2000);

   //
   // 	Close the socket
   //
   Close_Bluetooth_port();

   PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
   Sleep(2000);
   PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);

//
//	Reopen Bluetooth port
//
   Reopen_Bluetooth_port();
   //
   //	Now lets sleep a bit and wait for the infrared code to write the new int vectors
   //	then switch to the loader code then we'll re-connect the Bluetooth
   //
   Sleep(10000);


   for (;;)
   {
      x = Connect_To_Bluetooth_Device();
      if (x == 1)
         break;
   }

   test[0] = 254;
   test[1] = 0;
   test[2] = 0;
   //btconn->SetWindowText((LPCTSTR)&test[0]);

   PlaySound(TEXT("c:\\BascomTurnerMLDS\\program\\ding.wav"), NULL, SND_ASYNC | SND_LOOP);
   Sleep(2000);
   PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP);


   m_pMainGui->wPrintf(_T("Done, going idle"));
   Mode = Idle;		// idle
   fclose(Loader_Stream);

   if (Running_Mode == 'I')
   {
      Sleep(200);
      read_flashf_block();
      Sleep(200);
      read_flashc_block();
      m_pMainGui->wPrintf(_T("Send App Ready cmd"));
      wb[0] = 'R';
      wb[1] = 'e';
      wb[2] = 'a';
      wb[3] = 'd';
      wb[4] = 'y';
      sent = WriteBluetooth(&wb[0], 5, 0);
      Sleep(200);//might need to increase sleep
      process_flashc_block();
      Sleep(200);
      process_flashf_block();
      Sleep(200);
   }

   if (m_pMainGui->m_pCurrentProgressDlg)
   {
      m_pMainGui->m_pCurrentProgressDlg->PostMessage(WMU_PLEASEDLG_COMPLETE, 0, 0);
   }
   return 1;
}


void CBlueTooth::convert_microchip_hex_file()
{
   unsigned char buffer[100];
   int x, i;
   unsigned char buff[100];
   unsigned char saved_top_word[5];
   unsigned int address, saved_address, temp;

   FILE* Stream = fopen(m_szFWFileProduction, "r");// "c:\\BascomTurnerMLDS\\Program\\Infrared-Gas-Project.production.hex", "r");
   FILE* Stream1 = fopen(m_szFWFile, "wb");// "c:\\BascomTurnerMLDS\\Program\\Infrared.hex", "wb");

   for (x = 0; x < 100; ++x)
      buffer[x] = 0;

   for (x = 0; x < 100; ++x)
      buff[x] = 0;

   for (;;)
   {
      i = 0;
      for (;;)
      {
         x = fread(&buffer[i], 1, 1, Stream);
         if (x <= 0)
         {
            fclose(Stream);
            fclose(Stream1);
            return;
         }
         if (buffer[i] == 0x0A)
         {
            buffer[i] = 0;
            break;
         }
         i = i + 1;
      }
      //
      //	Get top 2 bytes of address and save
      //
      if (buffer[7] == '0' && buffer[8] == '4')
      {
         saved_top_word[0] = buffer[9];
         saved_top_word[1] = buffer[10];
         saved_top_word[2] = buffer[11];
         saved_top_word[3] = buffer[12];
         saved_top_word[4] = 0;

         continue;
      }

      //
      //	Get lower 2 bytes of address and do conversion. Need to put together
      //	an int then divide by 2 and we have our address.
      //
      if (buffer[7] == '0' && buffer[8] == '0')
      {
         buff[0] = saved_top_word[0];
         buff[1] = saved_top_word[1];
         buff[2] = saved_top_word[2];
         buff[3] = saved_top_word[3];

         buff[4] = buffer[3];
         buff[5] = buffer[4];
         buff[6] = buffer[5];
         buff[7] = buffer[6];

         buff[8] = 0;

         sscanf((const char *)&buff[0], "%lx", (const char *)&saved_address);
         saved_address = saved_address / 2;
      }
      //
      //			One data word
      //
      if (buffer[1] == '0' && buffer[2] == '4')
      {
         sprintf((char *const)&buff[0], "%06x", saved_address);

         buff[6] = ' ';

         buff[11] = buffer[9];
         buff[12] = buffer[10];
         buff[9] = buffer[11];
         buff[10] = buffer[12];
         buff[7] = buffer[13];
         buff[8] = buffer[14];

         buff[13] = 0;
         printf("%s\n", &buff[0]);

         buff[13] = 0x0D;
         buff[14] = 0x0A;
         buff[15] = 0;

         fwrite(buff, 1, 15, Stream1);
      }
      //
      //			Two data words
      //
      if (buffer[1] == '0' && buffer[2] == '8')
      {
         sprintf((char *const)&buff[0], "%06x", saved_address);

         buff[6] = ' ';

         buff[11] = buffer[9];
         buff[12] = buffer[10];
         buff[9] = buffer[11];
         buff[10] = buffer[12];
         buff[7] = buffer[13];
         buff[8] = buffer[14];
         buff[13] = ' ';

         buff[18] = buffer[17];
         buff[19] = buffer[18];
         buff[16] = buffer[19];
         buff[17] = buffer[20];
         buff[14] = buffer[21];
         buff[15] = buffer[22];

         buff[20] = 0;
         printf("%s\n", &buff[0]);

         buff[20] = 0x0D;
         buff[21] = 0x0A;
         buff[22] = 0;

         fwrite(buff, 1, 22, Stream1);
      }
      //
      //			Three data words
      //???
      if (buffer[1] == '0' && buffer[2] == 'c')
      {
         sprintf((char *const)&buff[0], "%06x", saved_address);

         buff[6] = ' ';

         buff[11] = buffer[9];
         buff[12] = buffer[10];
         buff[9] = buffer[11];
         buff[10] = buffer[12];
         buff[7] = buffer[13];
         buff[8] = buffer[14];
         buff[13] = ' ';

         buff[18] = buffer[17];
         buff[19] = buffer[18];
         buff[16] = buffer[19];
         buff[17] = buffer[20];
         buff[14] = buffer[21];
         buff[15] = buffer[22];
         buff[20] = ' ';

         buff[25] = buffer[25];
         buff[26] = buffer[26];
         buff[23] = buffer[27];
         buff[24] = buffer[28];
         buff[21] = buffer[29];
         buff[22] = buffer[30];
         //steve
         buff[27] = 0;
         printf("%s\n", &buff[0]);

         buff[27] = 0x0D;
         buff[28] = 0x0A;
         buff[29] = 0;

         fwrite(buff, 1, 29, Stream1);
      }
      //
      //			Four data words
      //
      if (buffer[1] == '1' && buffer[2] == '0')
      {
         sprintf((char *const)&buff[0], "%06x", saved_address);

         buff[6] = ' ';

         buff[11] = buffer[9];
         buff[12] = buffer[10];
         buff[9] = buffer[11];
         buff[10] = buffer[12];
         buff[7] = buffer[13];
         buff[8] = buffer[14];
         buff[13] = ' ';

         buff[18] = buffer[17];
         buff[19] = buffer[18];
         buff[16] = buffer[19];
         buff[17] = buffer[20];
         buff[14] = buffer[21];
         buff[15] = buffer[22];
         buff[20] = ' ';

         buff[25] = buffer[25];
         buff[26] = buffer[26];
         buff[23] = buffer[27];
         buff[24] = buffer[28];
         buff[21] = buffer[29];
         buff[22] = buffer[30];
         buff[27] = ' ';

         buff[32] = buffer[33];
         buff[33] = buffer[34];
         buff[30] = buffer[35];
         buff[31] = buffer[36];
         buff[28] = buffer[37];
         buff[29] = buffer[38];

         buff[34] = 0;
         printf("%s\n", &buff[0]);

         buff[34] = 0x0D;
         buff[35] = 0x0A;
         buff[36] = 0;

         fwrite(buff, 1, 36, Stream1);
      }
   }
}

void CBlueTooth::Preprocess_File(FILE* Loader_Stream)
{
   int x, y, z, nbytes, i, sent;
   unsigned char buffer[100], buff2[50], buff1[50];
   unsigned char wb[20];
   unsigned char temp[10];
   unsigned int address, data1, data2, data3, data4;
   unsigned short test[3];
   int progress, prog, saved_prog;

   prog = 0;
   saved_prog = 0;
   program_progress = 0;

   for (x = 0; x < 100; ++x)
      buffer[x] = 0;

   for (;;)
   {
      x = 0;
      for (;;)
      {
         i = fread(&buffer[x], 1, 1, Loader_Stream);
         if (i <= 0)
            break;
         if (buffer[x] == 0x0a)
            break;
         x = x + 1;
      }

      if (i <= 0)
         break;
      //
      //		One data word
      //
      if (x == 13)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);
      }
      //
      //		Two data words
      //
      if (x == 20)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);
      }
      //
      //		Three data word
      //???
      if (x == 27)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);

         temp[0] = buffer[21];
         temp[1] = buffer[22];
         temp[2] = buffer[23];
         temp[3] = buffer[24];
         temp[4] = buffer[25];
         temp[5] = buffer[26];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data3);
      }
      //
      //		Four data word
      //
      if (x == 34)
      {
         temp[0] = buffer[0];
         temp[1] = buffer[1];
         temp[2] = buffer[2];
         temp[3] = buffer[3];
         temp[4] = buffer[4];
         temp[5] = buffer[5];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&address);

         temp[0] = buffer[7];
         temp[1] = buffer[8];
         temp[2] = buffer[9];
         temp[3] = buffer[10];
         temp[4] = buffer[11];
         temp[5] = buffer[12];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data1);

         temp[0] = buffer[14];
         temp[1] = buffer[15];
         temp[2] = buffer[16];
         temp[3] = buffer[17];
         temp[4] = buffer[18];
         temp[5] = buffer[19];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data2);

         temp[0] = buffer[21];
         temp[1] = buffer[22];
         temp[2] = buffer[23];
         temp[3] = buffer[24];
         temp[4] = buffer[25];
         temp[5] = buffer[26];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data3);

         temp[0] = buffer[28];
         temp[1] = buffer[29];
         temp[2] = buffer[30];
         temp[3] = buffer[31];
         temp[4] = buffer[32];
         temp[5] = buffer[33];
         temp[6] = 0;
         sscanf((const char *)&temp[0], "%x", (const char *)&data4);
      }
      //
      //	Skip this range. This is the loaders C-Lib area
      //
      if (address >= 0x3800 && address < 0x7000)
      {
         for (x = 0; x < 100; ++x)
            buffer[x] = 0;
         continue;
      }

      if (address >= 0x13800 && address < 0x157fe)
      {
         for (x = 0; x < 100; ++x)
            buffer[x] = 0;
         continue;
      }

      for (x = 0; x < 100; ++x)
         buffer[x] = 0;

      program_progress = program_progress + 1;
   }
}

