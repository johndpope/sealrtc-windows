#ifndef __UICHOP_H__
#define __UICHOP_H__

#pragma once

namespace DuiLib
{
	typedef enum MOUSEPOS
	{
		MOUSEPOS_IN,
		MOUSEPOS_OUT,
		MOUSEPOS_TOP,
		MOUSEPOS_BOTTOM,
		MOUSEPOS_LEFT,
		MOUSEPOS_RIGHT,
		MOUSEPOS_LEFT_TOP,
		MOUSEPOS_RIGHT_TOP,
		MOUSEPOS_LEFT_BOTTOM,
		MOUSEPOS_RIGHT_BOTTOM
	};

	typedef enum MOUSETYPE
	{
		MOUSETYPE_NORMAL,
		MOUSETYPE_SIZING,
		MOUSETYPE_MOVING
	};
	
	class UILIB_API CChopUI : public CControlUI
	{
	public:
		CChopUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		UINT GetControlFlags() const;

		bool Activate();
		void DoEvent(TEventUI& event);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoPaint(HDC hDC, const RECT& rcPaint);

		void SetPos(RECT rc);
		void GetChopRect(RECT &rc);
		void GetPaintRect(RECT &rc);

		void SetChopWidth(int width);
		void SetChopHeight(int height);
	private:
		void SetChopRect();
		void SetLineRect();
		void SetBorderRect();
		void SetAnchorRect();
		void SetInRect();
		void PaintAnchor(HDC hDC);
		void PaintLine(HDC hDC);
		void PaintBorder(HDC hDC);
		MOUSEPOS HitTest(POINT point);
		void AdjustChopRect();
	private:
		CDuiString m_sMaskImage;
		int m_nChopWidth;
		int m_nChopHeight;

		int m_nAnchorWidth;
		int m_nAnchorHeight;
		DWORD m_nAnchorColor;

		int m_nLineSize;
		DWORD m_dwLineColor;
		DWORD m_dwBorderColor;
		
	private:
		RECT m_rcChop;
		RECT m_rcIn;
		RECT m_rcBorder;
		RECT m_rcHLine;
		RECT m_rcHLine2;
		RECT m_rcVLine;
		RECT m_rcVLine2;

		RECT m_rcLeft;
		RECT m_rcTop;
		RECT m_rcRight;
		RECT m_rcBottom;
		RECT m_rcTopLeft;
		RECT m_rcTopRight;
		RECT m_rcBottomLeft;
		RECT m_rcBottomRight;

		MOUSEPOS m_MousePos;
		MOUSETYPE m_MouseType;
		POINT m_pointMouseLast;
	};
}	// namespace DuiLib

#endif // __UICHOP_H__