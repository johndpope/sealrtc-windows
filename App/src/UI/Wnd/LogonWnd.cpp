#include "StdAfx.h"
#include "LogonWnd.h"
#include "WndHelper.h"
#include "PeerConnectionHelper.h"
#include "UpStringUtility.h"
#include "..\jsoncpp\include\json\json.h"
#include "UpDictManager.h"
#include "Integer.h"

IMPL_APPWND_CONTROL(LogonWnd, ID_WND_LOGON)

std::wstring wstrUserDataFileName = _T("user_") + UpStringUtility::StringToWstring(APP_VERSION) + _T(".data");

std::string strNaviUrl = "https://navqa.cn.ronghub.com";
std::string strGetCodeUrl = "https://sealrtc.rongcloud.cn/user/send_code";
std::string strGetTokenUrl = "https://sealrtc.rongcloud.cn/user/verify_code";
std::string strAppKey = "z3v5yqkbv8v30";

LogonWnd::LogonWnd(void):m_bFromMain(false),
m_bRelease(true),
m_iGetCodeTime(30)
{
#ifdef APP_Release
	m_bRelease = true;
#else
	m_bRelease = false;
#endif // APP_Release

	SetSkinFile(_T("logon.xml"));
}

LogonWnd::~LogonWnd(void)
{
}

NOTIFY_BEGIN(LogonWnd)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("closebtn"), OnClickCloseBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("channel_id_tip"), OnClickChannelEditTip)
	ON_NOTIFY_MSG(DUI_MSGTYPE_KILLFOCUS, _T("channel_edit"), OnChannelEditKillFocus)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("name_tip"), OnClickNameEditTip)
	ON_NOTIFY_MSG(DUI_MSGTYPE_KILLFOCUS, _T("name_edit"), OnNameEditKillFocus)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("phone_tip"), OnClickPhoneEditTip)
	ON_NOTIFY_MSG(DUI_MSGTYPE_KILLFOCUS, _T("phone_edit"), OnPhoneEditKillFocus)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("verify_code_tip"), OnClickVerifyCodeEditTip)
	ON_NOTIFY_MSG(DUI_MSGTYPE_KILLFOCUS, _T("verify_code_edit"), OnVerifyCodeEditKillFocus)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("get_verify_code"), OnClickGetVerifyCodeBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("join_channel_btn"), OnClickJoinChannelBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("select_video"), OnVideoCheckBoxChanged)
	ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("select_observer"), OnObserverCheckBoxChanged)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("setting_btn"), OnClickSettingBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("setting_text_btn"), OnClickSettingBtn)
NOTIFY_END()

LRESULT LogonWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (VK_RETURN == wParam)
		{
			DoClickJoinChannelBtn();
		}
		else
		{
			bHandled = FALSE;
		}
		break;
	}
	case WM_UP_BROADCAST:
	{
		if (wParam == BDT_GET_TOKEN_OK)
		{
			MsgItem* pMsgItem = (MsgItem*)lParam;
			if (pMsgItem)
			{
				PeerConnectionHelper::m_strToken = pMsgItem->GetBuf(0);
				SaveRoomInfo();
			}
			CBaseWnd* pMainWnd = WndHelper::CreateMainWnd();
			if (pMainWnd)
			{
				Close();
			}
		}
		else if (wParam == BDT_GET_CODE_FAILED)
		{
			std::wstring wstrTip = UPSTR(logon_get_verify_code_failed_tip);
			MsgItem* pMsgItem = (MsgItem*)lParam;
			if (pMsgItem)
			{
				wstrTip.append(L" ");
				wstrTip.append(UpStringUtility::UTF8ToUnicode(pMsgItem->GetBuf(0)));
			}
			::MessageBox(m_hWnd, wstrTip.c_str(), UPSTR(app_tip).c_str(), S_OK);
		}
		else if (wParam == BDT_GET_TOKEN_FAILED)
		{
			std::wstring wstrTip = UPSTR(logon_get_token_failed_tip);
			MsgItem* pMsgItem = (MsgItem*)lParam;
			if (pMsgItem)
			{
				wstrTip.append(L" ");
				wstrTip.append(UpStringUtility::UTF8ToUnicode(pMsgItem->GetBuf(0)));
			}
			::MessageBox(m_hWnd, wstrTip.c_str(), UPSTR(app_tip).c_str(), S_OK);
		}
	}
	default:
		bHandled = FALSE;
		break;
	}
	if (bHandled) return lRes;

    return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT LogonWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == 0)
	{
		bHandled = TRUE;
		::PostQuitMessage(0);
	}
	return __super::OnClose(uMsg, wParam, lParam,bHandled);
}

void LogonWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LPCTSTR LogonWnd::GetWindowClassName() const
{
	return _T("LogonWnd");
}

void LogonWnd::Timeout(UpTimer * timer)
{
	if (timer == &m_clGetCodeTimer)
	{
		CButtonUI* pGetVerifyCodeBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("get_verify_code")));
		if (pGetVerifyCodeBtn)
		{
			if (m_iGetCodeTime == 0)
			{
				m_clGetCodeTimer.Stop();
				m_iGetCodeTime = 30;
				pGetVerifyCodeBtn->SetText(UPSTR(logon_get_verify_code_tip).c_str());
				pGetVerifyCodeBtn->SetEnabled(true);
			}
			else
			{
				m_iGetCodeTime--;
				pGetVerifyCodeBtn->SetText(up_utility::Integer::ToWString(m_iGetCodeTime).c_str());
			}
		}
	}
}

void LogonWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{
	m_bFromMain = *(bool*)(void*)aInitData.Find(_T("bFromMain"));
}

void LogonWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);

	InitText();

	if (m_bFromMain == false)
	{
		std::wstring wstrAppKey = UpStringUtility::UTF8ToUnicode(strAppKey);
		char strFile[64];
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		const char* strFormat = "%04d%02d%02d%02d%02d%02d";
		sprintf(strFile, strFormat, sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
		std::wstring strLogDir = CPlatform::GetAppDataPath(); 
		std::wstring strUserDataFilePath = strLogDir;
		strLogDir.append(wstrAppKey);
		strLogDir.append(L"\\");
		RongRTCEngine::SetLogEnabled(true, UpStringUtility::UnicodeToUTF8(strLogDir).c_str(), strFile);
		strUserDataFilePath.append(wstrAppKey);
		strUserDataFilePath.append(L"\\");
		strUserDataFilePath.append(wstrUserDataFileName);
		if (UpFile::IsFileExistedW(strUserDataFilePath.c_str()))
		{
			std::string* pJson = UpFile::ReadFileToString(strUserDataFilePath);
			Json::Reader reader;
			Json::Value jmessage;
			if (reader.parse(*pJson, jmessage))
			{
				PeerConnectionHelper::m_strUserName = jmessage["name"].asString();
				PeerConnectionHelper::m_strChannelId = jmessage["room_id"].asString();
				PeerConnectionHelper::m_strPhone = jmessage["phone"].asString();
				PeerConnectionHelper::m_strToken = jmessage["token"].asString();
			}
			UpFile::ReleaseFileString(pJson);
		}
		int i = RongRTCEngine::InitWithAppKey(strAppKey.c_str(), strNaviUrl.c_str(),
			PeerConnectionHelper::GetInstance());
		RongRTCEngineDev::InitDev(PeerConnectionHelper::GetInstance());
	}
	PeerConnectionHelper::m_strUserId = CPlatform::GetMac();
	InitCtrl();
}

void LogonWnd::OnClickCloseBtn(TNotifyUI &msg)
{
	::PostQuitMessage(0);
}

void LogonWnd::OnClickChannelEditTip(TNotifyUI & msg)
{
	CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
	if (pButton && pEdit)
	{
		pButton->SetVisible(false);
		pEdit->SetFocus();
	}
}

void LogonWnd::OnChannelEditKillFocus(TNotifyUI & msg)
{
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(msg.pSender);
	if (pEdit && pEdit->GetText().GetLength() == 0)
	{
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("channel_id_tip")));
		if (pButton) pButton->SetVisible(true);
	}
}

