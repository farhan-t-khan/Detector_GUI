
// stdafx.cpp : source file that includes just the standard includes
// Detector_gui.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

SYSTEMTIME SystemTimeDiff(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl)
{
   SYSTEMTIME t_res;
   FILETIME v_ftime;
   ULARGE_INTEGER v_ui;
   __int64 v_right, v_left, v_res;
   SystemTimeToFileTime(&pSr, &v_ftime);
   v_ui.LowPart = v_ftime.dwLowDateTime;
   v_ui.HighPart = v_ftime.dwHighDateTime;
   v_right = v_ui.QuadPart;

   SystemTimeToFileTime(&pSl, &v_ftime);
   v_ui.LowPart = v_ftime.dwLowDateTime;
   v_ui.HighPart = v_ftime.dwHighDateTime;
   v_left = v_ui.QuadPart;

   v_res = v_right - v_left;

   v_ui.QuadPart = v_res;
   v_ftime.dwLowDateTime = v_ui.LowPart;
   v_ftime.dwHighDateTime = v_ui.HighPart;
   FileTimeToSystemTime(&v_ftime, &t_res);
   return t_res;
}

__int64 Timediff(const SYSTEMTIME& pSr, const SYSTEMTIME& pSl)
{
   FILETIME v_ftime;
   ULARGE_INTEGER v_ui;
   __int64 v_right, v_left, v_res;
   SystemTimeToFileTime(&pSr, &v_ftime);
   v_ui.LowPart = v_ftime.dwLowDateTime;
   v_ui.HighPart = v_ftime.dwHighDateTime;
   v_right = v_ui.QuadPart;

   SystemTimeToFileTime(&pSl, &v_ftime);
   v_ui.LowPart = v_ftime.dwLowDateTime;
   v_ui.HighPart = v_ftime.dwHighDateTime;
   v_left = v_ui.QuadPart;
   v_res = v_right - v_left;

   return v_res;
}