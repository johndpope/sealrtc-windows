
#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <map>
#include <GdiPlus.h>
#include "..\Duilib\Duilib\UIlib.h"

#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
using namespace std;
#endif
#include "resource.h"
#include "Up_type.h"
#include "UP.h"
#include "WndManager.h"
#include "Platform.h"
#include "UpDefineMsg.h"
#include "MsgBroadcast.h"

#define APP_NAME _T("SealRTC")
#define APP_VERSION " V3.0.0"
//#define APP_Release
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)