void LogonWnd::OnClickNameEditTip(TNotifyUI & msg)
{
	CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("name_edit")));
	if (pButton && pEdit)
	{
		pButton->SetVisible(false);
		pEdit->SetFocus();
	}
}

void LogonWnd::OnNameEditKillFocus(TNotifyUI & msg)
{
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(msg.pSender);
	if (pEdit && pEdit->GetText().GetLength() == 0)
	{
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("name_tip")));
		if (pButton) pButton->SetVisible(true);
	}
}

void LogonWnd::OnClickPhoneEditTip(TNotifyUI & msg)
{
	CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("phone_edit")));
	if (pButton && pEdit)
	{
		pButton->SetVisible(false);
		pEdit->SetFocus();
	}
}

void LogonWnd::OnPhoneEditKillFocus(TNotifyUI & msg)
{
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(msg.pSender);
	if (pEdit && pEdit->GetText().GetLength() == 0)
	{
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("phone_tip")));
		if (pButton) pButton->SetVisible(true);
	}
}

void LogonWnd::OnClickVerifyCodeEditTip(TNotifyUI & msg)
{
	CButtonUI* pButton = static_cast<CButtonUI*>(msg.pSender);
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("verify_code_edit")));
	if (pButton && pEdit)
	{
		pButton->SetVisible(false);
		pEdit->SetFocus();
	}
}

void LogonWnd::OnVerifyCodeEditKillFocus(TNotifyUI & msg)
{
	CRichEditUI* pEdit = static_cast<CRichEditUI*>(msg.pSender);
	if (pEdit && pEdit->GetText().GetLength() == 0)
	{
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("verify_code_tip")));
		if (pButton) pButton->SetVisible(true);
	}
}

void LogonWnd::OnClickGetVerifyCodeBtn(TNotifyUI & msg)
{
	CButtonUI* pBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pBtn)
	{
		pBtn->SetText(_T("30"));
		pBtn->SetEnabled(false);
		m_clGetCodeTimer.Stop();
		m_clGetCodeTimer.Start(1000,this);
		CRichEditUI* pSecondPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("second_phone_edit")));
		if (pSecondPhoneEdit)
		{
			GetCode(UpStringUtility::UnicodeToUTF8(pSecondPhoneEdit->GetText().GetData()));
		}
	}
}

void LogonWnd::OnClickJoinChannelBtn(TNotifyUI &msg)
{
	DoClickJoinChannelBtn();
}

void LogonWnd::OnVideoCheckBoxChanged(TNotifyUI & msg)
{
	CCheckBoxUI* pVideoCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
	if (pVideoCheckBox->IsSelected())
	{
		CCheckBoxUI* pObserverCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer"))); 
		if (pObserverCheckBox)
		{
			pObserverCheckBox->ChangeSelected(false);
		}
	}
}

void LogonWnd::OnObserverCheckBoxChanged(TNotifyUI & msg)
{
	CCheckBoxUI* pObserverCheckBox = static_cast<CCheckBoxUI*>(msg.pSender);
	if (pObserverCheckBox->IsSelected())
	{
		CCheckBoxUI* pVideoCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
		if (pVideoCheckBox)
		{
			pVideoCheckBox->ChangeSelected(false);
		}
	}
}

void LogonWnd::OnClickSettingBtn(TNotifyUI & msg)
{
	CVerticalLayoutUI* pCenterLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("center_layout")));
	CVerticalLayoutUI* pSettingLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("setting_layout"))); 
	if (pCenterLayout && pSettingLayout)
	{
		int iHeight = pCenterLayout->GetHeight();

		if (iHeight == 460)
		{
			pSettingLayout->SetVisible(true);
			pCenterLayout->SetFixedHeight(460 + 60);
		}
		else
		{
			pSettingLayout->SetVisible(false);
			pCenterLayout->SetFixedHeight(460);
		}
	}
}

