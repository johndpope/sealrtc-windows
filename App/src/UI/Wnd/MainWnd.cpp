#include "StdAfx.h"
#include "MainWnd.h"
#include "WndHelper.h"
#include "Integer.h"
#include "MainTipWnd.h"
#include "..\jsoncpp\include\json\json.h"
#include "UpDictManager.h"
#include "UpStringUtility.h"

const CDuiString strForbidBigVideoRenderBg = _T("file='main\\only_audio_120.png' dest='353,250,473,370'");
const CDuiString strForbidSmallVideoRenderBg = _T("file='main\\only_audio_48.png' dest='42,42,90,90'");

const CDuiString strBigScreenSharedBg = _T("file='main\\screen_shared_bg_124.png' dest='351,249,475,373'");
const CDuiString strSmallScreenSharedBg = _T("file='main\\screen_shared_bg_124.png' dest='4,4,128,128'");

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

const CDuiString strPortraitHost = _T("file='main\\icon_host_member.png'");
const CDuiString strPortraitNormal = _T("file='main\\icon_normal_member.png'");
const CDuiString strPortraitObserver = _T("file='main\\icon_observer_member.png'");

const CDuiString strItemMicOpenBtnBg = _T("file='main\\icon_open_mic.png' dest='1,0,13,14'");
const CDuiString strItemMicCloseBtnBg = _T("file='main\\icon_close_mic.png' dest='1,0,13,14'");

const CDuiString strItemCameraOpenBtnBg = _T("file='main\\icon_open_camera.png' dest='2,0,13,14'");
const CDuiString strItemCameraCloseBtnBg = _T("file='main\\icon_close_camera.png' dest='2,0,13,14'");

const CDuiString strItemUpLevelBtnBg = _T("file='main\\icon_up_level.png' dest='2,1,12,14'");
const CDuiString strItemDownLevelBtnBg = _T("file='main\\icon_down_level.png' dest='2,1,12,14'");

const CDuiString strSignalBg_0 = _T("file='main\\signal_0.png'");
const CDuiString strSignalBg_1 = _T("file='main\\signal_1.png'");
const CDuiString strSignalBg_2 = _T("file='main\\signal_2.png'");
const CDuiString strSignalBg_3 = _T("file='main\\signal_3.png'");
const CDuiString strSignalBg_4 = _T("file='main\\signal_4.png'");
const CDuiString strSignalBg_5 = _T("file='main\\signal_5.png'");

const CDuiString strFullScreenNormal = _T("file='main\\fullscreen_normal.png'");
const CDuiString strFullScreenHover = _T("file='main\\fullscreen_hot.png'");
const CDuiString strFullScreenPushed = _T("file='main\\fullscreen_hot.png'");

const CDuiString strResotreNormal = _T("file='main\\restore_normal.png'");
const CDuiString strResotreHover = _T("file='main\\restore_hot.png'");
const CDuiString strResotrePushed = _T("file='main\\restore_hot.png'");

const CDuiString strLeftBar = _T("file='main\\bar_left.png'");
const CDuiString strRightBar = _T("file='main\\bar_right.png'");

const CDuiString strHDBtnBg = _T("file = 'main\\hd.png' dest = '31,16,47,32'");
const CDuiString strSmoothBtnBg = _T("file='main\\smooth.png' dest='31,16,47,32'");

IMPL_APPWND_CONTROL(MainWnd, ID_WND_MAIN)

MainWnd::MainWnd(void) :
	m_bIsExistApp(false),
	m_iConnectionSecond(0),
	m_bHaveExistWB(false),
	m_bCameraExist(true),
	m_bFirstJionChannel(true),
	m_bFullScreen(false),
	m_iNowResolutionType(-1)
{
	SetSkinFile(_T("main.xml"));
}

MainWnd::~MainWnd(void)
{
	ClearVideoRender();
	m_clConnectionTimer.Stop();
	m_clCallRequestTimer.Stop();
}

NOTIFY_BEGIN(MainWnd)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("change_video_size_btn"), OnChangeVideoSizeBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("closebtn"), OnClickCloseBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("hangup_btn"), OnClickHangeupBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("camera_btn"), OnClickCameraBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("mic_btn"), OnClickMicBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("loudspeaker_btn"), OnClickLoudSpeakerBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("whiteboard_btn"), OnClickWBBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("get_invite_url_btn"), OnClickInviteBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("screen_share_btn"), OnClickScreenShareBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("request_host_btn"), OnGetHostPower)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("exchange_video_btn"), OnExchangeVideoBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("up_down_btn"), OnClickUpAndDownBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("notification_list_btn"), OnTabOptionChanged)
ON_NOTIFY_MSG(DUI_MSGTYPE_SELECTCHANGED, _T("member_list_btn"), OnTabOptionChanged)
ON_NOTIFY_MSG(DUI_MSGTYPE_MOUSEENTER, _T("list_member_item_layout"), OnEnterMemberListItem)
ON_NOTIFY_MSG(DUI_MSGTYPE_MOUSELEAVE, _T("list_member_item_layout"), OnLeaveMemberListItem)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("item_audio_btn"), OnClickAudioBtnOnMemberList)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("item_video_btn"), OnClickVideoBtnOnMemberList)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("item_up_down_level_btn"), OnClickUpDownLevelBtnOnMemberList)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("item_delete_member_btn"), OnClickDeleteMemberBtnOnMemberList)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("call_refuse_btn"), OnClickRefuseCall)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("call_agree_btn"), OnClickAgreeCall)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("request_speak_btn"), OnClickRequestSpeak)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("fullscreen_restore_btn"), OnClickFullScreenOrRestore)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("show_hide_right_float_layout_btn"), OnClickShowOrHideRightFloatLayout)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("more_btn"), OnClickMoreBtn)
ON_NOTIFY_MSG(DUI_MSGTYPE_CLICK, _T("hd_smooth_switch_btn"), OnClickHDSmoothSwitchBtn)

NOTIFY_END()

LRESULT MainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			if (m_bFullScreen)
			{
				m_bFullScreen = false;
				::SetWindowPos(m_hWnd, NULL, m_rcOldWnd.left, m_rcOldWnd.top, m_rcOldWnd.right - m_rcOldWnd.left, m_rcOldWnd.bottom - m_rcOldWnd.top, SWP_NOZORDER);
				SetFullScreenAndRestoreBtn(false);
				CVerticalLayoutUI* pv = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vmore")));
				pv->SetAttribute(_T("pos"), _T("153,233,233,389"));
			}
		}
	}
	break;
	case WM_UP_BROADCAST:
	{
		switch (wParam)
		{
			case BDT_JION_CAHNNEL_OK:
			{
				if (m_bFirstJionChannel)
				{
					DoUserJoinChannel(PeerConnectionHelper::m_strUserId, m_strName.GetData(), PeerConnectionHelper::m_iMediaType, PeerConnectionHelper::m_iRoleType);

					if (PeerConnectionHelper::m_iRoleType == UserRoleType_Observer)
					{
						AddMessageTipToMessageList(CreateNotificationText(L"", UPSTR(main_meeting_message_1).c_str()));
					}
					else
					{
						AddMessageTipToMessageList(CreateNotificationText(L"", UPSTR(main_meeting_message_2).c_str()));
					}

					if (PeerConnectionHelper::m_iRoleType != UserRoleType_Observer)
					{
						EngineErrorType errorcode = RongRTCEngine::StartLocalAudioVideo();
						if (errorcode == EngineErrorType_CaptureNotFound)
						{
							m_bCameraExist = false;
							structTipInfo* pStructTipInfo = new structTipInfo();
							if (pStructTipInfo)
							{
								pStructTipInfo->m_iDoType = 23;
								if (m_listTipInfo.size() == 0)
								{
									std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
									ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
								}
								m_listTipInfo.push_back(pStructTipInfo);
							}
						}
					}
					CDuiString strText = UPSTR(main_meeting_room).c_str();
					strText.Append(_T(" "));
					strText.Append(m_strChannelId);
					SetJoinChannelStatus(strText.GetData());

					if (PeerConnectionHelper::m_iRoleType == UserRoleType_Host)
					{
						EnableOperationBtn(false, true, false, m_bCameraExist, m_bCameraExist, true);
					}
					else if (PeerConnectionHelper::m_iRoleType == UserRoleType_Normal)
					{
						EnableOperationBtn(false, true, true, m_bCameraExist, m_bCameraExist, true);
					}
					else if (PeerConnectionHelper::m_iRoleType == UserRoleType_Observer)
					{
						EnableOperationBtn(true, false, false, false, false, false, false);
					}


					if (m_clConnectionTimer.GetIsWork())
					{
						m_clConnectionTimer.Stop();
						m_iConnectionSecond = 0;
					}
					m_clConnectionTimer.Start(1000, this);
					ShowConnectionInfo(true);
					RongRTCEngine::QueryEWBServiceUrl();

					m_bFirstJionChannel = false;
				}
			}
			break;
			case BDT_USER_JOIN_CHANNEL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId, strUserName;
					strUserId.append(pMsgItem->GetBuf(0));
					strUserName.append(pMsgItem->GetBuf(1));
					int iRoleType = (int)pMsgItem->GetId(0);
					int iMediaType = (int)pMsgItem->GetId(1);
					UserJoinChannel(strUserId, strUserName, iRoleType, iMediaType);
					if (iRoleType != 2)
					{
						AddMessageTipToMessageList(CreateNotificationText(UpStringUtility::StringToWstring(strUserName), UPSTR(main_meeting_message_3).c_str()));
					}
				}
			}
			break;
			case BDT_USER_LEAVE_CHANNEL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					bool bIsObserver = pMsgItem->IsOK();
					std::wstring wstrName = RemoveMemberFromMeetingList(strUserId);
					SetVideoInvalid(strUserId);
					ShowCallRequest(false, strUserId);
					if (bIsObserver == false && wstrName.empty() != true)
					{
						AddMessageTipToMessageList(CreateNotificationText(wstrName, UPSTR(main_meeting_message_4).c_str()));
					}
				}
			}
			break;
			case BDT_USER_SCREEN_SHARED:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					bool bOpen = pMsgItem->IsOK();
					std::wstring wstrName = GetUserNameFromMemberList(strUserId);
					if (wstrName.empty() != true)
					{
						AddMessageTipToMessageList(CreateNotificationText(wstrName, bOpen ? UPSTR(main_meeting_message_5).c_str() : UPSTR(main_meeting_message_6).c_str()));
					}
				}
			}
			break;
			case BDT_SEND_RECV_LOST_PERCENT_PERCENT:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					SetSendRecvLosPacektPercent((int)pMsgItem->GetId(0), (int)pMsgItem->GetId(1));
				}
			}
			break;
			case BDT_REMOTE_VIDEO_OK:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					UpdateUserInfo(strUserId, -1, -1);
				}
			}
			break;
			case BDT_START_CAMERA_OK:
			{
				UpdateUserInfo(PeerConnectionHelper::m_strUserId, PeerConnectionHelper::m_iMediaType, PeerConnectionHelper::m_iRoleType);
			}
			break;
			case BDT_STOP_CAMERA_OK:
			{
				//SetVideoInvalid(PeerConnectionHelper::m_strUserId);
				//UpdateUserInfo(PeerConnectionHelper::m_strUserId, -1, 2);
			}
			break;
			case BDT_OBSERVER_AGREE_HOST_REQUEST_SPEAK:
			{
				ObserverAgreeHostRequestSpeak();
			}
			break;
			case BDT_NOTIFY_OBSERVER_AGREE_HOST_REQUEST_SPEAK:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					NotifyObserverAgreeHostRequestSpeak(strUserId);
					std::wstring wstrName = GetUserNameFromMemberList(strUserId);
					if (wstrName.empty() != true)
					{
						AddMessageTipToMessageList(CreateNotificationText(wstrName, UPSTR(main_meeting_message_3).c_str()));
					}
				}
			}
			break;
			case BDT_NOTIFY_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					NotifyHostDoObserverRequestBecomeNormalUser(strUserId);
				}
			}
			break;
			case BDT_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					HostDoObserverRequestBecomeNormalUser(strUserId);
				}
			}
			break;
			case BDT_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER:
			{
				SetVideoInvalid(PeerConnectionHelper::m_strUserId);
				UpdateUserInfo(PeerConnectionHelper::m_strUserId, -1, 2);
				UpdateMicAndCameraStatus(1);
				EnableOperationBtn(true, false, false, false, false, false, false);
			}
			break;
			case BDT_NOTIFY_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					SetVideoInvalid(strUserId);
					UpdateUserInfo(strUserId, -1, 2);
				}
			}
			break;
			case BDT_NOTIFY_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER:
			{
				UpdateWBUrl();
			}
			break;
			case BDT_HOST_TICK_OUT:
			case BDT_LEAVE_CAHNNEL_OK:
			{
				if (m_bIsExistApp == false)
				{
					WndHelper::BackLogonWnd(m_strChannelId);
					Close();
					ShowTip(false, L"");
				}
			}
			break;
			case BDT_GET_WB_URL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem && pMsgItem->GetBuf(0).length() != 0)
				{
					m_bHaveExistWB = true;
					m_strWBUrl = UpStringUtility::StringToWstring(pMsgItem->GetBuf(0)).c_str();
					WndHelper::ShowWbWnd(m_strWBUrl);
				}
			}
			break;
			case BDT_QUERY_WB_URL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					if (pMsgItem->GetBuf(0).length() != 0)
					{
						m_bHaveExistWB = true;
						m_strWBUrl = UpStringUtility::StringToWstring(pMsgItem->GetBuf(0)).c_str();
					}
				}
			}
			break;
			case BDT_CREATE_WB:
			{
				m_bHaveExistWB = true;
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					std::wstring wstrName = GetUserNameFromMemberList(strUserId);
					if (wstrName.empty() != true)
					{
						AddMessageTipToMessageList(CreateNotificationText(wstrName, UPSTR(main_meeting_message_7).c_str()));
					}
				}
			}
			break;
			case BDT_HOST_INVITE_OBSERVER_SPEAK:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strHostId;
					strHostId.append(pMsgItem->GetBuf(0));
					structTipInfo* pStructTipInfo = new structTipInfo();
					if (pStructTipInfo)
					{
						pStructTipInfo->m_iDoType = 1;
						pStructTipInfo->m_strResponsedUserId = strHostId;
						if (m_listTipInfo.size() == 0)
						{
							std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
							ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
							m_listTipInfo.push_back(pStructTipInfo);
						}
						else
						{
							bool bHaveExist = TipInfoHaveExist(pStructTipInfo->m_iDoType);
							if (bHaveExist)
							{
								delete pStructTipInfo;
							}
							else
							{
								m_listTipInfo.push_back(pStructTipInfo);
							}
						}
					}
				}
			}
			break;
			case BDT_OBSERVER_REQUSET_SPEAK:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					HandleObserverRequestSpeak(pMsgItem->GetBuf(0));
				}
			}
			break;
			case BDT_HOST_REQUSET_OPEN_MEDIA:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					OnHostRequestOpenMedia(pMsgItem->GetBuf(0), (int)(pMsgItem->GetId(0)));
				}
			}
			break;
			case BDT_GET_INVITE_URL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					bool bSuccessed = pMsgItem->IsOK();
					structTipInfo* pStructTipInfo = new structTipInfo();
					if (pStructTipInfo)
					{
						pStructTipInfo->m_iDoType = 20;
						if (m_listTipInfo.size() == 0)
						{
							std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
							ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
						}
						m_listTipInfo.push_back(pStructTipInfo);
					}
					CDuiString strText;
					strText.Format(UPSTR(main_invite_tip).c_str(), m_strChannelId.GetData(), UpStringUtility::StringToWstring(pMsgItem->GetBuf(0)).c_str());
					WndHelper::CopyTextToClipboard(strText.GetData());
				}
			}
			break;
			case BDT_REMOVE_VIDEO_RENDER:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					int iType = (int)(pMsgItem->GetId(0));
					DeleteVideoRender(strUserId, iType);
				}
			}
			break;
			case BDT_GET_HOST_POWER_SUCCEED:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					bool bSucceed = pMsgItem->IsOK();
					MemberGetHostPower(strUserId, bSucceed);
					if (bSucceed)
					{
						if (PeerConnectionHelper::m_strUserId.compare(strUserId) != 0)
						{
							std::wstring wstrName = GetUserNameFromMemberList(strUserId);
							if (wstrName.empty() != true)
							{
								AddMessageTipToMessageList(CreateNotificationText(wstrName, UPSTR(main_meeting_message_8).c_str()));
							}
						}
						else
						{
							AddMessageTipToMessageList(CreateNotificationText(L"", UPSTR(main_meeting_message_9).c_str()));
						}
					}
				}
			}
			break;
			case BDT_HOST_BUSY:
			{
				structTipInfo* pStructTipInfo = new structTipInfo();
				if (pStructTipInfo)
				{
					pStructTipInfo->m_iDoType = 27;
					if (m_listTipInfo.size() == 0)
					{
						std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
						ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
					}
					m_listTipInfo.push_back(pStructTipInfo);
				}
			}
			break;
			case BDT_CLOSE_SCREEN_SHARED:
			{
				TNotifyUI msg;
				CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
				if (pScreenShareBtn && pScreenShareBtn->GetUserData().CompareNoCase(_T("0")) == 0)
				{
					msg.pSender = pScreenShareBtn;
					OnClickScreenShareBtn(msg);
				}
			}
			break;
			case BDT_USER_MEDIA_CHANGE:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strUserId;
					strUserId.append(pMsgItem->GetBuf(0));
					int iMediaType = (int)pMsgItem->GetId(0);
					UpdateUserInfo(strUserId, iMediaType, -1);
					if (PeerConnectionHelper::m_strUserId.compare(strUserId) == 0)
					{
						UpdateMicAndCameraStatus(iMediaType);
					}
				}
			}
			break;
			case BDT_NOTIFY_AUDIO_LEVEL:
			{
				MsgItem* pMsgItem = (MsgItem*)lParam;
				if (pMsgItem)
				{
					std::string strJson;
					strJson.append(pMsgItem->GetBuf(0));
					UpdateAudioLevel(strJson);
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
		RongRTCEngine::LeaveChannel();
		::PostQuitMessage(0);
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
	m_strChannelId = *(CDuiString*)(void*)aInitData.Find(_T("strChannelId"));
	m_strName = *(CDuiString*)(void*)aInitData.Find(_T("strName"));
}

void MainWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
	InitText();
	InitCtrl();
	std::string strChannelId = UpStringUtility::WstringToString(m_strChannelId.GetData());
	std::string strName = UpStringUtility::WstringToString(m_strName.GetData());
	RongRTCEngine::JoinChannel(strChannelId.c_str(), strName.c_str(), PeerConnectionHelper::m_strToken.c_str(), PeerConnectionHelper::m_iRoleType == 2 ? true : false, PeerConnectionHelper::m_iMediaType == 0 ? UserMediaType_Audio : UserMediaType_Audio_Video);
}

