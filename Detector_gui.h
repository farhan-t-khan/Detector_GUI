
// Detector_gui.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define  WM_APP_BASE	(WM_APP + 1)
extern const int UWM_BUMP_TEST;
extern const int UWM_LASER_FAIL;
extern const int UWM_UPDATE_GRAPH;
// CDetectorguiApp:
// See Detector_gui.cpp for the implementation of this class
//

class CDetectorguiApp : public CWinApp
{
public:
	CDetectorguiApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CDetectorguiApp theApp;
