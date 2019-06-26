#include "StdAfx.h"
#include "MainWnd.h"
#include "WndHelper.h"
#include "Integer.h"
#include "MainTipWnd.h"
#include "..\jsoncpp\include\json\json.h"
#include "UpDictManager.h"
#include "UpStringUtility.h"

const CDuiString strSmallOnlyAudioBg = _T("file='main\\audio_only.png'");

const CDuiString strCameraOpen = _T("file='main\\icon_open_camera.png' dest='32,16,43,30'");
const CDuiString strCameraClose = _T("file='main\\icon_close_camera.png' dest='32,16,43,30'");

const CDuiString strMicOpen = _T("file='main\\icon_open_mic.png' dest='31,16,43,30'");
const CDuiString strMicClose = _T("file='main\\icon_close_mic.png' dest='31,16,43,30'");

const CDuiString strLoundSpeakerOpen = _T("file='main\\icon_open_ loudspeaker.png' dest='30,16,46,30'");
const CDuiString strLoundSpeakerClose = _T("file='main\\icon_close_ loudspeaker.png' dest='30,16,46,30'");

const CDuiString strWBNormal = _T("file='main\\icon_wb_normal.png' dest='29,16,46,28'");
const CDuiString strWBHot = _T("file='main\\icon_wb_hot.png' dest='29,16,46,28'");

const CDuiString strShareNormal = _T("file='main\\icon_share_normal.png' dest='29,16,45,31'");
const CDuiString strShareHot = _T("file='main\\icon_share_hot.png' dest='29,16,45,31'");

const CDuiString strUpBtnBg = _T("main\\icon_up.png");
const CDuiString strDownBtnBg = _T("main\\icon_down.png");

const CDuiString strItemMicOpenBtnBg = _T("file='main\\icon_open_mic.png' dest='1,0,13,14'");
const CDuiString strItemMicCloseBtnBg = _T("file='main\\icon_close_mic.png' dest='1,0,13,14'");

const CDuiString strItemCameraOpenBtnBg = _T("file='main\\icon_open_camera.png' dest='2,0,13,14'");
const CDuiString strItemCameraCloseBtnBg = _T("file='main\\icon_close_camera.png' dest='2,0,13,14'");

IMPL_APPWND_CONTROL(MainWnd, ID_WND_MAIN)

MainWnd::MainWnd(void) :
	m_bQuiteApp(false),
	m_bFirstJionChannel(true),
	m_bMuteMic(false),
	m_bMuteCamera(false),
	m_pLocalVideoCtrl(NULL),
	m_pRemoteVideoLayout(NULL),
	m_pUserList(NULL),
	m_iConnectionSecond(0)
{
	SetSkinFile(_T("main.xml"));
}

MainWnd::~MainWnd(void)
{
	for (std::map<std::string, UserInfo*>::iterator it = m_mapUsersInfo.begin(); it != m_mapUsersInfo.end(); ++it)
	{
		delete it->second;
	}
	m_mapUsersInfo.clear();
	for (std::list<StreamInfo*>::iterator it = m_listStreamsInfo.begin(); it != m_listStreamsInfo.end(); ++it)
	{
		delete *it;
	}
	m_listStreamsInfo.clear();
	m_clConnectionTimer.Stop();
}

NOTIFY_BEGIN(MainWnd)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("show_info_btn"), OnClickShowInfoBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("closebtn"), OnClickCloseBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("screen_share_btn"), OnClickScreenShareBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("camera_btn"), OnClickCameraBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("mic_btn"), OnClickMicBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("loudspeaker_btn"), OnClickSpeakerBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("hangup_btn"), OnClickHangeupBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("notification_list_btn"), OnTabOptionChanged)
ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("member_list_btn"), OnTabOptionChanged)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("exchange_video_btn"), OnExchangeBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("up_down_btn"), OnClickUpAndDownBtn)
NOTIFY_END()

LRESULT MainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_VIDEO_DATA:
	{
		MsgItem* pMsgItem = (MsgItem*)wParam;
		if (pMsgItem)
		{
			std::string strStreamId = pMsgItem->GetBuf(0);
			BITMAPINFO* pBitmapInfo = (BITMAPINFO*)pMsgItem->GetId(0);
			unsigned char* pData = (unsigned char*)pMsgItem->GetId(1);
			RenderVideoData(strStreamId, pBitmapInfo, pData);
		}
	}
	break;
	case WM_UP_BROADCAST:
	{
		switch (wParam)
		{
			case BDT_CONNECT_SIGNAL_SERVER:
			{
				RongRTCEngine::JoinRoom(PeerConnectionHelper::m_strChannelId.c_str(), RongRTCRoomType_Normal, JoinRoomCallBack::GetInstance());
			}
			break;
			case BDT_JOIN_ROOM_OK:
			{
				CDuiString strTitleText = UPSTR(main_meeting_room).c_str();
				strTitleText.Append(_T(" "));
				strTitleText.Append(UpStringUtility::UTF8ToUnicode(PeerConnectionHelper::m_strChannelId).c_str());
				CLabelUI* pTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("channel_id")));
				if (pTitle) pTitle->SetText(strTitleText);
				
				if (m_clConnectionTimer.GetIsWork())
				{
					m_clConnectionTimer.Stop();
					m_iConnectionSecond = 0;
				}
				m_clConnectionTimer.Start(1000, this);

				AddOrUpdateVideoCtrl(CreateLocalStreamJson(PeerConnectionHelper::m_iResourceType));
				if (PeerConnectionHelper::m_iResourceType != 0)
				{
					RongRTCEngine::PublishDefaultStream((RongRTCStreamType)PeerConnectionHelper::m_iResourceType, PeerConnectionHelper::GetInstance());
				}
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strStreamJson = pMsgItem->GetBuf(0);
					AddOrUpdateVideoCtrl(strStreamJson);
					if (strStreamJson.length() == 0)
					{
						AddMessageTipToMessageList(CreateNotificationText(L"", UPSTR(main_wait_user_join).c_str()));
					}
					std::string strSubscribeStreams = CreateSubscribeStreamJson();
					RongRTCEngine::SubscribeStream(strSubscribeStreams.c_str(), PeerConnectionHelper::GetInstance());
					SetLocalUserInfo();
					RongRTCEngine::GetRoomAttributeValue("", GetRoomAttributeValueCallBack::GetInstance());
				}
			}
			break;
			case BDT_LEAVE_ROOM_OK:
			{
				if (m_bQuiteApp) 
				{
					::PostQuitMessage(0);
				}
			}
			break;
			case BDT_GET_ROOM_ATTRIBUTE_VALUE:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUsersInfo = pMsgItem->GetBuf(0);
					AddOrUpdateUserInfoFromGet(strUsersInfo);
				}
			}
			break;
			case BDT_NOTIFY_SET_ROOM_ATTRIBUTE_VALUE:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUsersInfo = pMsgItem->GetBuf(0);
					AddOrUpdateUserInfoFromNotify(strUsersInfo);
				}
			}
			break;
			case BDT_REMOTE_USER_JOIN:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId = pMsgItem->GetBuf(0);
					AddMessageTipToMessageList(CreateNotificationText(UpStringUtility::UTF8ToUnicode(strUserId), UPSTR(main_user_join).c_str()));
				}
			}
			break;
			case BDT_REMOTE_USER_LEFT:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId = pMsgItem->GetBuf(0);
					RemoveVideoCtrlAndUserInfo(strUserId);
					AddMessageTipToMessageList(CreateNotificationText(UpStringUtility::UTF8ToUnicode(strUserId), UPSTR(main_user_leave).c_str()));
				}
			}
			break;
			case BDT_REMOTE_STREAM_PUBLISH:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strStreamJson = pMsgItem->GetBuf(0);
					AddOrUpdateVideoCtrl(strStreamJson);
					RongRTCEngine::SubscribeStream(strStreamJson.c_str(), PeerConnectionHelper::GetInstance());
				}
			}
			break;
			case BDT_REMOTE_STREAM_UNPUBLISH:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strStreamJson = pMsgItem->GetBuf(0);
					RemoveVideoCtrl(strStreamJson);
				}
			}
			break;
			case BDT_REMOTE_STREAM_STATE_CHANGED:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strStreamJson = pMsgItem->GetBuf(0);
					AddOrUpdateVideoCtrl(strStreamJson);
				}
			}
			break;
			default:
			{

			}
			break;
		}
	}
	break;
	default:
		bHandled = FALSE;
		break;
	}
	if (bHandled) return lRes;

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (wParam == 0)
	{
		bHandled = TRUE;
		int code = RongRTCEngine::LeaveRoom(LeaveRoomCallBack::GetInstance());
		if (code != 0)
		{
			::PostQuitMessage(0);
		}
		else
		{
			ShowWindow(false);
			m_bQuiteApp = true;
		}
	}
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void MainWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LPCTSTR MainWnd::GetWindowClassName() const
{
	return _T("MainWnd");
}

void MainWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{
}

void MainWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
	PeerConnectionHelper::m_hMainWnd = m_hWnd;
	InitText();
	InitCtrl(); 
	RongRTCEngine::SetBoolStreamParam(RongRTCStreamParamKey_SRTP, false);
	RongRTCEngine::SetIntStreamParam(RongRTCStreamParamKey_VideoSolution, (RongRTCVideoSolution)PeerConnectionHelper::m_iResolutionType);
	RongRTCEngine::SetBoolStreamParam(RongRTCStreamParamKey_VideoSimulcast, PeerConnectionHelper::m_bOpenTinyStream);
	if (PeerConnectionHelper::m_bConnected == false)
	{
		RongRTCEngine::ConnectWithToken(PeerConnectionHelper::m_strToken.c_str());
	}
	else
	{
		RongRTCEngine::JoinRoom(PeerConnectionHelper::m_strChannelId.c_str(), RongRTCRoomType_Normal, JoinRoomCallBack::GetInstance());
	}
}

void MainWnd::OnClickShowInfoBtn(TNotifyUI& msg)
{
	WndHelper::CreateStateInfoWnd();
}

void MainWnd::OnClickCloseBtn(TNotifyUI &msg)
{
	CBaseWnd* pStateInfoWnd = WndHelper::GetStateInfoWnd();
	if (pStateInfoWnd)
	{
		pStateInfoWnd->Close();
	}
	int code = RongRTCEngine::LeaveRoom(LeaveRoomCallBack::GetInstance());
	if (code != 0) 
	{
		::PostQuitMessage(0);
	}
	else
	{
		ShowWindow(false);
		m_bQuiteApp = true;
	}
}

void MainWnd::OnClickMicBtn(TNotifyUI & msg)
{
	CButtonUI* pMicBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pMicBtn)
	{
		if (pMicBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			pMicBtn->SetForeImage(strMicClose);
			pMicBtn->SetTextColor(0xff3682f2);
			pMicBtn->SetUserData(_T("0"));
			m_bMuteMic = true;
			RongRTCEngine::SetLocalDefaultAudioStreamEnabled(false, PeerConnectionHelper::GetInstance());
			AddOrUpdateVideoCtrl(CreateLocalStreamJson(RongRTCStreamType_Audio));
		}
		else if (pMicBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			pMicBtn->SetForeImage(strMicOpen);
			pMicBtn->SetTextColor(0xff000000);
			pMicBtn->SetUserData(_T("1"));
			m_bMuteMic = false;
			RongRTCEngine::SetLocalDefaultAudioStreamEnabled(true, PeerConnectionHelper::GetInstance());
			AddOrUpdateVideoCtrl(CreateLocalStreamJson(RongRTCStreamType_Audio));
		}
	}
}

void MainWnd::OnClickScreenShareBtn(TNotifyUI & msg)
{
	CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pScreenShareBtn)
	{
		if (pScreenShareBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			pScreenShareBtn->SetForeImage(strShareHot);
			pScreenShareBtn->SetTextColor(0xff3682f2);
			pScreenShareBtn->SetUserData(_T("0"));
			RongRTCEngine::PublishScreenShareStream(PeerConnectionHelper::GetInstance());
		}
		else if (pScreenShareBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			pScreenShareBtn->SetForeImage(strShareNormal);
			pScreenShareBtn->SetTextColor(0xff000000);
			pScreenShareBtn->SetUserData(_T("1"));
			RongRTCEngine::UnPublishScreenShareStream(PeerConnectionHelper::GetInstance());
		}
	}
}

