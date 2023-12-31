// SingleInstanceApp.cpp : implementation file
//
// CWinApp with single-instance support
//
// Copyright (C) 1997, 1998 Giancarlo Iovino (giancarlo@saria.com)
// All rights reserved. May not be sold for profit.
//
// Thanks to Kevin Lussier for the base idea.
//
// This code was developed for MFC Programmers SourceBook
// (http://www.codeguru.com)
//

#include "stdafx.h"
#include "SingleInstanceApp.h"
#define _AFX_NO_OLE_SUPPORT
#if _MSC_VER >= 1300
   #include <..\src\mfc\afximpl.h>
#else
   #include <../mfc/src/afximpl.h>
#endif


CSingleInstanceApp::CSingleInstanceApp()
{
#ifndef _DEBUG
   ELCTRY
#endif

   // Set our default values
   m_hMutex = NULL;

#ifndef _DEBUG  
   ELCCATCH
#endif
}

//***************************************************************************************

CSingleInstanceApp::~CSingleInstanceApp()
{
#ifndef _DEBUG
   ELCTRY
#endif

   // Release the mutex
   if ( m_hMutex != NULL ) 
   {
      ReleaseMutex( m_hMutex );
   }

#ifndef _DEBUG  
   ELCCATCH
#endif
}

//***************************************************************************************

BOOL CSingleInstanceApp::CheckSingleInstance(UINT nID) 
{
#ifndef _DEBUG
   ELCTRY
#endif

   CString strFullString;

   // Generate a synthetic name for this class using the
   // AfxRegisterWndClass convention
   HINSTANCE hInst   = AfxGetInstanceHandle();
   UINT nClassStyle  = CS_DBLCLKS;  
   HCURSOR hCursor   = LoadCursor(IDC_ARROW );
   HICON hIcon       = LoadIcon(MAKEINTRESOURCE( nID ) );   

   LPCTSTR lpstrAppName = AfxGetAppName();
   ASSERT(lpstrAppName != NULL);

   if (hCursor == NULL && hIcon == NULL)
   {
      m_strClassName.Format(_T("%s:%x:%x"), lpstrAppName, hInst, nClassStyle);
   }
   else
   {
      m_strClassName.Format(_T("%s:%x:%x:%x:%x"), lpstrAppName, hInst, nClassStyle, hCursor, (UINT)NULL);
   }
   
   // Create the mutex with the class name
   m_hMutex = CreateMutex(NULL, FALSE, m_strClassName);

   // Check for errors
   if ( GetLastError() == ERROR_ALREADY_EXISTS ) 
   {
      // Reset our mutex handle (just in case)
      m_hMutex = NULL;
      // The mutex already exists: an instance is already
      // running. Find the app window and bring it to foreground
      HWND hWnd = FindWindowEx(NULL, NULL, m_strClassName, NULL);
      if ( hWnd != NULL ) 
      {
         ShowWindow( hWnd, SW_RESTORE );
         BringWindowToTop( hWnd );
         SetForegroundWindow( hWnd );
      }
      return FALSE;
   }

   // Register the unique window class name
   WNDCLASS wndclass;   
   ZeroMemory(&wndclass, sizeof(WNDCLASS));
 
   wndclass.style = nClassStyle;
   wndclass.lpfnWndProc = AfxWndProc;
   wndclass.hInstance = hInst;
   wndclass.hIcon = hIcon; 
   wndclass.hCursor = hCursor;
   wndclass.hbrBackground = NULL;
   wndclass.lpszMenuName = NULL;
   wndclass.lpszClassName = m_strClassName; // The class name
   
   // Use AfxRegisterClass to register the class, exit if it fails
   if(!AfxRegisterClass(&wndclass)) 
   {
      AfxMessageBox( _T("Failed to register window class!"), MB_ICONSTOP | MB_OK );
      return FALSE;
   }

#ifndef _DEBUG  
   ELCCATCH
#endif
	
   // Return success
   return TRUE;
}

//***************************************************************************************

CString CSingleInstanceApp::GetClassName() const
{
   return m_strClassName;
}

//***************************************************************************************
//***************************************************************************************