void MainWnd::OnChangeVideoSizeBtn(TNotifyUI& msg)
{
	if (m_iNowResolutionType == -1)
	{
		m_iNowResolutionType = PeerConnectionHelper::m_iResolutionType;
	}
	if (m_iNowResolutionType > 1)
	{
		m_iNowResolutionType--;
	}
	else if (m_iNowResolutionType == 1)
	{
		m_iNowResolutionType = PeerConnectionHelper::m_iResolutionType;
	}
	if (m_iNowResolutionType != -1)
	{
		RongRTCEngine::ChangeVideoSize(RongRTCEngine::VideoResolutionProfile(m_iNowResolutionType));
	}
}

void MainWnd::OnClickCloseBtn(TNotifyUI &msg)
{
	m_bIsExistApp = true;
	if (m_bHaveExistWB && PeerConnectionHelper::m_iRoleType != 2 && m_lsVideoRender.size() == 1)
	{
		structTipInfo* pStructTipInfo = new structTipInfo();
		if (pStructTipInfo)
		{
			pStructTipInfo->m_iDoType = 21;
			if (m_listTipInfo.size() == 0)
			{
				std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
				ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
			}
			m_listTipInfo.push_back(pStructTipInfo);
		}
	}
	else
	{
		RongRTCEngine::LeaveChannel();
		::PostQuitMessage(0);
	}
}

void MainWnd::OnClickHangeupBtn(TNotifyUI & msg)
{
	if (m_bHaveExistWB && PeerConnectionHelper::m_iRoleType != 2 && m_lsVideoRender.size() == 1)
	{
		structTipInfo* pStructTipInfo = new structTipInfo();
		if (pStructTipInfo)
		{
			pStructTipInfo->m_iDoType = 21;
			if (m_listTipInfo.size() == 0)
			{
				std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
				ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
			}
			m_listTipInfo.push_back(pStructTipInfo);
		}
	}
	else
	{
		RongRTCEngine::LeaveChannel();
		CBaseWnd* pWbWnd = WndHelper::GetWbWnd();
		if (pWbWnd)
		{
			pWbWnd->Close();
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
			CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
			if (pScreenShareBtn && pScreenShareBtn->GetUserData().CompareNoCase(_T("0")) == 0)
			{
				structTipInfo* pStructTipInfo = new structTipInfo();
				if (pStructTipInfo)
				{
					pStructTipInfo->m_iDoType = 25;
					if (m_listTipInfo.size() == 0)
					{
						std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
						ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
					}
					m_listTipInfo.push_back(pStructTipInfo);
				}
			}
			else
			{
				if (RongRTCEngine::SetVideoEnabled(false) == EngineErrorType_Success)
				{
					pCameraBtn->SetForeImage(strCameraClose);
					pCameraBtn->SetTextColor(0xff3682f2);
					pCameraBtn->SetUserData(_T("0"));

					VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
					PeerConnectionHelper::UpdateMediaType(1, 0); //视频关闭
					pVideoRender->SetMediaType(PeerConnectionHelper::m_iMediaType);

					if (pVideoRender && pVideoRender->GetVideoUI())
					{
						if (pVideoRender->GetVideoUI()->GetName().Compare(_T("local_video")) == 0)
						{
							CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoRender->GetVideoUI(), _T("local_name_portrait")));
							if (pPortrait)
							{
								RECT rc = { 0 };
								rc = pPortrait->GetPos();
								if (rc.bottom == rc.top)
								{
									rc.bottom = rc.top + 80;
								}
								pPortrait->SetPos(rc);
								pPortrait->SetBkColor(pVideoRender->GetRgb());
								pPortrait->SetText(pVideoRender->GetShortName().c_str());
							}
						}
						else
						{
							CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoRender->GetVideoUI(), _T("name_portrait")));
							if (pPortrait)
							{
								pPortrait->SetVisible(true);
								pPortrait->SetBkColor(pVideoRender->GetRgb());
								pPortrait->SetText(pVideoRender->GetShortName().c_str());
							}
						}
						pVideoRender->SetOutputFrame(false);
						pVideoRender->SetBitmap(NULL, NULL);
					}
				}
			}
		}
		else if (pCameraBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			if (RongRTCEngine::SetVideoEnabled(true) == EngineErrorType_Success)
			{
				VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
				PeerConnectionHelper::UpdateMediaType(1, 1); //打开视频
				pVideoRender->SetMediaType(PeerConnectionHelper::m_iMediaType);
				

				pCameraBtn->SetForeImage(strCameraOpen);
				pCameraBtn->SetTextColor(0xff000000);
				pCameraBtn->SetUserData(_T("1"));
				if (pVideoRender && pVideoRender->GetVideoUI())
				{
					pVideoRender->SetOutputFrame(true);
					if (pVideoRender->GetVideoUI()->GetName().Compare(_T("local_video")) == 0)
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoRender->GetVideoUI(), _T("local_name_portrait")));
						if (pPortrait)
						{
							RECT rc = { 0 };
							rc = pPortrait->GetPos();
							if (rc.bottom != rc.top)
							{
								rc.bottom = rc.top;
							}
							pPortrait->SetPos(rc);
						}
					}
					else
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoRender->GetVideoUI(), _T("name_portrait")));
						if (pPortrait)
						{
							pPortrait->SetVisible(false);
						}
					}
				}
			}
		}
	}
}

void MainWnd::OnClickMicBtn(TNotifyUI & msg)
{
	CButtonUI* pMicBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pMicBtn)
	{
		if (pMicBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			if (RongRTCEngine::SetAudioEnabled(false) == EngineErrorType_Success)
			{
				PeerConnectionHelper::UpdateMediaType(2, 0); //音频关闭
				pMicBtn->SetForeImage(strMicClose);
				pMicBtn->SetTextColor(0xff3682f2);
				pMicBtn->SetUserData(_T("0"));
			}
		}
		else if (pMicBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			if (RongRTCEngine::SetAudioEnabled(true) == EngineErrorType_Success)
			{
				PeerConnectionHelper::UpdateMediaType(2, 1); //音频打开
				pMicBtn->SetForeImage(strMicOpen);
				pMicBtn->SetTextColor(0xff000000);
				pMicBtn->SetUserData(_T("1"));
			}
		}
	}
}

void MainWnd::OnClickLoudSpeakerBtn(TNotifyUI & msg)
{
	CButtonUI* pLoudSpeakerBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pLoudSpeakerBtn)
	{
		if (pLoudSpeakerBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			if (RongRTCEngine::SetSpeakerEnabled(false) == EngineErrorType_Success)
			{
				pLoudSpeakerBtn->SetForeImage(strLoundSpeakerClose);
				pLoudSpeakerBtn->SetTextColor(0xff3682f2);
				pLoudSpeakerBtn->SetUserData(_T("0"));
			}
		}
		else if (pLoudSpeakerBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			if (RongRTCEngine::SetSpeakerEnabled(true) == EngineErrorType_Success)
			{
				pLoudSpeakerBtn->SetForeImage(strLoundSpeakerOpen);
				pLoudSpeakerBtn->SetTextColor(0xff000000);
				pLoudSpeakerBtn->SetUserData(_T("1"));
			}
		}
	}
}

void MainWnd::OnClickWBBtn(TNotifyUI & msg)
{
	CButtonUI* pBWBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pBWBtn)
	{
		if (m_strWBUrl.GetLength() == 0)
		{
			if (PeerConnectionHelper::m_iRoleType != 2)
			{
				RongRTCEngine::GetEWBServiceUrl();
			}
			else
			{
				structTipInfo* pStructTipInfo = new structTipInfo();
				if (pStructTipInfo)
				{
					pStructTipInfo->m_iDoType = 22;
					if (m_listTipInfo.size() == 0)
					{
						std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
						ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
					}
					m_listTipInfo.push_back(pStructTipInfo);
				}
			}
		}
		else
		{
			WndHelper::ShowWbWnd(m_strWBUrl);
		}
	}
}

void MainWnd::OnClickInviteBtn(TNotifyUI & msg)
{
	RongRTCEngine::GetInviteUrl();
}

