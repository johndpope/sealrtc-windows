#ifndef __UIFADEBUTTON_H__
#define __UIFADEBUTTON_H__

#pragma once

#include "UIAnimation.h"

namespace DuiLib {

	class CButtonUI;

	class UILIB_API CFadeButtonUI : public CButtonUI, public CUIAnimation
	{
	public:
		CFadeButtonUI();
		virtual ~CFadeButtonUI();

		LPCTSTR GetClass() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void DoEvent(TEventUI& event);
		void SetFadeEllapse(int iFadeEllapse){ m_iFadeEllapse = iFadeEllapse; }
		void SetFadeFrameCount(int iFadeFrameCount){ m_iFadeFrameCount = iFadeFrameCount; }
		void SetFadeEffect(BOOL hasFadeEffect);
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void OnTimer( int nTimerID );
		void PaintStatusImage(HDC hDC);

		virtual void OnAnimationStart(INT nAnimationID, BOOL bFirstLoop) {}
		virtual void OnAnimationStep(INT nTotalFrame, INT nCurFrame, INT nAnimationID);
		virtual void OnAnimationStop(INT nAnimationID) {}
		
	protected:
		CDuiString m_sOldImage;
		CDuiString m_sNewImage;
		CDuiString m_sLastImage;
		BYTE       m_bFadeAlpha;
		BOOL       m_bMouseHove;
		BOOL       m_bMouseLeave;
		int		   m_iFadeEllapse;
		int        m_iFadeFrameCount;
		enum
		{
			FADE_IN_ID			= 8,
			FADE_OUT_ID			= 9,
			FADE_ELLAPSE		= 10,
			FADE_FRAME_COUNT	= 20,
		};

	private:
		static CDuiString UpdateFadeAttribute(const CDuiString& imageAttribute, int fadeAlpha);

	private:
		BOOL m_hasFadeEffect;
	};

} // namespace DuiLib

#endif // __UIFADEBUTTON_H__