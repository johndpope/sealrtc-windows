#include "StdAfx.h"
#include "Platform.h"
#include <shobjidl.h>
#include "DialogWnd.h"

#define IDI_ICON IDI_ICON_APP

const int MASK_MAGIN = 3;
const int _9GRID_MASK_MAGIN = 6; //GDI+绘图拉伸会有渐变效果，所以需要MAGIN的基础上+1
 
namespace DuiLib 
{
AllFactMap CWndManager::m_AllFact;

static LRESULT CALLBACK NewWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}


////////////////////////////////////////////////////////////////////
//Class CAppWnd
/////////////////////////////////////////////////////////////////////
CBaseWnd::CBaseWnd() :
	m_bFixSize(TRUE),
	m_nWndID(ID_UNKNOWN),
	m_maskWnd(NULL),
	m_isModal(false),
	m_isOpenFileDialog(false)
{
	
}

CBaseWnd::~CBaseWnd()
{

}

void CBaseWnd::OnFinalMessage(HWND hWnd)
{

	__super::OnFinalMessage(hWnd);
	
	CWndManager::GetWndManager()->RemoveWnd(m_nWndID,m_strWndToken);
}

LPCTSTR CBaseWnd::GetWindowClassName( void ) const
{
	throw std::exception("The method or operation is not implemented.");
}

UINT CBaseWnd::GetClassStyle() const
{
	return CS_DBLCLKS;
}

LRESULT CBaseWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	LRESULT lRes = 0;
	BOOL bHandled = FALSE;
	switch (uMsg)
	{
	case WM_MOVING:
		{
			lRes = OnMoving(uMsg, wParam, lParam, bHandled); 
		}
		break;
	case WM_MOVE:
		{
			lRes = OnMove(uMsg, wParam, lParam, bHandled); 
		}
		break;
	case WM_EXITSIZEMOVE:
		{
			lRes = OnExitSizing(uMsg, wParam, lParam, bHandled); 
		}
		break;
	case WM_MOUSEENTER:
		{
			lRes = OnMouseEnter(uMsg, wParam, lParam, bHandled);
		}
		break;
	case WM_MOUSELEAVE:
	case WM_NCMOUSELEAVE:
		{
			lRes = OnMouseLeave(uMsg, wParam, lParam, bHandled);
		}
		break;
	case WM_HOTKEY:	
		{
			lRes = OnHotKey(uMsg, wParam, lParam, bHandled);
		}
		break;
    case WM_TIMER:	
        {
            lRes = OnTimer(uMsg, wParam, lParam, bHandled);
        }
        break;
	case WM_UP_BROADCAST:
		{
			switch (wParam)
			{
			case BDT_LANGUAGE_CHANGED:
				{
					OnLanguageChanged();
				}
				break;
			}
		}
	default:	
		break;
	}

	if (bHandled) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CBaseWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	RECT rcClient;
	::GetClientRect(*this, &rcClient);
	::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
		rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

	m_PaintManager.Init(m_hWnd);
	m_PaintManager.AddPreMessageFilter(this);

	CDialogBuilder builder;
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += GetSkinFolder().GetData();
	CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

	CControlUI* pRoot = builder.Create(GetSkinFile().GetData(), (UINT)0, this, &m_PaintManager);
	if (pRoot != NULL)
	{
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);
		InitWindow();
	}
	
	return 0;
}

LRESULT CBaseWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) 
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left )
			{
				return HTTOPLEFT;
			}
			if( pt.x > rcClient.right - rcSizeBox.right )
			{
				return HTTOPRIGHT;
			}
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) 
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) 
			{
				return HTBOTTOMLEFT;
			}
			if( pt.x > rcClient.right - rcSizeBox.right ) 
			{
				return HTBOTTOMRIGHT;
			}
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left )
		{
			return HTLEFT;
		}
		if( pt.x > rcClient.right - rcSizeBox.right )
		{
			return HTRIGHT;
		}
	}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && \
			_tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&   \
			_tcscmp(pControl->GetClass(), _T("TextUI")) != 0 &&		\
			_tcscmp(pControl->GetClass(), _T("RichEditUI")) != 0 &&		\
			_tcscmp(pControl->GetClass(), _T("CheckBoxUI")) != 0 &&		\
			_tcscmp(pControl->GetClass(), _T("FadeButtonUI")) != 0)
		{
			return HTCAPTION;
		}
	}

	return HTCLIENT;
}

