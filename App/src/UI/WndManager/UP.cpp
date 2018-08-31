#include "StdAfx.h"
#include "UP.h"

#include "LogonWnd.h"
#include "MainWnd.h"
#include "MainTipWnd.h"
#include "WbWnd.h"

void InitAppWnd()
{
	LogonWnd::RegisterWnd();
	MainWnd::RegisterWnd();
	MainTipWnd::RegisterWnd();
	WbWnd::RegisterWnd();
}

void InitMsgBroadcast()
{
	MsgBroadcast::GetMsgBroadcast();
}