void MainWnd::OnClickScreenShareBtn(TNotifyUI & msg)
{
	CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pScreenShareBtn)
	{
		if (pScreenShareBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			CButtonUI* pCameraBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
			if (pCameraBtn && pCameraBtn->GetUserData().CompareNoCase(_T("0")) == 0)
			{
				structTipInfo* pStructTipInfo = new structTipInfo();
				if (pStructTipInfo)
				{
					pStructTipInfo->m_iDoType = 24;
					if (m_listTipInfo.size() == 0)
					{
						std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
						ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
					}
					m_listTipInfo.push_back(pStructTipInfo);
				}
			}
			else
			{
				DEVMODE dm;
				ZeroMemory(&dm, sizeof(dm));
				dm.dmSize = sizeof(dm);
				dm.dmDriverExtra = 0;
				EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);

				int iScreenWidth = dm.dmPelsWidth;
				int iScreenHeight = dm.dmPelsHeight;

				EngineErrorType error = RongRTCEngine::StartOrStopScreenShare(true, 0, 0, iScreenWidth, iScreenHeight);
				if (error == EngineErrorType_Success)
				{
					pScreenShareBtn->SetForeImage(strShareHot);
					pScreenShareBtn->SetTextColor(0xff3682f2);
					pScreenShareBtn->SetUserData(_T("0"));
					VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
					if (pVideoRender)
					{
						pVideoRender->SetOutputFrame(false);
						pVideoRender->SetBitmap(NULL, NULL);
						CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(pVideoRender->GetVideoUI());
						if (pVideoCtrl)
						{
							if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
							{
								CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_video_tip")));
								if (pLocalVideoTip)
								{
									RECT rc = pLocalVideoTip->GetPos();
									rc.bottom = rc.top + 20;
									pLocalVideoTip->SetPos(rc);
								}
							}
							else
							{
								CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("exchange_video_btn")));
								if (pExchangeBtn)
								{
									pExchangeBtn->SetText(UPSTR(main_screen_share_tip).c_str());
								}
							}
						}
					}
				}
				else if (error == EngineErrorType_CaptureNotFound)
				{
					pScreenShareBtn->SetForeImage(strShareHot);
					pScreenShareBtn->SetTextColor(0xff3682f2);
					pScreenShareBtn->SetUserData(_T("0"));
					VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
					if (pVideoRender)
					{
						pVideoRender->SetOutputFrame(false);
						pVideoRender->SetBitmap(NULL, NULL);
					}

					structTipInfo* pStructTipInfo = new structTipInfo();
					if (pStructTipInfo)
					{
						pStructTipInfo->m_iDoType = 23;
						if (m_listTipInfo.size() == 0)
						{
							std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
							ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
						}
						m_listTipInfo.push_back(pStructTipInfo);
					}
				}
			}
		}
		else if (pScreenShareBtn->GetUserData().CompareNoCase(_T("0")) == 0)
		{
			EngineErrorType error = RongRTCEngine::StartOrStopScreenShare(false);
			if (error == EngineErrorType_Success)
			{
				pScreenShareBtn->SetForeImage(strShareNormal);
				pScreenShareBtn->SetTextColor(0xff000000);
				pScreenShareBtn->SetUserData(_T("1"));
				VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
				if (pVideoRender)
				{
					pVideoRender->SetOutputFrame(true);
					pVideoRender->SetBitmap(NULL, NULL);

					CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(pVideoRender->GetVideoUI());
					if (pVideoCtrl)
					{
						if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
						{
							CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_video_tip")));
							if (pLocalVideoTip)
							{
								RECT rc = pLocalVideoTip->GetPos();
								rc.bottom = rc.top;
								pLocalVideoTip->SetPos(rc);
							}
						}
						else
						{
							CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("exchange_video_btn")));
							if (pExchangeBtn)
							{
								pExchangeBtn->SetText(_T(""));
							}
						}
					}
				}
			}
			else if (error == EngineErrorType_CaptureNotFound)
			{
				pScreenShareBtn->SetForeImage(strShareNormal);
				pScreenShareBtn->SetTextColor(0xff000000);
				pScreenShareBtn->SetUserData(_T("1"));
				VideoRender* pVideoRender = GetVideoRender(PeerConnectionHelper::m_strUserId);
				if (pVideoRender)
				{
					pVideoRender->SetOutputFrame(true);
					pVideoRender->SetBitmap(NULL, NULL);

					CVideoUI* pVideoCtrl = static_cast<CVideoUI*>(pVideoRender->GetVideoUI());
					if (pVideoCtrl)
					{
						if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
						{
							CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_video_tip")));
							if (pLocalVideoTip)
							{
								RECT rc = pLocalVideoTip->GetPos();
								rc.bottom = rc.top;
								pLocalVideoTip->SetPos(rc);
							}
						}
						else
						{
							CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("exchange_video_btn")));
							if (pExchangeBtn)
							{
								pExchangeBtn->SetText(_T(""));
							}
						}
					}
				}

				structTipInfo* pStructTipInfo = new structTipInfo();
				if (pStructTipInfo)
				{
					pStructTipInfo->m_iDoType = 23;
					if (m_listTipInfo.size() == 0)
					{
						std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
						ShowTip(true, strTipInfo, L"", UPSTR(main_messagebox_know).c_str());
					}
					m_listTipInfo.push_back(pStructTipInfo);
				}
			}
		}
	}
}

void MainWnd::OnGetHostPower(TNotifyUI & msg)
{
	if (PeerConnectionHelper::m_iRoleType != 3)
	{
		structTipInfo* pStructTipInfo = new structTipInfo();
		if (pStructTipInfo)
		{
			pStructTipInfo->m_iDoType = 12;
			if (m_listTipInfo.size() == 0)
			{
				std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
				ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
			}
			m_listTipInfo.push_back(pStructTipInfo);
		}
	}
}

void MainWnd::OnExchangeVideoBtn(TNotifyUI & msg)
{
	CButtonUI* pExchangeVideoBtn = static_cast<CButtonUI*>(msg.pSender);
	CVideoUI* pRemoteVideoCtrl = static_cast<CVideoUI*>(pExchangeVideoBtn->GetParent());
	CVideoUI* pLocalVideoCtrl = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("local_video")));
	if (pRemoteVideoCtrl && pLocalVideoCtrl)
	{
		VideoRender* pVideoRender_1 = (VideoRender*)pLocalVideoCtrl->GetTag();
		VideoRender* pVideoRender_2 = (VideoRender*)pRemoteVideoCtrl->GetTag();
		if (pVideoRender_1 && pVideoRender_2)
		{
			std::string strSubscribeStreamJson = "[";
			if (pVideoRender_1->GetStreamId().compare(PeerConnectionHelper::m_strUserId) != 0)
			{
				strSubscribeStreamJson.append("{\"user_id\":\"");
				strSubscribeStreamJson.append(pVideoRender_1->GetStreamId());
				strSubscribeStreamJson.append("\",\"stream_type\":2}");
			}
			if (pVideoRender_2->GetStreamId().compare(PeerConnectionHelper::m_strUserId) != 0)
			{
				if (strSubscribeStreamJson.compare("[") != 0)
				{
					strSubscribeStreamJson.append(",");
				}
				strSubscribeStreamJson.append("{\"user_id\":\"");
				strSubscribeStreamJson.append(pVideoRender_2->GetStreamId());
				strSubscribeStreamJson.append("\",\"stream_type\":1}");
			}
			strSubscribeStreamJson.append("]");
			RongRTCEngine::SubscribeStream(strSubscribeStreamJson.c_str());

			CGifAnimUI* pLocalSpeakingFlag = static_cast<CGifAnimUI*>(m_PaintManager.FindSubControlByName(pLocalVideoCtrl, _T("local_speaking_flag")));
			if (pLocalSpeakingFlag)
			{
				RECT rc = { 0 };
				rc = pLocalSpeakingFlag->GetPos();
				if (rc.top != rc.bottom)
				{
					rc.top = rc.bottom;
					pLocalSpeakingFlag->SetPos(rc);
				}
			}
			CGifAnimUI* pRemoteSpeakingFlag = static_cast<CGifAnimUI*>(m_PaintManager.FindSubControlByName(pLocalVideoCtrl, _T("speaking_flag")));
			if (pRemoteSpeakingFlag)
			{
				RECT rc = { 0 };
				rc = pRemoteSpeakingFlag->GetPos();
				if (rc.top != rc.bottom)
				{
					rc.top = rc.bottom;
					pRemoteSpeakingFlag->SetPos(rc);
				}
			}

			pRemoteVideoCtrl->SetTag(UINT_PTR(pVideoRender_1));
			pLocalVideoCtrl->SetTag(UINT_PTR(pVideoRender_2));

			BITMAPINFO* pInfo_1 = NULL; BITMAPINFO* pInfo_2 = NULL;
			unsigned char* buffer_1 = NULL; unsigned char* buffer_2 = NULL;
			bool bIsHaveVideo_1 = false; bool bIsHaveVideo_2 = false;
			bool bIsLoading_1 = false; bool bIsLoading_2 = false;
			bool bIsScreenShared_1 = false; bool bIsScreenShared_2 = false;
			
			CLabelUI* pLocalPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pLocalVideoCtrl, _T("local_name_portrait")));
			CLabelUI* pRemotePortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pRemoteVideoCtrl, _T("name_portrait")));
			if (pLocalPortrait && pRemotePortrait)
			{
				RECT rcLocal = { 0 };
				rcLocal = pLocalPortrait->GetPos();
				if (rcLocal.bottom != rcLocal.top)
				{
					bIsHaveVideo_1 = true;
					if (_tcslen(pLocalPortrait->GetBkImage()) != 0)
					{
						bIsLoading_1 = true;
					}
				}

				if (pRemotePortrait->IsVisible())
				{
					bIsHaveVideo_2 = true;

					if (_tcslen(pRemotePortrait->GetBkImage()) != 0)
					{
						bIsLoading_2 = true;
					}
				}

				if (bIsHaveVideo_1)
				{
					if (pRemotePortrait->IsVisible() == false)
					{
						pRemotePortrait->SetVisible(true);
					}
					pRemotePortrait->SetBkColor(pVideoRender_1->GetRgb());
					pRemotePortrait->SetText(pVideoRender_1->GetShortName().c_str());
					if (bIsLoading_1)
					{
						pRemotePortrait->SetBkImage(_T("file='main\\loading_42.png' source='0,0,42,42' dest='19,19,61,61' step='35'"));
						pRemotePortrait->OnActionLoop2(NULL);
					}
					else
					{
						pRemotePortrait->StopAction(_T("loop2"));
						pRemotePortrait->SetBkImage(_T(""));
					}
				}
				else
				{
					if (pRemotePortrait->IsVisible())
					{
						pRemotePortrait->SetVisible(false);
					}
					if (bIsLoading_1 == false)
					{
						pRemotePortrait->StopAction(_T("loop2"));
						pRemotePortrait->SetBkImage(_T(""));
					}
				}

				if (bIsHaveVideo_2)
				{
					if (rcLocal.bottom == rcLocal.top)
					{
						rcLocal.bottom = rcLocal.top + 80;
					}
					pLocalPortrait->SetPos(rcLocal);
					pLocalPortrait->SetBkColor(pVideoRender_2->GetRgb());
					pLocalPortrait->SetText(pVideoRender_2->GetShortName().c_str());
					if (bIsLoading_2)
					{
						pLocalPortrait->SetBkImage(_T("file='main\\loading_42.png' source='0,0,42,42' dest='19,19,61,61' step='35'"));
						pLocalPortrait->OnActionLoop2(NULL);
					}
					else
					{
						pLocalPortrait->StopAction(_T("loop2"));
						pLocalPortrait->SetBkImage(_T(""));
					}
				}
				else
				{
					if (rcLocal.bottom != rcLocal.top)
					{
						rcLocal.bottom = rcLocal.top;
					}
					pLocalPortrait->SetPos(rcLocal);
					if (bIsLoading_2 == false)
					{
						pLocalPortrait->StopAction(_T("loop2"));
						pLocalPortrait->SetBkImage(_T(""));
					}
				}
			}
			
			CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pLocalVideoCtrl, _T("local_video_tip")));
			CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pRemoteVideoCtrl, _T("exchange_video_btn")));
			if (pLocalVideoTip && pExchangeBtn)
			{
				RECT rc = pLocalVideoTip->GetPos();
				if (rc.top != rc.bottom)
				{
					bIsScreenShared_1 = true;
				}
				if (pExchangeBtn->GetText().GetLength() != 0)
				{
					bIsScreenShared_2 = true;
				}
				if (bIsScreenShared_2)
				{
					if (rc.top == rc.bottom)
					{
						rc.bottom = rc.top + 20;
					}
					pLocalVideoTip->SetPos(rc);
				}
				else
				{
					if (rc.top != rc.bottom)
					{
						rc.bottom = rc.top;
					}
					pLocalVideoTip->SetPos(rc);
				}
				if (bIsScreenShared_1)
				{
					pExchangeBtn->SetText(UPSTR(main_screen_share_tip).c_str());
				}
				else
				{
					pExchangeBtn->SetText(_T(""));
				}
			}

			if (bIsHaveVideo_1 || !bIsScreenShared_1)
			{
				pVideoRender_1->GetBitmap(pInfo_1, buffer_1);
			}
			if (bIsHaveVideo_2 || !bIsScreenShared_2)
			{
				pVideoRender_2->GetBitmap(pInfo_2, buffer_2);
			}
			pVideoRender_1->SetBitmap(pInfo_2, buffer_2);
			pVideoRender_2->SetBitmap(pInfo_1, buffer_1);

			pVideoRender_1->ExchangeVideo(pVideoRender_2);
		}
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

void MainWnd::OnEnterMemberListItem(TNotifyUI & msg)
{
	CListContainerElementUI* pListItem = static_cast<CListContainerElementUI*>(msg.pSender->GetParent());
	if (pListItem)
	{
		CDuiString strStreamId = pListItem->GetUserData();
		if (strStreamId.Compare(UpStringUtility::StringToWchar(PeerConnectionHelper::m_strUserId)) == 0)
		{
			return;
		}
	}
	if (PeerConnectionHelper::m_iRoleType == 3)
	{
		ShowDoBtnOnMeetingList((CListContainerElementUI*)(msg.pSender->GetParent()), true);
	}
}