LRESULT CBaseWnd::OnMoving( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_maskWnd != NULL)
	{
		if (::IsWindowVisible(m_hWnd))
		{
			m_maskWnd->MoveRect();
		}
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_maskWnd != NULL)
	{
		if (::IsWindowVisible(m_hWnd))
		{
			m_maskWnd->DrawRect();	
		}
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnSize( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__super::OnSize(uMsg, wParam, lParam, bHandled);
	if (m_maskWnd != NULL  && ::IsWindowVisible(m_hWnd))
	{
		if( SIZE_MAXIMIZED == wParam || SIZE_MINIMIZED == wParam )
		{
			m_maskWnd->Hide();
		}
		else
		{
			m_maskWnd->DrawRect();
		}
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnExitSizing( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	if (m_maskWnd != NULL)
	{
		if (::IsWindowVisible(m_hWnd) )
		{
			m_maskWnd->DrawRect();
		}
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnClose( UINT , WPARAM , LPARAM , BOOL& bHandled )
{
	bHandled = FALSE;
	if( !IsModal() )
	{
		bHandled =  CWndManager::GetWndManager()->IsHasModal(GetHWND());
	}

	if( m_isOpenFileDialog )
	{
		HWND hwndAfter = NULL;
		while(hwndAfter = ::FindWindowEx(m_hWnd,hwndAfter,NULL,NULL));
		{
			::PostMessage(hwndAfter,WM_CLOSE,0,0);
		}
		bHandled = true;
	}

	if( !bHandled && m_maskWnd != NULL )
	{
		delete m_maskWnd;
		m_maskWnd = NULL;
	}
	return 0;
}

void CBaseWnd::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType,DUI_MSGTYPE_WINDOWINIT) == 0)
	{
		OnWinInit(msg);
	}
	__super::Notify(msg);
}

void CBaseWnd::OnWinInit(TNotifyUI& msg)
{
	SetSkinColor();
	if( m_maskWnd != NULL )
	{
		m_maskWnd->Show();
	}
}

LRESULT CBaseWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnMouseEnter( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnMouseLeave( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBaseWnd::OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

CControlUI* CBaseWnd::CreateControl(LPCTSTR pstrClass)
{
	return __super::CreateControl(pstrClass);
}

void CBaseWnd::SetWndIcon()
{
	SetIcon(IDI_ICON);
}

HWND CBaseWnd::CreateWnd( HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, const RECT rc, CUString maskFile, RECT* pRectMargin , RECT* pRect9GridMagrin)
{
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	strResourcePath += GetSkinFolder();
	strResourcePath += maskFile;

//#ifdef _DEBUG 
//	if(  (dwExStyle & WS_EX_TOPMOST) == WS_EX_TOPMOST )
//	{
//		dwExStyle &=  ~WS_EX_TOPMOST;
//		dwExStyle &=  ~WS_EX_TOOLWINDOW;
//	}
//#endif


	SetMaskWindow(strResourcePath);
	m_parentHwnd = hwndParent;
	HWND ret = __super::Create(hwndParent, pstrName, dwStyle, dwExStyle,  rc, NULL);
	SetWndIcon();

	if( !m_maskFile.IsEmpty() && UpFile::IsFileExistedW(m_maskFile.GetData())) 
	{
		if( m_maskWnd == NULL  )
		{
			m_maskWnd = new CTransparentWnd();
			RECT rcMargin = {MASK_MAGIN, MASK_MAGIN, MASK_MAGIN, MASK_MAGIN};
			if (pRectMargin)
			{
				rcMargin = *pRectMargin;
			}
			m_maskWnd->SetMargin(rcMargin);
			RECT rc9GridMargin = {_9GRID_MASK_MAGIN, _9GRID_MASK_MAGIN, _9GRID_MASK_MAGIN, _9GRID_MASK_MAGIN};
			if (pRect9GridMagrin)
			{
				rc9GridMargin = *pRect9GridMagrin;
			}
			m_maskWnd->Set9GridMargin(rc9GridMargin);
			m_maskWnd->SetBKImage(m_maskFile);
			RECT rect = {-1, -1, -1 , -1};
			m_maskWnd->Create(ret, L"", CHILD_UI_STYLE,  dwExStyle  , rect);
		}
	}
	return ret;
}

void CBaseWnd::SetSkinFolder(LPCTSTR pstrSkinFolder)
{
	SkinFolder = pstrSkinFolder;
}

CUString CBaseWnd::GetSkinFolder()
{
	return SkinFolder;
}

CUString CBaseWnd::GetSkinFile()
{
	return m_strCurSkinFile;
}

void CBaseWnd::CenterWindow()
{
	__super::CenterWindow();
	
	if( m_maskWnd != NULL )
	{
		m_maskWnd->CenterWindow();
	}
}

HWND CBaseWnd::GetTransparentWnd()
{
	return m_maskWnd->GetHWND();
}

void CBaseWnd::SetMaskWindow( LPCTSTR fileName )
{
	m_maskFile = fileName;
}

bool CBaseWnd::ShowInTaskbar(bool bShow)
{
	HRESULT hr; 
	ITaskbarList* pTaskbarList;
	hr = CoCreateInstance( CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,  
		IID_ITaskbarList, (void**)&pTaskbarList );

	if(SUCCEEDED(hr))
	{
		pTaskbarList->HrInit();
		if(bShow)
			pTaskbarList->AddTab(GetHWND());
		else
			pTaskbarList->DeleteTab(GetHWND());

		pTaskbarList->Release();
		return TRUE;
	}

	return FALSE;
}


void CBaseWnd::Destroy()
{
	if( IsWindow(this->GetHWND()) )
	{
		if( m_maskWnd != NULL )
		{
			delete m_maskWnd;
			m_maskWnd = NULL;
		}
		DestroyWindow(this->GetHWND());
	}
}

void CBaseWnd::ShowWnd(bool bShow)
{
	if (bShow)
	{
		if (IsWindowVisible(m_hWnd) != TRUE)
		{
			ShowWindow(true);
			if (m_maskWnd)
			{
				m_maskWnd->Show();
				m_maskWnd->ShowWindow(true);
			}
		}
	}
	else
	{
		if (IsWindowVisible(m_hWnd))
		{
			ShowWindow(false);
			if (m_maskWnd)
			{
				m_maskWnd->Hide();
			}
		}
	}
}

void CBaseWnd::ShowMaskWindow(bool isShow)
{
    if( m_maskWnd != NULL )
	{
		m_maskWnd->ShowWindow(isShow);
	}
}

void CBaseWnd::SetWndCallbackNULL()
{
	SetWindowLongPtr(m_hWnd,GWL_WNDPROC, (long)NewWndProc);
}


UINT CBaseWnd::ShowModal()
{
	m_isModal = true;
	UINT ret = __super::ShowModal();
	return ret;
}


CUString CBaseWnd::TrimTextByCtrlSize(CUString fulltext, int ctrlWidth, int ctrlHeight, int maxLength)
{
	// 不需要省略号
	SIZE sz = {ctrlWidth, ctrlHeight};
	sz = CRenderEngine::GetTextSize(m_PaintManager.GetPaintDC(), &m_PaintManager, sz, fulltext.GetData(), 0, 0);
	if (sz.cx <= ctrlWidth) return fulltext;

	// 需要省略号
	int pos = 0;
	CUString trimText = _T("");
	do 
	{
		pos++;
		if (pos>maxLength) break;
		trimText = fulltext.Left(fulltext.GetLength()-pos);
		trimText = trimText + _T("...");
		sz = CRenderEngine::GetTextSize(m_PaintManager.GetPaintDC(), &m_PaintManager, sz, trimText.GetData(), 0, 0);

	} while (sz.cx > ctrlWidth);

	return trimText;
}

////////////////////////////////////////////////////////////////////////////////////////////////
///Class CWndManager
///////////////////////////////////////////////////////////////////////////////////////////////

CWndManager* CWndManager::m_wm = NULL;

CWndManager* CWndManager::GetWndManager()
{
	if( m_wm == NULL )
	{
		m_wm = new CWndManager();
	}
	return m_wm;
}

CWndManager::~CWndManager()
{
	DestroyAllWnd();
}

BOOL CWndManager::IsExsitWnd(int nWndID,LPCTSTR pstrWndToken)
{
	CUString strID;
	strID.Format(_T("%d%s"),nWndID,pstrWndToken);
	AllWndMap::iterator it = m_AllWnd.find(strID);
	if(it != m_AllWnd.end())
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CWndManager::IsExsitFact(int nWndID)
{
	AllFactMap::iterator it = m_AllFact.find(nWndID);
	if(it != m_AllFact.end())
	{
		return TRUE;
	}
	return FALSE;
}

CBaseWnd* CWndManager::FindWnd(int nWndID,LPCTSTR pstrWndToken,BOOL bCreate)
{
	CBaseWnd* pWnd = NULL;
	CUString strID;
	strID.Format(_T("%d%s"),nWndID,pstrWndToken);
	if (bCreate && !IsExsitWnd(nWndID,pstrWndToken) && IsExsitFact(nWndID))
	{
		CAppWndFactory* pFact = m_AllFact[nWndID];
		if (pFact != NULL && pFact->pCtor != NULL)
		{
			pWnd = pFact->pCtor();
			if(pWnd != NULL)
			{
				pWnd->SetWndID(nWndID);
				pWnd->SetWndToken(pstrWndToken);
				m_AllWnd[strID] = pWnd;
			}
		}
	}
	else
	{
		AllWndMap::iterator it = m_AllWnd.find(strID);
		if (it != m_AllWnd.end())
		{
			pWnd = (CBaseWnd*)it->second;
		}
	}
	return pWnd;
}


CBaseWnd* CWndManager::GetWnd(int index)
{
	CBaseWnd* wnd = NULL;
	AllWndMap::iterator it = m_AllWnd.begin();
	for( int i = 0; i < index; i++ )
	{
		it++;
	}
    if( it != m_AllWnd.end() )
	{
		wnd = (CBaseWnd*)it->second;
	}
	return wnd;
}

void CWndManager::RemoveWnd(int nWndID,LPCTSTR pstrWndToken)
{
	if(IsExsitWnd(nWndID,pstrWndToken))
	{
		CUString strID;
		strID.Format(_T("%d%s"),nWndID,pstrWndToken);
		CBaseWnd* pWnd = m_AllWnd[strID];
		if(pWnd != NULL && ::IsWindow(pWnd->GetHWND()))
		{
			pWnd->Close();
		}
		m_AllWnd.erase(strID);
	}
}


void CWndManager::RegisterWnd(int nWndID,CAppWndFactory* pFact)
{
	AllFactMap::iterator it = m_AllFact.find(nWndID);
	if(it == m_AllFact.end())
	{
		m_AllFact[nWndID] = pFact;
	}	
}

void CWndManager::DestroyAllWnd()
{

	while (m_AllWnd.size() > 0)
	{
		AllWndMap::iterator it = m_AllWnd.begin();
		CBaseWnd* pWnd = (CBaseWnd*)it->second;
		if(pWnd != NULL)
		{
			pWnd->SetWndCallbackNULL();
			pWnd->Destroy();
			CUString strID;
			strID.Format(_T("%d%s"),pWnd->GetWndID(),pWnd->GetWndToken());
			m_AllWnd.erase(strID);
			delete pWnd;
		}
	}


	AllFactMap::iterator it = m_AllFact.begin();
	for( ;it != m_AllFact.end(); it++ )
	{
		CAppWndFactory* pFac = (CAppWndFactory*)it->second;
		if(pFac != NULL)
		{
			delete pFac;
		}
	}
	m_AllFact.clear();
}

void CWndManager::CloseAllWindow(void)
{
	while (m_AllWnd.size() > 0)
	{
		AllWndMap::iterator it = m_AllWnd.begin();
		CBaseWnd* pWnd = (CBaseWnd*)it->second;
		if(pWnd != NULL)
		{
			pWnd->Close();
			CUString strID;
			strID.Format(_T("%d%s"),pWnd->GetWndID(),pWnd->GetWndToken());
			m_AllWnd.erase(strID);
		}
	}
}

void CWndManager::CloseSubWindows(HWND parentHwnd)
{
	for( AllWndMap::iterator it = m_AllWnd.begin(); it != m_AllWnd.end(); it++)
	{		
		CBaseWnd* pWnd = (CBaseWnd*)it->second;
		if(pWnd != NULL && pWnd->GetParentHwnd() == parentHwnd)
		{
			pWnd->Close();
		}
	}
}

bool CWndManager::IsHasModal( HWND parentHwnd )
{
	bool ret = false;
	AllWndMap::iterator it = m_AllWnd.begin();
	for(  ; it != m_AllWnd.end(); it++)
	{
		CBaseWnd* pWnd = (CBaseWnd*)it->second;
		if(pWnd != NULL && pWnd->GetParentHwnd() == parentHwnd)
		{
			if( pWnd->IsModal() )
			{
				ret = true;
				break;
			}
		}
	}
	return ret;
}


} // namespace DuiLib
