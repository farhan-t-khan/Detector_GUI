#pragma once

#define SETTINGS_FILE _T("Settings.txt")




struct structFileName
{
   bool bDate;
   bool bTime;
   CString sLocation;
};

#ifdef SIMULATE_HW
struct structConnections
{
   structConnections() : bBluetooth(false), bLaser(false), bBattery(false), bGPS(false) {};
   int nRefreshConnectionMS;
   bool bBluetooth;
   bool bLaser;
   bool bBattery;
   bool bGPS;
};
#else
struct structHWConnections
{
	structHWConnections() : bBluetooth(false), bLaser(false), bBattery(false), bGPS(false) {};
	int nRefreshConnectionMS;
	bool bBluetooth;
	bool bLaser;
	bool bBattery;
	bool bGPS;
};
#endif

class CSettings
{

   CSettings();
   CSettings(CSettings const&);
   void operator=(CSettings const&);
   CString m_sWD;
public:
   static CSettings* GetInstance(void);
   ~CSettings();
   void ReadSettings(void);
   void SaveCurrentCalTime(CTime& time);
   inline void SaveSetting(const CString& sSection, const CString& sKey, const CString& sVal)
   {
      WritePrivateProfileString(sSection, sKey, sVal, m_sWD + SETTINGS_FILE);
   }

   void RemoveBackSlash(CString& sPath)
   {
      while (sPath.GetLength() > 0 && sPath[sPath.GetLength() - 1] == _T('\\'))
      {
         sPath.Delete(sPath.GetLength() - 1);
      }
   };

   void SaveSurveyDataPath(CString sVal)
   {
      RemoveBackSlash(sVal);
      m_sSurveyDataPath = sVal;
      CString sSection(_T("General"));
      CString sKey(_T("SurveyDataPath"));
      SaveSetting(sSection, sKey, sVal);
      VerifyDirectories();
   }
   void SaveCalPath(CString sVal)
   {
      RemoveBackSlash(sVal);
      m_sCalDataPath = sVal;
      CString sSection(_T("Calibration"));
      CString sKey(_T("DataPath"));
      SaveSetting(sSection, sKey, sVal);
      VerifyDirectories();
   }
   void SaveAlarmWavFile(CString sVal)
   {
      m_sWavFile = sVal;
      CString sSection(_T("Alarm"));
      CString sKey(_T("WavFile"));
      SaveSetting(sSection, sKey, sVal);
   }
   void SaveAlarmLevel(int nVal)
   {
      m_nAlarmLevel = nVal;
      CString sVal;
      sVal.Format(_T("%d"), nVal);
      CString sSection(_T("Alarm"));
      CString sKey(_T("AlarmLevel"));
      SaveSetting(sSection, sKey, sVal);
   }

   void Save24hClock(bool bVal)
   {
      m_b24hClock = bVal;
      CString sVal;
      sVal.Format(_T("%d"), bVal);
      CString sSection(_T("General"));
      CString sKey(_T("24hClock"));
      SaveSetting(sSection, sKey, sVal);
   }

   void SaveFileNameLocation(CString& sVal)
   {
      m_fileNameInfo.sLocation = sVal;
      CString sSection(_T("FileNameInfo"));
      CString sKey(_T("Location"));
      SaveSetting(sSection, sKey, sVal);
   }

   void SaveFileNameDate(bool bVal)
   {
      m_b24hClock = bVal;
      CString sVal;
      sVal.Format(_T("%d"), bVal);
      CString sSection(_T("FileNameInfo"));
      CString sKey(_T("Date"));
      SaveSetting(sSection, sKey, sVal);
   }
   void SaveFileNameTime(bool bVal)
   {
      m_b24hClock = bVal;
      CString sVal;
      sVal.Format(_T("%d"), bVal);
      CString sSection(_T("FileNameInfo"));
      CString sKey(_T("Time"));
      SaveSetting(sSection, sKey, sVal);
   }

   void SaveSubstractBrk(bool bVal)
   {
      m_bSubtractBackground = bVal;
      CString sVal;
      sVal.Format(_T("%d"), bVal);
      CString sSection(_T("General"));
      CString sKey(_T("SubtractBackground"));
      SaveSetting(sSection, sKey, sVal);
   }
   void SaveXAxis(int nVal)
   {
      m_nXAxis = nVal;
      CString sVal;
      sVal.Format(_T("%d"), nVal);
      CString sSection(_T("General"));
      CString sKey(_T("XAxis"));
      SaveSetting(sSection, sKey, sVal);
   }
   void SaveYAxis(int nVal)
   {
      m_nXAxis = nVal;
      CString sVal;
      sVal.Format(_T("%d"), nVal);
      CString sSection(_T("General"));
      CString sKey(_T("YAxis"));
      SaveSetting(sSection, sKey, sVal);
   }
   void SaveCalTimeDue(int nVal);
   void VerifyDirectories(void);
   void ResetCFlashFlag(void);

   structFileName m_fileNameInfo;
   CString m_sSurveyDataPath;
   CString m_sCalDataPath;
   CString m_sWavFile;
   CString m_sWavFile2;
   int m_nXAxis;
   int m_nYAxis;
   bool m_bSubtractBackground;
   int m_nCalDuePeriod;
   int m_nAlarmLevel;
   int m_nAlarmVolume;
   bool m_b24hClock;
   bool m_bResetCFlash;
   CTime m_TimeNextCal;
   int m_nRefreshConnectionMS;
   int graph_ppm;
   int m_nCalibrationCell;
   char m_szHeader[128];
   char m_szcalrecord[64];

#ifdef SIMULATE_HW
   bool CalibrationSucess()
   {
      TCHAR szBuffer[32];
      GetPrivateProfileString(_T("Debug"), _T("CalibrationSucess"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
      bool bResult = !!_tstoi(szBuffer);
      return bResult;
   }
   bool BumpTestSucess()
   {
      TCHAR szBuffer[32];
      GetPrivateProfileString(_T("Debug"), _T("BumpTestSucess"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
      bool bResult = !!_tstoi(szBuffer);
      return bResult;
   }
   int ppm_Measurement()
   {
      return graph_ppm;
      //TCHAR szBuffer[32];
      //GetPrivateProfileString(_T("Debug"), _T("ppm_Measurement"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
      //return _tstoi(szBuffer);
   }
   structConnections m_ConnectionStatus;
   void ReadConnectionStatus(void);
   CString m_sStabilizingTimer;
#else
   bool HWCalibrationSucess()
   {
	   TCHAR szBuffer[32];
	   GetPrivateProfileString(_T("Debug"), _T("CalibrationSucess"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
	   bool bResult = !!_tstoi(szBuffer);
	   return bResult;
   }
   bool HWBumpTestSucess()
   {
	   TCHAR szBuffer[32];
	   GetPrivateProfileString(_T("Debug"), _T("BumpTestSucess"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
	   bool bResult = !!_tstoi(szBuffer);
	   return bResult;
   }
   int HWppm_Measurement()
   {
	   TCHAR szBuffer[32];
	   GetPrivateProfileString(_T("Debug"), _T("ppm_Measurement"), _T("1"), szBuffer, ARRAYSIZE(szBuffer), m_sWD + SETTINGS_FILE);
	   return _tstoi(szBuffer);
   }
   structHWConnections m_HWConnectionStatus;
   void ReadHWConnectionStatus(void);
   CString m_sHWStabilizingTimer;

#endif
};