void MainWnd::OnLeaveMemberListItem(TNotifyUI & msg)
{
	ShowDoBtnOnMeetingList(((CListContainerElementUI*)msg.pSender->GetParent()), false);
}

void MainWnd::OnClickAudioBtnOnMemberList(TNotifyUI & msg)
{
	CButtonUI* pAudioBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pAudioBtn)
	{
		CDuiString strUserId = pAudioBtn->GetParent()->GetParent()->GetParent()->GetUserData();

		if (pAudioBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			structTipInfo* pStructTipInfo = new structTipInfo();
			if (pStructTipInfo)
			{
				pStructTipInfo->m_iDoType = 15;
				pStructTipInfo->m_strResponsedUserId = UpStringUtility::WcharToChar(strUserId);
				pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Audio;
				if (m_listTipInfo.size() == 0)
				{
					std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
					ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
				}
				m_listTipInfo.push_back(pStructTipInfo);
			}
		}
		else
		{
			RongRTCEngine::HostRequestControlUserDevice(UpStringUtility::WcharToChar(strUserId), ControlAction_Open, ControlMediaType_Audio);
		}
	}
}

void MainWnd::OnClickVideoBtnOnMemberList(TNotifyUI & msg)
{
	CButtonUI* pVideoBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pVideoBtn)
	{
		CDuiString strUserId = pVideoBtn->GetParent()->GetParent()->GetParent()->GetUserData();
		//1是打开状态 0是关闭状态

		if (pVideoBtn->GetUserData().CompareNoCase(_T("1")) == 0)
		{
			structTipInfo* pStructTipInfo = new structTipInfo();
			if (pStructTipInfo)
			{
				pStructTipInfo->m_iDoType = 16;
				pStructTipInfo->m_strResponsedUserId = UpStringUtility::WcharToChar(strUserId);
				pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Video;
				if (m_listTipInfo.size() == 0)
				{
					std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
					ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
				}
				m_listTipInfo.push_back(pStructTipInfo);
			}
		}
		else
		{
			RongRTCEngine::HostRequestControlUserDevice(UpStringUtility::WcharToChar(strUserId), ControlAction_Open, ControlMediaType_Video);
		}
	}
}

void MainWnd::OnClickUpDownLevelBtnOnMemberList(TNotifyUI & msg)
{
	CButtonUI* pUpDownLevelBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pUpDownLevelBtn)
	{
		CDuiString strUserId = pUpDownLevelBtn->GetParent()->GetParent()->GetParent()->GetUserData();
		if (pUpDownLevelBtn->GetUserData().Compare(_T("0")) == 0)
		{
			RongRTCEngine::HostRequestUpgradeObserverToNormalUser(UpStringUtility::WcharToChar(strUserId));
		}
		else
		{
			structTipInfo* pStructTipInfo = new structTipInfo();
			if (pStructTipInfo)
			{
				pStructTipInfo->m_iDoType = 18;
				pStructTipInfo->m_strResponsedUserId = UpStringUtility::WcharToChar(strUserId);
				pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Video;
				if (m_listTipInfo.size() == 0)
				{
					std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
					ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
				}
				m_listTipInfo.push_back(pStructTipInfo);
			}
		}
	}
}

void MainWnd::OnClickDeleteMemberBtnOnMemberList(TNotifyUI & msg)
{
	CButtonUI* pDeleteMemberBtn = static_cast<CButtonUI*>(msg.pSender);
	if (pDeleteMemberBtn)
	{
		CDuiString strUserId = pDeleteMemberBtn->GetParent()->GetParent()->GetParent()->GetUserData();
		structTipInfo* pStructTipInfo = new structTipInfo();
		if (pStructTipInfo)
		{
			pStructTipInfo->m_iDoType = 19;
			pStructTipInfo->m_strResponsedUserId = UpStringUtility::WcharToChar(strUserId);
			pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Video;
			if (m_listTipInfo.size() == 0)
			{
				std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
				ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
			}
			m_listTipInfo.push_back(pStructTipInfo);
		}
	}
}

void MainWnd::OnClickAgreeCall(TNotifyUI & msg)
{
	std::string strUserId;
	strUserId.append(UpStringUtility::WcharToChar(msg.pSender->GetParent()->GetUserData()));
	if (strUserId.length() > 0)
	{
		RongRTCEngine::HostDoObserverRequestBecomeNormalUser(strUserId.c_str(), OperationType_Accept);
	}
	ShowCallRequest(false, "");
}

void MainWnd::OnClickRefuseCall(TNotifyUI & msg)
{
	std::string strUserId;
	strUserId.append(UpStringUtility::WcharToChar(msg.pSender->GetParent()->GetUserData()));
	if (strUserId.length() > 0)
	{
		RongRTCEngine::HostDoObserverRequestBecomeNormalUser(strUserId.c_str(), OperationType_Refuse);
	}
	ShowCallRequest(false, "");
}

void MainWnd::OnClickRequestSpeak(TNotifyUI & msg)
{
	if (PeerConnectionHelper::m_iRoleType == 2)
	{
		RongRTCEngine::ObserverRequestBecomeNormalUser();
	}
}

void MainWnd::OnClickFullScreenOrRestore(TNotifyUI & msg)
{
	if (m_bFullScreen)
	{
		m_bFullScreen = false;
		::SetWindowPos(m_hWnd, NULL, m_rcOldWnd.left, m_rcOldWnd.top, m_rcOldWnd.right - m_rcOldWnd.left, m_rcOldWnd.bottom - m_rcOldWnd.top, SWP_NOZORDER);
		SetFullScreenAndRestoreBtn(false);

		CVerticalLayoutUI* pv = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vmore")));

		pv->SetAttribute(_T("pos"), _T("153,233,233,389"));
	}
	else
	{
		m_bFullScreen = true;

		::SetWindowLong(m_hWnd, GWL_STYLE, WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_POPUP | WS_CLIPCHILDREN | WS_VISIBLE);

		int screenX = GetSystemMetrics(SM_CXSCREEN);
		int screenY = GetSystemMetrics(SM_CYSCREEN);

		::GetWindowRect(m_hWnd, &m_rcOldWnd);
		CVerticalLayoutUI* pv = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("vmore")));
		std::basic_string<char> strAttr;
		char chAttr[1000];
		sprintf(chAttr, "%d,%d,%d,%d", 153, screenY - 240 - 156, 233, screenY - 240);
		pv->SetAttribute(_T("pos"), UpStringUtility::CharToWchar(chAttr));
		RECT rcScreen;
		rcScreen.left = 0;
		rcScreen.top = 0;
		rcScreen.right = screenX;
		rcScreen.bottom = screenY;

		WINDOWPLACEMENT structOldWndPlacement;
		structOldWndPlacement.length = sizeof(WINDOWPLACEMENT);
		structOldWndPlacement.flags = 0;
		structOldWndPlacement.showCmd = SW_SHOWNORMAL;
		structOldWndPlacement.rcNormalPosition = rcScreen;
		SetWindowPlacement(m_hWnd, &structOldWndPlacement);
		SetFullScreenAndRestoreBtn(true);
	}
}

void MainWnd::OnClickShowOrHideRightFloatLayout(TNotifyUI & msg)
{
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	CVerticalLayoutUI* pRightFloatLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("right_float_layout")));
	if (pRightFloatLayout)
	{
		CButtonUI* pShowOrHideBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pRightFloatLayout, _T("show_hide_right_float_layout_btn")));
		if (pShowOrHideBtn)
		{
			CDuiString strUserData = pRightFloatLayout->GetUserData();
			if (m_bFullScreen)
			{
				if (strUserData.Compare(_T("0")) == 0)
				{
					RECT rc = { screenX - 231 - 15, 0, screenX, screenY };
					pRightFloatLayout->SetPos(rc);
					pRightFloatLayout->SetUserData(_T("1"));
					pShowOrHideBtn->SetBkImage(strRightBar);

					CButtonUI* pFullScreenAndRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("fullscreen_restore_btn")));
					if (pFullScreenAndRestoreBtn)
					{
						RECT rc = { 0 };
						rc = pFullScreenAndRestoreBtn->GetPos();
						rc.left = rc.left - 231;
						rc.right = rc.right - 231;
						pFullScreenAndRestoreBtn->SetPos(rc);
					}

					CVerticalLayoutUI* pCallLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("call_layout")));
					if (pCallLayout)
					{
						RECT rc = { 0 };
						rc = pCallLayout->GetPos();
						rc.left = rc.left - 231;
						rc.right = rc.right - 231;
						pCallLayout->SetPos(rc);
					}
				}
				else
				{
					RECT rc = { screenX - 15, 0, screenX, screenY };
					pRightFloatLayout->SetPos(rc);
					pRightFloatLayout->SetUserData(_T("0"));
					pShowOrHideBtn->SetBkImage(strLeftBar);

					CButtonUI* pFullScreenAndRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("fullscreen_restore_btn")));
					if (pFullScreenAndRestoreBtn)
					{
						RECT rc = { 0 };
						rc = pFullScreenAndRestoreBtn->GetPos();
						rc.left = rc.left + 231;
						rc.right = rc.right + 231;
						pFullScreenAndRestoreBtn->SetPos(rc);
					}

					CVerticalLayoutUI* pCallLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("call_layout")));
					if (pCallLayout)
					{
						RECT rc = { 0 };
						rc = pCallLayout->GetPos();
						rc.left = rc.left + 231;
						rc.right = rc.right + 231;
						pCallLayout->SetPos(rc);
					}
				}
			}
		}
	}
}

void MainWnd::OnClickMoreBtn(TNotifyUI & msg)
{
	CVerticalLayoutUI * more = static_cast<CVerticalLayoutUI *>(m_PaintManager.FindControl(_T("vmore")));
	more->SetVisible(!more->IsVisible());
}

void MainWnd::OnClickHDSmoothSwitchBtn(TNotifyUI & msg)
{
	CButtonUI * switchBtn = static_cast<CButtonUI *>(msg.pSender);
	if (!lstrcmpW(switchBtn->GetText().GetData(), UPSTR(main_high_resolution).c_str()))
	{
		switchBtn->SetText(UPSTR(main_smooth).c_str());
		switchBtn->SetForeImage(strSmoothBtnBg);
		RongRTCEngine::SetVideoMode(RongRTCEngine::VideoProfile_Smooth);
	}
	else if (!lstrcmpW(switchBtn->GetText().GetData(), UPSTR(main_smooth).c_str()))
	{
		switchBtn->SetText(UPSTR(main_high_resolution).c_str());
		switchBtn->SetForeImage(strHDBtnBg);
		RongRTCEngine::SetVideoMode(RongRTCEngine::VideoMode_High_Resolution);
	}
}

