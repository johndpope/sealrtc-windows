#include "PeerConnectionHelper.h"
#include "RongRTCEngineVideoRender.h"
#include "WndHelper.h"
#include "MainWnd.h"

PeerConnectionHelper* PeerConnectionHelper::m_pPeerConnectionHelper = NULL;
std::string PeerConnectionHelper::m_strUserId;
std::string PeerConnectionHelper::m_strSDKVersion;
std::string PeerConnectionHelper::m_strToken;
int PeerConnectionHelper::m_iMediaType = 1;
int PeerConnectionHelper::m_iRoleType = 1;
int PeerConnectionHelper::m_iResolutionType = 5;
bool PeerConnectionHelper::m_bOpenObserver = false;
bool PeerConnectionHelper::m_bOpenOnlyAudio = false;
bool PeerConnectionHelper::m_bOpenBeauty = false;
bool PeerConnectionHelper::m_bOpenTinyStream = false;

PeerConnectionHelper::PeerConnectionHelper()
{

}

PeerConnectionHelper::~PeerConnectionHelper()
{

}

void PeerConnectionHelper::CreatePeerConnectionHelper()
{
	if (m_pPeerConnectionHelper == NULL)
	{
		m_pPeerConnectionHelper = new PeerConnectionHelper();
	}
}

void PeerConnectionHelper::DestoryPeerConnectionHelper()
{
	if (m_pPeerConnectionHelper)
	{
		delete m_pPeerConnectionHelper;
		m_pPeerConnectionHelper = NULL;
	}
}

PeerConnectionHelper* PeerConnectionHelper::GetInstance()
{
	return m_pPeerConnectionHelper;
}

void PeerConnectionHelper::OnJoinChannelOK(UserRoleType enumUserRoleType)
{
	m_iRoleType = enumUserRoleType;
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_JION_CAHNNEL_OK, NULL);
}

void PeerConnectionHelper::OnJoinChannelFailed(JoinChannelFailedType enumJoinChannelFailedType)
{
}

void PeerConnectionHelper::OnLeaveChannelOK()
{
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_LEAVE_CAHNNEL_OK, NULL);
}

void PeerConnectionHelper::OnDisconnectWhitChannel()
{
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_LEAVE_CAHNNEL_OK, NULL);
}

IVideoRender * PeerConnectionHelper::OnStartLocalAudioVideoOK(UserMediaType enumUserMediaType)
{
	IVideoRender* pVideoRender = NULL;
	MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
	if (pMainWnd)
	{
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_START_CAMERA_OK, NULL);
		pVideoRender = pMainWnd->GetVideoRender(m_strUserId);
	}
	return pVideoRender;
}

void PeerConnectionHelper::OnNotifyLocalAudioVideoStop()
{
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_STOP_CAMERA_OK, NULL);
}

IVideoRender* PeerConnectionHelper::OnNotifyUserVideoCreated(const char* strUserID, UserMediaType enumUserMediaType)
{
	IVideoRender* pVideoRender = NULL;
	MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
	if (pMainWnd)
	{
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserID);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_VIDEO_OK, pMsgItem);
		pVideoRender = pMainWnd->GetVideoRender(strUserID);
	}
	return pVideoRender;
}

void PeerConnectionHelper::OnNotifyUserJoinChannel(const char* strUserID, const char* strName, UserRoleType enumUserRoleType, UserMediaType enumUserMediaType)
{
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutBuf(strUserID);
	pMsgItem->PutBuf(strName);
	pMsgItem->PutId(enumUserRoleType);
	pMsgItem->PutId(enumUserMediaType);

	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_JOIN_CHANNEL, pMsgItem);
}

void PeerConnectionHelper::OnNotifyUserLeaveChannel(const char * strUserID, bool bIsObserver)
{
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutBuf(strUserID);
	pMsgItem->SetIsOK(bIsObserver);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_LEAVE_CHANNEL, pMsgItem);
}

