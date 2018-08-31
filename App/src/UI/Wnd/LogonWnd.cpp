#include "StdAfx.h"
#include "LogonWnd.h"
#include "WndHelper.h"
#include "PeerConnectionHelper.h"
#include "UpStringUtility.h"
#include "..\jsoncpp\include\json\json.h"
#include "UpDictManager.h"

IMPL_APPWND_CONTROL(LogonWnd, ID_WND_LOGON)

std::wstring wstrServerConfigFileName = _T("app.config");
std::wstring wstrUserDataFileName = _T("user.data");
std::string strConfigListTcpUrl = "https://rtcapi.ronghub.com/nav/rtclist";
std::string strDemoAppKey = "x4vkb1qpxfrzk";

LogonWnd::LogonWnd(void):m_bFromMain(false),
m_bGetTokenSuccess(false),
m_bGetCrtSuccess(false),
m_bGetConfigListSuccess(false),
m_bQuic(false)
{
	SetSkinFile(_T("logon.xml"));
}

LogonWnd::~LogonWnd(void)
{
	ClearServerConfig();
}

NOTIFY_BEGIN(LogonWnd)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("closebtn"), OnClickCloseBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("join_channel_btn"), OnClickJoinChannelBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_ITEMCLICK, _T("network_item"), OnClickNetworkListItem)
	ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("select_video"), OnVideoCheckBoxChanged)
	ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("select_observer"), OnObserverCheckBoxChanged)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("setting_btn"), OnClickSettingBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("setting_text_btn"), OnClickSettingBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("modify_btn"), OnClickSettingOKBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("empty_btn"), OnClickSettingEmptyBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("cancel_modify_btn"), OnClickSettingCancelBtn)
	ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("open_setting_btn"), OnClickShowSettingLayout)
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
			CButtonUI* pJoinChannelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("join_channel_btn")));
			if (pJoinChannelBtn && pJoinChannelBtn->IsEnabled())
			{
				JoinChannel();
			}
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
			if (m_bGetTokenSuccess == false)
			{
				m_bGetTokenSuccess = true;

				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					bool bCanInit = SetToken(pMsgItem->GetBuf(0));
					if (bCanInit)
					{
						bool bRet = InitRtcEngine("");
						if (bRet)
						{
							SetJoinChannelEnabled(true);
							CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
							if (pChannelEdit) pChannelEdit->SetFocus();
						}
					}
					else
					{
						m_clGetCrtTimer.Stop();
						m_clGetCrtTimer.Start(500, this);
					}
				}
			}
		}
		else if (wParam == BDT_GET_SERVER_CONFIG)
		{
			if (m_bGetConfigListSuccess == false)
			{
				m_bGetConfigListSuccess = true;

				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					InitServerConfig(pMsgItem->GetBuf(0));

					m_clGetTokenTimer.Stop();
					m_clGetTokenTimer.Start(500, this);
				}
				CButtonUI* pSettingBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_btn")));
				if (pSettingBtn) pSettingBtn->SetFocus();
			}
		}
		else if (wParam == BDT_GET_CRT_OK)
		{
			if (m_bGetCrtSuccess == false)
			{
				m_bGetCrtSuccess = true;

				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					bool bRet = InitRtcEngine(pMsgItem->GetBuf(0));
					if (bRet)
					{
						SetJoinChannelEnabled(true);
						CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
						if (pChannelEdit) pChannelEdit->SetFocus();
					}
				}
			}
		}
		break;
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

void LogonWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{
	m_bFromMain = *(bool*)(void*)aInitData.Find(_T("bFromMain"));
	m_strChannelId = *(CDuiString*)(void*)aInitData.Find(_T("strChannelId"));
}

void LogonWnd::Timeout(UpTimer * timer)
{
	if (timer == &m_clGetConfigListTimer)
	{
		m_clGetConfigListTimer.Stop();
		if (m_bGetConfigListSuccess == false)
		{
			GetConfigList();
			m_clGetConfigListTimer.Start(5000, this);
		}
	}
	else if (timer == &m_clGetTokenTimer)
	{
		m_clGetTokenTimer.Stop();
		
		if (m_bGetTokenSuccess == false)
		{
			bool bRet = GetToken(m_strNetworkName);
			if (bRet)
			{
				m_clGetTokenTimer.Start(5000, this);
			}
		}
	}
	else if (timer == &m_clGetCrtTimer)
	{
		m_clGetCrtTimer.Stop();

		if (m_bGetCrtSuccess == false)
		{
			bool bRet = GetCrt(m_strNetworkName);
			if (bRet)
			{
				m_clGetCrtTimer.Start(5000, this);
			}
		}
	}
}

void LogonWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
	
	InitText();

	CRichEditUI* pNameEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("name_edit")));
	if (pNameEdit)
	{
		std::wstring strUserDataFilePath = CPlatform::GetAppDataPath();
		strUserDataFilePath.append(wstrUserDataFileName);
		if (UpFile::IsFileExistedW(strUserDataFilePath.c_str()))
		{
			std::string* pJson = UpFile::ReadFileToString(strUserDataFilePath);
			pNameEdit->SetText(PaserName(*pJson).c_str());
			UpFile::ReleaseFileString(pJson);
		}
	}

	if (m_bFromMain)
	{
		CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
		if (pChannelEdit)
		{
			pChannelEdit->SetText(m_strChannelId);
		}
		if (PeerConnectionHelper::m_iResolutionType == RongRTCEngine::VideoProfile_320_240_15F)
		{
			COptionUI* pOption_1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_1")));
			if (pOption_1)
			{
				pOption_1->Selected(true);
			}
		}
		else if (PeerConnectionHelper::m_iResolutionType == RongRTCEngine::VideoProfile_1280_720_15F)
		{
			COptionUI* pOption_3 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_3")));
			if (pOption_3)
			{
				pOption_3->Selected(true);
			}
		}
		else if (PeerConnectionHelper::m_iResolutionType == RongRTCEngine::VideoProfile_1920_1080_15F)
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
		CCheckBoxUI* pSelectObserver = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
		if (pSelectObserver)
		{
			pSelectObserver->SetCheck(PeerConnectionHelper::m_bOpenObserver);
		}
		CCheckBoxUI* pSelectVideo = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
		if (pSelectVideo)
		{
			pSelectVideo->SetCheck(PeerConnectionHelper::m_bOpenOnlyAudio);
		}
		CCheckBoxUI* pSelectBeauty = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_beauty")));
		if (pSelectBeauty)
		{
			pSelectBeauty->SetCheck(PeerConnectionHelper::m_bOpenBeauty);
		}
		CCheckBoxUI* pSelectTiny = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
		if (pSelectTiny)
		{
			pSelectTiny->SetCheck(PeerConnectionHelper::m_bOpenTinyStream);
		}

		SetNetworkSelectLayoutVisible(false);
		SetJoinChannelEnabled(true);
	}
	else
	{
		SetNetworkSelectLayoutVisible(false);
		std::wstring strConfigFilePath = CPlatform::GetAppDataPath();
		strConfigFilePath.append(wstrServerConfigFileName);
		if (UpFile::IsFileExistedW(strConfigFilePath.c_str()))
		{
			//私有包，读取配置文件中服务器IP配置
			std::string* pJson = UpFile::ReadFileToString(strConfigFilePath);
			MsgItem* pMsgItem = new MsgItem(*pJson);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_SERVER_CONFIG, pMsgItem);
			UpFile::ReleaseFileString(pJson);
		}
		else
		{
			//共有包，服务器推送IP配置
			m_clGetConfigListTimer.Stop();
			m_clGetConfigListTimer.Start(500, this);
		}
	}
	CButtonUI* pOpenSettingBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("open_setting_btn")));
	if (pOpenSettingBtn)
	{
		RECT rc = { 0 };
		rc = pOpenSettingBtn->GetPos();
		rc.bottom = 598;
		pOpenSettingBtn->SetPos(rc);
	}
}

void LogonWnd::OnClickCloseBtn(TNotifyUI &msg)
{
	::PostQuitMessage(0);
}

void LogonWnd::OnClickJoinChannelBtn(TNotifyUI &msg)
{
	JoinChannel();
}

void LogonWnd::OnClickNetworkListItem(TNotifyUI & msg)
{
	SetNetworkSelectLayoutVisible(false);
	m_strNetworkName = msg.pSender->GetText();
	CListLabelElementUI * p_list_label_element_ui = dynamic_cast<CListLabelElementUI *>(msg.pSender);
	m_bGetTokenSuccess = false;
	m_clGetConfigListTimer.Stop();
	m_clGetTokenTimer.Stop();
	m_clGetTokenTimer.Start(500, this);
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
	CButtonUI* pSettingTextBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_text_btn")));
	CButtonUI* pSettingBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_btn")));
	if (pObserverCheckBox && pSettingTextBtn && pSettingBtn)
	{
		if (pObserverCheckBox->IsSelected())
		{
			CCheckBoxUI* pVideoCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
			if (pVideoCheckBox)
			{
				pVideoCheckBox->SetCheck(false);
			}
			CVerticalLayoutUI* pCenterLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("center_layout")));
			CVerticalLayoutUI* pSettingLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("setting_layout")));
			if (pCenterLayout && pSettingLayout)
			{
				pSettingLayout->SetVisible(false);
				pCenterLayout->SetFixedHeight(430);
			}
			pSettingTextBtn->SetEnabled(false);
			pSettingBtn->SetEnabled(false);
		}
		else
		{
			pSettingTextBtn->SetEnabled(true);
			pSettingBtn->SetEnabled(true);
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

		if (iHeight == 430)
		{
			pSettingLayout->SetVisible(true);
			pCenterLayout->SetFixedHeight(430 + 75);
		}
		else
		{
			pSettingLayout->SetVisible(false);
			pCenterLayout->SetFixedHeight(430);
		}
	}
}