void MainWnd::ClickTipClose()
{
	m_listTipInfo.pop_front();

	if (m_listTipInfo.size() == 0)
	{
		ShowTip(false, L"");
	}
	else
	{
		structTipInfo* pStructTipInfo = m_listTipInfo.front();

		if (pStructTipInfo)
		{
			std::wstring strInfo = PaserTipInfoToText(pStructTipInfo);
			ShowTip(true, strInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
		}
	}
}

void MainWnd::ClickTipYes()
{
	if (m_listTipInfo.size() == 0)
	{
		ShowTip(false, L"");
	}
	else
	{
		structTipInfo* pStructTipInfo = m_listTipInfo.front();

		if (pStructTipInfo)
		{
			PaserTipInfoToDo(pStructTipInfo, true);
			m_listTipInfo.pop_front();
			if (m_listTipInfo.size() == 0)
			{
				ShowTip(false, L"");
			}
			else
			{
				pStructTipInfo = m_listTipInfo.front();
				if (pStructTipInfo)
				{
					std::wstring strInfo = PaserTipInfoToText(pStructTipInfo);
					ShowTip(true, strInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
				}
				else
				{
					ShowTip(false, L"");
				}
			}
		}
		else
		{
			ShowTip(false, L"");
		}
	}
}

void MainWnd::ClickTipNo()
{
	if (m_listTipInfo.size() == 0)
	{
		ShowTip(false, L"");
	}
	else
	{
		structTipInfo* pStructTipInfo = m_listTipInfo.front();

		if (pStructTipInfo)
		{
			PaserTipInfoToDo(pStructTipInfo, false);
			m_listTipInfo.pop_front();
			if (m_listTipInfo.size() == 0)
			{
				ShowTip(false, L"");
			}
			else
			{
				pStructTipInfo = m_listTipInfo.front();
				if (pStructTipInfo)
				{
					std::wstring strInfo = PaserTipInfoToText(pStructTipInfo);
					ShowTip(true, strInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
				}
				else
				{
					ShowTip(false, L"");
				}
			}
		}
		else
		{
			ShowTip(false, L"");
		}
	}
}

VideoRender* MainWnd::GetVideoRender(std::string strStreamId)
{
	for (std::list<VideoRender*>::iterator it = m_lsVideoRender.begin(); it != m_lsVideoRender.end(); ++it)
	{
		if (*it && (*it)->GetStreamId().compare(strStreamId) == 0)
		{
			return *it;
		}
	}
	return NULL;
}

void MainWnd::UserJoinChannel(std::string strStreamId, std::string strName, int iUserRoleType, int iMediaType)
{
	DoUserJoinChannel(strStreamId, UpStringUtility::StringToWstring(strName), iMediaType, iUserRoleType);
}

void MainWnd::DeleteVideoRender(std::string strStreamId, int iType)
{
	if (PeerConnectionHelper::m_strUserId.compare(strStreamId) == 0 && iType == 1)
	{
		EnableOperationBtn(true, false, false, false, false, false, false);
		CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
		CButtonUI* pCameraBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
		CButtonUI* pMicBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mic_btn")));
		if (pScreenShareBtn && pCameraBtn && pMicBtn)
		{
			pScreenShareBtn->SetForeImage(strShareNormal);
			pScreenShareBtn->SetTextColor(0xff000000);
			pScreenShareBtn->SetUserData(_T("1"));

			pCameraBtn->SetForeImage(strCameraOpen);
			pCameraBtn->SetTextColor(0xff000000);
			pCameraBtn->SetUserData(_T("1"));

			pMicBtn->SetForeImage(strMicOpen);
			pMicBtn->SetTextColor(0xff000000);
			pMicBtn->SetUserData(_T("1"));
		}
	}
	for (auto it = m_lsNeedDeleteVideoRender.begin(); it != m_lsNeedDeleteVideoRender.end(); ++it)
	{
		if ((*it)->GetStreamId().compare(strStreamId) == 0)
		{
			CVideoUI* pVideoCtrl = (*it)->GetVideoUI();
			if (pVideoCtrl && pVideoCtrl->GetName().Compare(_T("local_video")) != 0)
			{
				CHorizontalLayoutUI* pRemoteVideoLayout = static_cast<CHorizontalLayoutUI*>(pVideoCtrl->GetParent());
				if (pRemoteVideoLayout)
				{
					pRemoteVideoLayout->Remove(pVideoCtrl);
				}
			}
			delete *it;
			m_lsNeedDeleteVideoRender.erase(it);
			break;
		}
	}
}

void MainWnd::ClearVideoRender()
{
	for (auto it = m_lsVideoRender.begin(); it != m_lsVideoRender.end(); ++it)
	{
		delete *it;
		m_lsVideoRender.remove(*it);
		break;
	}
	for (auto it = m_lsNeedDeleteVideoRender.begin(); it != m_lsNeedDeleteVideoRender.end(); ++it)
	{
		delete *it;
		m_lsNeedDeleteVideoRender.remove(*it);
		break;
	}
}

void MainWnd::HandleObserverRequestSpeak(std::string strStreamId)
{
	ShowCallRequest(true, strStreamId);
}

void MainWnd::ObserverAgreeHostRequestSpeak()
{
	DoUserJoinChannel(PeerConnectionHelper::m_strUserId, m_strName.GetData(),PeerConnectionHelper::m_iMediaType, PeerConnectionHelper::m_iRoleType);
	EnableOperationBtn(false, true, true, m_bCameraExist, m_bCameraExist, true);
	UpdateWBUrl();
}

void MainWnd::NotifyObserverAgreeHostRequestSpeak(std::string strStreamId)
{
	DoUserJoinChannel(strStreamId, GetUserNameFromMemberList(strStreamId), 1, 1);
}

void MainWnd::NotifyHostDoObserverRequestBecomeNormalUser(std::string strStreamId)
{
	if (strStreamId.compare(PeerConnectionHelper::m_strUserId) == 0)
	{
		DoUserJoinChannel(strStreamId, m_strName.GetData(), PeerConnectionHelper::m_iMediaType, PeerConnectionHelper::m_iRoleType);
		EnableOperationBtn(false, true, true, m_bCameraExist, m_bCameraExist, true);
		UpdateWBUrl();
	}
	else
	{
		DoUserJoinChannel(strStreamId, GetUserNameFromMemberList(strStreamId), 1, 1);
	}
}

void MainWnd::HostDoObserverRequestBecomeNormalUser(std::string strStreamId)
{
	DoUserJoinChannel(strStreamId, GetUserNameFromMemberList(strStreamId), 1, 1);
}

void MainWnd::MemberGetHostPower(std::string strUserId, bool bSuccessed)
{
	if (PeerConnectionHelper::m_strUserId.compare(strUserId) == 0)
	{
		if (bSuccessed)
		{
			EnableOperationBtn(false, true, false, m_bCameraExist, m_bCameraExist, true);
			UpdateUserInfo(PeerConnectionHelper::m_strUserId, PeerConnectionHelper::m_iMediaType, PeerConnectionHelper::m_iRoleType);
		}
	}
	else
	{
		if (bSuccessed)
		{
			if (PeerConnectionHelper::m_iRoleType != 2)
			{
				PeerConnectionHelper::m_iRoleType = 1;
				EnableOperationBtn(false, true, true, m_bCameraExist, m_bCameraExist, true);
			}
			else
			{
				EnableOperationBtn(true, false, false, false, false, false, false);
			}
			UpdateUserInfo(strUserId, -1, 3);
			
			m_clCallRequestTimer.Stop();
			ShowCallRequest(false, "");
		}
	}
}

void MainWnd::UpdateWBUrl()
{
	CBaseWnd* pWnd = WndHelper::GetWbWnd();
	if (pWnd && m_strWBUrl.GetLength() != 0)
	{
		RongRTCEngine::GetEWBServiceUrl();
	}
}

void MainWnd::DoUserJoinChannel(std::string strStreamId, std::wstring strFullName, int iMediaType, int iRoleType)
{
	if (strStreamId.length() == 0)
	{
		return;
	}

	//init video
	std::wstring wstrShortName = CreateShortName(strFullName).GetData();
	
	int iRgb = CreateRandomRgb(iRoleType);
	if (iRoleType != 2)
	{
		if (m_lsVideoRender.size() == 0)
		{
			CVideoUI* pLocalVideoCtrl = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("local_video")));
			if (pLocalVideoCtrl)
			{
				VideoRender* pVideoRender = new VideoRender(pLocalVideoCtrl, strStreamId, wstrShortName, iRgb, iMediaType, iRoleType);
				m_lsVideoRender.push_back(pVideoRender);
				pLocalVideoCtrl->SetTag(UINT_PTR(pVideoRender));

				pVideoRender->SetOutputFrame(false);
				pLocalVideoCtrl->SetBitmap(NULL, NULL);

				CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pLocalVideoCtrl, _T("local_name_portrait")));
				if (pPortrait)
				{
					RECT rc = { 0 };
					rc = pPortrait->GetPos();
					if (rc.bottom == rc.top)
					{
						rc.bottom = rc.top + 80;
					}
					pPortrait->SetPos(rc);
					pPortrait->SetBkColor(iRgb);
					pPortrait->SetText(wstrShortName.c_str());
					pPortrait->SetBkImage(_T("file='main\\loading_42.png' source='0,0,42,42' dest='19,19,61,61' step='35'"));
					pPortrait->OnActionLoop2(NULL);
				}
			}
		}
		else
		{
			CHorizontalLayoutUI* pRemoteVideoLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("remote_video_layout")));
			if (pRemoteVideoLayout)
			{
				CVideoUI* pRemoteVideoCtrl = new CVideoUI();
				pRemoteVideoCtrl->SetFixedWidth(80);
				pRemoteVideoCtrl->SetFixedHeight(80);
				pRemoteVideoCtrl->SetBkColor(0xFF000000);
				pRemoteVideoCtrl->SetAttribute(_T("fullscreen"), _T("false"));
				SIZE szBorderRound = {5,5};
				pRemoteVideoCtrl->SetBorderRound(szBorderRound);

				CLabelUI *pPortrait = new CLabelUI();
				pPortrait->SetName(_T("name_portrait"));
				pPortrait->SetFixedWidth(80);
				pPortrait->SetFixedHeight(80);
				pPortrait->SetFont(3);
				pPortrait->SetTextColor(0xffffffff);
				pPortrait->SetAttribute(_T("align"), _T("center"));
				pPortrait->SetBkColor(iRgb);
				pPortrait->SetText(wstrShortName.c_str());
				pPortrait->SetAttribute(_T("action"), _T("loop2"));
				pPortrait->SetBkImage(_T("file='main\\loading_42.png' source='0,0,42,42' dest='19,19,61,61' step='35'"));
				pPortrait->SetBorderRound(szBorderRound);
				pRemoteVideoCtrl->Add(pPortrait);

				CGifAnimUI* pSpeakingFlag = new CGifAnimUI();
				pSpeakingFlag->SetName(_T("speaking_flag"));
				pSpeakingFlag->SetFloat(true);
				CDuiString strImagePath = CPaintManagerUI::GetResourcePath() + _T("main\\icon_remote_speaking.gif");
				pSpeakingFlag->UpdateGif(strImagePath);
				pRemoteVideoCtrl->Add(pSpeakingFlag);
				
				CButtonUI* pExchangeBtn = new CButtonUI();
				pExchangeBtn->SetName(_T("exchange_video_btn"));
				pExchangeBtn->SetFixedWidth(80);
				pExchangeBtn->SetFixedHeight(80);
				pExchangeBtn->SetFont(1);
				pExchangeBtn->SetTextColor(0xffffffff);
				pExchangeBtn->SetBorderRound(szBorderRound);

				pRemoteVideoCtrl->Add(pExchangeBtn);
				pRemoteVideoLayout->Add(pRemoteVideoCtrl);

				RECT rcSeapkingFlag = { 5,80,74,80 };
				pSpeakingFlag->SetPos(rcSeapkingFlag);

				VideoRender* pVideoRender = new VideoRender(pRemoteVideoCtrl, strStreamId, wstrShortName, iRgb, iMediaType, iRoleType);
				m_lsVideoRender.push_back(pVideoRender);
				pRemoteVideoCtrl->SetTag(UINT_PTR(pVideoRender));
			}
		}
	}
	//init member list
	
	CListUI* pMeetingMemberList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));
	if (pMeetingMemberList)
	{
		CListContainerElementUI* pMeetingMemberListItem = NULL;
		for (int i = 0; i < pMeetingMemberList->GetCount(); ++i)
		{
			CListContainerElementUI* pMeetingMemberListItemTemp = NULL;
			pMeetingMemberListItemTemp = static_cast<CListContainerElementUI*>(pMeetingMemberList->GetItemAt(i));
			if (pMeetingMemberListItemTemp)
			{
				if (pMeetingMemberListItemTemp->GetUserData().Compare(UpStringUtility::StringToWchar(strStreamId)) == 0)
				{
					pMeetingMemberListItem = pMeetingMemberListItemTemp;
					break;
				}
			}
		}
		if(pMeetingMemberListItem == NULL)
		{
			CDialogBuilder dlgBuilder;
			pMeetingMemberListItem = static_cast<CListContainerElementUI*>(dlgBuilder.Create(_T("list_member_item.xml"), (UINT)0, NULL, &m_PaintManager));
			pMeetingMemberListItem->SetUserData(UpStringUtility::StringToWchar(strStreamId));
			pMeetingMemberList->Add(pMeetingMemberListItem);
		}
		
		if (pMeetingMemberListItem)
		{
			CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("portrait")));
			if (pPortrait)
			{
				pPortrait->SetTag(iRoleType);
				pPortrait->SetText(wstrShortName.c_str());
				pPortrait->SetBkColor(iRgb);
				CLabelUI* pHostIcon = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("host_icon")));
				if (pHostIcon)
				{
					if (iRoleType == 3)
					{
						pHostIcon->SetVisible(true);
					}
					else
					{
						pHostIcon->SetVisible(false);
					}
				}
			}
			CLabelUI* pName = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("name")));
			if (pName)
			{
				pName->SetText(strFullName.c_str());
			}
			int iAudio_Video_Observer = pMeetingMemberListItem->GetTag();
			if (iMediaType == 0)
			{
				iAudio_Video_Observer = 0x100;
			}
			else if (iMediaType == 1)
			{
				iAudio_Video_Observer = 0x110;
			}
			else if (iMediaType == 2)
			{
				iAudio_Video_Observer = 0x010;
			}
			else if (iMediaType == 3)
			{
				iAudio_Video_Observer = 0x000;
			}
			if (iRoleType == 2)
			{
				iAudio_Video_Observer |= 0x001;
			}
			pMeetingMemberListItem->SetTag(iAudio_Video_Observer);
			ShowDoBtnOnMeetingList(pMeetingMemberListItem, false);
		}
	}
	//init control btn
	if (PeerConnectionHelper::m_strUserId.compare(strStreamId) == 0)
	{
		UpdateMicAndCameraStatus(iMediaType);
	}
}