void PeerConnectionHelper::OnNotifyUserVideoDestroyed(const char * strUserID)
{
	MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
	if (pMainWnd)
	{
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserID);
		pMsgItem->PutId(0);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOVE_VIDEO_RENDER, pMsgItem);
	}
}

void PeerConnectionHelper::OnUserMediaChanged(const char * strUserID, UserMediaType enumUserMediaType)
{
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutBuf(strUserID);
	pMsgItem->PutId(enumUserMediaType);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_MEDIA_CHANGE, pMsgItem);
}

void PeerConnectionHelper::OnUserOpenOrCloseScreenShare(const char * strUserID, bool bOpen)
{
	MsgItem* pMsgItem = new MsgItem();
	if (pMsgItem)
	{
		pMsgItem->PutBuf(strUserID);
		pMsgItem->SetIsOK(bOpen);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_SCREEN_SHARED, pMsgItem);
	}
}

void PeerConnectionHelper::OnUserCreateWB(const char * strUserID)
{
	MsgItem* pMsgItem = new MsgItem();
	if (pMsgItem)
	{
		pMsgItem->PutBuf(strUserID);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_CREATE_WB, pMsgItem);
	}
}

void PeerConnectionHelper::OnObserverRequestBecomeNormalUser(bool bSuccessed)
{
	if (bSuccessed == true)
	{
		//观察者请求发言成功，等待主持人审批
	}
	else
	{
		//观察者请求发言失败
	}
}

void PeerConnectionHelper::OnNotifyObserverRequestBecomeNormalUser(const char * strUserID)
{
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutBuf(strUserID);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_OBSERVER_REQUSET_SPEAK, pMsgItem);
}

void PeerConnectionHelper::OnHostDoObserverRequestBecomeNormalUser(const char * strUserId, bool bAccept, bool bSuccessed)
{
	if (bSuccessed == true)
	{
		//主持人处理观察者请求发言成功
		if (bAccept)
		{
			MsgItem* pMsgItem = new MsgItem();
			pMsgItem->PutBuf(strUserId);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER, pMsgItem);
		}
	}
	else
	{
		//主持人处理观察者请求发言失败
	}
}

void PeerConnectionHelper::OnNotifyHostDoObserverRequestBecomeNormalUser(const char* strUserID, OperationType enumEngineOperationType)
{
	if (enumEngineOperationType == OperationType_Accept)
	{
		//bAccept == true 与会人员收到，观察者请求发言被允许
		if (m_strUserId.compare(strUserID) == 0)
		{
			m_iRoleType = 1;
			m_iMediaType = 1;
		}
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserID);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_HOST_DO_OBSERVER_REQUEST_BECOME_NORMAL_USER, pMsgItem);
	}
	else if (enumEngineOperationType == OperationType_Busy)
	{
		//忙碌
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_HOST_BUSY, NULL);
	}
	else
	{
		//bAccept == false 观察者收到通知，请求发言被拒绝
	}
}

void PeerConnectionHelper::OnHostRequestUpgradeObserverToNormalUser(const char * strUserId, bool bSuccessed)
{
	if (bSuccessed == true)
	{
		//主持人邀请观察者发言成功
	}
	else
	{
		//主持人邀请观察者发言失败
	}
}

void PeerConnectionHelper::OnNotifyHostRequestUpgradeObserverToNormalUser(const char * strHostID)
{
	//观察者收到主持人邀请发言通知
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutBuf(strHostID);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_HOST_INVITE_OBSERVER_SPEAK, pMsgItem);
}

void PeerConnectionHelper::OnObserverDoHostRequestUpgradeObserverToNormalUser(const char * strHostId, bool bAccpet, bool bSuccessed)
{
	//观察者调用ObserverDoHostInvite接口，返回操作结果
	//更新自己的状态
	if (bSuccessed && bAccpet)
	{
		MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
		if (pMainWnd)
		{
			m_iRoleType = 1;
			m_iMediaType = 1;
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_OBSERVER_AGREE_HOST_REQUEST_SPEAK, NULL);
		}
	}
}