void MainWnd::OnClickCameraBtn(TNotifyUI & msg)
{
	CButtonUI* pCameraBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pCameraBtn)
	{
		if (pCameraBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			pCameraBtn->SetForeImage(strCameraClose);
			pCameraBtn->SetTextColor(0xff3682f2);
			pCameraBtn->SetUserData(_T("0"));
			m_bMuteCamera = true;
			RongRTCEngine::SetLocalDefaultVideoStreamEnabled(false, PeerConnectionHelper::GetInstance());
			AddOrUpdateVideoCtrl(CreateLocalStreamJson(RongRTCStreamType_Video));
		}
		else if (pCameraBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			pCameraBtn->SetForeImage(strCameraOpen);
			pCameraBtn->SetTextColor(0xff000000);
			pCameraBtn->SetUserData(_T("1"));
			m_bMuteCamera = false;
			RongRTCEngine::SetLocalDefaultVideoStreamEnabled(true, PeerConnectionHelper::GetInstance());
			AddOrUpdateVideoCtrl(CreateLocalStreamJson(RongRTCStreamType_Video));
		}
	}
}

void MainWnd::OnClickSpeakerBtn(TNotifyUI & msg)
{
	CButtonUI* pLoudSpeakerBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pLoudSpeakerBtn)
	{
		if (pLoudSpeakerBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			if (RongRTCEngine::SetSpeakerEnabled(false, PeerConnectionHelper::GetInstance()) == RongRTCCodeOK)
			{
				pLoudSpeakerBtn->SetForeImage(strLoundSpeakerClose);
				pLoudSpeakerBtn->SetTextColor(0xff3682f2);
				pLoudSpeakerBtn->SetUserData(_T("0"));
			}
		}
		else if (pLoudSpeakerBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			if (RongRTCEngine::SetSpeakerEnabled(true, PeerConnectionHelper::GetInstance()) == RongRTCCodeOK)
			{
				pLoudSpeakerBtn->SetForeImage(strLoundSpeakerOpen);
				pLoudSpeakerBtn->SetTextColor(0xff000000);
				pLoudSpeakerBtn->SetUserData(_T("1"));
			}
		}
	}
}

void MainWnd::OnClickHangeupBtn(TNotifyUI & msg)
{
	CBaseWnd* pStateInfoWnd = WndHelper::GetStateInfoWnd();
	if (pStateInfoWnd)
	{
		pStateInfoWnd->Close();
	}
	RongRTCEngine::LeaveRoom(LeaveRoomCallBack::GetInstance());
	WndHelper::BackLogonWnd();
	Close();
}

void MainWnd::OnTabOptionChanged(TNotifyUI & msg)
{
	CTabLayoutUI* pTabListLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tab_list_layout")));
	if (pTabListLayout)
	{
		if (msg.pSender->GetUserData().Compare(L"0") == 0)
		{
			pTabListLayout->SelectItem(0);
		}
		else
		{
			pTabListLayout->SelectItem(1);
		}
	}
}

void MainWnd::OnExchangeBtn(TNotifyUI & msg)
{
	CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(msg.pSender);
	CVideoUI* pRemoteVideoCtrl = static_cast<CVideoUI*>(pExchangeBtn->GetParent());
	CLabelUI* pLocalNoVideoFlag = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(m_pLocalVideoCtrl, _T("local_no_video")));
	CDuiString strRemoteStreamId = pRemoteVideoCtrl->GetUserData();
	CDuiString strLocalStreamId = m_pLocalVideoCtrl->GetUserData();
	StreamInfo* pRemoteStreamInfo = (StreamInfo*)pRemoteVideoCtrl->GetTag();
	StreamInfo* pLocalStreamInfo = (StreamInfo*)m_pLocalVideoCtrl->GetTag();
	BITMAPINFO* pLocalInfo = NULL, *pRemoteInfo = NULL;
	unsigned char* pLocalBuffer = NULL, *pRemoteBuffer = NULL;
	m_pLocalVideoCtrl->GetBitmap(&pLocalInfo, &pLocalBuffer);
	pRemoteVideoCtrl->GetBitmap(&pRemoteInfo, &pRemoteBuffer);
	pRemoteVideoCtrl->SetUserData(strLocalStreamId);
	pRemoteVideoCtrl->SetTag((UINT_PTR)pLocalStreamInfo);
	if (((pLocalStreamInfo->m_iMediaType & 2) == 2 && pLocalStreamInfo->m_iState[1] == 0)
		|| (pLocalStreamInfo->m_iMediaType & 2) != 2) {
		pExchangeBtn->SetBkImage(strSmallOnlyAudioBg);
		pRemoteVideoCtrl->SetBitmap(NULL, NULL);
	} else {
		pExchangeBtn->SetBkImage(_T(""));
		pRemoteVideoCtrl->SetBitmap(pLocalInfo, pLocalBuffer);
	}
	m_pLocalVideoCtrl->SetUserData(strRemoteStreamId);
	m_pLocalVideoCtrl->SetTag((UINT_PTR)pRemoteStreamInfo);
	RECT rc = pLocalNoVideoFlag->GetPos();
	if (((pRemoteStreamInfo->m_iMediaType & 2) == 2 && pRemoteStreamInfo->m_iState[1] == 0)
		|| (pRemoteStreamInfo->m_iMediaType & 2) != 2) {
		rc.bottom = rc.top + 80;
		m_pLocalVideoCtrl->SetBitmap(NULL, NULL);
	} else {
		rc.bottom = rc.top;
		m_pLocalVideoCtrl->SetBitmap(pRemoteInfo, pRemoteBuffer);
	}
	pLocalNoVideoFlag->SetPos(rc);

	Json::Value uris;
	if (pLocalStreamInfo->m_strUserId.compare(PeerConnectionHelper::m_strUserId) != 0)
	{
		if ((pLocalStreamInfo->m_iMediaType & 2) == 2)
		{
			Json::Value uri_array;
			Json::Value uri;
			uri["tag"] = pLocalStreamInfo->m_strTag;
			uri["mediaType"] = 1;
			uri["simulcast"] = 2;
			uri_array[pLocalStreamInfo->m_strUserId].append(uri);
			uris.append(uri_array);
		}
	}
	if (pRemoteStreamInfo->m_strUserId.compare(PeerConnectionHelper::m_strUserId) != 0)
	{
		if ((pRemoteStreamInfo->m_iMediaType & 2) == 2)
		{
			Json::Value uri_array;
			Json::Value uri;
			uri["tag"] = pRemoteStreamInfo->m_strTag;
			uri["mediaType"] = 1;
			uri["simulcast"] = 1;
			uri_array[pRemoteStreamInfo->m_strUserId].append(uri);
			uris.append(uri_array);
		}
	}
	if (uris.size() != 0)
	{
		RongRTCEngine::SubscribeStream(uris.toStyledString().c_str(), PeerConnectionHelper::GetInstance());
	}
}