void LogonWnd::DoClickJoinChannelBtn()
{
	CVerticalLayoutUI* pFirstLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("first_layout")));
	CVerticalLayoutUI* pSecondLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("second_layout")));
	if (pFirstLayout && pSecondLayout)
	{
		if (pFirstLayout->IsVisible())
		{
			CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pFirstLayout, _T("channel_edit")));
			if (pChannelEdit)
			{
				if (pChannelEdit->GetText().GetLength() == 0)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_id_empty_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
			}

			CRichEditUI* pNameEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pFirstLayout, _T("name_edit")));
			if (pNameEdit)
			{
				int iRet = CheckName(pNameEdit->GetText());
				if (iRet == 1)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_name_char_error_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
				else if (iRet == 2)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_name_too_long_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
				else if (iRet == 3)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_name_empty_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
			}

			CRichEditUI* pPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pFirstLayout, _T("phone_edit")));
			if (pPhoneEdit)
			{
				if (pPhoneEdit->GetText().GetLength() == 0)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_phone_empty_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
			}
			std::string strPhone = UpStringUtility::UnicodeToUTF8(pPhoneEdit->GetText().GetData());
			if (PeerConnectionHelper::m_strToken.length() == 0 || strPhone.compare(PeerConnectionHelper::m_strPhone) != 0)
			{
				CRichEditUI* pSecondPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSecondLayout, _T("second_phone_edit")));
				if (pSecondPhoneEdit) pSecondPhoneEdit->SetText(UpStringUtility::UTF8ToUnicode(strPhone).c_str());
				pFirstLayout->SetVisible(false);
				pSecondLayout->SetVisible(true);
			}
			else
			{
				SaveRoomInfo();
				CBaseWnd* pMainWnd = WndHelper::CreateMainWnd();
				if (pMainWnd)
				{
					Close();
				}
			}
		}
		else
		{
			CRichEditUI* pVerifyCodeEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSecondLayout, _T("verify_code_edit")));
			if (pVerifyCodeEdit)
			{
				if (pVerifyCodeEdit->GetText().GetLength() == 0)
				{
					::MessageBox(m_hWnd, UPSTR(logon_channel_verify_code_empty_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
					return;
				}
			}
			CRichEditUI* pSecondPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSecondLayout, _T("second_phone_edit")));
			if (pSecondPhoneEdit)
			{
				GetToken(UpStringUtility::UnicodeToUTF8(pSecondPhoneEdit->GetText().GetData()).c_str(),
					UpStringUtility::UnicodeToUTF8(pVerifyCodeEdit->GetText().GetData()).c_str());
			}
		}
	}

	COptionUI* pOption_1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_1")));
	COptionUI* pOption_2 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_2")));
	COptionUI* pOption_3 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_3")));
	COptionUI* pOption_4 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_4")));
	if (pOption_1 && pOption_2 && pOption_3 && pOption_4)
	{
		if (pOption_1->IsSelected())
		{
			PeerConnectionHelper::m_iResolutionType = RongRTCVideoSolution_320_240;
		}
		else if (pOption_2->IsSelected())
		{
			PeerConnectionHelper::m_iResolutionType = RongRTCVideoSolution_640_480;
		}
		else if (pOption_3->IsSelected())
		{
			PeerConnectionHelper::m_iResolutionType = RongRTCVideoSolution_1280_720;
		}
		else if (pOption_4->IsSelected())
		{
			PeerConnectionHelper::m_iResolutionType = RongRTCVideoSolution_1920_1080;
		}
	}
	CCheckBoxUI* pSelectVideo = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
	CCheckBoxUI* pSelectObserver = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
	if (pSelectVideo && pSelectObserver)
	{
		if (pSelectObserver->GetCheck())
		{
			PeerConnectionHelper::m_iResourceType = 0;
		}
		else
		{
			PeerConnectionHelper::m_iResourceType = pSelectVideo->GetCheck() ? RongRTCStreamType_Audio : RongRTCStreamType_AudioVideo;
		}
	}
	CCheckBoxUI* pSelectTiny = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
	if (pSelectTiny)
	{
		PeerConnectionHelper::m_bOpenTinyStream = pSelectTiny->GetCheck() ? true : false;
	}
}

