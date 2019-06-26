#include "PeerConnectionHelper.h"
#include "WndHelper.h"
#include "MainWnd.h"
#include "..\jsoncpp\include\json\json.h"

std::string PeerConnectionHelper::m_strSDKVersion = "";
std::string PeerConnectionHelper::m_strUserId = "";
std::string PeerConnectionHelper::m_strUserName = "";
std::string PeerConnectionHelper::m_strChannelId = "";
std::string PeerConnectionHelper::m_strToken = "";
std::string PeerConnectionHelper::m_strPhone = "";
int PeerConnectionHelper::m_iResourceType = RongRTCStreamType_AudioVideo;
int PeerConnectionHelper::m_iResolutionType = 5;
bool PeerConnectionHelper::m_bOpenBeauty = false;
bool PeerConnectionHelper::m_bOpenTinyStream = true;
bool PeerConnectionHelper::m_bConnected = false;
HWND PeerConnectionHelper::m_hMainWnd = 0;

PeerConnectionHelper * PeerConnectionHelper::GetInstance()
{
	static PeerConnectionHelper m_clPeerConnectionHelper;
	return &m_clPeerConnectionHelper;
}

PeerConnectionHelper::PeerConnectionHelper()
{

}

PeerConnectionHelper::~PeerConnectionHelper()
{

}

void PeerConnectionHelper::NotifyDisconnectWithRoom()
{
}

void PeerConnectionHelper::NotifyConnectionStatusChanged(RongRTCConnectionStatus out_status)
{
	if (out_status == RongRTCConnectionStatus_Connected)
	{
		m_bConnected = true;
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_CONNECT_SIGNAL_SERVER, NULL);
	}
	else
	{
		m_bConnected = false;
	}
}

void PeerConnectionHelper::NotifyReceivedMessage(const char * out_object_name, const char * out_content)
{
	std::string strObjectName = out_object_name;
	if (strObjectName.compare("SealRTC:SetRoomInfo") == 0)
	{
		std::string strContent = out_content;
		MsgItem* msg = new MsgItem(strContent);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_NOTIFY_SET_ROOM_ATTRIBUTE_VALUE, msg);
	}
}

void PeerConnectionHelper::NotifyRemoteUserJoinedRoom(const char * out_user_id)
{
	std::string strUserId = out_user_id;
	MsgItem* msg = new MsgItem(strUserId);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_USER_JOIN, msg);
}

void PeerConnectionHelper::NotifyRemoteUserLeftRoom(const char * out_user_id)
{
	std::string strUserId = out_user_id;
	MsgItem* msg = new MsgItem(strUserId);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_USER_LEFT, msg);
}

void PeerConnectionHelper::NotifyRemoteUserOffLine(const char * out_user_id)
{
	std::string strUserId = out_user_id;
	MsgItem* msg = new MsgItem(strUserId);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_USER_LEFT, msg);
}

void PeerConnectionHelper::NotifyRemoteUserPublishedStreams(const char * out_streams_json)
{
	std::string streams = out_streams_json;
	MsgItem* msg = new MsgItem(streams);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_STREAM_PUBLISH, msg);
}

void PeerConnectionHelper::NotifyRemoteUserUnPublishedStreams(const char * out_streams_json)
{
	std::string streams = out_streams_json;
	MsgItem* msg = new MsgItem(streams);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_STREAM_UNPUBLISH, msg);
}

void PeerConnectionHelper::NotifyRemoteUserChangedStreamsState(const char * out_streams_json)
{
	std::string streams = out_streams_json;
	MsgItem* msg = new MsgItem(streams);
	MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_REMOTE_STREAM_STATE_CHANGED, msg);
}

