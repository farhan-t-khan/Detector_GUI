#pragma once
#include "Settings.h"
#include <ws2bth.h>
#include "EnumDevices.h"
#include "CDlgSelectBT.h"

class CDetectorguiDlg;
#define BT_MAX_INQUIRY_RETRY	5
#define BT_SUCCESS		0
#define BT_ERROR		1
#define BTArraySize 		10
enum {
   CH4_Sample = 0, Tec_Temp, Tec_Current, Tec_Error, avg_min_2F,
   avg_max_2F, avg_min_1F, avg_max_1F, Battery_Voltage, Laser_Temperature,
   Receiver_temperature, Laser_Humidity, Receiver_Humidity, avg_minlocation1, avg_maxlocation1,
   avg_minlocation2, avg_maxlocation2, Laser_Current, DATA_SIZE
};

extern TCHAR g_szLabels[][64];

struct structCalRecord
{
   enum {CAL_MAX_SIZE = 25, STRING_SIZE = 128};
   char InstrumentRecord[STRING_SIZE] = { "" };
   char CalRecord[CAL_MAX_SIZE][STRING_SIZE] = { "" };
   int m_nCount = 0;
   void Insert(char* szNewCal);
   void WriteToFlashf(char* Flash_Block, int nMaxSize);
   void WriteToFlashc(char* Flash_Block, int nMaxSize);
   bool m_WriteflashInProgress = false;
};

struct BlueToothList
{
   char	mac[20];
   //int	NameSnCount;
   CString name;
   //TCHAR	name[100];
};

class CBlueTooth
{
   CBlueTooth();
   CSettings& m_Settings = *CSettings::GetInstance();
protected:
   SOCKADDR_BTH	m_RemoteBthAddr = { 0 };
   bool ReConnect(void);
public:
   static CCriticalSection m_st_CS;
   //	Stop_Sampling		Mode = 0
   //	Start_Sampling		Mode = 1
   //	Idle			Mode = 2
   //	Calibrate		Mode = 3
   //	Dump_Data		Mode = 4;
   //	Download Code		Mode = 5;
   //	Read Flash Block	Mode = 6;
   //	Write Flash Block	Mode = 7;
   enum Status
   {
      Stop_Sampling = 0, Start_Sampling, Idle, Calibrate, Dump_Data, Download_Code, Read_Flashf_Block, Read_Flashc_Block, Write_Flashc_Block,
      Exit_Software
   };
   static CBlueTooth& GetInstance(void);
   CDetectorguiDlg* m_pMainGui = 0;
	~CBlueTooth();
	//SOCKET 		BtSocket = INVALID_SOCKET;
	bool btConnected;
	int Mode = Stop_Sampling;
   int saved_mode = 0;
   HANDLE 			bluetooth_port = 0;
   HANDLE 			gps_port = 0;
   int m_blu = 0;
   int m_gps = 0;
   bool m_bGPSDetect = false;
   unsigned char m_endpoint = 0;
   unsigned 	char Running_Mode;
   unsigned 	char bt_address[6];
   static void CallbackWrapper(CObject* pObj, const CEnumDevInfo *pInfo);
   void ProcessComport(const CEnumDevInfo *pInfo);
   int ScanForBTandGPS();
   CDlgSelectBT m_dlgSelectBT;
   int scan_bluetooth_devices();
   void Read_Lasers_Txt();
   int Connect_To_Bluetooth_Device();
   void Close_Bluetooth_port();
   void Reopen_Bluetooth_port();

   void SetGPSLED(int nOn);
   void FlashGPSLED();
   //void InitGPS();
   int m_nGSPSLEDState = 0;
   clock_t m_tGPSLEDTimer;
	// runs in initdialoge to connect bluetooth device on startup
	int Look_For_Bluetooth_DevicesOLD();
   int CalibrateFunc(void);
	int ReadBluetooth(unsigned char *ptr);
   int WriteBluetooth(unsigned char *ptr, int nbytes, int dummy);
	void Send_CONFIG_Parameters();
   void Request_Bump();
   void Request_CalP();
	void flush();
	int Sample_Data_And_Display(HANDLE hCom);
	friend UINT App_Thread(LPVOID pParam);
	void read_flashf_block();
   void read_flashc_block();
	void write_flashf_block();
   void write_flashc_block();
   void process_flashc_block();
   void process_flashf_block();
   bool IsFlashcBlockEmpty();
   bool IsFlashfBlockEmpty();
   void init_flashc_block();
   void init_flashf_block();
   structCalRecord m_CalRecords;
   unsigned char Flashf_Block[1536] = { 0 };
   unsigned char Flashc_Block[1536] = { 0 };
   unsigned char FlashPage[1536] = { 0 };
   short int m_aData[DATA_SIZE] = { 0 };
	static void UpdateCRC8(unsigned char c);
   char m_szFWFile[MAX_PATH] = "c:\\BascomTurnerMLDS\\Program\\Infrared.hex";
   char m_szFWFileProduction[MAX_PATH] = "c:\\BascomTurnerMLDS\\Program\\Infrared-Gas-Project.production.hex";
	int download_new_program();
   DOUBLE extrapolateGPS(DOUBLE GpsL2, DOUBLE GpsL1, DWORD dwTime2, DWORD dwTime1, DWORD dwTimeR);
 //  BOOL ExtractGPSBuffer(char* sGPSBuffer, CString GPSTime, CString GPSLat, CString GPSLon, CString GPSHDOP, CString GPSnSat);
   int program_progress = 0;
   void convert_microchip_hex_file();
   void Preprocess_File(FILE* Loader_Stream);
};

