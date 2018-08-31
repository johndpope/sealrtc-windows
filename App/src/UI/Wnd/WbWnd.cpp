#include "StdAfx.h"
#include "WbWnd.h"
#include "WndHelper.h"
#include "PeerConnectionHelper.h"
#include "UpDictManager.h"

IMPL_APPWND_CONTROL(WbWnd, ID_WND_WB)

WbWnd::WbWnd(void)
{
	SetSkinFile(_T("wb.xml"));
}

WbWnd::~WbWnd(void)
{
}

NOTIFY_BEGIN(WbWnd)
NOTIFY_END()

LRESULT WbWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_UP_BROADCAST:
	{
		switch (wParam)
		{
		case 0:
		{
			break;
		}
		default:
			break;
		}
		break;
	}
	default:
		bHandled = FALSE;
		break;
	}
	if (bHandled) return lRes;

    return __super::HandleMessage(uMsg, wParam, lParam);
}

void WbWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LPCTSTR WbWnd::GetWindowClassName() const
{
	return _T("WbWnd");
}

void WbWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{
	m_strUrl = *(CDuiString*)(void*)aInitData.Find(_T("strUrl"));
}

void WbWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
	InitText();
	InitControl();
}

void WbWnd::UpdateUrl(CDuiString strUrl)
{
	if (strUrl.GetLength() != 0)
	{
		m_strUrl = strUrl;
		CWebBrowserUI* pWeb = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("web")));
		if (pWeb && m_strUrl.GetLength() != 0)
		{
			pWeb->NavigateUrl(m_strUrl.GetData());
		}
	}
}

void WbWnd::InitControl()
{
	CWebBrowserUI* pWeb = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("web")));
	if (pWeb && m_strUrl.GetLength() != 0)
	{
		pWeb->NavigateUrl(m_strUrl.GetData());
	}
}

void WbWnd::InitText()
{
	CLabelUI* pTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	if (pTitle) pTitle->SetText(UPSTR(wb_title).c_str());
}
