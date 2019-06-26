#pragma once
#include "StdAfx.h"

class WndHelper
{
public:
	WndHelper(void);
	~WndHelper(void);

public:
	static CBaseWnd* CreateLogonWnd();
	static CBaseWnd* BackLogonWnd();
	static CBaseWnd* CreateMainWnd();
	static CBaseWnd* GetMainWnd();
	static CBaseWnd* CreateMainTipWnd();
	static CBaseWnd* GetMainTipWnd();
	static CBaseWnd* CreateStateInfoWnd();
	static CBaseWnd* GetStateInfoWnd();
	static CDuiString GetNowTime();
	static BOOL CopyTextToClipboard(std::wstring text);
};

