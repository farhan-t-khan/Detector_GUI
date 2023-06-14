#include "stdafx.h"
#include "Settings.h"
#include "IniEx.h"

static CCriticalSection g_ConfigurationCS;
CSettings* CSettings::GetInstance(void)
{
   CSingleLock singleLock(&g_ConfigurationCS, TRUE);
   static CSettings st_Settings;
   return &st_Settings;
}

CSettings::CSettings()
{
   m_fileNameInfo.bDate = true;
   m_fileNameInfo.bTime = true;
   m_fileNameInfo.sLocation = _T("Brockton");
   m_sSurveyDataPath = _T("c:\\BascomTurnerMLDS\\Data\\Survey");
   m_sCalDataPath = _T("c:\\BascomTurnerMLDS\\Data\\Calibration");
   m_sWavFile = _T("c:\\BascomTurnerMLDS\\Foghorn.wav");
   m_sWavFile2 = _T("c:\\BascomTurnerMLDS\\Flatline.wav");
   m_nXAxis = 30;
   m_nYAxis = 100;
   m_bSubtractBackground = true;
   m_nCalDuePeriod = 90;
   m_nAlarmLevel = 20;
   m_nAlarmVolume = 50;
   m_b24hClock = true;
   m_bResetCFlash = false;
}


CSettings::~CSettings()
{
}

bool g_ResetCFlash;

#ifdef SIMULATE_HW
void CSettings::ReadConnectionStatus(void)
{
   CIniEx iniFile;
   if (!iniFile.Open(SETTINGS_FILE, FALSE, TRUE, TRUE, FALSE))
   {

      return;
   }
   CString sBuffer;
   CString sSection = _T("Debug");
   CString sKey = _T("Bluetooth");
   //sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   //m_ConnectionStatus.bBluetooth = !!_tstoi(sBuffer);

   sKey = _T("Laser");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_ConnectionStatus.bLaser = !!_tstoi(sBuffer);

   sKey = _T("GPS");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_ConnectionStatus.bGPS = !!_tstoi(sBuffer);

   sKey = _T("Battery");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_ConnectionStatus.bBattery = !!_tstoi(sBuffer);

   sKey = _T("RefreshConnection_ms");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("2000"));
   m_nRefreshConnectionMS = _tstoi(sBuffer);

   sKey = _T("StabilizingTimer");
   m_sStabilizingTimer = iniFile.GetValue(sSection, sKey, _T("20"));
   
}
#else

void CSettings::ReadHWConnectionStatus()
{
	// TODO: Add your implementation code here.
}