void LogonWnd::OnClickSettingOKBtn(TNotifyUI & msg)
{
	CHorizontalLayoutUI* pSettingLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("setting_url_layout")));
	CRichEditUI* pCmpEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("cmp_edit")));
	CRichEditUI* pTokenEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("token_edit")));
	CRichEditUI* pAppKeyEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("appkey_edit")));
	CLabelUI* pSettingTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("setting_tip")));
	if (pSettingLayout && pCmpEdit && pTokenEdit && pAppKeyEdit && pSettingTip)
	{
		if (pCmpEdit->GetText().GetLength() <= 0)
		{
			pSettingTip->SetText(UPSTR(logon_change_ip_cmp_empty_tip).c_str());
			return;
		}
		else
		{
			if (pCmpEdit->GetText().Left(6).Compare(_T("tcp://")) != 0 && pCmpEdit->GetText().Left(7).Compare(_T("quic://")) != 0)
			{
				pSettingTip->SetText(UPSTR(logon_change_ip_cmp_error_tip).c_str());
				return;
			}
		}
		if (pTokenEdit->GetText().GetLength() <= 0)
		{
			pSettingTip->SetText(UPSTR(logon_change_ip_token_empty_tip).c_str());
			return;
		}
		if (pAppKeyEdit->GetText().GetLength() <= 0)
		{
			pSettingTip->SetText(UPSTR(logon_change_ip_appkey_empty_tip).c_str());
			return;
		}
		StopInit();
		SetJoinChannelEnabled(false);

		std::wstring strConfigFilePath = CPlatform::GetAppDataPath();
		strConfigFilePath.append(wstrServerConfigFileName);
		if (UpFile::IsFileExistedW(strConfigFilePath.c_str()))
		{
			UpFile::DeleteExistFileW(strConfigFilePath.c_str());
		}
		UpFile clFile(strConfigFilePath.c_str());
		std::string strInfo = "[{\"cmp\":\"%s\",\"token\":\"%s\",\"appkey\":\"%s\"}]";
		char info[512] = { 0 };
		sprintf_s(info, strInfo.c_str(), UpStringUtility::WcharToChar(pCmpEdit->GetText().GetData()), UpStringUtility::WcharToChar(pTokenEdit->GetText().GetData()), UpStringUtility::WcharToChar(pAppKeyEdit->GetText().GetData()));
		clFile.Write(info, strlen(info));

		pCmpEdit->SetText(_T(""));
		pTokenEdit->SetText(_T(""));
		pAppKeyEdit->SetText(_T(""));
		pSettingTip->SetText(_T(""));

		RECT rc = { 0 };
		rc = pSettingLayout->GetPos();
		rc.bottom = rc.top;
		pSettingLayout->SetPos(rc);

		std::string strJson;
		strJson.append(info);
		MsgItem* pMsgItem = new MsgItem(strJson);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_SERVER_CONFIG, pMsgItem);
	}
}

void LogonWnd::OnClickSettingEmptyBtn(TNotifyUI & msg)
{
	CHorizontalLayoutUI* pSettingLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("setting_url_layout")));
	CRichEditUI* pCmpEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("cmp_edit")));
	CRichEditUI* pTokenEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("token_edit")));
	CRichEditUI* pAppKeyEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("appkey_edit")));
	CLabelUI* pSettingTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("setting_tip")));
	if (pSettingLayout && pCmpEdit && pTokenEdit && pAppKeyEdit && pSettingTip)
	{
		StopInit();

		SetJoinChannelEnabled(false);

		pCmpEdit->SetText(_T(""));
		pTokenEdit->SetText(_T(""));
		pAppKeyEdit->SetText(_T(""));
		pSettingTip->SetText(_T(""));
		RECT rc = { 0 };
		rc = pSettingLayout->GetPos();
		rc.bottom = rc.top;
		pSettingLayout->SetPos(rc);
		std::wstring strConfigFilePath = CPlatform::GetAppDataPath();
		strConfigFilePath.append(wstrServerConfigFileName);
		if (UpFile::IsFileExistedW(strConfigFilePath.c_str()))
		{
			UpFile::DeleteExistFileW(strConfigFilePath.c_str());
		}
		m_clGetConfigListTimer.Stop();
		m_clGetConfigListTimer.Start(500, this);
	}
}

