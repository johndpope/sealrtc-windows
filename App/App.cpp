#include "StdAfx.h"
#include "UpMutex.h"
#include "PeerConnectionHelper.h"
#include "WndHelper.h"
#include "minidump.h"
#include "CrashManager.h"
#include "FindImp.h"

void Init()
{
	InitAppWnd();
	InitMsgBroadcast();
	CTextRes::SetIFind(FindImp::GetInstance());
	PeerConnectionHelper::CreatePeerConnectionHelper();
	CCrashManager::CreateInstance();
}

void Destroy()
{
	RongRTCEngine::UnInit();
	CCrashManager::DestoryInstance();
	PeerConnectionHelper::DestoryPeerConnectionHelper();
	MsgBroadcast::GetMsgBroadcast()->SetEnable(false);
	UpDictManager::DeleteInstance();
	delete CWndManager::GetWndManager();
	delete MsgBroadcast::GetMsgBroadcast();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	std::string APPKey = "{78C490B4-D58A-4B3A-92FF-7102235F70C8}";
	if( UpMutex::CheckSameId(APPKey) )
	{
        HWND  currentWnd =  FindWindowEx(NULL,NULL,_T("MainWnd"), APP_NAME);
		if( currentWnd == NULL )
		{
			currentWnd = FindWindowEx(NULL,NULL,_T("LogonWnd"), APP_NAME);
		}
		if(currentWnd != NULL )
		{
			::PostMessageW(currentWnd,WM_SHOW_APP,0,0);
		}
		return 0;
	}

	UpMutex onlyMutex(APPKey);

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
#ifndef _DEBUG
	CPaintManagerUI::SetResourceZip(_T("530CB693-BE42-4A2B-A87D-B14B4D6C957C"));
#endif

	GdiplusStartupInput   gdiplusStartupInput;
	ULONG_PTR             gdiplusToken;
	::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	::CoInitialize(NULL);
	::OleInitialize(NULL);

	CPlatform::InitAppPath(hInstance);
	Init();
	WndHelper::CreateLogonWnd();

	typedef BOOL (WINAPI FAR *CHANGEWINDOWMESSAGEFILTER)(UINT,DWORD); 
	CHANGEWINDOWMESSAGEFILTER lpChangeWindowMessageFilter; 
	lpChangeWindowMessageFilter = (CHANGEWINDOWMESSAGEFILTER)::GetProcAddress (::GetModuleHandle(_T("USER32")),"ChangeWindowMessageFilter"); 
	if (lpChangeWindowMessageFilter) 
	{ 
		lpChangeWindowMessageFilter(WM_SHOW_APP,MSGFLT_ADD); 
	}
	
	::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	CCrashManager::GetInstance()->UploadingDumpFile();

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();

	Destroy();

	::GdiplusShutdown(gdiplusToken);
	::OleUninitialize();
	::CoUninitialize();

	return 0;
}