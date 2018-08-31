#include "StdAfx.h"
#include "MainTipWnd.h"
#include "MainWnd.h"
#include "WndHelper.h"

IMPL_APPWND_CONTROL(MainTipWnd, ID_WND_MAIN_TIP)

MainTipWnd::MainTipWnd(void)
{
	SetSkinFile(_T("main_tip.xml"));
}

MainTipWnd::~MainTipWnd(void)
{
}

NOTIFY_BEGIN(MainTipWnd)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("tip_close_btn"), OnClickTipClose)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("tip_yes_btn"), OnClickTipYes)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("tip_no_btn"), OnClickTipNo)
NOTIFY_END()

LRESULT MainTipWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_UP_BROADCAST:
	{
		break;
	}
	default:
		bHandled = FALSE;
		break;
	}
	if (bHandled) return lRes;

    return __super::HandleMessage(uMsg, wParam, lParam);
}

void MainTipWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LPCTSTR MainTipWnd::GetWindowClassName() const
{
	return _T("MainTipWnd");
}

void MainTipWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{

}

void MainTipWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
}

void MainTipWnd::OnClickTipClose(TNotifyUI & msg)
{
	MainWnd* pMainWnd = (MainWnd*)(WndHelper::GetMainWnd());
	if (pMainWnd)
	{
		pMainWnd->ClickTipClose();
	}
}

void MainTipWnd::OnClickTipYes(TNotifyUI & msg)
{
	MainWnd* pMainWnd = (MainWnd*)(WndHelper::GetMainWnd());
	if (pMainWnd)
	{
		pMainWnd->ClickTipYes();
	}
}

void MainTipWnd::OnClickTipNo(TNotifyUI & msg)
{
	MainWnd* pMainWnd = (MainWnd*)(WndHelper::GetMainWnd());
	if (pMainWnd)
	{
		pMainWnd->ClickTipNo();
	}
}

void MainTipWnd::ShowTip(std::wstring strText, std::wstring strLeftText, std::wstring strRightText)
{
	CTextUI* pInfo = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("tip_info")));
	if (pInfo)
	{
		pInfo->SetText(strText.c_str());
	}
	CButtonUI* pLeftBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("tip_yes_btn")));
	if (pLeftBtn)
	{
		if (strLeftText.length() != 0)
		{
			pLeftBtn->SetVisible(true);
			pLeftBtn->SetText(strLeftText.c_str());
		}
		else
		{
			pLeftBtn->SetVisible(false);
		}
	}
	CButtonUI* pRightBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("tip_no_btn")));
	if (pRightBtn)
	{
		if (strRightText.length() != 0)
		{
			pRightBtn->SetVisible(true);
			pRightBtn->SetText(strRightText.c_str());
		}
		else
		{
			pRightBtn->SetVisible(false);
		}
	}
}
