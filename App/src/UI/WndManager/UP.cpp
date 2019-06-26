#include "StdAfx.h"
#include "UP.h"

#include "LogonWnd.h"
#include "MainWnd.h"
#include "MainTipWnd.h"
#include "StateInfoWnd.h"
#include "WbWnd.h"

void InitAppWnd()
{
	LogonWnd::RegisterWnd();
	MainWnd::RegisterWnd();
	MainTipWnd::RegisterWnd();
	StateInfoWnd::RegisterWnd();
	WbWnd::RegisterWnd();
}

void InitMsgBroadcast()
{
	MsgBroadcast::GetMsgBroadcast();
}