void LogonWnd::OnClickSettingCancelBtn(TNotifyUI & msg)
{
	CHorizontalLayoutUI* pSettingLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("setting_url_layout")));
	CRichEditUI* pCmpEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("cmp_edit")));
	CRichEditUI* pTokenEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("token_edit")));
	CRichEditUI* pAppKeyEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("appkey_edit")));
	CLabelUI* pSettingTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("setting_tip")));
	if (pSettingLayout && pCmpEdit && pTokenEdit && pAppKeyEdit && pSettingTip)
	{
		pCmpEdit->SetText(_T(""));
		pTokenEdit->SetText(_T(""));
		pAppKeyEdit->SetText(_T(""));
		pSettingTip->SetText(_T(""));
		RECT rc = {0};
		rc = pSettingLayout->GetPos();
		rc.bottom = rc.top;
		pSettingLayout->SetPos(rc);
	}
}

void LogonWnd::OnClickShowSettingLayout(TNotifyUI & msg)
{
	CHorizontalLayoutUI* pSettingLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("setting_url_layout")));
	CRichEditUI* pCmpEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("cmp_edit")));
	CRichEditUI* pTokenEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("token_edit")));
	CRichEditUI* pAppKeyEdit = static_cast<CRichEditUI*>(m_PaintManager.FindSubControlByName(pSettingLayout, _T("appkey_edit")));
	if (pSettingLayout && pCmpEdit && pTokenEdit && pAppKeyEdit)
	{
		RECT rc = {0};
		rc = pSettingLayout->GetPos();
		if (rc.top == rc.bottom)
		{
			rc.bottom = 568;
			pSettingLayout->SetPos(rc);

			std::wstring strConfigFilePath = CPlatform::GetAppDataPath();
			strConfigFilePath.append(wstrServerConfigFileName);
			if (UpFile::IsFileExistedW(strConfigFilePath.c_str()))
			{
				std::string* pJson = UpFile::ReadFileToString(strConfigFilePath);
				Json::Reader reader;
				Json::Value jmessage;
				if (pJson)
				{
					if (reader.parse(*pJson, jmessage))
					{
						int iSize = jmessage.size();
						if (iSize == 1)
						{
							pCmpEdit->SetText(UpStringUtility::UTF8ToUnicode(jmessage[0]["cmp"].asString()).c_str());
							pTokenEdit->SetText(UpStringUtility::UTF8ToUnicode(jmessage[0]["token"].asString()).c_str());
							pAppKeyEdit->SetText(UpStringUtility::UTF8ToUnicode(jmessage[0]["appkey"].asString()).c_str());
						}
					}
					UpFile::ReleaseFileString(pJson);
				}
			}
			pCmpEdit->SetFocus();
		}
	}
}