void PeerConnectionHelper::NotifyRenderVideoData(const char * out_stream_id, RongRTCVideoFrame out_video_frame)
{
	if (out_stream_id == NULL || strlen(out_stream_id) == 0 ||
		out_video_frame.iWidth == 0 || out_video_frame.iHeight == 0 ||
		out_video_frame.pVideoData == NULL || out_video_frame.iDataSize == 0)
	{
		return;
	}
	MsgItem* msg = new MsgItem();
	msg->PutBuf(out_stream_id);
	BITMAPINFO BitmapInfo;
	//::ZeroMemory(&BitmapInfo, sizeof(BitmapInfo));
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader.biWidth = out_video_frame.iWidth;
	BitmapInfo.bmiHeader.biHeight = -out_video_frame.iHeight;
	BitmapInfo.bmiHeader.biSizeImage = out_video_frame.iDataSize;
	msg->PutId((int64_t)&BitmapInfo);
	msg->PutId((int64_t)out_video_frame.pVideoData);
	::SendMessage(m_hMainWnd,WM_VIDEO_DATA, (WPARAM)msg , 0);
}

void PeerConnectionHelper::NotifyPreprocessingDefaultVideoData(RongRTCVideoFrame out_video_frame)
{
	//数据处理测试
	//memset(out_video_frame.pVideoData, 0, out_video_frame.pDataSize);
}

void PeerConnectionHelper::NotifyStatisticsData(const char * out_statistics_data_json)
{
	if (out_statistics_data_json != NULL && strlen(out_statistics_data_json) != 0)
	{
		MsgItem* pMsgItem = new MsgItem();
		if (pMsgItem)
		{
			pMsgItem->PutBuf(out_statistics_data_json);
			MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_SEND_RECV_INFO, pMsgItem);
		}
	}
}

void PeerConnectionHelper::OnCallBack(int code)
{
}

void PeerConnectionHelper::OnCallBack(int code, const char * ret)
{
}

GetRoomAttributeValueCallBack * GetRoomAttributeValueCallBack::m_pGetRoomAttributeValueCallBack = NULL;
GetRoomAttributeValueCallBack * GetRoomAttributeValueCallBack::GetInstance()
{
	if (!m_pGetRoomAttributeValueCallBack) {
		m_pGetRoomAttributeValueCallBack = new GetRoomAttributeValueCallBack();
	}
	return m_pGetRoomAttributeValueCallBack;
}

void GetRoomAttributeValueCallBack::OnCallBack(int code, const char * ret)
{
	if (code == RongRTCCodeOK)
	{
		if (ret != NULL && strlen(ret) != 0)
		{
			MsgItem* pMsgItem = new MsgItem();
			if (pMsgItem)
			{
				pMsgItem->PutBuf(ret);
				MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_GET_ROOM_ATTRIBUTE_VALUE, pMsgItem);
			}
		}
	}
}

JoinRoomCallBack* JoinRoomCallBack::m_pJoinRoomCallBack = nullptr;
JoinRoomCallBack * JoinRoomCallBack::GetInstance()
{
	if (!m_pJoinRoomCallBack) {
		m_pJoinRoomCallBack = new JoinRoomCallBack();
	}
	return m_pJoinRoomCallBack;
}

void JoinRoomCallBack::OnCallBack(int code, const char * ret)
{
	if (code == RongRTCCodeOK)
	{
		std::string users_streams = ret;
		MsgItem* msg = new MsgItem(users_streams);
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_JOIN_ROOM_OK, msg);
	}
}

LeaveRoomCallBack* LeaveRoomCallBack::m_pLeaveRoomCallBack = nullptr;
LeaveRoomCallBack * LeaveRoomCallBack::GetInstance()
{
	if (!m_pLeaveRoomCallBack) {
		m_pLeaveRoomCallBack = new LeaveRoomCallBack();
	}
	return m_pLeaveRoomCallBack;
}

void LeaveRoomCallBack::OnCallBack(int code)
{
	if (code == RongRTCCodeOK)
	{
		MsgBroadcast::GetMsgBroadcast()->SendMsg(BDT_LEAVE_ROOM_OK, NULL);
	}
}
