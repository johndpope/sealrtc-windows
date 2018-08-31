#include "stdafx.h"
#include "WndHelper.h"
#include "WbWnd.h"

WndHelper::WndHelper(void)
{
}

WndHelper::~WndHelper(void)
{
}

CBaseWnd* WndHelper::CreateLogonWnd()
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_LOGON, _T("LogonWnd"),FALSE);
	if (pWnd == NULL)
	{
		pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_LOGON,_T("LogonWnd"));
		pWnd->CreateWnd(NULL, APP_NAME, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX, WS_EX_OVERLAPPEDWINDOW, CURect(0, 0, 0, 0));
		pWnd->CenterWindow();
	}
	if (pWnd)
	{
		pWnd->TopWindow();
	}
	return pWnd;
}

CBaseWnd* WndHelper::BackLogonWnd(CDuiString strChannelId)
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_LOGON, _T("LogonWnd"), FALSE);
	if (pWnd == NULL)
	{
		pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_LOGON, _T("LogonWnd"));
		CStdStringPtrMap aInitData;
		bool bFromMain = true;
		aInitData.Insert(_T("bFromMain"), (void*)&bFromMain);
		aInitData.Insert(_T("strChannelId"), (void*)&strChannelId);
		pWnd->PreCreateWindowInit(aInitData);
		pWnd->CreateWnd(NULL, APP_NAME, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX, WS_EX_OVERLAPPEDWINDOW, CURect(0, 0, 0, 0));
		pWnd->CenterWindow();
	}
	if (pWnd)
	{
		pWnd->TopWindow();
	}
	return pWnd;
}

CBaseWnd * WndHelper::CreateMainWnd(CDuiString strChannelId, CDuiString strName)
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN, _T("MainWnd"), FALSE);
	if (pWnd == NULL)
	{
		pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN, _T("MainWnd"));
		CStdStringPtrMap aInitData;
		aInitData.Insert(_T("strChannelId"), (void*)&strChannelId);
		aInitData.Insert(_T("strName"), (void*)&strName);
		pWnd->PreCreateWindowInit(aInitData);

		pWnd->CreateWnd(NULL, APP_NAME, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX, 0, CURect(0, 0, 0, 0), L"");
		pWnd->CenterWindow();
	}
	if (pWnd)
	{
		pWnd->TopWindow();
	}
	return pWnd;
}

CBaseWnd* WndHelper::GetMainWnd()
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN,_T("MainWnd"),FALSE);
	return pWnd;
}

CBaseWnd * WndHelper::CreateMainTipWnd()
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN_TIP, _T("MainTipWnd"), FALSE);
	if (pWnd == NULL)
	{
		pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN_TIP, _T("MainTipWnd"));
		pWnd->CreateWnd(NULL, APP_NAME, UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX, WS_EX_TOPMOST | WS_EX_TOOLWINDOW, CURect(0, 0, 0, 0));
		pWnd->CenterWindow();
	}
	if (pWnd)
	{
		pWnd->TopWindow();
	}
	return pWnd;
}

CBaseWnd * WndHelper::GetMainTipWnd()
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_MAIN_TIP, _T("MainTipWnd"), FALSE);
	return pWnd;
}

CBaseWnd * WndHelper::ShowWbWnd(CDuiString strUrl)
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_WB, _T("WbWnd"), FALSE);
	if (pWnd == NULL)
	{
		pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_WB, _T("WbWnd"));
		
		CStdStringPtrMap aInitData;
		aInitData.Insert(_T("strUrl"), (void*)&strUrl);
		pWnd->PreCreateWindowInit(aInitData);

		pWnd->CreateWnd(NULL, APP_NAME, UI_WNDSTYLE_FRAME, 0, CURect(0, 0, 0, 0));
		pWnd->CenterWindow();

		if (pWnd)
		{
			pWnd->TopWindow();
		}
	}
	else
	{
		WbWnd* pWbWnd = static_cast<WbWnd*>(pWnd);
		pWbWnd->UpdateUrl(strUrl);
		pWnd->TopWindow();
	}
	return pWnd;
}

CBaseWnd * WndHelper::GetWbWnd()
{
	CBaseWnd* pWnd = CWndManager::GetWndManager()->FindWnd(ID_WND_WB, _T("WbWnd"), FALSE);
	return pWnd;
}

CDuiString WndHelper::GetNowTime()
{
	SYSTEMTIME stTime = { 0 };
	::GetLocalTime(&stTime);
	CDuiString strTime;
	//%04d-%02d-%02d %02d:%02d:%02d
	strTime.Format(_T("%02d:%02d:%02d"), stTime.wHour, stTime.wMinute, stTime.wSecond);
	return strTime;
}

BOOL WndHelper::CopyTextToClipboard(std::wstring text)
{
	if (::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		int length = text.length() + 1;
		HGLOBAL clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, length * 2);
		wchar_t* buffer = (wchar_t*)::GlobalLock(clipbuffer);
		wcscpy_s(buffer, length, text.c_str());
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_UNICODETEXT, clipbuffer);
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}