void MainWnd::OnClickUpAndDownBtn(TNotifyUI & msg)
{
	CButtonUI* pUpAndDownBtn = static_cast<CButtonUI*>(msg.pSender);
	CVideoUI* pLocalVideoLayout = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("local_video")));
	if (pUpAndDownBtn && pLocalVideoLayout)
	{
		RECT rc = pLocalVideoLayout->GetInset();
		if (pUpAndDownBtn->GetUserData().Compare(L"1") == 0)
		{
			rc.top += 94;
			pUpAndDownBtn->SetNormalImage(strUpBtnBg);
			pUpAndDownBtn->SetUserData(L"0");
		}
		else
		{
			rc.top = rc.top - 94;
			pUpAndDownBtn->SetNormalImage(strDownBtnBg);
			pUpAndDownBtn->SetUserData(L"1");
		}
		pLocalVideoLayout->SetInset(rc);
	}
}

void MainWnd::AddOrUpdateVideoCtrl(std::string strStreams)
{
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(strStreams, root))
	{
		for (unsigned i = 0; i < root.size(); i++)
		{
			Json::Value uris_object = root[i];
			Json::Value::Members members = uris_object.getMemberNames();
			if (members.size() == 0)
			{
				continue;
			}
			std::string user_id = members.front();
			Json::Value uris = uris_object[user_id];
			for (unsigned i = 0; i < uris.size(); ++i) {
				StreamInfo* pStreamInfo = NULL;
				std::string msid = uris[i]["msid"].asString();
				int media_type = uris[i]["mediaType"].asInt();
				for (std::list<StreamInfo*>::iterator it = m_listStreamsInfo.begin(); it != m_listStreamsInfo.end(); ++it)
				{
					if ((*it)->m_strStreamId == msid) {
						pStreamInfo = *it;
						break;
					}
				}
				if (pStreamInfo == NULL)
				{
					pStreamInfo = new StreamInfo();
					m_listStreamsInfo.push_back(pStreamInfo);
					pStreamInfo->m_iMediaType |= (media_type == 0 ? 1 : 2);
				}
				else
				{
					pStreamInfo->m_iMediaType |= (media_type == 0 ? 1 : 2);
				}
				pStreamInfo->m_strUserId = user_id;
				pStreamInfo->m_strTag = uris[i]["tag"].asString();
				pStreamInfo->m_strStreamId = msid;
				if (media_type == 0) 
				{
					pStreamInfo->m_iState[0] = uris[i]["state"].asInt();
				}
				else
				{
					pStreamInfo->m_iState[1] = uris[i]["state"].asInt();
					pStreamInfo->m_bSimulcast = uris[i]["features"].isNull() ? false : true;
				}
				AddOrUpdateVideoCtrl(pStreamInfo);
			}
		}
	}
}

void MainWnd::AddOrUpdateVideoCtrl(StreamInfo * pStreamInfo)
{
	if (m_pLocalVideoCtrl->GetUserData().GetLength() == 0)
	{
		m_pLocalVideoCtrl->SetUserData(UpStringUtility::StringToWstring(pStreamInfo->m_strStreamId).c_str());
		m_pLocalVideoCtrl->SetTag((UINT_PTR)pStreamInfo);
		m_pLocalVideoCtrl->SetBitmap(NULL, NULL);
		CLabelUI* pLocalNoVideo = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(m_pLocalVideoCtrl, _T("local_no_video")));
		RECT rc = pLocalNoVideo->GetPos();
		if (((pStreamInfo->m_iMediaType & 2 ) == 2 && pStreamInfo->m_iState[1] == 0) 
			|| (pStreamInfo->m_iMediaType & 2) != 2) {
			rc.bottom = rc.top + 80;
		}
		else {
			rc.bottom = rc.top;
		}
		pLocalNoVideo->SetPos(rc);
	}
	else
	{
		if (m_pLocalVideoCtrl->GetUserData().Compare(UpStringUtility::StringToWstring(pStreamInfo->m_strStreamId).c_str()) == 0)
		{
			if ((pStreamInfo->m_iMediaType & 2) == 2) {
				m_pLocalVideoCtrl->SetTag((UINT_PTR)pStreamInfo);
				CLabelUI* pLocalNoVideo = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(m_pLocalVideoCtrl, _T("local_no_video")));
				RECT rc = pLocalNoVideo->GetPos();
				if (pStreamInfo->m_iState[1] == 0) {
					rc.bottom = rc.top + 80;
					m_pLocalVideoCtrl->SetBitmap(NULL, NULL);
				} else {
					rc.bottom = rc.top;
				}
				pLocalNoVideo->SetPos(rc);
			}
		}
		else
		{
			CVideoUI* pRemoteVideoCtrl = NULL;
			int iCount = m_pRemoteVideoLayout->GetCount();
			for (int i = 0; i < iCount; ++i) {
				CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(i));
				if (pVideoCtrl->GetUserData().Compare(UpStringUtility::StringToWstring(pStreamInfo->m_strStreamId).c_str()) == 0) {
					pRemoteVideoCtrl = pVideoCtrl;
					break;
				}
			}
			if (pRemoteVideoCtrl)
			{
				if ((pStreamInfo->m_iMediaType & 2) == 2) {
					pRemoteVideoCtrl->SetTag((UINT_PTR)pStreamInfo);
					CButtonUI* pRemoteNoVideo = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pRemoteVideoCtrl, _T("exchange_video_btn")));
					if (pStreamInfo->m_iState[1] == 0) {
						pRemoteNoVideo->SetBkImage(strSmallOnlyAudioBg);
						pRemoteVideoCtrl->SetBitmap(NULL, NULL);
					}
					else {
						pRemoteNoVideo->SetBkImage(_T(""));
					}
				}
			}
			else
			{
				CVideoUI* pRemoteVideoCtrl = new CVideoUI();
				pRemoteVideoCtrl->SetUserData(UpStringUtility::StringToWstring(pStreamInfo->m_strStreamId).c_str());
				pRemoteVideoCtrl->SetTag((UINT_PTR)pStreamInfo);
				pRemoteVideoCtrl->SetFixedWidth(80);
				pRemoteVideoCtrl->SetFixedHeight(80);
				pRemoteVideoCtrl->SetBkColor(0xFF000000);
				pRemoteVideoCtrl->SetAttribute(_T("fullscreen"), _T("false"));
				SIZE szBorderRound = { 5,5 };
				pRemoteVideoCtrl->SetBorderRound(szBorderRound);
				CButtonUI* pExchangeBtn = new CButtonUI();
				pExchangeBtn->SetName(_T("exchange_video_btn"));
				pExchangeBtn->SetFixedWidth(80);
				pExchangeBtn->SetFixedHeight(80);
				pExchangeBtn->SetFont(1);
				pExchangeBtn->SetTextColor(0xffffffff);
				pExchangeBtn->SetBorderRound(szBorderRound);
				if (((pStreamInfo->m_iMediaType & 2) == 2 && pStreamInfo->m_iState[1] == 0)
					|| (pStreamInfo->m_iMediaType & 2) != 2) {
					pExchangeBtn->SetBkImage(strSmallOnlyAudioBg);
				}
				pRemoteVideoCtrl->Add(pExchangeBtn);
				m_pRemoteVideoLayout->Add(pRemoteVideoCtrl);
				pRemoteVideoCtrl->SetBitmap(NULL, NULL);
			}
		}
	}
}

