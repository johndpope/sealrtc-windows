//-------------------------------------------------------------------------
//  GifRichEdit - RichEdit Plus Control
//--------------------------------------------------------------------------
#pragma once

#include <richole.h>
#include <GdiPlus.h>
#include <Tom.h>
#include <atlstr.h>

static const GUID Guid_OleImageObject = { 0x574b00fe,0x66dc,0x458d,{ 0x99,0xc7,0x36,0x22,0xea,0xd2,0x56, 0xa1 } };

#pragma comment( lib, "GdiPlus.lib" )
using namespace Gdiplus;

namespace DuiLib{
	//COleImageObject
	class COleImageObject : public IOleObject, public IViewObject2
	{
	public:
		COleImageObject();
		~COleImageObject();

		// implement for IUnknown
		virtual HRESULT WINAPI QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG   WINAPI AddRef(void);
		virtual ULONG   WINAPI Release(void);

		// implement for IOleObject
		virtual HRESULT WINAPI SetClientSite(IOleClientSite *pClientSite);
		virtual HRESULT WINAPI GetClientSite(IOleClientSite **ppClientSite);
		virtual HRESULT WINAPI SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj);
		virtual HRESULT WINAPI Close(DWORD dwSaveOption);
		virtual HRESULT WINAPI SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk);
		virtual HRESULT WINAPI GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
		virtual HRESULT WINAPI InitFromData(IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved);
		virtual HRESULT WINAPI GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject);
		virtual HRESULT WINAPI DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
		virtual HRESULT WINAPI EnumVerbs(IEnumOLEVERB **ppEnumOleVerb);
		virtual HRESULT WINAPI Update(void);
		virtual HRESULT WINAPI IsUpToDate(void);
		virtual HRESULT WINAPI GetUserClassID(CLSID *pClsid);
		virtual HRESULT WINAPI GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType);
		virtual HRESULT WINAPI SetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection);
		virtual HRESULT WINAPI Unadvise(DWORD dwConnection);
		virtual HRESULT WINAPI EnumAdvise(IEnumSTATDATA **ppenumAdvise);
		virtual HRESULT WINAPI GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus);
		virtual HRESULT WINAPI SetColorScheme(LOGPALETTE *pLogpal);

		// implement for IViewObject
		virtual HRESULT WINAPI Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
			HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
			BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
		virtual HRESULT WINAPI GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
			DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet);
		virtual HRESULT WINAPI Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze);
		virtual HRESULT WINAPI Unfreeze(DWORD dwFreeze);
		virtual HRESULT WINAPI SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
		virtual HRESULT WINAPI GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);

		// implement for IViewObject2
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel);
		
		//Inside interface
		void LoadImageFromFile(LPCTSTR pFilePath, UINT cx, UINT cy);
		//insert OleControlUi
		static BOOL InsertImageRichEdit(IRichEditOle* pRichEditOle,LPCTSTR pFilePath,DWORD dwUserData,UINT cx,UINT cy,long nPos = -1);
	
	private:
		Gdiplus::Image* m_pImage;
		IOleClientSite* m_pOleClientSite;
		ULONG	m_ulRef;
		UINT	m_cx;
		UINT	m_cy;
	};

	// CGifOleObject
	class CGifOleObject : public IOleObject, public IViewObject2
	{
	public:
		CGifOleObject();
		~CGifOleObject();

		// implement for IUnknown
		virtual HRESULT WINAPI QueryInterface(REFIID iid, void ** ppvObject);
		virtual ULONG   WINAPI AddRef(void);
		virtual ULONG   WINAPI Release(void);

		// implement for IOleObject
		virtual HRESULT WINAPI SetClientSite(IOleClientSite *pClientSite);
		virtual HRESULT WINAPI GetClientSite(IOleClientSite **ppClientSite);
		virtual HRESULT WINAPI SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj);
		virtual HRESULT WINAPI Close(DWORD dwSaveOption);
		virtual HRESULT WINAPI SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk);
		virtual HRESULT WINAPI GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
		virtual HRESULT WINAPI InitFromData(IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved);
		virtual HRESULT WINAPI GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject);
		virtual HRESULT WINAPI DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect);
		virtual HRESULT WINAPI EnumVerbs(IEnumOLEVERB **ppEnumOleVerb);
		virtual HRESULT WINAPI Update(void);
		virtual HRESULT WINAPI IsUpToDate(void);
		virtual HRESULT WINAPI GetUserClassID(CLSID *pClsid);
		virtual HRESULT WINAPI GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType);
		virtual HRESULT WINAPI SetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, SIZEL *psizel);
		virtual HRESULT WINAPI Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection);
		virtual HRESULT WINAPI Unadvise(DWORD dwConnection);
		virtual HRESULT WINAPI EnumAdvise(IEnumSTATDATA **ppenumAdvise);
		virtual HRESULT WINAPI GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus);
		virtual HRESULT WINAPI SetColorScheme(LOGPALETTE *pLogpal);
		
		// implement for IViewObject
		virtual HRESULT WINAPI Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DVTARGETDEVICE *ptd,
			HDC hdcTargetDev, HDC hdcDraw, LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
			BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), ULONG_PTR dwContinue);
		virtual HRESULT WINAPI GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
			DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet);
		virtual HRESULT WINAPI Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze);
		virtual HRESULT WINAPI Unfreeze(DWORD dwFreeze);
		virtual HRESULT WINAPI SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink);
		virtual HRESULT WINAPI GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink);

		// implement for IViewObject2
		virtual HRESULT WINAPI GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel);

		void SetRichEditHwnd(HWND hWnd);
		HWND GetRichEditHwnd();

		BOOL LoadGifFromFile(const TCHAR *lpszGifFile, const LPCTSTR sType = NULL);
		void GetGifSize(LPSIZE lpSize);
		UINT GetFrameCount();
		void OnTimer(UINT_PTR idEvent);
		void GetOleObjectRect(LPRECT lpRect);
		void DrawFrame(HDC hDC, LPRECT lpRect);

		static BOOL InsertGifRichEdit(CControlUI* owner, HWND hWnd, IRichEditOle* pRichEditOle, 
			                                          const TCHAR *lpGifPath, DWORD dwUserData, LPCTSTR sType = NULL);
		static VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);

	protected:
		void DeleteGif();
		BOOL IsAnimatedGIF();

	protected:
		CControlUI* m_pOwner;
		ULONG m_ulRef;
		TCHAR m_szGifFile[MAX_PATH];
		Gdiplus::Image *m_pGifImage;
		UINT *m_puFrameElapse;
		UINT m_nFrameCount;
		UINT m_nFramePosition;
		IOleClientSite *m_pOleClientSite;
		HWND m_hWndRichEdit;
	};
}