void MainWnd::UpdateUserInfo(std::string strStreamId, int iMediaType, int iRoleType)
{
	if (strStreamId.length() == 0)
	{
		return;
	}

	std::wstring wstrShortName;
	int iRgb = -1;
	iRgb = CreateRandomRgb(iRoleType);

	//Update video
	for (auto it = m_lsVideoRender.begin(); it != m_lsVideoRender.end(); ++it)
	{
		if ((*it)->GetStreamId().compare(strStreamId) == 0)
		{
			CVideoUI* pVideoCtrl = (*it)->GetVideoUI();
			if (iMediaType != -1)
			{
				(*it)->SetMediaType(iMediaType);
			}
			if (iRoleType != -1)
			{
				if ((((*it)->GetRoleType() == 1 || (*it)->GetRoleType() == 3) && iRoleType == 2)
					|| ((*it)->GetRoleType() == 2 && (iRoleType == 1 || iRoleType == 3)))
				{
					(*it)->SetRoleType(iRoleType);
					(*it)->SetRgb(iRgb);
				}
			}
			wstrShortName = (*it)->GetShortName();
			iRgb = (*it)->GetRgb();
			if (pVideoCtrl)
			{
				if ((*it)->GetMediaType() == 0 || (*it)->GetMediaType() == 3)
				{
					if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_name_portrait")));
						RECT rc = { 0 };
						rc = pPortrait->GetPos();
						if (rc.bottom == rc.top)
						{
							rc.bottom = rc.top + 80;
						}
						pPortrait->SetPos(rc);
						pPortrait->SetText((*it)->GetShortName().c_str());
						if ((*it)->GetRoleType() == 1)
						{
							pPortrait->SetBkColor((*it)->GetRgb());
						}
						else if ((*it)->GetRoleType() == 2) //观察者
						{
							//观察者没有render
						}
						else //主持人
						{
							pPortrait->SetBkColor((*it)->GetRgb());
						}
						pPortrait->StopAction(_T("loop2"));
						pPortrait->SetBkImage(_T(""));
					}
					else
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("name_portrait")));
						pPortrait->SetVisible(true);
						pPortrait->SetText((*it)->GetShortName().c_str());
						if ((*it)->GetRoleType() == 1)
						{
							pPortrait->SetBkColor((*it)->GetRgb());
						}
						else if ((*it)->GetRoleType() == 2) //观察者
						{
							//观察者没有render
						}
						else //主持人
						{
							pPortrait->SetBkColor((*it)->GetRgb());
						}
						pPortrait->StopAction(_T("loop2"));
						pPortrait->SetBkImage(_T(""));
					}
					(*it)->SetOutputFrame(false);
					(*it)->SetBitmap(NULL, NULL);
				}
				else
				{
					if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_name_portrait")));
						RECT rc = { 0 };
						rc = pPortrait->GetPos();
						if (rc.bottom != rc.top)
						{
							rc.bottom = rc.top;
						}
						pPortrait->SetPos(rc);
						pPortrait->StopAction(_T("loop2"));
						pPortrait->SetBkImage(_T(""));
						CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_video_tip")));
						if (pLocalVideoTip)
						{
							RECT rc = pLocalVideoTip->GetPos();
							if (rc.top == rc.bottom)
							{
								(*it)->SetOutputFrame(true);
							}
							else
							{
								(*it)->SetOutputFrame(false);
							}
						}
					}
					else
					{
						CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("name_portrait")));
						pPortrait->SetVisible(false);
						pPortrait->StopAction(_T("loop2"));
						pPortrait->SetBkImage(_T(""));
						CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("exchange_video_btn")));
						if (pExchangeBtn)
						{
							if (pExchangeBtn->GetText().GetLength() != 0)
							{
								(*it)->SetOutputFrame(false);
							}
							else
							{
								(*it)->SetOutputFrame(true);
							}
						}
					}
				}

				if ((*it)->GetRoleType() != 2 && PeerConnectionHelper::m_strUserId.compare(strStreamId) != 0)
				{
					if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
					{
						std::string strSubscribeStreamJson = "[{\"user_id\":\"";
						strSubscribeStreamJson.append(strStreamId);
						strSubscribeStreamJson.append("\",\"stream_type\":1}]");
						RongRTCEngine::SubscribeStream(strSubscribeStreamJson.c_str());
					}
					else
					{
						std::string strSubscribeStreamJson = "[{\"user_id\":\"";
						strSubscribeStreamJson.append(strStreamId);
						strSubscribeStreamJson.append("\",\"stream_type\":2}]");
						RongRTCEngine::SubscribeStream(strSubscribeStreamJson.c_str());
					}
				}
			}
			break;
		}
	}

	CListUI* pMeetingMemberList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));
	if (pMeetingMemberList)
	{
		CListContainerElementUI* pMeetingMemberListItem = NULL;
		for (int i = 0; i < pMeetingMemberList->GetCount(); ++i)
		{
			CListContainerElementUI* pMeetingMemberListItemTemp = NULL;
			pMeetingMemberListItemTemp = static_cast<CListContainerElementUI*>(pMeetingMemberList->GetItemAt(i));
			if (pMeetingMemberListItemTemp)
			{
				if (pMeetingMemberListItemTemp->GetUserData().Compare(UpStringUtility::StringToWchar(strStreamId)) == 0)
				{
					pMeetingMemberListItem = pMeetingMemberListItemTemp;
					CLabelUI* pHostIcon = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItemTemp, _T("host_icon")));
					if (pHostIcon)
					{
						if (iRoleType == 3)
						{
							pHostIcon->SetVisible(true);
						}
					}
				}
				else
				{
					CLabelUI* pHostIcon = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItemTemp, _T("host_icon")));
					if (pHostIcon)
					{
						if (iRoleType == 3)
						{
							pHostIcon->SetVisible(false);
						}
					}
				}
			}
		}
		if (pMeetingMemberListItem)
		{
			CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("portrait")));
			if (pPortrait)
			{
				if (wstrShortName.length() != 0)
				{
					pPortrait->SetText(wstrShortName.c_str());
				}
				if (iRoleType == 1)
				{
					if (iRgb != -1)
					{
						pPortrait->SetBkColor(iRgb);
					}
				}
				else if (iRoleType == 2) //观察者
				{
					if (iRgb != -1)
					{
						pPortrait->SetBkColor(iRgb);
					}
				}
				else //主持人
				{
					if (iRgb != -1)
					{
						pPortrait->SetBkColor(iRgb);
					}
				}
			}

			int iAudio_Video_Observer = pMeetingMemberListItem->GetTag();
			if (iMediaType == 0)
			{
				iAudio_Video_Observer = 0x100;
			}
			else if (iMediaType == 1)
			{
				iAudio_Video_Observer = 0x110;
			}
			else if (iMediaType == 2)
			{
				iAudio_Video_Observer = 0x010;
			}
			else if (iMediaType == 3)
			{
				iAudio_Video_Observer = 0x000;
			}
			if (iRoleType == 2)
			{
				iAudio_Video_Observer |= 0x001;
			}
			pMeetingMemberListItem->SetTag(iAudio_Video_Observer);
		}
	}
}

void MainWnd::SetVideoInvalid(std::string strStreamId)
{
	VideoRender* pVideoRender = GetVideoRender(strStreamId);
	if (pVideoRender == NULL)
	{
		return;
	}
	CVideoUI* pVideoCtrl = pVideoRender->GetVideoUI();
	if (pVideoCtrl)
	{
		if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
		{
			CHorizontalLayoutUI* pRemoteVideoLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("remote_video_layout")));
			CVideoUI* pVideoCtrl_1 = NULL;
			for (int i = 0; i < pRemoteVideoLayout->GetCount(); ++i)
			{
				CVideoUI* pTempVideoCtrl_1 = static_cast<CVideoUI*>(pRemoteVideoLayout->GetItemAt(i));
				if (pTempVideoCtrl_1->IsVisible())
				{
					pVideoCtrl_1 = pTempVideoCtrl_1;
					break;
				}
			}

			if (pVideoCtrl_1)
			{
				VideoRender* pVideoRender_1 = (VideoRender*)pVideoCtrl_1->GetTag();
				pVideoCtrl->SetTag(UINT_PTR(pVideoRender_1));
				pVideoRender_1->SetOutputFrame(false);
				pVideoRender_1->SetVideoUI(pVideoCtrl);

				if (PeerConnectionHelper::m_strUserId.compare(pVideoRender_1->GetStreamId()) != 0)
				{
					std::string strSubscribeStreamJson = "[{\"user_id\":\"";
					strSubscribeStreamJson.append(pVideoRender_1->GetStreamId());
					strSubscribeStreamJson.append("\",\"stream_type\":1}]");
					RongRTCEngine::SubscribeStream(strSubscribeStreamJson.c_str());
				}

				if (pVideoRender_1->GetMediaType() == 0 || pVideoRender_1->GetMediaType() == 3)
				{
					CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_name_portrait")));
					RECT rc = { 0 };
					rc = pPortrait->GetPos();
					if (rc.bottom == rc.top)
					{
						rc.bottom = rc.top + 80;
					}
					
					pPortrait->SetPos(rc);
					pPortrait->SetText(pVideoRender_1->GetShortName().c_str());
					if (pVideoRender_1->GetRoleType() == 1)
					{
						pPortrait->SetBkColor(pVideoRender_1->GetRgb());
					}
					else if (pVideoRender_1->GetRoleType() == 2) //观察者
					{
						pPortrait->SetBkColor(pVideoRender_1->GetRgb());
					}
					else //主持人
					{
						pPortrait->SetBkColor(pVideoRender_1->GetRgb());
					}
					pVideoRender_1->SetBitmap(NULL, NULL);
				}
				else
				{
					CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_name_portrait")));
					RECT rc = { 0 };
					rc = pPortrait->GetPos();
					if (rc.bottom != rc.top)
					{
						rc.bottom = rc.top;
					}
					pPortrait->SetPos(rc);

					CLabelUI* pLocalVideoTip = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_video_tip")));
					CButtonUI* pExchangeBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl_1, _T("exchange_video_btn")));
					if (pLocalVideoTip && pExchangeBtn)
					{
						if (pExchangeBtn->GetText().GetLength() != 0)
						{
							RECT rc = pLocalVideoTip->GetPos();
							if (rc.top == rc.bottom)
							{
								rc.bottom = rc.top + 20;
							}
							pLocalVideoTip->SetPos(rc);
							pVideoRender_1->SetOutputFrame(false);
							pVideoRender_1->SetBitmap(NULL, NULL);
						}
						else
						{
							RECT rc = pLocalVideoTip->GetPos();
							if (rc.top != rc.bottom)
							{
								rc.bottom = rc.top;
							}
							pLocalVideoTip->SetPos(rc);

							BITMAPINFO* pInfo = NULL;
							unsigned char* buffer = NULL;
							pVideoCtrl_1->GetBitmap(&pInfo, &buffer);
							pVideoCtrl->SetBitmap(pInfo, buffer);
							pVideoRender_1->SetOutputFrame(true);
						}
					}
				}
				pRemoteVideoLayout->Remove(pVideoCtrl_1);
			}
			else
			{
				CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_name_portrait")));
				RECT rc = { 0 };
				rc = pPortrait->GetPos();
				if (rc.bottom != rc.top)
				{
					rc.bottom = rc.top;
				}
				pPortrait->SetPos(rc);

				BITMAPINFO* pInfo = NULL;
				unsigned char* buffer = NULL;
				pVideoCtrl->SetBitmap(pInfo, buffer);
				pVideoCtrl->SetTag(0);
			}
		}
		else
		{
			pVideoCtrl->SetVisible(false);
		}
	}
	
	for (std::list<VideoRender*>::iterator it = m_lsVideoRender.begin(); it != m_lsVideoRender.end(); ++it)
	{
		if ((*it) == pVideoRender)
		{
			m_lsVideoRender.erase(it);
			break;
		}
	}
	m_lsNeedDeleteVideoRender.push_back(pVideoRender);
}

void MainWnd::OnHostRequestOpenMedia(std::string strHostId, int iMediaType)
{
	structTipInfo* pStructTipInfo = new structTipInfo();
	if (pStructTipInfo)
	{
		if (iMediaType == 2)
		{
			pStructTipInfo->m_iDoType = 3;
			pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Audio;
		}
		else if (iMediaType == 1)
		{
			pStructTipInfo->m_iDoType = 4;
			pStructTipInfo->m_iHostControlMediaType = ControlMediaType_Video;
		}
		else
		{
			return; //其它暂不处理
		}
		pStructTipInfo->m_strResponsedUserId = strHostId;

		if (m_listTipInfo.size() == 0)
		{
			std::wstring strTipInfo = PaserTipInfoToText(pStructTipInfo);
			ShowTip(true, strTipInfo, UPSTR(main_messagebox_yes).c_str(), UPSTR(main_messagebox_no).c_str());
			m_listTipInfo.push_back(pStructTipInfo);
		}
		else
		{
			bool bHaveExist = TipInfoHaveExist(pStructTipInfo->m_iDoType);
			if (bHaveExist)
			{
				delete pStructTipInfo;
			}
			else
			{
				m_listTipInfo.push_back(pStructTipInfo);
			}
		}
	}
}

void MainWnd::InitCtrl()
{
	EnableOperationBtn(false, false, false, false, false, false);

	CButtonUI* pChangeVideoSizeBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("change_video_size_btn")));
	if (pChangeVideoSizeBtn)
	{
		bool bRelease = false;
#ifdef APP_Release
		bRelease = true;
#else
		bRelease = false;
#endif // APP_Release
		if (bRelease)
		{
			pChangeVideoSizeBtn->SetVisible(false);
		}
	}
	CGifAnimUI* pLocalSpeakingFlag = static_cast<CGifAnimUI*>(m_PaintManager.FindControl(_T("local_speaking_flag")));
	if (pLocalSpeakingFlag)
	{
		CDuiString strImagePath = CPaintManagerUI::GetResourcePath() + _T("main\\icon_remote_speaking.gif");
		pLocalSpeakingFlag->UpdateGif(strImagePath);
	}
}

void MainWnd::InitText()
{
	CDuiString strTitleText = UPSTR(main_title_tip).c_str();
	strTitleText.Append(_T(" "));
	strTitleText.Append(m_strChannelId);
	SetJoinChannelStatus(strTitleText.GetData());
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
	CButtonUI* pSwitch = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hd_smooth_switch_btn")));
	if (pSwitch) pSwitch->SetText(UPSTR(main_smooth).c_str());
	CButtonUI* pMore = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("more_btn")));
	if (pMore) pMore->SetText(UPSTR(main_more).c_str());
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
	else if (timer == &m_clCallRequestTimer)
	{
		ShowCallRequest(false, "");
	}
}

void MainWnd::SetJoinChannelStatus(std::wstring strText)
{
	CLabelUI* pChannelId = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("channel_id")));
	if (pChannelId)
	{
		pChannelId->SetText(strText.c_str());
	}
}

