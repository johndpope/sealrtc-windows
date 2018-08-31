#include "StdAfx.h"
#include "DialogWnd.h"

IMPL_APPWND_CONTROL(CDialogWnd,ID_WND_DIALOG)
CDialogWnd::TDialogResult CDialogWnd::m_result = EDialogClose;
CDialogWnd::CDialogWnd(void)
{
	m_result = EDialogClose;
	m_isShow = false;
	SetSkinFile(_T("dialog.xml"));
}

CDialogWnd::~CDialogWnd(void)
{
}

NOTIFY_BEGIN(CDialogWnd)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("btncancel"), OnClickCancel)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("btnok"), OnClickOK)
NOTIFY_END()

LRESULT CDialogWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (WM_UP_BROADCAST ==uMsg && BDT_CHANGE_SKIN_COLOR ==wParam )
	{
		SetSkinColor();
	}
    return __super::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CDialogWnd::GetWindowClassName() const
{
	return _T("dialogwnd");
}

void CDialogWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

bool CDialogWnd::m_isShowing = false;

CDialogWnd::TDialogResult CDialogWnd::ShowDialog( HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag )
{
	static __int64 index = 0;
	if( index < 0 )
	{
		index = 0;
	}
	++index;

	m_isShowing = true;

	CDuiString token;
	token.Format(L"dialog%I64d", index);
	TDialogResult rlt = EDialogClose;
	CDialogWnd* wnd =(CDialogWnd*)(CWndManager::GetWndManager()->FindWnd(ID_WND_DIALOG, token));
	if( wnd != NULL )
	{
		m_result = EDialogClose;
		RECT rect ={  0, 0, 0, 0 };
		wnd->InitInfo(titile, leftBtn, rightBtn, info, flag);
		wnd->CreateWnd(hwnd, _T("Dialog"), UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX,  WS_EX_TOPMOST | WS_EX_TOOLWINDOW, rect);
		wnd->CenterWindow();
		wnd->SetShow(true);
		wnd->ShowModal();
		rlt  = CDialogWnd::m_result;
		if (::IsWindow(hwnd) == FALSE)
		{
			rlt = EDialogNotDo;
		}
	}
	m_isShowing = false;
	return rlt;
}


CDialogWnd::TDialogResult CDialogWnd::ShowSingleDialog( HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag /*= EDialogTWoBtn*/ )
{
	CDuiString token;
	token.Format(L"dialog%d", (unsigned int)hwnd);

	m_isShowing = true;

	TDialogResult rlt = EDialogClose;
	CDialogWnd* wnd =(CDialogWnd*)(CWndManager::GetWndManager()->FindWnd(ID_WND_DIALOG, token, FALSE));
	if( wnd == NULL )
	{
		wnd =(CDialogWnd*)(CWndManager::GetWndManager()->FindWnd(ID_WND_DIALOG, token));
		m_result = EDialogClose;
		RECT rect ={  0, 0, 0, 0 };
		wnd->InitInfo(titile, leftBtn, rightBtn, info, flag);
		wnd->CreateWnd(hwnd, _T("Dialog"), UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX,  WS_EX_TOPMOST | WS_EX_TOOLWINDOW, rect);
		wnd->CenterWindow();
		wnd->SetShow(true);
		wnd->ShowModal();
		rlt  = CDialogWnd::m_result;
		if (::IsWindow(hwnd) == FALSE)
		{
			rlt = EDialogNotDo;
		}
	}

	m_isShowing = false;
	return rlt;

}


void CDialogWnd::ShowSingleWnd( HWND hwnd, std::wstring titile, std::wstring leftBtn, std::wstring rightBtn, std::wstring info, TDialogFlag flag /*= EDialogTWoBtn*/ )
{
	CDuiString token;
	token.Format(L"dialogwnd%d", (unsigned int)hwnd);
	CDialogWnd* wnd =(CDialogWnd*)(CWndManager::GetWndManager()->FindWnd(ID_WND_DIALOG, token, FALSE));
	if( wnd == NULL )
	{
		wnd =(CDialogWnd*)(CWndManager::GetWndManager()->FindWnd(ID_WND_DIALOG, token));
		m_result = EDialogClose;
		RECT rect ={  0, 0, 0, 0 };
		wnd->InitInfo(titile, leftBtn, rightBtn, info, flag);
		wnd->CreateWnd(hwnd, _T("Dialog"), UI_WNDSTYLE_FRAME & ~WS_MAXIMIZEBOX,  WS_EX_TOPMOST | WS_EX_TOOLWINDOW, rect);
		wnd->CenterWindow();
		wnd->SetShow(true);
		wnd->ShowWindow();
	}
}


void CDialogWnd::OnClickOK( TNotifyUI& msg )
{
	m_result = EDialogRigtht;
	Close();
}

void CDialogWnd::OnClickCancel( TNotifyUI& msg )
{
	m_result = EDialogLeft;
	Close();
}

void CDialogWnd::InitInfo( std::wstring& title, std::wstring& leftBtn, std::wstring& rightBtn, std::wstring& info,  TDialogFlag& flag )
{
	m_title = title;
	m_left = leftBtn;
	m_right = rightBtn;
	m_info = info;
	m_flag = flag;
}

void CDialogWnd::OnWinInit( TNotifyUI& msg )
{
	__super::OnWinInit(msg);

	CLabelUI* title = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("labtitle")));
	if (title)
	{
		title->SetText(m_title.c_str());
	}
	CVerticalLayoutUI* panel = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("panel")));
	if (panel)
	{
		CLabelUI* icon = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("Icon")));
		if (icon)
		{
			int height = panel->GetFixedHeight();
			CVerticalLayoutUI* iconpanel = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("iconpanel")));
			if( iconpanel )
			{
				RECT rc= {0, (height-icon->GetFixedHeight())/2, 0,0};
				iconpanel->SetInset(rc);
			}
		}

		CTextUI* info = static_cast<CTextUI*>(m_PaintManager.FindControl(_T("info")));
		if (info)
		{
			info->SetText(m_info.c_str());
			SIZE TempSize = {info->GetFixedWidth(),info->GetFixedHeight()};
			SIZE size = CRenderEngine::GetTextSize(m_PaintManager.GetPaintDC(),&m_PaintManager,TempSize,m_info.c_str(),0,DT_WORDBREAK);
			int height = panel->GetFixedHeight();
			CVerticalLayoutUI* infopanel = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("infopanel")));
			if( infopanel )
			{
				RECT rc= {0, (height-size.cy)/2, 0,0};
				infopanel->SetInset(rc);
			}
		}
	}
	CButtonUI* btnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btncancel")));
	if (btnCancel)
	{
		btnCancel->SetText(m_left.c_str());
		if( m_flag == EDialogOneBtn )
		{
			btnCancel->SetVisible(false);
		}
	}
	CButtonUI* btnOK = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnok")));
	if (btnOK)
	{
		btnOK->SetText(m_right.c_str());
	}

}


void CDialogWnd::SetSkinColor()
{
	//CControlUI* pBg = m_PaintManager.FindControl(_T("bg"));
	//DWORD color = GlobalManager::GetInstance()->GetSysSettingManager()->GetColorTheme(); 
	//if (pBg)
	//{
	//	pBg->SetBkColor(color);

	//}
	//CButtonUI* btnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btncancel")));
	//if (btnCancel)
	//{
	//	btnCancel->SetBkColor(color);
	//}
	//CButtonUI* btnOK = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("btnok")));
	//if (btnOK)
	//{
	//	btnOK->SetBkColor(color);
	//}

}