void PeerConnectionHelper::OnNotifyObserverDoHostRequestUpgradeObserverToNormalUser(const char * strUserId, bool bAccpet)
{
	if (bAccpet)
	{
		//bAccept == true 与会人员收到，观察者接受邀请的通知
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserId);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_OBSERVER_AGREE_HOST_REQUEST_SPEAK, pMsgItem);
	}
	else
	{
		//bAccept == false 主持人收到通知，邀请发言被观察者拒绝
	}
}

void PeerConnectionHelper::OnHostRequestDegradeNormalUserToObserver(const char * strUserID, bool bSuccessed)
{
}

void PeerConnectionHelper::OnNotifyHostRequestDegradeNormalUserToObserver(const char * strHostID)
{
	//默认通知降级
	RongRTCEngine::NoramlUserDoHostRequestDegradeNormalUserToObserver(strHostID, true);

	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER, NULL);
}

void PeerConnectionHelper::OnNoramlUserDoHostRequestDegradeNormalUserToObserver(const char * strHostID, bool bAccpet, bool bSuccessed)
{
	if (bAccpet && bSuccessed)
	{
		m_iRoleType = 2;
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER, NULL);
	}
}

void PeerConnectionHelper::OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver(const char * strUserId, bool bAccpet)
{
	if (bAccpet)
	{
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserId);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_NORAML_USER_DO_HOST_REQUEST_DEGRADE_NORMAL_USER_TO_OBSERVER, pMsgItem);
	}
}

void PeerConnectionHelper::OnHostRequestControlUserDevice(const char * strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bSuccessed)
{
}

void PeerConnectionHelper::OnNotifyHostRequestControlUserDevice(const char * strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType)
{
	if (enumControlAction == ControlAction_Open)
	{
		//主持人请求打开音、视频，需要用户二次确认是否接受请求
		MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
		if (pMainWnd && strHostId && strlen(strHostId) != 0)
		{
			MsgItem* pMsgItem = new MsgItem();
			pMsgItem->PutBuf(strHostId);
			pMsgItem->PutId(enumControlMediaType);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_HOST_REQUSET_OPEN_MEDIA, pMsgItem);
		}
	}
	else
	{
		RongRTCEngine::UserDoHostRequestControlUserDevice(strHostId, enumControlAction, enumControlMediaType, true);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_CLOSE_SCREEN_SHARED, NULL);
	}
}

void PeerConnectionHelper::OnUserDoHostRequestControlUserDevice(const char * strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccept, bool bSuccessed, UserMediaType enumMediaType)
{
	//被请求者处理主持人的请求，通知操作结果
	if (bSuccessed)
	{
		if (bAccept)
		{
			m_iMediaType = enumMediaType;
			MsgItem* pMsgItem = new MsgItem();
			pMsgItem->PutBuf(m_strUserId);
			pMsgItem->PutId(enumMediaType);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_MEDIA_CHANGE, pMsgItem);
		}
	}
	else
	{

	}
}

void PeerConnectionHelper::OnNotifyUserDoHostRequestControlUserDevice(const char * strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccpet, UserMediaType enumMediaType)
{
	if (bAccpet && enumMediaType != -1)
	{
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUserId);
		pMsgItem->PutId(enumMediaType);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_USER_MEDIA_CHANGE, pMsgItem);
	}
}

void PeerConnectionHelper::OnGetEWBServiceUrl(const char * strUrl)
{
	if (strUrl && strlen(strUrl) != 0)
	{
		//UpFile::OpenURL(UpStringUtility::StringToWstring(strUrl));
		
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUrl);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_WB_URL, pMsgItem);
	}
	else
	{
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_WB_URL, NULL);
	}
}

void PeerConnectionHelper::OnQueryEWBServiceUrl(const char * strUrl)
{
	if (strUrl && strlen(strUrl) != 0)
	{
		MsgItem* pMsgItem = new MsgItem();
		pMsgItem->PutBuf(strUrl);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_QUERY_WB_URL, pMsgItem);
	}
	else
	{
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_QUERY_WB_URL, NULL);
	}
}

