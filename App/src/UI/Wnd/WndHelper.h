#pragma once
#include "StdAfx.h"

class WndHelper
{
public:
	WndHelper(void);
	~WndHelper(void);

public:
	static CBaseWnd* CreateLogonWnd();
	static CBaseWnd* BackLogonWnd(CDuiString strChannelId);
	static CBaseWnd* CreateMainWnd(CDuiString strChannelId, CDuiString strName);
	static CBaseWnd* GetMainWnd();
	static CBaseWnd* CreateMainTipWnd();
	static CBaseWnd* GetMainTipWnd();
	static CBaseWnd* ShowWbWnd(CDuiString strUrl);
	static CBaseWnd* GetWbWnd();
	static CDuiString GetNowTime();
	static BOOL CopyTextToClipboard(std::wstring text);
};

