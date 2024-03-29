/*
	creation date��	2012/11/05 15:09:48
	author��			daviyang35@gmail.com
	description��	FlashEventHandler
*/
#pragma once
//#include <ExDisp.h>

namespace DuiLib
{
	class CFlashEventHandler
	{
	public:
		CFlashEventHandler() {}
		~CFlashEventHandler() {}

		virtual ULONG STDMETHODCALLTYPE Release( void ) { return S_OK;}
		virtual ULONG STDMETHODCALLTYPE AddRef( void ) { return S_OK;}

		virtual HRESULT OnReadyStateChange ( long newState )
		{
			return S_OK;
		}

		virtual HRESULT OnProgress (long percentDone )
		{
			return S_OK;
		}

		virtual HRESULT FSCommand ( LPCTSTR command, LPCTSTR args )
		{
			return S_OK;
		}
		
		virtual HRESULT FlashCall ( LPCTSTR request )
		{
			return S_OK;
		}
	};
}