void MainWnd::SetSendRecvLosPacektPercent(int iSendLostPacketsPercent, int iReceiveLostPacketsPercent)
{
	CLabelUI* pSignalBg = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("net_signal")));
	if (pSignalBg)
	{
		if (iSendLostPacketsPercent <= 5)
		{
			pSignalBg->SetBkImage(strSignalBg_5);
		}
		else if (iSendLostPacketsPercent <= 15)
		{
			pSignalBg->SetBkImage(strSignalBg_4);
		}
		else if (iSendLostPacketsPercent <= 30)
		{
			pSignalBg->SetBkImage(strSignalBg_3);
		}
		else if (iSendLostPacketsPercent <= 45)
		{
			pSignalBg->SetBkImage(strSignalBg_2);
		}
		else if (iSendLostPacketsPercent < 100)
		{
			pSignalBg->SetBkImage(strSignalBg_1);
		}
		else if (iSendLostPacketsPercent == 100)
		{
			pSignalBg->SetBkImage(strSignalBg_0);
		}
	}
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

void MainWnd::ShowConnectionInfo(bool bShow)
{
	CHorizontalLayoutUI* pConnectionInfoLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("connection_info_layout")));
	if (pConnectionInfoLayout)
	{
		RECT rc = pConnectionInfoLayout->GetPos();
		if (bShow)
		{
			rc.top = rc.bottom - 20;
		}
		else
		{
			rc.top = rc.bottom;
		}
		pConnectionInfoLayout->SetPos(rc);
	}
}

void MainWnd::EnableOperationBtn(bool bRequestSpeakBtn, bool bGetInviteUrlBtn, bool bRequestHostBtn, bool bScreenShareBtn, bool bCameraBtn, bool bMicBtn, bool bHDSmoothBtn)
{
	CButtonUI* pRequestSpeakBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("request_speak_btn")));
	CButtonUI* pGetInviteUrlBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("get_invite_url_btn")));
	CButtonUI* pRequestHostBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("request_host_btn")));
	CButtonUI* pScreenShareBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("screen_share_btn")));
	CButtonUI* pCameraBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
	CButtonUI* pMicBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mic_btn")));
	CButtonUI* pHDSmoothBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("hd_smooth_switch_btn")));

	if (pRequestSpeakBtn && pGetInviteUrlBtn && pRequestHostBtn && pCameraBtn && pMicBtn && pScreenShareBtn)
	{
		pRequestSpeakBtn->SetEnabled(bRequestSpeakBtn);
		pGetInviteUrlBtn->SetEnabled(bGetInviteUrlBtn);
		pRequestHostBtn->SetEnabled(bRequestHostBtn);
		pScreenShareBtn->SetEnabled(bScreenShareBtn);
		pCameraBtn->SetEnabled(bCameraBtn);
		pMicBtn->SetEnabled(bMicBtn);
		pHDSmoothBtn->SetEnabled(bHDSmoothBtn);
	}
}

std::wstring MainWnd::RemoveMemberFromMeetingList(std::string strStreamId)
{
	std::wstring wstrName;
	if (strStreamId.length() != 0)
	{
		CListUI* pMeetingMemberList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));
		if (pMeetingMemberList)
		{
			CListContainerElementUI* pMeetingMemberListItem = NULL;
			for (int i = 0; i < pMeetingMemberList->GetCount(); ++i)
			{
				pMeetingMemberListItem = static_cast<CListContainerElementUI*>(pMeetingMemberList->GetItemAt(i));
				if (pMeetingMemberListItem
					&& pMeetingMemberListItem->GetUserData().Compare(UpStringUtility::StringToWchar(strStreamId)) == 0)
				{
					CLabelUI* pName = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("name")));
					if (pName)
					{
						wstrName.append(pName->GetText().GetData());
					}
					pMeetingMemberList->Remove(pMeetingMemberListItem);
					break;
				}
			}
		}
	}
	return wstrName;
}

void MainWnd::ShowDoBtnOnMeetingList(CListContainerElementUI* pListItem, bool bShow)
{
	if (pListItem)
	{
		int iAudio_Video_Observer = pListItem->GetTag();
		CButtonUI* pAudioBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pListItem, _T("item_audio_btn")));
		if (pAudioBtn)
		{
			if (iAudio_Video_Observer & 0x001 || bShow == false)
			{
				pAudioBtn->SetVisible(false);
			}
			else
			{
				pAudioBtn->SetVisible(true);
				if (iAudio_Video_Observer & 0x100)
				{
					pAudioBtn->SetUserData(L"1");
					pAudioBtn->SetForeImage(strItemMicOpenBtnBg);
				}
				else
				{
					pAudioBtn->SetUserData(L"0");
					pAudioBtn->SetForeImage(strItemMicCloseBtnBg);
				}
			}
		}
		CButtonUI* pVideoBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pListItem, _T("item_video_btn")));
		if (pVideoBtn)
		{
			if (iAudio_Video_Observer & 0x001 || bShow == false)
			{
				pVideoBtn->SetVisible(false);
			}
			else
			{
				pVideoBtn->SetVisible(true);
				if (iAudio_Video_Observer & 0x010)
				{
					pVideoBtn->SetUserData(L"1");
					pVideoBtn->SetForeImage(strItemCameraOpenBtnBg);
				}
				else
				{
					pVideoBtn->SetUserData(L"0");
					pVideoBtn->SetForeImage(strItemCameraCloseBtnBg);
				}
			}
		}
		CButtonUI* pUpOrDownLevelBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pListItem, _T("item_up_down_level_btn")));
		if (pUpOrDownLevelBtn)
		{
			if (bShow == false)
			{
				pUpOrDownLevelBtn->SetVisible(false);
			}
			else
			{
				pUpOrDownLevelBtn->SetVisible(true);
				if (iAudio_Video_Observer & 0x001)
				{
					pUpOrDownLevelBtn->SetUserData(L"0");
					pUpOrDownLevelBtn->SetForeImage(strItemUpLevelBtnBg);
				}
				else
				{
					pUpOrDownLevelBtn->SetUserData(L"1");
					pUpOrDownLevelBtn->SetForeImage(strItemDownLevelBtnBg);
				}
			}
		}
		CButtonUI* pDeleteMemberBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pListItem, _T("item_delete_member_btn")));
		if (pDeleteMemberBtn)
		{
			if (bShow == false)
			{
				pDeleteMemberBtn->SetVisible(false);
			}
			else
			{
				pDeleteMemberBtn->SetVisible(true);
			}
		}
	}
}

void MainWnd::ShowCallRequest(bool bShow, std::string strStreamId)
{
	CHorizontalLayoutUI* pCallRequestLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("call_layout")));
	if (pCallRequestLayout)
	{
		RECT rc = pCallRequestLayout->GetPos();
		if (bShow)
		{
			if (rc.bottom == rc.top)
			{
				CLabelUI* pCallNameAndPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pCallRequestLayout, _T("call_name_portrait")));
				if (pCallRequestLayout)
				{
					std::wstring wstrShortName;
					int iRgb;
					GetShortNameAndRgb(strStreamId, wstrShortName, iRgb);
					pCallNameAndPortrait->SetText(wstrShortName.c_str());
					pCallNameAndPortrait->SetBkColor(iRgb);
				}
				rc.bottom = rc.top + 196;
				pCallRequestLayout->SetUserData(UpStringUtility::StringToWchar(strStreamId));
				pCallRequestLayout->SetPos(rc);
				m_clCallRequestTimer.Start(30000, this);
			}
			else
			{
				RongRTCEngine::HostDoObserverRequestBecomeNormalUser(strStreamId.c_str(), OperationType_Busy);
			}
		}
		else
		{
			CDuiString strUserId = pCallRequestLayout->GetUserData();
			if (strUserId.Compare(UpStringUtility::StringToWchar(strStreamId)) == 0 || strStreamId.length() == 0)
			{
				rc.bottom = rc.top;
				pCallRequestLayout->SetUserData(L"");
				pCallRequestLayout->SetPos(rc);
				m_clCallRequestTimer.Stop();
			}
		}
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