void PeerConnectionHelper::OnGetSendRecvStatisticsData(unsigned __int64 iSendLostPacketPercent, unsigned __int64 iReceiveLostPacketPercent)
{
	MsgItem* pMsgItem = new MsgItem();
	pMsgItem->PutId(iSendLostPacketPercent);
	pMsgItem->PutId(iReceiveLostPacketPercent);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_SEND_RECV_LOST_PERCENT_PERCENT, pMsgItem);
}

void PeerConnectionHelper::OnNotifyUserAudioLevel(const char * strJson)
{
	if (strJson)
	{
		std::string strAudioLevelJson;
		strAudioLevelJson.append(strJson);
		MsgItem* pMsgItem = new MsgItem();
		if (pMsgItem)
		{
			pMsgItem->PutBuf(strAudioLevelJson);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_AUDIO_LEVEL, pMsgItem);
		}
	}
}

void PeerConnectionHelper::OnHostReomveMemberOrObserver(const char * strUserId, bool bSuccessed)
{
}

void PeerConnectionHelper::OnNotifyHostReomveMemberOrObserver(const char * strUserID)
{
	//异步操作，防止UI卡死回调线程
	MsgBroadcast::GetMsgBroadcast()->PostMsg(BDT_HOST_TICK_OUT, NULL);
}

void PeerConnectionHelper::OnGetHostPower(bool bSuccessed)
{
	MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
	if (pMainWnd)
	{
		MsgItem* pMsgItem = new MsgItem();
		if (pMsgItem)
		{
			m_iRoleType = 3;
			pMsgItem->SetIsOK(bSuccessed);
			pMsgItem->PutBuf(m_strUserId);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_HOST_POWER_SUCCEED, pMsgItem);
		}
	}
}

void PeerConnectionHelper::OnNotifyGetHostPower(const char * strUserID)
{
	//通知strUserID获取了主持人权限
	MainWnd* pMainWnd = (MainWnd*)WndHelper::GetMainWnd();
	if (pMainWnd)
	{
		MsgItem* pMsgItem = new MsgItem();
		if (pMsgItem)
		{
			bool bSucceed = true;
			pMsgItem->SetIsOK(bSucceed);
			pMsgItem->PutBuf(strUserID);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_HOST_POWER_SUCCEED, pMsgItem);
		}
	}
}

void PeerConnectionHelper::OnGetInviteUrl(const char * strUrl, bool bSuccessed)
{
	if (strUrl && strlen(strUrl) != 0)
	{
		MsgItem* pMsgItem = new MsgItem();
		if (pMsgItem)
		{
			pMsgItem->SetIsOK(bSuccessed);
			pMsgItem->PutBuf(strUrl);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_INVITE_URL, pMsgItem);
		}
	}
}

void PeerConnectionHelper::UpdateMediaType(int iOperationMediaType, int iDoType) //1 打开 0关闭
{
	if (iOperationMediaType == 1) //视频
	{
		if (m_iMediaType == 0 && iDoType == 1)
		{
			m_iMediaType = 1;
		}
		else if (m_iMediaType == 1 && iDoType == 0)
		{
			m_iMediaType = 0;
		}
		else if (m_iMediaType == 2 && iDoType == 0)
		{
			m_iMediaType = 3;
		}
		else if (m_iMediaType == 3 && iDoType == 1)
		{
			m_iMediaType = 2;
		}
	}
	else if (iOperationMediaType == 2) //音频
	{
		if (m_iMediaType == 0 && iDoType == 0)
		{
			m_iMediaType = 3;
		}
		else if (m_iMediaType == 1 && iDoType == 0)
		{
			m_iMediaType = 2;
		}
		else if (m_iMediaType == 2 && iDoType == 1)
		{
			m_iMediaType = 1;
		}
		else if (m_iMediaType == 3 && iDoType == 1)
		{
			m_iMediaType = 0;
		}
	}
}