void LogonWnd::GetToken(std::string strPhone, std::string strVerifyCode)
{
	Json::Value root;
	root["phone"] = strPhone;
	root["region"] = "86";
	root["code"] = strVerifyCode;
	root["key"] = PeerConnectionHelper::m_strUserId; //user_id
	std::string strData;
	strData = root.toStyledString();
	std::wstring strHeader;
	strHeader.append(L"Content-type: application/json\r\n");

	if (strGetTokenUrl.find("https") != std::string::npos)
	{
		m_clGetToken.SendHttpsRequest(new HttpRequestGetToken(), strGetTokenUrl.c_str(), strData.c_str(), strData.length(), &strHeader);
	}
	else
	{
		m_clGetToken.SendHttpRequest(new HttpRequestGetToken(), strGetTokenUrl.c_str(), strData.c_str(), strData.length(), &strHeader);
	}
}

void LogonWnd::GetCode(std::string strPhone)
{
	Json::Value root;
	root["phone"] = strPhone;
	root["region"] = "86";
	std::string strData;
	strData = root.toStyledString();
	std::wstring strHeader;
	strHeader.append(L"Content-type: application/json\r\n");

	if (strGetCodeUrl.find("https") != std::string::npos)
	{
		m_clGetCode.SendHttpsRequest(new HttpRequestGetCode(), strGetCodeUrl.c_str(), strData.c_str(), strData.length(), &strHeader);
	}
	else
	{
		m_clGetCode.SendHttpRequest(new HttpRequestGetCode(), strGetCodeUrl.c_str(), strData.c_str(), strData.length(), &strHeader);
	}
}

int LogonWnd::CheckName(CDuiString strName)
{
	int iNameCharLength = 0;
	for (int i = 0; i < strName.GetLength(); ++i)
	{
		TCHAR tempTchar = strName.GetAt(i);
		if (tempTchar >= 0x4e00 && tempTchar <=0x9fcb)
		{
			iNameCharLength += 2;
		}
		else if (tempTchar == 0x3000 || tempTchar == 0x0020)
		{
			iNameCharLength += 1;
		}
		else if (tempTchar <= 'z' && tempTchar >= 'a')
		{
			iNameCharLength += 1;
		}
		else if (tempTchar <= 'Z' && tempTchar >= 'A')
		{
			iNameCharLength += 1;
		}
		else
		{
			return 1;
		}
	}
	if (iNameCharLength > 12)
	{
		return 2;
	}
	else if (iNameCharLength == 0)
	{
		return 3;
	}
	return 0;
}

void LogonWnd::SaveRoomInfo()
{
	CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
	CRichEditUI* pNameEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("name_edit")));
	CRichEditUI* pPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("phone_edit")));
	if (pChannelEdit && pNameEdit && pPhoneEdit)
	{
		PeerConnectionHelper::m_strChannelId = UpStringUtility::UnicodeToUTF8(pChannelEdit->GetText().GetData());
		PeerConnectionHelper::m_strUserName = UpStringUtility::UnicodeToUTF8(pNameEdit->GetText().GetData());
		PeerConnectionHelper::m_strPhone = UpStringUtility::UnicodeToUTF8(pPhoneEdit->GetText().GetData());
	}
	if (PeerConnectionHelper::m_strChannelId.length() == 0
		|| PeerConnectionHelper::m_strUserName.length() == 0
		|| PeerConnectionHelper::m_strPhone.length() == 0
		|| PeerConnectionHelper::m_strToken.length() == 0)
	{
		return;
	}
	std::wstring wstrAppKey = UpStringUtility::UTF8ToUnicode(strAppKey);
	std::wstring strUserDataFilePath = CPlatform::GetAppDataPath();
	strUserDataFilePath.append(wstrAppKey);
	strUserDataFilePath.append(L"\\");
	strUserDataFilePath.append(wstrUserDataFileName);
	if (UpFile::IsFileExistedW(strUserDataFilePath.c_str()))
	{
		UpFile::DeleteExistFileW(strUserDataFilePath.c_str());
	}
	UpFile clFile(strUserDataFilePath.c_str());
	Json::Value info;
	info["room_id"] = PeerConnectionHelper::m_strChannelId;
	info["name"] = PeerConnectionHelper::m_strUserName;
	info["phone"] = PeerConnectionHelper::m_strPhone;
	info["token"] = PeerConnectionHelper::m_strToken;
	std::string strInfo = info.toStyledString();
	clFile.Write(strInfo.c_str(), strInfo.length());
}