void MainWnd::AddOrUpdateUserItem(std::string strUserId)
{
	if (m_pUserList)
	{
		CListContainerElementUI* pUserListItem = NULL;
		for (int i = 0; i < m_pUserList->GetCount(); ++i)
		{
			CListContainerElementUI* pTempUserListItem = static_cast<CListContainerElementUI*>(m_pUserList->GetItemAt(i));
			if (pTempUserListItem && pTempUserListItem->GetUserData().Compare(UpStringUtility::StringToWchar(strUserId)) == 0)
			{
				pUserListItem = pTempUserListItem;
				break;
			}
		}
		if (pUserListItem == NULL)
		{
			CDialogBuilder dlgBuilder;
			pUserListItem = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("list_member_item.xml"), (UINT)0, NULL, &m_PaintManager));
			pUserListItem->SetUserData(UpStringUtility::UTF8ToUnicode(strUserId).c_str());
			m_pUserList->AddAt(pUserListItem, 0);
		}
		if (pUserListItem)
		{
			if (m_mapUsersInfo.find(strUserId) != m_mapUsersInfo.end())
			{
				CLabelUI* pName = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pUserListItem, _T("name")));
				if (pName)
				{
					pName->SetText(UpStringUtility::UTF8ToUnicode(m_mapUsersInfo[strUserId]->m_strUserName).c_str());
				}
				CLabelUI* pJoinMode = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pUserListItem, _T("type")));
				if (pJoinMode)
				{
					if (m_mapUsersInfo[strUserId]->m_iJoinMode == 0)
					{
						pJoinMode->SetText(UPSTR(main_video_mode_join).c_str());
					}
					else if (m_mapUsersInfo[strUserId]->m_iJoinMode == 1)
					{
						pJoinMode->SetText(UPSTR(main_audio_mode_join).c_str());
					}
					else if (m_mapUsersInfo[strUserId]->m_iJoinMode == 2)
					{
						pJoinMode->SetText(UPSTR(main_observer_mode_join).c_str());
					}
				}
			}
		}
	}
}

void MainWnd::AddOrUpdateUserInfoFromGet(std::string strUserInfo)
{
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(strUserInfo, root))
	{
		for (unsigned i = 0; i < root.size(); ++i)
		{
			Json::Value::Members members = root[i].getMemberNames();
			if (members.size() == 0)
			{
				continue;
			}
			std::string user_id = members.front();
			Json::Value user_info = root[i][user_id];
			std::string strUserId = user_info["userId"].asString();
			UserInfo* pUserInfo = NULL;
			if (m_mapUsersInfo.find(strUserId) == m_mapUsersInfo.end()) 
			{
				if (user_info.isMember("userName")) {
					pUserInfo = new UserInfo();
					m_mapUsersInfo.insert(std::pair<std::string, UserInfo*>(strUserId, pUserInfo));
				}
			}
			else
			{
				pUserInfo = m_mapUsersInfo[strUserId];
			}
			if (pUserInfo)
			{
				pUserInfo->m_strUserName = user_info["userName"].asString();
				if (user_info["joinTime"].isInt64())
				{
					pUserInfo->m_iJoinTime = user_info["joinTime"].asInt64();
				}
				else
				{
					pUserInfo->m_iJoinTime = std::stoll(user_info["joinTime"].asString());
				}
				pUserInfo->m_iJoinMode = user_info["joinMode"].asInt();
				AddOrUpdateUserItem(strUserId);
			}
		}
	}
}

void MainWnd::AddOrUpdateUserInfoFromNotify(std::string strUserInfo)
{
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(strUserInfo, root))
	{
		std::string strUserId = root["infoKey"].asString();
		UserInfo* pUserInfo = NULL;
		if (m_mapUsersInfo.find(strUserId) == m_mapUsersInfo.end())
		{
			pUserInfo = new UserInfo();
			m_mapUsersInfo.insert(std::pair<std::string, UserInfo*>(strUserId, pUserInfo));
		}
		else
		{
			pUserInfo = m_mapUsersInfo[strUserId];
		}
		if (pUserInfo)
		{
			Json::Value user_info = root["infoValue"];
			pUserInfo->m_strUserName = user_info["userName"].asString();
			if (user_info["joinTime"].isInt64())
			{
				pUserInfo->m_iJoinTime = user_info["joinTime"].asInt64();
			}
			else
			{
				pUserInfo->m_iJoinTime = std::stoll(user_info["joinTime"].asString());
			}
			pUserInfo->m_iJoinMode = user_info["joinMode"].asInt();
			AddOrUpdateUserItem(strUserId);
		}
	}
}

void MainWnd::RemoveVideoCtrl(std::string strStreams)
{
	Json::Value root;
	Json::Reader reader;
	if (reader.parse(strStreams, root))
	{
		for (unsigned i = 0; i < root.size(); i++)
		{
			Json::Value uris_object = root[i];
			Json::Value::Members members = uris_object.getMemberNames();
			if (members.size() == 0)
			{
				continue;
			}
			std::string user_id = members.front();
			Json::Value uris = uris_object[user_id];
			for (unsigned i = 0; i < uris.size(); ++i) {
				std::string msid = uris[i]["msid"].asString();
				int media_type = uris[i]["mediaType"].asInt();
				for (std::list<StreamInfo*>::iterator it = m_listStreamsInfo.begin(); it != m_listStreamsInfo.end(); ++it)
				{
					if ((*it)->m_strStreamId == msid) {
						StreamInfo* pStreamInfo = *it;
						if (pStreamInfo == NULL)
						{
							continue;
						}
						else
						{
							pStreamInfo->m_iMediaType &= ~(media_type == 0 ? 1 : 2);
						}
						RemoveVideoCtrl(pStreamInfo);
						if (pStreamInfo->m_iMediaType == 0)
						{
							delete *it;
							m_listStreamsInfo.erase(it);
						}
						break;
					}
				}
			}
		}
	}
}