void LogonWnd::JoinChannel()
{
	CRichEditUI* pChannelEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("channel_edit")));
	if (pChannelEdit)
	{
		if (pChannelEdit->GetText().GetLength() == 0)
		{
			::MessageBox(m_hWnd, UPSTR(logon_channel_id_empty_tip).c_str(), UPSTR(app_tip).c_str(), S_OK);
			return;
		}
	}

	CRichEditUI* pNameEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("name_edit")));
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
		std::wstring strUserDataFilePath = CPlatform::GetAppDataPath();
		strUserDataFilePath.append(wstrUserDataFileName);
		if (UpFile::IsFileExistedW(strUserDataFilePath.c_str()))
		{
			UpFile::DeleteExistFileW(strUserDataFilePath.c_str());
		}
		UpFile clFile(strUserDataFilePath.c_str());
		std::string strInfo = "[{\"name\":\"%s\"}]";
		char info[128] = { 0 };
		sprintf_s(info, strInfo.c_str(), UpStringUtility::WcharToChar(pNameEdit->GetText().GetData()));
		clFile.Write(info, strlen(info));
	}

	COptionUI* pOption_1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_1")));
	COptionUI* pOption_2 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_2")));
	COptionUI* pOption_3 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_3")));
	COptionUI* pOption_4 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_4")));

	if (pOption_1 && pOption_2 && pOption_3 && pOption_4)
	{
		if (pOption_1->IsSelected())
		{
			RongRTCEngine::SelectResolution(RongRTCEngine::VideoProfile_320_240_15F);
			PeerConnectionHelper::m_iResolutionType = RongRTCEngine::VideoProfile_320_240_15F;
		}
		else if (pOption_2->IsSelected())
		{
			RongRTCEngine::SelectResolution(RongRTCEngine::VideoProfile_640_480_15F);
			PeerConnectionHelper::m_iResolutionType = RongRTCEngine::VideoProfile_640_480_15F;
		}
		else if (pOption_3->IsSelected())
		{
			RongRTCEngine::SelectResolution(RongRTCEngine::VideoProfile_1280_720_15F);
			PeerConnectionHelper::m_iResolutionType = RongRTCEngine::VideoProfile_1280_720_15F;
		}
		else if (pOption_4->IsSelected())
		{
			RongRTCEngine::SelectResolution(RongRTCEngine::VideoProfile_1920_1080_15F);
			PeerConnectionHelper::m_iResolutionType = RongRTCEngine::VideoProfile_1920_1080_15F;
		}
	}

	CCheckBoxUI* pSelectObserver = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
	if (pSelectObserver)
	{
		PeerConnectionHelper::m_bOpenObserver = pSelectObserver->GetCheck() ? true : false;
		PeerConnectionHelper::m_iRoleType = pSelectObserver->GetCheck() ? 2 : 1;
	}
	CCheckBoxUI* pSelectVideo = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
	if (pSelectVideo)
	{
		PeerConnectionHelper::m_bOpenOnlyAudio = pSelectVideo->GetCheck() ? true : false;
		PeerConnectionHelper::m_iMediaType = pSelectVideo->GetCheck() ? 0 : 1;
	}
	CCheckBoxUI* pSelectBeauty = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_beauty")));
	if (pSelectBeauty)
	{
		PeerConnectionHelper::m_bOpenBeauty = pSelectBeauty->GetCheck() ? true : false;
		RongRTCEngine::SetBeautyEnabled(PeerConnectionHelper::m_bOpenBeauty);
	}
	CCheckBoxUI* pSelectTiny = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
	if (pSelectTiny)
	{
		PeerConnectionHelper::m_bOpenTinyStream = pSelectTiny->GetCheck() ? true : false;
		RongRTCEngine::SetTinyStreamEnabled(PeerConnectionHelper::m_bOpenTinyStream);
	}

	m_strChannelId = pChannelEdit->GetText();
	CBaseWnd* pMainWnd = WndHelper::CreateMainWnd(pChannelEdit->GetText(), pNameEdit->GetText());
	if (pMainWnd)
	{
		Close();
	}
}

void LogonWnd::SetJoinChannelEnabled(bool bEnable)
{
	CButtonUI* pJoinChannelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("join_channel_btn")));
	if (pJoinChannelBtn)
	{
		pJoinChannelBtn->SetEnabled(bEnable);
	}
}

void LogonWnd::SetNetworkSelectLayoutVisible(bool bVisible)
{
	CHorizontalLayoutUI* pNetworkSelectLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("network_select_layout")));
	if (pNetworkSelectLayout)
	{
		pNetworkSelectLayout->SetVisible(bVisible);
	}
}

bool LogonWnd::GetToken(std::wstring strNetworkName)
{
	bool bRet = false;
	std::string strToken;
	ServerConfig* pServerConfig = GetServerConfig(strNetworkName);
	if (pServerConfig)
	{
		strToken = pServerConfig->strToken;
	}
	if (strToken.length() > 0)
	{
		if (PeerConnectionHelper::m_strUserId.empty())
		{
			PeerConnectionHelper::m_strUserId = CPlatform::GetMac();
		}
		std::string strAppKey = strDemoAppKey;
		if (pServerConfig->strAppKey.length() > 0)
		{
			strAppKey = pServerConfig->strAppKey;
		}
		std::string strData;
		strData = "uid=" + PeerConnectionHelper::m_strUserId + "&appid=" + strAppKey;
		std::wstring strHeader;
		strHeader.append(L"Content-type: application/x-www-form-urlencoded\r\n");

		std::string strTokenType = strToken.substr(0, 5);
		if (strTokenType.compare("https") == 0)
		{
			if (m_bQuic)
			{

			}
			else
			{

			}
			m_clGetToken.SendHttpsRequest(new HttpRequestGetToken(), strToken.c_str(), strData.c_str(), strData.length(), &strHeader);
		}
		else
		{
			if (m_bQuic)
			{

			}
			else
			{

			}
			m_clGetToken.SendHttpRequest(new HttpRequestGetToken(), strToken.c_str(), strData.c_str(), strData.length(), &strHeader);
		}
		bRet = true;
	}
	return bRet;
}

