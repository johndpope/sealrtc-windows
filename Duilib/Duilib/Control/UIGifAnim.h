#ifndef GifAnimUI_h__
#define GifAnimUI_h__


#pragma once

namespace DuiLib
{
	class CLabelUI;

#define EVENT_TIEM_ID	100

	class  UILIB_API CGifAnimUI : public CButtonUI
	{
	public:
		CGifAnimUI(void);
		~CGifAnimUI(void);

		LPCTSTR GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoPaint(HDC hDC, const RECT& rcPaint);
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void	DoEvent(TEventUI& event);
		void	SetAutoPlayState(bool bAtuoPlay);
		bool	GetAutoPlayState();
		void	StartPlay();
		void	StopPlay();
	public:
		void	UpdateGif(LPCTSTR pStrGif);
	private:
		void	InitGifImage();
		void	DeleteGif();
		void   OnTimer( UINT_PTR idEvent );
		void	DrawFrame( HDC hDC );		// draw a Frame for GIF

		void   Invalidate();
	private:
		Image*m_pGifImage;
		UINT	m_nFrameCount;				// frame count of GIF
		UINT	m_nFramePosition;			// current frame position
		PropertyItem*	m_pPropertyItem;	// interval of frames
		bool	m_bAutoPlayState;
		bool	m_bStopGif;
		bool	m_bIsGif;
	};
}

#endif // GifAnimUI_h__