void MainWnd::RemoveVideoCtrl(StreamInfo * pStreamInfo)
{
	std::string strLocalStreamId = UpStringUtility::WstringToString(m_pLocalVideoCtrl->GetUserData().GetData());
	if (strLocalStreamId == pStreamInfo->m_strStreamId)
	{
		StreamInfo * pStreamInfoTemp = NULL;
		if (pStreamInfo->m_iMediaType == 0)
		{
			CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(0));
			if (pVideoCtrl)
			{
				pStreamInfoTemp = (StreamInfo*)pVideoCtrl->GetTag();
				m_pLocalVideoCtrl->SetUserData(pVideoCtrl->GetUserData());
				m_pLocalVideoCtrl->SetTag(pVideoCtrl->GetTag());
				m_pRemoteVideoLayout->RemoveAt(0);
			}
			else
			{
				m_pLocalVideoCtrl->SetUserData(L"");
				m_pLocalVideoCtrl->SetTag(0);
			}
			m_pLocalVideoCtrl->SetBitmap(NULL, NULL);
		}
		CLabelUI* pLocalNoVideo = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(m_pLocalVideoCtrl, _T("local_no_video")));
		RECT rc = pLocalNoVideo->GetPos();
		if (pStreamInfoTemp)
		{
			if (((pStreamInfoTemp->m_iMediaType & 2) == 2 && pStreamInfoTemp->m_iState[1] == 0)
				|| (pStreamInfoTemp->m_iMediaType & 2) != 2) {
				rc.bottom = rc.top + 80;
			}
			else {
				rc.bottom = rc.top;
			}
		}
		else
		{
			rc.bottom = rc.top;
		}
		pLocalNoVideo->SetPos(rc);
	}
	else
	{
		std::vector<int> vectorNeedRemoveVideoCtrl;
		int iCount = m_pRemoteVideoLayout->GetCount();
		for (int i = 0; i < iCount; ++i)
		{
			CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(i));
			if (pVideoCtrl)
			{
				std::string strRemoteStreamId = UpStringUtility::WstringToString(pVideoCtrl->GetUserData().GetData());
				if (strRemoteStreamId == pStreamInfo->m_strStreamId)
				{
					if (pStreamInfo->m_iMediaType == 0)
					{
						m_pRemoteVideoLayout->RemoveAt(i);
					}
					else
					{
						if (((pStreamInfo->m_iMediaType & 2) == 2 && pStreamInfo->m_iState[1] == 0)
							|| (pStreamInfo->m_iMediaType & 2) != 2) {
							CButtonUI* pRemoteNoVideo = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("exchange_video_btn")));
							if (pStreamInfo->m_iState[1] == 0) {
								pRemoteNoVideo->SetBkImage(strSmallOnlyAudioBg);
								pVideoCtrl->SetBitmap(NULL, NULL);
							}
							else {
								pRemoteNoVideo->SetBkImage(_T(""));
							}
						}
					}
					break;
				}
			}
		}
	}
}

void MainWnd::RemoveVideoCtrlAndUserInfo(std::string strUserId)
{
	std::vector<CVideoUI*> vectorNeedRemoveVideoCtrl;
	int iCount = m_pRemoteVideoLayout->GetCount();
	for (int i = 0; i < iCount; ++i)
	{
		CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(i));
		if (pVideoCtrl)
		{
			std::string strRemoteUserId = GetUserIdFromStreamId(UpStringUtility::WstringToString(pVideoCtrl->GetUserData().GetData()));
			if (strRemoteUserId.compare(strUserId) == 0)
			{
				vectorNeedRemoveVideoCtrl.push_back(pVideoCtrl);
			}
		}
	}
	for (std::vector<CVideoUI*>::iterator it = vectorNeedRemoveVideoCtrl.begin(); it != vectorNeedRemoveVideoCtrl.end(); ++it)
	{
		m_pRemoteVideoLayout->Remove(*it);
	}
	std::string strLocalUserId = GetUserIdFromStreamId(UpStringUtility::WstringToString(m_pLocalVideoCtrl->GetUserData().GetData()));
	if (strLocalUserId.compare(strUserId) == 0)
	{
		StreamInfo* pStreamInfoTemp = NULL;
		CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(0));
		if (pVideoCtrl)
		{
			m_pLocalVideoCtrl->SetUserData(pVideoCtrl->GetUserData());
			m_pLocalVideoCtrl->SetTag(pVideoCtrl->GetTag());
			pStreamInfoTemp = (StreamInfo*)pVideoCtrl->GetTag();
			m_pRemoteVideoLayout->RemoveAt(0);
		}
		else
		{
			m_pLocalVideoCtrl->SetUserData(L"");
			m_pLocalVideoCtrl->SetTag(0);
		}
		m_pLocalVideoCtrl->SetBitmap(NULL, NULL);

		CLabelUI* pLocalNoVideo = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(m_pLocalVideoCtrl, _T("local_no_video")));
		RECT rc = pLocalNoVideo->GetPos();
		if (pStreamInfoTemp)
		{
			if (((pStreamInfoTemp->m_iMediaType & 2) == 2 && pStreamInfoTemp->m_iState[1] == 0)
				|| (pStreamInfoTemp->m_iMediaType & 2) != 2) {
				rc.bottom = rc.top + 80;
			}
			else {
				rc.bottom = rc.top;
			}
		}
		else
		{
			rc.bottom = rc.top;
		}
		pLocalNoVideo->SetPos(rc);
	}
	for (std::list<StreamInfo*>::iterator it = m_listStreamsInfo.begin(); it != m_listStreamsInfo.end();)
	{
		if ((*it)->m_strUserId.compare(strUserId) == 0)
		{
			delete *it;
			it = m_listStreamsInfo.erase(it);
		}
		else
		{
			++it;
		}
	}
	for (int i = 0; i < m_pUserList->GetCount(); ++i)
	{
		CListContainerElementUI* pUserListItem = static_cast<CListContainerElementUI*>(m_pUserList->GetItemAt(i));
		if (pUserListItem)
		{
			std::string strUserIdTemp = UpStringUtility::UnicodeToUTF8(pUserListItem->GetUserData().GetData());
			if (strUserIdTemp.compare(strUserId) == 0)
			{
				m_pUserList->Remove(pUserListItem);
				break;
			}
		}
	}
	if (m_mapUsersInfo.find(strUserId) != m_mapUsersInfo.end())
	{
		delete m_mapUsersInfo[strUserId];
		m_mapUsersInfo.erase(strUserId);
	}
}