bool LogonWnd::SetToken(std::string strToken)
{
	char szSDKVersion[128] = { 0 };
	RongRTCEngine::GetSDKVersion(szSDKVersion);
	PeerConnectionHelper::m_strSDKVersion.clear();
	PeerConnectionHelper::m_strSDKVersion.append(szSDKVersion);
	PeerConnectionHelper::m_strToken.clear();
	PeerConnectionHelper::m_strToken.append(strToken);
	std::wstring wstrDataPath = CPlatform::GetAppDataPath();
	wstrDataPath.append(L"RongRTCLog.txt");

	std::string strDataPath = UpStringUtility::WstringToString(wstrDataPath);
	std::wstring wstrUploadLogPath = CPlatform::GetAppDataPath(); 
	wstrUploadLogPath.append(L"UploadLog\\");
	std::string strUploadLogPath = UpStringUtility::WstringToString(wstrUploadLogPath);
	
	RongRTCEngine::SetLogEnabled(RongRTCEngine::Engine_LL_WARNING, strDataPath.c_str());
	
	bool bCanInit = false;
	ServerConfig* pServerConfig = GetServerConfig(m_strNetworkName);
	if (pServerConfig)
	{
		if (pServerConfig->strCmp.length() >0 && pServerConfig->strSniffer.length() >0 &&
			pServerConfig->strCRT.length() > 0)
		{
			bCanInit = false;
		}
		else
		{
			bCanInit = true;
		}
	}
	
	return bCanInit;
}

bool LogonWnd::GetCrt(std::wstring strNetworkName)
{
	bool bRet = true;
	std::string strCrtUrl;
	ServerConfig* pServerConfig = GetServerConfig(strNetworkName);
	if (pServerConfig)
	{
		strCrtUrl = pServerConfig->strCRT;
	}
	if (strCrtUrl.length() > 0)
	{
		std::string strType = strCrtUrl.substr(0, 5);
		if (strType.compare("https") == 0)
		{
			m_clGetCrt.SendHttpsRequest(new HttpRequestGetCrt(), strCrtUrl.c_str(), NULL, 0);
		}
		else
		{
			m_clGetCrt.SendHttpRequest(new HttpRequestGetCrt(), strCrtUrl.c_str(), NULL, 0);
		}
	}
	else
	{
		bRet = false;
	}
	return bRet;
}

void LogonWnd::GetConfigList()
{
	//服务器推送IP配置
	m_clGetServerConfig.SendHttpsRequest(new HttpRequestGetServerConfig(), strConfigListTcpUrl.c_str(), NULL, 0);
}

bool LogonWnd::InitRtcEngine(std::string strCrt)
{
	bool bRet = false;
	PeerConnectionHelper* pc = PeerConnectionHelper::GetInstance();
	ServerConfig* pServerConfig = GetServerConfig(m_strNetworkName);
	std::string strTempCmpIp;

	if (pServerConfig)
	{
		strTempCmpIp = pServerConfig->strCmp;
		bRet = true;
	}

	if (bRet)
	{
		std::string strCmpIp;
		if (m_bQuic == false)
		{
			size_t iPos = strTempCmpIp.find("tcp://");
			if (iPos == strTempCmpIp.npos)
			{
				strCmpIp.append("tcp://");
			}
			strCmpIp.append(strTempCmpIp);
		}
		else
		{
			size_t iPos = strTempCmpIp.find("quic://");
			if (iPos == strTempCmpIp.npos)
			{
				strCmpIp.append("quic://");
			}
			strCmpIp.append(strTempCmpIp);
		}
		if (RongRTCEngine::ReSetServerConfig(strCmpIp.c_str(), strCrt.c_str(), strCrt.length()) == EngineErrorType_EngineNotInit)
		{
			RongRTCEngine::Init(PeerConnectionHelper::m_strUserId.c_str(),
				strCmpIp.c_str(), strCrt.c_str(), strCrt.length(), pc);
		}
	}
	return bRet;
}

void LogonWnd::InitServerConfig(std::string strJson)
{
		ClearServerConfig();
		ParseJsonStr(strJson);
}

void LogonWnd::ClearServerConfig()
{
	for (std::vector<ServerConfigItem *>::iterator it = m_vectorServerConfigList.begin(); it != m_vectorServerConfigList.end(); ++it)
	{

		for (std::map<NetWorkType, ServerConfig *>::iterator _it = (*it)->mapConfig.begin(); _it != (*it)->mapConfig.end(); ++_it)
		{
			if (_it->second)
			{
				delete(_it->second);
			}
		}
		delete *it;
		*it = nullptr;
	}
	m_vectorServerConfigList.clear();
}

