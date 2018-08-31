#ifndef VIDEO_h__
#define VIDEO_h__
#pragma once

namespace DuiLib
{
	class IVideo
	{
	public:
		virtual void SetBitmap(BITMAPINFO* pInfo, unsigned char* buffer) = 0;
	};

	class UILIB_API CVideoUI : public CContainerUI, public IVideo
	{
	public:
		CVideoUI(void);
		~CVideoUI(void);

		LPCTSTR	GetClass() const;
		LPVOID	GetInterface(LPCTSTR pstrName);
		void	DoPaint(HDC hDC, const RECT& rcPaint);
		void	SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		virtual void PaintBkColor(HDC hDC) {};
		virtual void PaintBkImage(HDC hDC);
		virtual void SetBitmap(BITMAPINFO* pInfo, unsigned char* buffer);
		void GetBitmap(BITMAPINFO** pInfo, unsigned char** buffer);
	private:
		BITMAPINFO* m_pInfo;
		unsigned char* m_buf;
		BOOL m_bFullScreen;
		CRITICAL_SECTION buffer_lock;
	};
}

#endif // VideoUI_h__