void MainWnd::RenderVideoData(std::string strStreamId, BITMAPINFO * pBitmapInfo, unsigned char * pData)
{
	if (m_pLocalVideoCtrl && m_pRemoteVideoLayout)
	{
		std::string strLocalStreamId = UpStringUtility::WstringToString(m_pLocalVideoCtrl->GetUserData().GetData());
		if (strLocalStreamId.compare(strStreamId) == 0)
		{
			StreamInfo* pStreamInfo = (StreamInfo*)m_pLocalVideoCtrl->GetTag();
			if (pStreamInfo && pStreamInfo->m_iState[1] == 0) {
				pBitmapInfo = NULL; pData = NULL;
			}
			m_pLocalVideoCtrl->SetBitmap(pBitmapInfo, pData);
		}
		else
		{
			int iCount = m_pRemoteVideoLayout->GetCount();
			for (int i = 0; i < iCount; ++i)
			{
				CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(m_pRemoteVideoLayout->GetItemAt(i));
				if (pVideoCtrl)
				{
					std::string strRemoteStreamId = UpStringUtility::WstringToString(pVideoCtrl->GetUserData().GetData());
					if (strRemoteStreamId.compare(strStreamId) == 0)
					{
						StreamInfo* pStreamInfo = (StreamInfo*)pVideoCtrl->GetTag();
						if (pStreamInfo && pStreamInfo->m_iState[1] == 0) {
							pBitmapInfo = NULL; pData = NULL;
						}
						pVideoCtrl->SetBitmap(pBitmapInfo, pData);
					}
				}
			}
		}
	}
}

std::string MainWnd::CreateLocalStreamJson(int iPublishStreamType)
{
	Json::Value uri_array;
	if ((iPublishStreamType & RongRTCStreamType_Audio) == RongRTCStreamType_Audio)
	{
		Json::Value uri;
		uri["msid"] = PeerConnectionHelper::m_strUserId + "_RongCloudRTC";
		uri["tag"] = "RongCloudRTC";
		uri["mediaType"] = 0;
		uri["state"] = m_bMuteMic ? 0 : 1;
		uri_array[PeerConnectionHelper::m_strUserId].append(uri);
	}
	if ((iPublishStreamType & RongRTCStreamType_Video) == RongRTCStreamType_Video)
	{
		Json::Value uri;
		uri["msid"] = PeerConnectionHelper::m_strUserId + "_RongCloudRTC";
		uri["tag"] = "RongCloudRTC";
		uri["mediaType"] = 1;
		uri["state"] = m_bMuteCamera ? 0 : 1;
		uri_array[PeerConnectionHelper::m_strUserId].append(uri);
	}
	if (uri_array.size() == 0)
	{
		Json::Value uri;
		uri_array[PeerConnectionHelper::m_strUserId] = uri;
	}
	Json::Value uris;
	uris.append(uri_array);
	return uris.toStyledString();
}

std::string MainWnd::CreateSubscribeStreamJson()
{
	bool bDisplayOnLocalVideo = true;
	Json::Value uris;
	
	for (std::list<StreamInfo*>::iterator it = m_listStreamsInfo.begin(); it != m_listStreamsInfo.end(); ++it)
	{
		StreamInfo* pStreamInfo = *it;
		if (PeerConnectionHelper::m_strUserId.compare(pStreamInfo->m_strUserId) != 0)
		{
			Json::Value uri_array;
			if ((pStreamInfo->m_iMediaType & RongRTCStreamType_Audio) == RongRTCStreamType_Audio)
			{
				Json::Value uri;
				uri["tag"] = pStreamInfo->m_strTag;
				uri["mediaType"] = 0;
				uri_array[pStreamInfo->m_strUserId].append(uri);
			}
			if ((pStreamInfo->m_iMediaType & RongRTCStreamType_Video) == RongRTCStreamType_Video)
			{
				Json::Value uri;
				uri["tag"] = pStreamInfo->m_strTag;
				uri["mediaType"] = 1;
				if (PeerConnectionHelper::m_iResourceType == 0 && pStreamInfo->m_bSimulcast == true && bDisplayOnLocalVideo == true)
				{
					uri["simulcast"] = 1;
					bDisplayOnLocalVideo = false;
				}
				uri_array[pStreamInfo->m_strUserId].append(uri);
			}
			uris.append(uri_array);
		}
	}
	return uris.toStyledString();
}

void MainWnd::SetLocalUserInfo()
{
	Json::Value key_value;
	key_value["userId"] = PeerConnectionHelper::m_strUserId;
	key_value["userName"] = PeerConnectionHelper::m_strUserName;
	key_value["joinTime"] = 0;
	if (PeerConnectionHelper::m_iResourceType == RongRTCStreamType_AudioVideo)
	{
		key_value["joinMode"] = 0;
	}
	else if (PeerConnectionHelper::m_iResourceType == RongRTCStreamType_Audio)
	{
		key_value["joinMode"] = 1;
	}
	else
	{
		key_value["joinMode"] = 2;
	}
	Json::Value keyinfo_valueinfo;
	keyinfo_valueinfo["infoKey"] = PeerConnectionHelper::m_strUserId;
	keyinfo_valueinfo["infoValue"] = key_value;
	RongRTCEngine::SetRoomAttributeValue(PeerConnectionHelper::m_strUserId.c_str(), key_value.toStyledString().c_str(),
		"SealRTC:SetRoomInfo", keyinfo_valueinfo.toStyledString().c_str(), PeerConnectionHelper::GetInstance());
}

void MainWnd::SetConnectionTime(unsigned __int64 iTime)
{
	CLabelUI* pConnectTime = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("connect_time")));
	if (pConnectTime)
	{
		CDuiString strTemp;
		__int64 iHour = iTime / 3600;
		__int64 iMinute = (iTime - iHour * 3600) / 60;
		__int64 iSecond = iTime - iHour * 3600 - iMinute * 60;
		strTemp.Format(_T("%02I64d:%02I64d:%02I64d"), iHour, iMinute, iSecond);
		pConnectTime->SetText(strTemp);
	}
}