#endif
void CSettings::ReadSettings(void)
{
   CIniEx iniFile;
   TCHAR* buffer;
   // Get the current working directory: 
   if ((buffer = _tgetcwd(NULL, 0)) == NULL)
   {
      MessageBox(NULL,_T("_getcwd error"), _T("ERROR"), MB_OK);
   }
  else
   {
      m_sWD = buffer;
      //m_sWD = _T('c:\\BascomTurnerMLDS');
      m_sWD += _T("\\");
      free(buffer);
   }

   if (!iniFile.Open(SETTINGS_FILE, FALSE, TRUE, TRUE, FALSE))
   {
      
      return;
   }
   CString sBuffer;
   CString sSection = _T("FileNameInfo");
   CString sKey = _T("Date");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_fileNameInfo.bDate = !!_tstoi(sBuffer);
   sKey = _T("Time");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_fileNameInfo.bTime = !!_tstoi(sBuffer);
   sKey = _T("Location");
   sBuffer = iniFile.GetValue(sSection, sKey, m_fileNameInfo.sLocation);
   m_fileNameInfo.sLocation = sBuffer;

   sSection = _T("Calibration");
   sKey = _T("DataPath");
   sBuffer = iniFile.GetValue(sSection, sKey, m_sCalDataPath);
   m_sCalDataPath = sBuffer;
   sKey = _T("CalDuePeriod(days)");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("90"));
   m_nCalDuePeriod = _tstoi(sBuffer);
   sKey = _T("LastCalibration");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("90"));

   int nYear = 2018, nMonth = 1, nDay = 1, nHour = 1, nMinute = 0;
   if (_stscanf(sBuffer, _T("%d-%d-%d %d:%d"), &nMonth, &nDay, &nYear, &nHour, &nMinute) == 5)
   {
      CTime time(nYear, nMonth, nDay, nHour, nMinute, 0);
      CTimeSpan calPeriod(m_nCalDuePeriod, 0, 0, 0);
      m_TimeNextCal = time + calPeriod;
   }
   else
   {
      CTime time = CTime::GetCurrentTime();
      CTimeSpan calPeriod(m_nCalDuePeriod, 0, 0, 0);
      m_TimeNextCal = time - calPeriod;
   }
   sKey = _T("CalibrationCell");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("50"));
   m_nCalibrationCell = _tstoi(sBuffer);

   sKey = _T("ResetCFlashOnStart");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("0"));
   if  (_tstoi(sBuffer)==0)
      m_bResetCFlash = false;
   else
      m_bResetCFlash = true;
  
   sSection = _T("General");
   sKey = _T("SurveyDataPath");
   sBuffer = iniFile.GetValue(sSection, sKey, m_sSurveyDataPath);
   m_sSurveyDataPath = sBuffer;

   sKey = _T("24hClock");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_b24hClock = !!_tstoi(sBuffer);

   sKey = _T("SubtractBackground");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("1"));
   m_bSubtractBackground = !!_tstoi(sBuffer);

   sKey = _T("XAxis");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("30"));
   m_nXAxis = _tstoi(sBuffer);

   sKey = _T("YAxis");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("100"));
   m_nYAxis = _tstoi(sBuffer);

   sSection = _T("Alarm");
   sKey = _T("WavFile");
   sBuffer = iniFile.GetValue(sSection, sKey, m_sWavFile);
   m_sWavFile = sBuffer;

   sKey = _T("AlarmLevel");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("20"));
   m_nAlarmLevel = _tstoi(sBuffer);

   sKey = _T("AlarmVolume");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("50"));
   m_nAlarmVolume = _tstoi(sBuffer);

   sKey = _T("RefreshConnection_ms");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("2000"));
   m_nRefreshConnectionMS = _tstoi(sBuffer);

   sSection = _T("Flash");
   sKey = _T("header");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("BASCOMTURNERMLDS,01/01/2019,HWV-1.10,205,2390,12,04"));
   CT2A header(sBuffer);
   strncpy(m_szHeader, header, ARRAYSIZE(m_szHeader) - 1);
   sKey = _T("calrecord");
   sBuffer = iniFile.GetValue(sSection, sKey, _T("12-4-2018,45,43,1.0,45"));
   CT2A calrecord(sBuffer);
   strncpy(m_szcalrecord, calrecord, ARRAYSIZE(m_szcalrecord) - 1);


   VerifyDirectories();
}

void CSettings::SaveCurrentCalTime(CTime& time)
{
   CString sSection = _T("Calibration");
   CString sKey = _T("LastCalibration");
   CString sVal;
   sVal.Format(_T("%d-%d-%d %d:%d"), time.GetMonth(), time.GetDay(), time.GetYear(), time.GetHour(), time.GetMinute());
   SaveSetting(sSection, sKey, sVal);
   CTimeSpan calPeriod(m_nCalDuePeriod, 0, 0, 0);
   m_TimeNextCal = time + calPeriod;
}

void CSettings::SaveCalTimeDue(int nVal)
{
   int nCalDuePeriodPrev = m_nCalDuePeriod;
   m_nCalDuePeriod = nVal;
   CString sVal;
   sVal.Format(_T("%d"), nVal);
   CString sSection(_T("Calibration"));
   CString sKey(_T("CalDuePeriod(days)"));
   SaveSetting(sSection, sKey, sVal);

   CTimeSpan calPeriod(m_nCalDuePeriod - nCalDuePeriodPrev, 0, 0, 0);
   TRACE(_T("%d-%d-%d\n"), m_TimeNextCal.GetMonth(), m_TimeNextCal.GetDay(), m_TimeNextCal.GetYear());
   m_TimeNextCal = m_TimeNextCal - calPeriod;
   TRACE(_T("%d-%d-%d\n"), m_TimeNextCal.GetMonth(), m_TimeNextCal.GetDay(), m_TimeNextCal.GetYear());
}

void CSettings::ResetCFlashFlag(void)
{
   CString sVal = _T("0");
   CString sSection(_T("Calibration"));
   CString sKey(_T("ResetCFlashOnStart"));
   SaveSetting(sSection, sKey, sVal);
}

void CSettings::VerifyDirectories(void)
{
   if (GetFileAttributes(m_sSurveyDataPath) == INVALID_FILE_ATTRIBUTES)
   {
      SHCreateDirectoryEx(NULL, m_sSurveyDataPath, NULL);
   }
   if (GetFileAttributes(m_sCalDataPath) == INVALID_FILE_ATTRIBUTES)
   {
      SHCreateDirectoryEx(NULL, m_sCalDataPath, NULL);
   }
}