ServerConfig * LogonWnd::GetServerConfig(std::wstring strName)
{
	ServerConfig* pServerConfig = NULL;
	if (m_vectorServerConfigList.size() > 0)
	{
		ServerConfigItem * pItem = *(m_vectorServerConfigList.rbegin());
		if (pItem)
		{
			for (auto iter = pItem->mapConfig.begin(); iter != pItem->mapConfig.end(); iter++)
			{
				if (iter->first == NETWORKTYPE_QUIC && m_bQuic)
				{
					pServerConfig = iter->second;
					break;
				}
				if (iter->first == NETWORKTYPE_TCP && !m_bQuic)
				{
					pServerConfig = iter->second;
					break;
				}
			}
		}
	}
	return pServerConfig;
}

void LogonWnd::StopInit()
{
	m_clGetTokenTimer.Stop();
	m_clGetCrtTimer.Stop();
	m_clGetServerConfig.CancelRequest();
	m_clGetToken.CancelRequest();
	m_clGetCrt.CancelRequest();
	m_strNetworkName = L"";
	m_bGetConfigListSuccess = false;
	m_bGetTokenSuccess = false;
	m_bGetCrtSuccess = false;
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

std::wstring LogonWnd::PaserName(std::string strJson)
{
	std::wstring wstrName;
	Json::Reader reader;
	Json::Value jmessage;
	if (reader.parse(strJson, jmessage))
	{
		int iSize = jmessage.size();
		for (int i = 0; i < iSize; ++i)
		{
			wstrName = UpStringUtility::StringToWstring(jmessage[i]["name"].asString());
		}
	}
	return wstrName;
}

void LogonWnd::ParseJsonStr(std::string strJson)
{
	Json::Reader reader;
	Json::Value jmessage;
	if (reader.parse(strJson, jmessage))
	{
		CListUI * pNetworkListQuic = static_cast<CListUI *>(m_PaintManager.FindControl(_T("network_list_quic")));
		CListUI * pNetworkListTcp = static_cast<CListUI *>(m_PaintManager.FindControl(_T("network_list_tcp")));
		ClearServerConfig();
		int iSize = jmessage.size();
		for (int i = 0; i < iSize; i++)
		{
			ServerConfigItem * pServerConfigItem = new ServerConfigItem;
			
			pServerConfigItem->strName = UpStringUtility::UTF8ToUnicode(jmessage[i]["name"].asString());

			Json::Value Quic = jmessage[i]["quic"];
			int nQuic = Quic.size();
			if (nQuic >= 1)
			{
				ServerConfig * pServerConfig = new ServerConfig;
				pServerConfigItem->mapConfig.insert(make_pair<NetWorkType, ServerConfig *>(NETWORKTYPE_QUIC, pServerConfig));
				for (Json::Value::iterator QuicIter = Quic.begin(); QuicIter != Quic.end(); QuicIter++)
				{
					std::string name = QuicIter.name();
					if (name == "nav")
					{
						std::string _strnav = QuicIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_QUIC]->strNav = _strnav;
					}
					else if (name == "cmp")
					{
						std::string _strcmp = QuicIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_QUIC]->strCmp = _strcmp;
					}
					else if (name == "token")
					{
						std::string _strtoken = QuicIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_QUIC]->strToken = _strtoken;
					}
				}
			}

			Json::Value Tcp = jmessage[i]["tcp"];
			int nTcp = Tcp.size();
			if (nTcp >= 1)
			{
				ServerConfig * pServerConfig = new ServerConfig;
				pServerConfigItem->mapConfig.insert(make_pair<NetWorkType, ServerConfig *>(NETWORKTYPE_TCP, pServerConfig));
				for (Json::Value::iterator TcpIter = Tcp.begin(); TcpIter != Tcp.end(); TcpIter++)
				{
					std::string name = TcpIter.name();
					if (name == "nav")
					{
						std::string _strnav = TcpIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_TCP]->strNav = _strnav;
					}
					else if (name == "cmp")
					{
						std::string _strcmp = TcpIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_TCP]->strCmp = _strcmp;
					}
					else if (name == "token")
					{
						std::string _strtoken = TcpIter->asString();
						pServerConfigItem->mapConfig[NETWORKTYPE_TCP]->strToken = _strtoken;
					}
				}
			}
			if (nTcp == 0 && nQuic == 0)
			{
				ServerConfig * pServerConfig = new ServerConfig;
				pServerConfig->strCmp = jmessage[i]["cmp"].asString();
				pServerConfig->strCmpTls = jmessage[i]["cmptls"].asString();
				pServerConfig->strSniffer = jmessage[i]["sniffer"].asString();
				pServerConfig->strSnifferTls = jmessage[i]["sniffertls"].asString();
				pServerConfig->strToken = jmessage[i]["token"].asString();
				pServerConfig->strJKS = jmessage[i]["jks"].asString();
				pServerConfig->strDER = jmessage[i]["der"].asString();
				pServerConfig->strCRT = jmessage[i]["crt"].asString();
				if (jmessage[i].isMember("appkey"))
				{
					pServerConfig->strAppKey = jmessage[i]["appkey"].asString();
				}
				else
				{
					pServerConfig->strAppKey = strDemoAppKey;
				}
				pServerConfigItem->mapConfig.insert(make_pair<NetWorkType, ServerConfig *>(NETWORKTYPE_TCP, pServerConfig));

			}
			m_vectorServerConfigList.push_back(pServerConfigItem);
		}
	}
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

	CLabelUI* pChannelIdTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("channel_id_tip")));
	if (pChannelIdTip) pChannelIdTip->SetText(UPSTR(logon_channel_id_tip).c_str());
	CLabelUI* pNameTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("name_tip")));
	if (pNameTip) pNameTip->SetText(UPSTR(logon_name_tip).c_str());
	CLabelUI* pRoleIdTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("role_id_tip")));
	if (pRoleIdTip) pRoleIdTip->SetText(UPSTR(logon_role_id_tip).c_str());
	CCheckBoxUI* pIsOberver = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_observer")));
	if (pIsOberver) pIsOberver->SetText(UPSTR(logon_normal_join_tip).c_str());
	CButtonUI* pJoin = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("join_channel_btn")));
	if (pJoin) pJoin->SetText(UPSTR(logon_join_channel_tip).c_str());
	CButtonUI* pSettingText = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_text_btn")));
	if (pSettingText) pSettingText->SetText(UPSTR(logon_channel_setting_tip).c_str());
	CCheckBoxUI* pSelectVideo = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_video")));
	if (pSelectVideo) pSelectVideo->SetText(UPSTR(logon_no_video_tip).c_str());
	CLabelUI* pSelectSolutionTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("select_solution_tip")));
	if (pSelectSolutionTip) pSelectSolutionTip->SetText(UPSTR(logon_solution_tip).c_str());
	COptionUI* pSelectSolution_4 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("select_solution_4")));
	if (pSelectSolution_4)
	{
		CDuiString strText = _T("1920x1080(");
		strText.Append(UPSTR(logon_solution_1920_1080_tip).c_str());
		strText.Append(_T(")"));
		pSelectSolution_4->SetText(strText);
	}
	CCheckBoxUI* pSelectBeauty = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_beauty")));
	if (pSelectBeauty) pSelectBeauty->SetText(UPSTR(logon_open_beauty_tip).c_str());
	CCheckBoxUI* pSelectTinyStream = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("select_tiny_stream")));
	if (pSelectTinyStream) pSelectTinyStream->SetText(UPSTR(logon_open_tiny_stream_tip).c_str());

	CLabelUI* pNetSelectTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("net_select_tip")));
	if (pNetSelectTip) pNetSelectTip->SetText(UPSTR(logon_net_select_tip).c_str());
	
	CLabelUI* pChangeIpTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("change_ip_title")));
	if (pChangeIpTitle) pChangeIpTitle->SetText(UPSTR(logon_change_ip_title).c_str());
	CButtonUI* pModify = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("modify_btn")));
	if (pModify) pModify->SetText(UPSTR(logon_change_ip_modify_tip).c_str());
	CButtonUI* pEmpty = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("empty_btn")));
	if (pEmpty) pEmpty->SetText(UPSTR(logon_change_ip_restore_tip).c_str());
	CButtonUI* pCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancel_modify_btn")));
	if (pCancel) pCancel->SetText(UPSTR(logon_change_ip_cancel_tip).c_str());
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
			std::string strToken;
			strToken.append(body,bodyLen);
			MsgItem* pMsgItem = new MsgItem(strToken);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_TOKEN_OK, pMsgItem);
		}
	}
}

void HttpRequestGetToken::OnHttpRequestFaild(int error)
{
}

void HttpRequestGetServerConfig::OnHttpRequestSuccess(const char * headers, int headerLen, const char * body, int bodyLen)
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
			std::string strJson;
			strJson.append(body, bodyLen);
			MsgItem* pMsgItem = new MsgItem(strJson);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_SERVER_CONFIG, pMsgItem);
		}
	}
}

void HttpRequestGetServerConfig::OnHttpRequestFaild(int error)
{
}

void HttpRequestGetCrt::OnHttpRequestSuccess(const char * headers, int headerLen, const char * body, int bodyLen)
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
			std::string strCrt;
			strCrt.append(body, bodyLen);
			MsgItem* pMsgItem = new MsgItem(strCrt);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_CRT_OK, pMsgItem);
		}
	}
}

void HttpRequestGetCrt::OnHttpRequestFaild(int error)
{
}