void MainWnd::AddMessageTipToMessageList(std::wstring wstrText)
{
	CListUI* pNotificationList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("notification_list")));
	if (pNotificationList)
	{
		CDialogBuilder dlgBuilder;
		CListContainerElementUI* pMessageListItem = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("list_message_item.xml"), (UINT)0, NULL, &m_PaintManager));
		if (pMessageListItem)
		{
			CLabelUI* pTime = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMessageListItem, _T("message_time")));
			CTextUI* pMessage = static_cast<CTextUI*>(m_PaintManager.FindSubControlByName(pMessageListItem, _T("message_text")));

			if (pTime && pMessage && wstrText.empty() == false)
			{
				CDuiString strTime = WndHelper::GetNowTime();
				pTime->SetText(strTime);
				CDuiString strMessage = wstrText.c_str();
				pMessage->SetText(strMessage);
				SIZE TempSize = { pMessage->GetFixedWidth(), 1000 };
				SIZE size = CRenderEngine::GetTextSize(m_PaintManager.GetPaintDC(), &m_PaintManager, TempSize, strMessage.GetData(), 1, DT_WORDBREAK);
				pMessage->SetFixedHeight(size.cy);
				pMessageListItem->SetFixedHeight(size.cy + 5 + 18);
				if (pNotificationList->GetCount() > 1000)
				{
					pNotificationList->RemoveAll();
				}
				pNotificationList->Add(pMessageListItem);

				::UpdateWindow(m_hWnd);
				pNotificationList->EndDown();
			}
		}
	}
}

std::wstring MainWnd::CreateNotificationText(std::wstring wstrName, std::wstring wstrText)
{
	std::wstring wstrInfo;
	if (wstrName.empty() == false)
	{
		if (wstrName.length() > 17)
		{
			wstrInfo.append(wstrName.substr(0, 17));
			wstrInfo.append(L"...");
		}
		else
		{
			wstrInfo.append(wstrName);
		}
	}
	wstrInfo.append(L" ");
	wstrInfo.append(wstrText);
	return wstrInfo;
}

std::string MainWnd::GetUserIdFromStreamId(const std::string & stream_id)
{
	std::string user_id;
	if (stream_id.length() >= 3) {
		int iPos = stream_id.rfind("_");
		if (iPos != stream_id.npos) {
			user_id = stream_id.substr(0, iPos);
		}
		else {
			user_id = stream_id;
		}
	}
	else {
		user_id = stream_id;
	}
	return user_id;
}

void MainWnd::InitCtrl()
{
	CButtonUI* pShowStateInfoBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("show_info_btn")));
	if (pShowStateInfoBtn)
	{
		bool bRelease = false;
#ifdef APP_Release
		bRelease = true;
#else
		bRelease = false;
#endif // APP_Release
		if (bRelease)
		{
			pShowStateInfoBtn->SetVisible(false);
		}
	}
	m_pLocalVideoCtrl = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("local_video")));
	m_pRemoteVideoLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("remote_video_layout")));
	m_pUserList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));

	CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
	CButtonUI* pCameraBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
	if (pScreenShareBtn && pCameraBtn) 
	{
		bool bEnable = (PeerConnectionHelper::m_iResourceType & RongRTCStreamType_Video) == RongRTCStreamType_Video;
		pScreenShareBtn->SetEnabled(bEnable);
		pCameraBtn->SetEnabled(bEnable);
	}
	CButtonUI* pMicBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mic_btn")));
	if (pMicBtn)
	{
		bool bEnable = (PeerConnectionHelper::m_iResourceType & RongRTCStreamType_Audio) == RongRTCStreamType_Audio;
		pMicBtn->SetEnabled(bEnable);
	}
}

void MainWnd::InitText()
{
	CDuiString strTitleText = UPSTR(main_title_tip).c_str();
	strTitleText.Append(_T(" "));
	strTitleText.Append(UpStringUtility::UTF8ToUnicode(PeerConnectionHelper::m_strChannelId).c_str());
	CLabelUI* pTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("channel_id")));
	if (pTitle) pTitle->SetText(strTitleText);

	CLabelUI* pTime = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("connect_time")));
	if (pTime) pTime->SetText(UPSTR(main_time_tip).c_str());
	CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("local_video_tip")));
	if (pLocalVideoTip) pLocalVideoTip->SetText(UPSTR(main_screen_share_tip).c_str());
	COptionUI* pMeetingMessage = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("notification_list_btn")));
	if (pMeetingMessage) pMeetingMessage->SetText(UPSTR(main_meeting_message).c_str());
	COptionUI* pMeetingMember = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("member_list_btn")));
	if (pMeetingMember) pMeetingMember->SetText(UPSTR(main_meeting_member).c_str());

	CButtonUI* pRequestSpeak = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("request_speak_btn")));
	if (pRequestSpeak) pRequestSpeak->SetText(UPSTR(main_request_speak).c_str());
	CButtonUI* pGetInviteUrl = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("get_invite_url_btn")));
	if (pGetInviteUrl) pGetInviteUrl->SetText(UPSTR(main_get_invite_url).c_str());
	CButtonUI* pRequestHost = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("request_host_btn")));
	if (pRequestHost) pRequestHost->SetText(UPSTR(main_request_host).c_str());
	CButtonUI* pShowWb = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("whiteboard_btn")));
	if (pShowWb) pShowWb->SetText(UPSTR(main_show_wb).c_str());
	CButtonUI* pScreenShare = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
	if (pScreenShare) pScreenShare->SetText(UPSTR(main_share_screen).c_str());
	CButtonUI* pLoudSpeak = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("loudspeaker_btn")));
	if (pLoudSpeak) pLoudSpeak->SetText(UPSTR(main_loudspeaker).c_str());
	CButtonUI* pCamera = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
	if (pCamera) pCamera->SetText(UPSTR(main_camera).c_str());
	CButtonUI* pMic = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mic_btn")));
	if (pMic) pMic->SetText(UPSTR(main_mic_mute).c_str());
	CButtonUI* pHangUp = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hangup_btn")));
	if (pHangUp) pHangUp->SetText(UPSTR(main_hangeup).c_str());
}

void MainWnd::Timeout(UpTimer * timer)
{
	if (timer == &m_clConnectionTimer)
	{
		m_clConnectionTimer.Stop();
		m_iConnectionSecond++;
		SetConnectionTime(m_iConnectionSecond);
		m_clConnectionTimer.Start(1000, this);
	}
}