void LogonWnd::InitText()
{
	CLabelUI* pTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("title")));
	if (pTitle) pTitle->SetText(UPSTR(logon_title).c_str());
	CLabelUI* pWelcomeTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("welcome_tip")));
	if (pWelcomeTip) pWelcomeTip->SetText(UPSTR(logon_welcome_tip).c_str());

	CLabelUI* pAppVersion = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("app_version")));
	if (pAppVersion)
	{
		CDuiString strText = UPSTR(app_name).c_str();
		strText.Append(UpStringUtility::StringToWstring(APP_VERSION).c_str());
		pAppVersion->SetText(strText);
	}
	CLabelUI* pInfoTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("info_tip")));
	if (pInfoTip) pInfoTip->SetText(UPSTR(logon_info_tip).c_str());
	CLabelUI* pInfo2Tip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("second_info_tip")));
	if (pInfo2Tip) pInfo2Tip->SetText(UPSTR(logon_info_2_tip).c_str());

	CLabelUI* pChannelIdTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("channel_id_tip")));
	if (pChannelIdTip) pChannelIdTip->SetText(UPSTR(logon_channel_id_tip).c_str());
	CLabelUI* pNameTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("name_tip")));
	if (pNameTip) pNameTip->SetText(UPSTR(logon_name_tip).c_str());
	CLabelUI* pPhoneTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("phone_tip")));
	if (pPhoneTip) pPhoneTip->SetText(UPSTR(logon_phone_tip).c_str());
	CLabelUI* pVerifyCodeTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("verify_code_tip")));
	if (pVerifyCodeTip) pVerifyCodeTip->SetText(UPSTR(logon_verify_code_tip).c_str());
	CButtonUI* pGetVerifyCode = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("get_verify_code")));
	if (pGetVerifyCode) pGetVerifyCode->SetText(UPSTR(logon_get_verify_code_tip).c_str());
	CButtonUI* pJoin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("join_channel_btn")));
	if (pJoin) pJoin->SetText(UPSTR(logon_join_channel_tip).c_str());
	CButtonUI* pSettingText = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_text_btn")));
	if (pSettingText) pSettingText->SetText(UPSTR(logon_channel_setting_tip).c_str());
	CCheckBoxUI* pSelectVideo = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
	if (pSelectVideo) pSelectVideo->SetText(UPSTR(logon_no_video_tip).c_str());
	CCheckBoxUI* pIsOberver = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
	if (pIsOberver) pIsOberver->SetText(UPSTR(logon_normal_join_tip).c_str());
	CLabelUI* pSelectSolutionTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("select_solution_tip")));
	if (pSelectSolutionTip) pSelectSolutionTip->SetText(UPSTR(logon_solution_tip).c_str());
	CCheckBoxUI* pSelectTinyStream = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
	if (pSelectTinyStream) pSelectTinyStream->SetText(UPSTR(logon_open_tiny_stream_tip).c_str());
}

