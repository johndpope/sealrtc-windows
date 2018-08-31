#include "TransparentWnd.h"
#include "UpImageUtility.h"
#include "UpTimer.h"

using namespace Gdiplus;

namespace DuiLib 
{
CTransparentWnd::CTransparentWnd():
m_isUpdate(true)
{
}

CTransparentWnd::~CTransparentWnd(void)
{
	if(IsWindow(this->GetHWND()))
	{
		DestroyWindow(this->GetHWND());
	}
}

LPCTSTR CTransparentWnd::GetWindowClassName() const
{
	
	return _T("Transparent");
}

UINT CTransparentWnd::GetClassStyle() const
{
     return CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
}

void CTransparentWnd::Show()
{
	if( m_BKImage.IsEmpty() )
	{
		return;
	}
	RECT rect = {0};
	HWND hParentWnd = ::GetParent(m_hWnd);
	::GetWindowRect(hParentWnd, &rect);
	
	rect.left -= m_rcMargin.left;
	rect.right += m_rcMargin.right;
	rect.top -= m_rcMargin.top;
	rect.bottom += m_rcMargin.bottom;
	int width = rect.right-rect.left;
	int height = rect.bottom-rect.top;
	MoveWindow(m_hWnd, rect.left, rect.top, width, height, false);
	HDC hdc, hdcMem;
	hdc = GetDC(m_hWnd);
	DWORD dw = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	if( (dw & WS_EX_LAYERED) != WS_EX_LAYERED )
	{
#ifdef _DEBUG
		// When transparentWnd with WS_EX_TOOLWINDOW be hidden, the main window jump behind.
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dw | WS_EX_LAYERED | WS_EX_TRANSPARENT ); 
#else
		SetWindowLong(m_hWnd, GWL_EXSTYLE, dw | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW );
#endif
	}

	SIZE size = {width, height};

	HBITMAP hbmp;
	hdcMem = CreateCompatibleDC(hdc);
	hbmp = CreateCompatibleBitmap(hdc ,width , height);
	SelectObject(hdcMem,hbmp);
	Graphics graphics(hdcMem);
	Bitmap   bmp(m_BKImage);
	RECT destRc = {0,0,width, height};
	UpImageUtility::DrawImage(&bmp, &graphics, destRc, m_rc9GridMargin);
	POINT top = { rect.left, rect.top };
	BLENDFUNCTION blend;
	blend.AlphaFormat = 1;
	blend.BlendFlags  = 0;
	blend.BlendOp     = AC_SRC_OVER ;
	blend.SourceConstantAlpha = 255;

	POINT ptSrc = {0, 0};
	UpdateLayeredWindow(m_hWnd,hdc, &top,  &size,hdcMem,&ptSrc,0,&blend,ULW_ALPHA);

	graphics.ReleaseHDC(hdcMem);
	DeleteDC(hdcMem);
	DeleteObject(hbmp);
}

LRESULT CTransparentWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_KILLFOCUS:
		{
			HWND hParentWnd = ::GetParent(m_hWnd);
			if ((HWND)wParam != hParentWnd)
			{
				::SendMessageW(hParentWnd,uMsg,wParam,lParam);
			}
		}
		break;
	case WM_NCHITTEST:
		{
			return HTCLIENT;
		}
		break;
	case WM_NCCALCSIZE:
		{
			return 0;
		}
	case WM_PAINT:
		{
			Show();
		}
		break;
	default:
		
		break;;
	}
	return __super::HandleMessage( uMsg, wParam, lParam );
}



void CTransparentWnd::DrawRect()
{
	RECT rcParent;
	HWND hParentWnd = ::GetParent(m_hWnd);
	::GetWindowRect(hParentWnd,&rcParent);	

	int width = rcParent.right - rcParent.left;
	int height = rcParent.bottom - rcParent.top;
	int oldWidth = m_rect.right - m_rect.left;
	int oldHeight = m_rect.bottom - m_rect.top;
	m_rect = rcParent;
	if( oldWidth == width && oldHeight == height )
	{
		RECT rect = rcParent;
		rect.left -= m_rcMargin.left;
		rect.right += m_rcMargin.right;
		rect.top -= m_rcMargin.top;
		rect.bottom += m_rcMargin.bottom;
		MoveWindow(m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
	}
	else
	{
		Show(); 
	}
	ShowWindow(true);
}

void DuiLib::CTransparentWnd::SetBKImage( LPCTSTR fileName )
{
	m_BKImage = fileName;
}

void DuiLib::CTransparentWnd::MoveRect()
{
	RECT rcParent;
	HWND hParentWnd = ::GetParent(m_hWnd);
	::GetWindowRect(hParentWnd,&rcParent);	

	RECT rect= rcParent;
	m_rect = rcParent;
	rect.left -= m_rcMargin.left;
	rect.right += m_rcMargin.right;
	rect.top -= m_rcMargin.top;
	rect.bottom += m_rcMargin.bottom;
	MoveWindow(m_hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, false);
	ShowWindow(true);
}

void DuiLib::CTransparentWnd::Hide()
{
	ShowWindow(false);
}

}