std::wstring MainWnd::PaserTipInfoToText(structTipInfo * pStructTipInfo)
{
	// 被动  1，观察者收到主持人邀请观察者发言，2，主持人收到观察者主动要求发言（呼叫画面），3，用户收到主持人邀请打开用户音频，4，用户收到主持人邀请打开用户视频
	//主动 11，观察者请求发言（不弹框），12，用户获取主持人权限，13，主持人邀请打开音频（不弹框），14，主持人邀请打开视频（不弹框），15，主持人邀请关闭音频，16，主持人邀请关闭视频
	//17，主持人邀请观察者发言（不弹框），18，主持人请求降级正常用户为观察者，19，主持人踢人，20，获取邀请链接，21，保存白板提示，22，观察者打开白板，无白板提醒，23，音视频设备异常
	//24，关闭摄像头情况下，打开屏幕共享，25，屏幕共享情况下，关闭摄像头，26，操作太频繁，27，主持人忙
	std::wstring strRet;
	//主动
	if (pStructTipInfo->m_iDoType == 1)
	{
		strRet.append(UPSTR(main_messagebox_tip_1).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 3)
	{
		strRet.append(UPSTR(main_messagebox_tip_2).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 4)
	{
		strRet.append(UPSTR(main_messagebox_tip_3).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 12)
	{
		strRet.append(UPSTR(main_messagebox_tip_4).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 15)
	{
		strRet.append(UPSTR(main_messagebox_tip_5).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 16)
	{
		strRet.append(UPSTR(main_messagebox_tip_6).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 18)
	{
		strRet.append(UPSTR(main_messagebox_tip_7).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 19)
	{
		strRet.append(UPSTR(main_messagebox_tip_8).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 20)
	{
		strRet.append(UPSTR(main_messagebox_tip_9).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 21)
	{
		strRet.append(UPSTR(main_messagebox_tip_10).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 22)
	{
		strRet.append(UPSTR(main_messagebox_tip_11).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 23)
	{
		strRet.append(UPSTR(main_messagebox_tip_12).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 24)
	{
		strRet.append(UPSTR(main_messagebox_tip_13).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 25)
	{
		strRet.append(UPSTR(main_messagebox_tip_14).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 26)
	{
		strRet.append(UPSTR(main_messagebox_tip_15).c_str());
	}
	else if (pStructTipInfo->m_iDoType == 27)
	{
		strRet.append(UPSTR(main_messagebox_tip_16).c_str());
	}
	return strRet;
}

void MainWnd::PaserTipInfoToDo(structTipInfo * pStructTipInfo, bool bAccept)
{
	if (pStructTipInfo->m_iDoType == 1)
	{
		RongRTCEngine::ObserverDoHostRequestUpgradeObserverToNormalUser(pStructTipInfo->m_strResponsedUserId.c_str(), bAccept);
	}
	else if (pStructTipInfo->m_iDoType == 3 || pStructTipInfo->m_iDoType == 4)
	{
		RongRTCEngine::UserDoHostRequestControlUserDevice(pStructTipInfo->m_strResponsedUserId.c_str(), ControlAction_Open, (ControlMediaType)pStructTipInfo->m_iHostControlMediaType, bAccept);
	}
	else if (pStructTipInfo->m_iDoType == 12)
	{
		if (PeerConnectionHelper::m_iRoleType != 3 && bAccept)
		{
			RongRTCEngine::GetHostPower();
		}
	}
	else if (pStructTipInfo->m_iDoType == 15)
	{
		if (bAccept)
		{
			RongRTCEngine::HostRequestControlUserDevice(pStructTipInfo->m_strResponsedUserId.c_str(), ControlAction_Close, ControlMediaType_Audio);
		}
	}
	else if (pStructTipInfo->m_iDoType == 16)
	{
		if (bAccept)
		{
			RongRTCEngine::HostRequestControlUserDevice(pStructTipInfo->m_strResponsedUserId.c_str(), ControlAction_Close, ControlMediaType_Video);
		}
	}
	else if (pStructTipInfo->m_iDoType == 18)
	{
		if (bAccept)
		{
			RongRTCEngine::HostRequestDegradeNormalUserToObserver(pStructTipInfo->m_strResponsedUserId.c_str());
		}
	}
	else if (pStructTipInfo->m_iDoType == 19)
	{
		if (bAccept)
		{
			RongRTCEngine::HostReomveUser(pStructTipInfo->m_strResponsedUserId.c_str());
		}
	}
	else if (pStructTipInfo->m_iDoType == 20)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 21)
	{
		if (bAccept)
		{
			RongRTCEngine::LeaveChannel();
			if (m_bIsExistApp)
			{
				::PostQuitMessage(0);
			}
			else
			{
				CBaseWnd* pWbWnd = WndHelper::GetWbWnd();
				if (pWbWnd)
				{
					pWbWnd->Close();
				}
			}
		}
	}
	else if (pStructTipInfo->m_iDoType == 22)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 23)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 24)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 25)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 26)
	{
		//无操作
	}
	else if (pStructTipInfo->m_iDoType == 27)
	{
		//无操作
	}
}

void MainWnd::ShowTip(bool bShow, std::wstring strText, std::wstring strLeftText, std::wstring strRightText)
{
	CVerticalLayoutUI* pEnabledLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("enalbed_layout")));
	MainTipWnd* pMainTipWnd = (MainTipWnd*)(WndHelper::CreateMainTipWnd());
	if (pEnabledLayout && pMainTipWnd)
	{
		RECT rc = pEnabledLayout->GetPos();
		if (bShow)
		{
			pMainTipWnd->ShowTip(strText, strLeftText, strRightText);
			if (m_bFullScreen)
			{
				int screenX = GetSystemMetrics(SM_CXSCREEN);
				int screenY = GetSystemMetrics(SM_CYSCREEN);
				rc.bottom = rc.top + screenY;
				rc.right = screenX;
			}
			else
			{
				rc.bottom = rc.top + 660;
				rc.right = 1068;
			}
		}
		else
		{
			pMainTipWnd->Close();
			rc.bottom = rc.top;
		}
		pEnabledLayout->SetPos(rc);
	}
}

bool MainWnd::TipInfoHaveExist(int iDoType)
{
	bool bRet = false;
	for (std::list<structTipInfo*>::iterator it = m_listTipInfo.begin(); it != m_listTipInfo.end(); ++it)
	{
		if ((*it)->m_iDoType == iDoType)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

void MainWnd::SetFullScreenAndRestoreBtn(bool bToFullScreen)
{
	int screenX = GetSystemMetrics(SM_CXSCREEN);
	int screenY = GetSystemMetrics(SM_CYSCREEN);

	CButtonUI* pFullScreenAndRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("fullscreen_restore_btn")));
	if (pFullScreenAndRestoreBtn)
	{
		RECT rc = {0};
		rc = pFullScreenAndRestoreBtn->GetPos();
		if (bToFullScreen)
		{
			rc.right = screenX - 10;
			rc.left = rc.right - 40;
			pFullScreenAndRestoreBtn->SetNormalImage(strResotreNormal);
			pFullScreenAndRestoreBtn->SetHotImage(strResotreHover);
			pFullScreenAndRestoreBtn->SetPushedImage(strResotrePushed);
		}
		else
		{
			rc.left = 785;
			rc.right = 825;

			pFullScreenAndRestoreBtn->SetNormalImage(strFullScreenNormal);
			pFullScreenAndRestoreBtn->SetHotImage(strFullScreenHover);
			pFullScreenAndRestoreBtn->SetPushedImage(strFullScreenPushed);

		}
		pFullScreenAndRestoreBtn->SetPos(rc);
	}

	CLabelUI* pLocalTip = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("local_video_tip")));
	if (pLocalTip)
	{
		RECT rc = { 0 };
		rc = pLocalTip->GetPos();
		if (bToFullScreen)
		{
			if (rc.top != rc.bottom)
			{
				rc.right = screenX - 10;
				rc.top = screenY / 2 - 10;
				rc.bottom = rc.top + 20;
			}
			else
			{
				rc.right = screenX - 10;
				rc.top = screenY / 2 - 10;
				rc.bottom = rc.top;
			}
		}
		else
		{
			if (rc.top != rc.bottom)
			{
				rc.right = 818;
				rc.top = 305;
				rc.bottom = rc.top + 20;
			}
			else
			{
				rc.right = 818;
				rc.top = 305;
				rc.bottom = rc.top;
			}
		}
		pLocalTip->SetPos(rc);
	}

	CLabelUI* pLocalNamePortrait = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("local_name_portrait")));
	if (pLocalNamePortrait)
	{
		RECT rc = { 0 };
		rc = pLocalNamePortrait->GetPos();
		if (bToFullScreen)
		{
			if (rc.top != rc.bottom)
			{
				rc.left = screenX / 2 - 40;
				rc.right = rc.left + 80;
				rc.top = screenY / 2 - 40;
				rc.bottom = rc.top + 80;
			}
			else
			{
				rc.left = screenX / 2 - 40;
				rc.right = rc.left + 80;
				rc.top = screenY / 2 - 40;
				rc.bottom = rc.top;
			}
		}
		else
		{
			if (rc.top != rc.bottom)
			{
				rc.left = 380;
				rc.top = 275;
				rc.right = 460;
				rc.bottom = rc.top + 80;
			}
			else
			{
				rc.left = 380;
				rc.top = 275;
				rc.right = 460;
				rc.bottom = rc.top;
			}
		}
		pLocalNamePortrait->SetPos(rc);
	}

	CVerticalLayoutUI* pCallLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("call_layout")));
	if (pCallLayout)
	{
		RECT rc = { 0 };
		rc = pCallLayout->GetPos();
		if (bToFullScreen)
		{
			rc.right = screenX - 41;
			rc.left = rc.right - 164;
		}
		else
		{
			rc.left = 616;
			rc.right = rc.left + 164;
		}
		pCallLayout->SetPos(rc);
	}

	CVideoUI* pLocalVideo = static_cast<CVideoUI*>(m_PaintManager.FindControl(_T("local_video")));
	CButtonUI* pUpAndDownBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("up_down_btn")));
	if (pLocalVideo && pUpAndDownBtn)
	{
		RECT rc = pLocalVideo->GetInset();

		if (pUpAndDownBtn->GetUserData().Compare(L"1") == 0)
		{
			if (bToFullScreen)
			{
				rc.top = screenY - 120;
			}
			else
			{
				rc.top = 660 - 31 - 120;
			}
		}
		else
		{
			if (bToFullScreen)
			{
				rc.top = screenY - 26;
			}
			else
			{
				rc.top = 660 - 31 - 26;
			}
		}
		pLocalVideo->SetInset(rc);
	}
	
	CVerticalLayoutUI* pRightFloatLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("right_float_layout")));
	if (pRightFloatLayout)
	{
		if (bToFullScreen)
		{
			RECT rc = {screenX - 15, 31, screenX, screenY + 31};
			pRightFloatLayout->SetPos(rc);
			pRightFloatLayout->SetUserData(_T("0"));
		}
		else
		{
			RECT rc = { 837,0,1068,629 };
			pRightFloatLayout->SetPos(rc);
		}
	}
	CVerticalLayoutUI* pShowHideLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("show_hide_layout")));
	if (pShowHideLayout)
	{
		CButtonUI* pShowOrHideBtn = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(pShowHideLayout, _T("show_hide_right_float_layout_btn")));
		if (pShowOrHideBtn)
		{
			if (bToFullScreen)
			{
				pShowHideLayout->SetVisible(true);
				pShowOrHideBtn->SetBkImage(strLeftBar);
			}
			else
			{
				pShowHideLayout->SetVisible(false);
			}
		}
	}
	CHorizontalLayoutUI* pTopLayout = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("top_layout")));
	if (pTopLayout)
	{
		if (bToFullScreen)
		{
			pTopLayout->SetVisible(false);
		}
		else
		{
			pTopLayout->SetVisible(true);
		}
	}
	CVerticalLayoutUI* pRightLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("right_layout")));
	if (pRightLayout)
	{
		if (bToFullScreen)
		{
			pRightLayout->SetVisible(false);
		}
		else
		{
			pRightLayout->SetVisible(true);
		}
	}
}

CDuiString MainWnd::CreateShortName(std::wstring strFullName)
{
	CDuiString wstrShortName;
	CDuiString wstrFullName = UpStringUtility::StringTrim(strFullName).c_str();
	std::list<TCHAR> listTchar;

	int iNameCharLength = 0;
	for (int i = wstrFullName.GetLength() - 1; i >= 0; --i)
	{
		TCHAR tempTchar = wstrFullName.GetAt(i);
		if (tempTchar >= 0x4e00 && tempTchar <= 0x9fcb)
		{
			if (iNameCharLength == 4)
			{
				break;
			}
			iNameCharLength += 2;
			listTchar.push_front(tempTchar);
		}
		else if (tempTchar == 0x3000 || tempTchar == 0x0020)
		{
			break;
		}
		else if (tempTchar <= 'z' && tempTchar >= 'a')
		{
			iNameCharLength += 1;
			listTchar.push_front(tempTchar);
		}
		else if (tempTchar <= 'Z' && tempTchar >= 'A')
		{
			iNameCharLength += 1;
			listTchar.push_front(tempTchar);
		}
		else
		{
			iNameCharLength += 1;
			listTchar.push_front(tempTchar);
		}
		if (iNameCharLength == 5)
		{
			break;
		}
	}
	for (std::list<TCHAR>::iterator it = listTchar.begin(); it != listTchar.end(); ++it)
	{
		wstrShortName += *it;
	}
	return wstrShortName;
}

int MainWnd::CreateRandomRgb(int iRoleType)
{
	int iRet = -1;
	if (iRoleType == 2)
	{
		iRet = 0xFFC3C3C3;
	}
	else
	{
		unsigned int arrayRgb[6] = { 0xFF0066CC, 0xFF009900, 0xFFCC3333, 0xFFCC9966, 0xFFFF9900, 0xFFCC33CC };
		int iPos = ::GetTickCount() % 6;
		iRet = arrayRgb[iPos];
	}
	return iRet;
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

std::wstring MainWnd::GetUserNameFromMemberList(std::string strUserId)
{
	std::wstring wstrName;
	CListUI* pMeetingMemberList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));
	if (pMeetingMemberList)
	{
		CListContainerElementUI* pMeetingMemberListItem = NULL;
		for (int i = 0; i < pMeetingMemberList->GetCount(); ++i)
		{
			pMeetingMemberListItem = static_cast<CListContainerElementUI*>(pMeetingMemberList->GetItemAt(i));
			if (pMeetingMemberListItem && pMeetingMemberListItem->GetUserData().Compare(UpStringUtility::StringToWchar(strUserId)) == 0)
			{
				CLabelUI* pName = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("name")));
				if (pName)
				{
					wstrName.append(pName->GetText().GetData());
					break;
				}
			}
		}
	}
	return wstrName;
}

void MainWnd::GetShortNameAndRgb(std::string strUserId, std::wstring & wstrShortName, int & iRgb)
{
	CListUI* pMeetingMemberList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("member_list")));
	if (pMeetingMemberList)
	{
		CListContainerElementUI* pMeetingMemberListItem = NULL;
		for (int i = 0; i < pMeetingMemberList->GetCount(); ++i)
		{
			pMeetingMemberListItem = static_cast<CListContainerElementUI*>(pMeetingMemberList->GetItemAt(i));
			if (pMeetingMemberListItem && pMeetingMemberListItem->GetUserData().Compare(UpStringUtility::StringToWchar(strUserId)) == 0)
			{
				CLabelUI* pPortrait = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pMeetingMemberListItem, _T("portrait")));
				if (pPortrait)
				{
					wstrShortName.append(pPortrait->GetText().GetData());
					iRgb = pPortrait->GetBkColor();
					break;
				}
			}
		}
	}
}

void MainWnd::UpdateAudioLevel(std::string strJson)
{
	Json::Reader reader;
	Json::Value jmessage;
	if (reader.parse(strJson, jmessage))
	{
		int iSize = jmessage.size();
		for (int i = 0; i < iSize; ++i)
		{
			std::string strId = jmessage[i]["id"].asString();
			int iAudioLevel = jmessage[i]["audio_level"].asInt();
			for (std::list<VideoRender*>::iterator it = m_lsVideoRender.begin(); it != m_lsVideoRender.end(); ++it)
			{
				VideoRender* pVideoRender = *it;
				if (pVideoRender && pVideoRender->GetStreamId().compare(strId) == 0)
				{
					CVideoUI* pVideoCtrl = pVideoRender->GetVideoUI();
					if (pVideoCtrl)
					{
						if (pVideoCtrl->GetName().Compare(_T("local_video")) == 0)
						{
							CGifAnimUI* pAudioLevel = static_cast<CGifAnimUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("local_speaking_flag")));
							if (pAudioLevel)
							{
								RECT rc = { 0 };
								rc = pAudioLevel->GetPos();
								if (iAudioLevel > 0)
								{
									if (rc.bottom == rc.top)
									{
										rc.top = rc.bottom - 20;
										pAudioLevel->SetPos(rc);
									}
								}
								else
								{
									if (rc.bottom != rc.top)
									{
										rc.top = rc.bottom;
										pAudioLevel->SetPos(rc);
									}
								}
							}
						}
						else
						{
							CGifAnimUI* pAudioLevel = static_cast<CGifAnimUI*>(m_PaintManager.FindSubControlByName(pVideoCtrl, _T("speaking_flag")));
							if (pAudioLevel)
							{
								RECT rc = { 0 };
								rc = pAudioLevel->GetPos();
								if (iAudioLevel > 0)
								{
									if (rc.bottom == rc.top)
									{
										rc.top = rc.bottom - 20;
										pAudioLevel->SetPos(rc);
									}
								}
								else
								{
									if (rc.bottom != rc.top)
									{
										rc.top = rc.bottom;
										pAudioLevel->SetPos(rc);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void MainWnd::UpdateMicAndCameraStatus(int iMediaType)
{
	CButtonUI* pCameraBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("camera_btn")));
	CButtonUI* pMicBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mic_btn")));
	if (pCameraBtn && pMicBtn)
	{
		if (iMediaType == 0)
		{
			pCameraBtn->SetForeImage(strCameraClose);
			pCameraBtn->SetTextColor(0xff3682f2);
			pCameraBtn->SetUserData(_T("0"));

			pMicBtn->SetForeImage(strMicOpen);
			pMicBtn->SetTextColor(0xff000000);
			pMicBtn->SetUserData(_T("1"));
		}
		else if (iMediaType == 1)
		{
			pCameraBtn->SetForeImage(strCameraOpen);
			pCameraBtn->SetTextColor(0xff000000);
			pCameraBtn->SetUserData(_T("1"));

			pMicBtn->SetForeImage(strMicOpen);
			pMicBtn->SetTextColor(0xff000000);
			pMicBtn->SetUserData(_T("1"));
		}
		else if (iMediaType == 2)
		{
			pCameraBtn->SetForeImage(strCameraOpen);
			pCameraBtn->SetTextColor(0xff000000);
			pCameraBtn->SetUserData(_T("1"));

			pMicBtn->SetForeImage(strMicClose);
			pMicBtn->SetTextColor(0xff3682f2);
			pMicBtn->SetUserData(_T("0"));
		}
		else if (iMediaType == 3)
		{
			pCameraBtn->SetForeImage(strCameraClose);
			pCameraBtn->SetTextColor(0xff3682f2);
			pCameraBtn->SetUserData(_T("0"));

			pMicBtn->SetForeImage(strMicClose);
			pMicBtn->SetTextColor(0xff3682f2);
			pMicBtn->SetUserData(_T("0"));
		}
	}
}
