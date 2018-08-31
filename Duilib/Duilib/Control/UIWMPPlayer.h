#ifndef __UIWMPLAYER_H__
#define __UIWMPLAYER_H__

#pragma once
#include "wmp.h"


namespace DuiLib
{

	class UILIB_API CWMPPlayer : public CActiveXUI, public IDispatch, public IWMPEvents
	{
	public:
		CWMPPlayer();

	protected:
		HRESULT SetProperty( IDispatch *pObj, DISPID dispid, VARIANT *pValue );
		HRESULT GetProperty( IDispatch *pObj, DISPID dispid, VARIANT *pValue );
		HRESULT InvokeMethod( IDispatch *pObj, DISPID dispid, VARIANT *pVarResult, VARIANT *ps, int cArgs );
		DISPID FindId( IDispatch *pObj, LPOLESTR pName );

	protected:
		// IDispatch
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( __RPC__out UINT *pctinfo );
		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( UINT iTInfo, LCID lcid, __RPC__deref_out_opt ITypeInfo **ppTInfo );
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( __RPC__in REFIID riid, __RPC__in_ecount_full(cNames ) LPOLESTR *rgszNames, __RPC__in_range(0,16384) UINT cNames, LCID lcid, __RPC__out_ecount_full(cNames) DISPID *rgDispId);

		virtual HRESULT STDMETHODCALLTYPE Invoke( DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr );

		// IUnknown
		STDMETHOD_(ULONG,AddRef)();
		STDMETHOD_(ULONG,Release)();
		STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject);


		// methods
	public:

		virtual void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

		void close(){  InvokeMethod(m_dispatch, 0x3, NULL, NULL, 0); }

		CDuiString get_URL();

		void put_URL(CDuiString newValue);

		long get_openState();

		long get_playState();

		BOOL get_fullScreen();

		void put_fullScreen(BOOL newValue);

		void put_enableContextMenu( BOOL newValue );

		BOOL get_stretchToFit();

		void put_stretchToFit(BOOL newValue);

		BOOL get_enabled();

		void put_enabled(BOOL newValue);

		void put_uiMode(CDuiString newValue);

		CDuiString get_uiMode();

		LPDISPATCH get_controls();

		LPDISPATCH get_Medias();

		virtual void ReleaseControl();

		virtual bool DoCreateControl();

	public:
		virtual LPCTSTR GetClass() const;
		virtual LPVOID GetInterface( LPCTSTR pstrName );
		HRESULT RegisterEventHandler( BOOL inAdvise );
		BOOL GetEventIID(IID* piid);

		virtual LRESULT MessageHandler( UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled );

	public:
		void Play();
		void Pause();
		void Stop();

		double GetDuration();
		double GetCurPos();

		virtual void STDMETHODCALLTYPE OpenStateChange( long NewState );

		virtual void STDMETHODCALLTYPE PlayStateChange( long NewState );

		virtual void STDMETHODCALLTYPE AudioLanguageChange( long LangID );

		virtual void STDMETHODCALLTYPE StatusChange( void );

		virtual void STDMETHODCALLTYPE ScriptCommand( BSTR scType, BSTR Param );

		virtual void STDMETHODCALLTYPE NewStream( void );

		virtual void STDMETHODCALLTYPE Disconnect( long Result );

		virtual void STDMETHODCALLTYPE Buffering( VARIANT_BOOL Start );

		virtual void STDMETHODCALLTYPE Error( void );

		virtual void STDMETHODCALLTYPE Warning( long WarningType, long Param, BSTR Description );

		virtual void STDMETHODCALLTYPE EndOfStream( long Result );

		virtual void STDMETHODCALLTYPE PositionChange( double oldPosition, double newPosition );

		virtual void STDMETHODCALLTYPE MarkerHit( long MarkerNum );

		virtual void STDMETHODCALLTYPE DurationUnitChange( long NewDurationUnit );

		virtual void STDMETHODCALLTYPE CdromMediaChange( long CdromNum );

		virtual void STDMETHODCALLTYPE PlaylistChange( IDispatch *Playlist, WMPPlaylistChangeEventType change );

		virtual void STDMETHODCALLTYPE CurrentPlaylistChange( WMPPlaylistChangeEventType change );

		virtual void STDMETHODCALLTYPE CurrentPlaylistItemAvailable( BSTR bstrItemName );

		virtual void STDMETHODCALLTYPE MediaChange( IDispatch *Item );

		virtual void STDMETHODCALLTYPE CurrentMediaItemAvailable( BSTR bstrItemName );

		virtual void STDMETHODCALLTYPE CurrentItemChange( IDispatch *pdispMedia );

		virtual void STDMETHODCALLTYPE MediaCollectionChange( void );

		virtual void STDMETHODCALLTYPE MediaCollectionAttributeStringAdded( BSTR bstrAttribName, BSTR bstrAttribVal );

		virtual void STDMETHODCALLTYPE MediaCollectionAttributeStringRemoved( BSTR bstrAttribName, BSTR bstrAttribVal );

		virtual void STDMETHODCALLTYPE MediaCollectionAttributeStringChanged( BSTR bstrAttribName, BSTR bstrOldAttribVal, BSTR bstrNewAttribVal );

		virtual void STDMETHODCALLTYPE PlaylistCollectionChange( void );

		virtual void STDMETHODCALLTYPE PlaylistCollectionPlaylistAdded( BSTR bstrPlaylistName );

		virtual void STDMETHODCALLTYPE PlaylistCollectionPlaylistRemoved( BSTR bstrPlaylistName );

		virtual void STDMETHODCALLTYPE PlaylistCollectionPlaylistSetAsDeleted( BSTR bstrPlaylistName, VARIANT_BOOL varfIsDeleted );

		virtual void STDMETHODCALLTYPE ModeChange( BSTR ModeName, VARIANT_BOOL NewValue );

		virtual void STDMETHODCALLTYPE MediaError( IDispatch *pMediaObject );

		virtual void STDMETHODCALLTYPE OpenPlaylistSwitch( IDispatch *pItem );

		virtual void STDMETHODCALLTYPE DomainChange( BSTR strDomain );

		virtual void STDMETHODCALLTYPE SwitchedToPlayerApplication( void );

		virtual void STDMETHODCALLTYPE SwitchedToControl( void );

		virtual void STDMETHODCALLTYPE PlayerDockedStateChange( void );

		virtual void STDMETHODCALLTYPE PlayerReconnect( void );

		virtual void STDMETHODCALLTYPE Click( short nButton, short nShiftState, long fX, long fY );

		virtual void STDMETHODCALLTYPE DoubleClick( short nButton, short nShiftState, long fX, long fY );

		virtual void STDMETHODCALLTYPE KeyDown( short nKeyCode, short nShiftState );

		virtual void STDMETHODCALLTYPE KeyPress( short nKeyAscii );

		virtual void STDMETHODCALLTYPE KeyUp( short nKeyCode, short nShiftState );

		virtual void STDMETHODCALLTYPE MouseDown( short nButton, short nShiftState, long fX, long fY );

		virtual void STDMETHODCALLTYPE MouseMove( short nButton, short nShiftState, long fX, long fY );

		virtual void STDMETHODCALLTYPE MouseUp( short nButton, short nShiftState, long fX, long fY );

	private:
		IDispatch* m_dispatch;
		LONG m_dwRef;
		DWORD m_dwCookie;
	    CDuiString m_uimode;
	    BOOL         m_stretchtofit;
	};

}
#endif //__UIWMPLAYER_H__