void LogonWnd::InitCtrl()
{
	CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
	if (pChannelEdit)
	{
		pChannelEdit->SetText(UpStringUtility::UTF8ToUnicode(PeerConnectionHelper::m_strChannelId).c_str());
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("channel_id_tip")));
		if (pButton)
		{
			if (pChannelEdit->GetText().GetLength() == 0)
			{
				pButton->SetVisible(true);
			}
			else
			{
				pButton->SetVisible(false);
			}
		}
	}
	CRichEditUI* pNameEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("name_edit")));
	if (pNameEdit)
	{
		pNameEdit->SetText(UpStringUtility::UTF8ToUnicode(PeerConnectionHelper::m_strUserName).c_str());
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("name_tip")));
		if (pButton)
		{
			if (pNameEdit->GetText().GetLength() == 0)
			{
				pButton->SetVisible(true);
			}
			else
			{
				pButton->SetVisible(false);
			}
		}
	}
	CRichEditUI* pPhoneEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("phone_edit")));
	if (pPhoneEdit)
	{
		pPhoneEdit->SetText(UpStringUtility::UTF8ToUnicode(PeerConnectionHelper::m_strPhone).c_str());
		CButtonUI* pButton = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("phone_tip")));
		if (pButton)
		{
			if (pPhoneEdit->GetText().GetLength() == 0)
			{
				pButton->SetVisible(true);
			}
			else
			{
				pButton->SetVisible(false);
			}
		}
	}
	if (PeerConnectionHelper::m_iResolutionType == RongRTCVideoSolution_320_240)
	{
		COptionUI* pOption_1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_1")));
		if (pOption_1)
		{
			pOption_1->Selected(true);
		}
	}
	else if (PeerConnectionHelper::m_iResolutionType == RongRTCVideoSolution_1280_720)
	{
		COptionUI* pOption_3 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_3")));
		if (pOption_3)
		{
			pOption_3->Selected(true);
		}
	}
	else if (PeerConnectionHelper::m_iResolutionType == RongRTCVideoSolution_1920_1080)
	{

		COptionUI* pOption_4 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_4")));
		if (pOption_4)
		{
			pOption_4->Selected(true);
		}
	}
	else
	{
		COptionUI* pOption_2 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_2")));
		if (pOption_2)
		{
			pOption_2->Selected(true);
		}
	}
	CCheckBoxUI* pObserverCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
	CCheckBoxUI* pVideoCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
	if (pObserverCheckBox && pVideoCheckBox)
	{
		if (PeerConnectionHelper::m_iResourceType != 0)
		{
			pObserverCheckBox->SetCheck(false);
			pVideoCheckBox->SetCheck(PeerConnectionHelper::m_iResourceType == RongRTCStreamType_Audio? true: false);
		}
		else
		{
			pObserverCheckBox->SetCheck(true);
			pVideoCheckBox->SetCheck(false);
		}
	}
	CCheckBoxUI* pSelectTiny = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
	if (pSelectTiny)
	{
		pSelectTiny->SetCheck(PeerConnectionHelper::m_bOpenTinyStream);
	}
	CButtonUI* pJoinChannelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("join_channel_btn")));
	if (pJoinChannelBtn)
	{
		pJoinChannelBtn->SetFocus();
	}
}

void HttpRequestGetToken::OnHttpRequestSuccess(const char * headers, int headerLen, const char * body, int bodyLen)
{
	std::string strHeaders;
	strHeaders.append(headers);
	int nPos = strHeaders.find(" ", 0);
	if (nPos != -1)
	{
		int nCr = strHeaders.find(" ", nPos + 1);
		std::string strValue;
		strValue = strHeaders.substr(nPos + 1, nCr - nPos);
		DWORD dwValue = atoi(strValue.c_str());
		bool bDelete = false;
		if (dwValue == 200)
		{
			std::string strData;
			strData.append(body,bodyLen);
			Json::Value root;
			Json::Reader reader;
			if (reader.parse(strData, root))
			{
				int code = root["code"].asInt();
				if (code == 200)
				{
					MsgItem* pMsgItem = new MsgItem(root["result"]["token"].asString());
					MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_TOKEN_OK, pMsgItem);
				}
				else
				{
					MsgItem* pMsgItem = new MsgItem(root["message"].asString());
					MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_TOKEN_FAILED, pMsgItem);
				}
			}
		}
	}
}

void HttpRequestGetToken::OnHttpRequestFaild(int error)
{
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_TOKEN_FAILED, NULL);
}

void HttpRequestGetCode::OnHttpRequestSuccess(const char * headers, int headerLen, const char * body, int bodyLen)
{
	std::string strHeaders;
	strHeaders.append(headers);
	int nPos = strHeaders.find(" ", 0);
	if (nPos != -1)
	{
		int nCr = strHeaders.find(" ", nPos + 1);
		std::string strValue;
		strValue = strHeaders.substr(nPos + 1, nCr - nPos);
		DWORD dwValue = atoi(strValue.c_str());
		bool bDelete = false;
		if (dwValue == 200)
		{
		}
	}
}

void HttpRequestGetCode::OnHttpRequestFaild(int error)
{
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_CODE_FAILED, NULL);
}
