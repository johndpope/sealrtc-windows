#include "StdAfx.h"
#include "StateInfoWnd.h"
#include "WndHelper.h"
#include "PeerConnectionHelper.h"
#include "Integer.h"
#include "..\jsoncpp\include\json\json.h"
#include "UpStringUtility.h"

IMPL_APPWND_CONTROL(StateInfoWnd, ID_WND_STATE_INFO)

StateInfoWnd::StateInfoWnd(void)
{
	SetSkinFile(_T("state_info.xml"));
}

StateInfoWnd::~StateInfoWnd(void)
{
}

NOTIFY_BEGIN(StateInfoWnd)
NOTIFY_END()

LRESULT StateInfoWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg)
	{
	case WM_UP_BROADCAST:
	{
		switch (wParam)
		{
		case BDT_SEND_RECV_INFO:
		{
			MsgItem* pMsgItem = (MsgItem*)lParam;
			if (pMsgItem)
			{
				//paser json
				PaserStateInfo(pMsgItem->GetBuf(0));
			}
			break;
		}
		default:
			break;
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

void StateInfoWnd::OnFinalMessage(HWND hWnd)
{
	__super::OnFinalMessage(hWnd);
	delete this;
}

LPCTSTR StateInfoWnd::GetWindowClassName() const
{
	return _T("StateInfoWnd");
}

void StateInfoWnd::PreCreateWindowInit(const CStdStringPtrMap & aInitData)
{

}

void StateInfoWnd::OnWinInit(TNotifyUI& msg)
{
	__super::OnWinInit(msg);
	InitControl();
}

void StateInfoWnd::InitControl()
{
	
}

void StateInfoWnd::PaserStateInfo(std::string strJson)
{
	Json::Reader reader;
	Json::Value jmessage;
	if (reader.parse(strJson, jmessage))
	{
		CListUI* pNetworkList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("network_list")));

		int iSize = jmessage.size();
		if (iSize > 0)
		{
			int iTotalSendBitrate = 0;
			int iTotalReceiveBitrate = 0;
			int iAudioRtt = 0;
			int iVideoRtt = 0;
			std::vector<StructStateInfo*> vectorStructStateInfo;
			for (int i = 0; i < iSize; ++i)
			{
				StructStateInfo* pStructStateInfo = new StructStateInfo();
				pStructStateInfo->strId = jmessage[i]["id"].asString();
				pStructStateInfo->iMediaType = jmessage[i]["media_type"].asInt(); //0 视频 1 音频
				pStructStateInfo->strCodecName = jmessage[i]["codec_name"].asString();
				pStructStateInfo->iRtt = jmessage[i]["rtt"].asInt();
				pStructStateInfo->iSendOrReceiveType = jmessage[i]["send_receive_type"].asInt(); //0 发送 1 接受
				pStructStateInfo->iSendOrReceiveBitrate = jmessage[i]["send_receive_bitrate"].asInt(); // kbps
				pStructStateInfo->iSendOrReceiveFrameRate = jmessage[i]["send_receive_frame_rate"].asInt();
				pStructStateInfo->iSendOrReceiveFrameWidth = jmessage[i]["send_receive_frame_width"].asInt();
				pStructStateInfo->iSendOrReceiveFrameHeight = jmessage[i]["send_receive_frame_height"].asInt();
				pStructStateInfo->iSendOrReceivePackets = jmessage[i]["send_receive_packets"].asInt();
				pStructStateInfo->iSendOrReceiveLostPackets = jmessage[i]["send_receive_lost_packets"].asInt();
				pStructStateInfo->iSendOrReceiveLostPacketPercent = jmessage[i]["send_receive_lost_packets_percent"].asInt();
				if (pStructStateInfo->strId.find(PeerConnectionHelper::m_strUserId) != std::string::npos)
				{
					iTotalSendBitrate += pStructStateInfo->iSendOrReceiveBitrate;
					if (pStructStateInfo->iMediaType == 0)
					{
						if (iAudioRtt == 0 || iAudioRtt > pStructStateInfo->iRtt)
						{
							iAudioRtt = pStructStateInfo->iRtt;
						}
					}
					else
					{
						if (iVideoRtt == 0 || iVideoRtt > pStructStateInfo->iRtt)
						{
							iVideoRtt = pStructStateInfo->iRtt;
						}
					}
				}
				else
				{
					iTotalReceiveBitrate += pStructStateInfo->iSendOrReceiveBitrate;
				}
				vectorStructStateInfo.push_back(pStructStateInfo);
			}
			UpdateStateInfo(vectorStructStateInfo);
			UpdateTotalStateInfo(iTotalSendBitrate, iTotalReceiveBitrate, iAudioRtt, iVideoRtt);
		}
	}
}

void StateInfoWnd::UpdateStateInfo(std::vector<StructStateInfo*> vectorStructStateInfo)
{
	CVerticalLayoutUI* pDataLayout = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("data_layout")));
	if (pDataLayout)
	{
		int iStateCount = vectorStructStateInfo.size();
		int iCount = pDataLayout->GetCount();
		for (int i = 1; i < iCount; ++i)
		{
			CHorizontalLayoutUI* pDataItemLayout = static_cast<CHorizontalLayoutUI*>(pDataLayout->GetItemAt(i));

			CLabelUI* pUserId = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_user_id")));
			CLabelUI* pName = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_name")));
			CLabelUI* pCodec = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_codec")));
			CLabelUI* pSolution = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_solution")));
			CLabelUI* pFrameRate = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_fram_rate")));
			CLabelUI* pBitrate = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_bit_rate")));
			CLabelUI* pLostPacketPercent = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pDataItemLayout, _T("data_item_lost_pecket_percent")));

			if (pUserId && pName && pCodec && pSolution && pFrameRate && pBitrate && pLostPacketPercent)
			{
				if (i <= iStateCount)
				{
					//更新
					if (vectorStructStateInfo[i-1]->strId.compare(PeerConnectionHelper::m_strUserId) == 0)
					{
						pUserId->SetText(_T("本地"));
					}
					else if (vectorStructStateInfo[i - 1]->strId.compare(PeerConnectionHelper::m_strUserId + "_tiny") == 0)
					{
						pUserId->SetText(_T("本地小流"));
					}
					else if (vectorStructStateInfo[i - 1]->strId.compare(PeerConnectionHelper::m_strUserId + "_screen") == 0)
					{
						pUserId->SetText(_T("本地屏幕共享"));
					}
					else
					{
						pUserId->SetText(UpStringUtility::StringToWstring(vectorStructStateInfo[i-1]->strId).c_str());
					}
					if (vectorStructStateInfo[i-1]->iSendOrReceiveType == 0)
					{
						if (vectorStructStateInfo[i-1]->iMediaType == 0)
						{
							pName->SetText(_T("视频发送"));
						}
						else
						{
							pName->SetText(_T("音频发送"));
						}
					}
					else
					{
						if (vectorStructStateInfo[i-1]->iMediaType == 0)
						{
							pName->SetText(_T("视频接收"));
						}
						else
						{
							pName->SetText(_T("音频接收"));
						}
					}
					pCodec->SetText(UpStringUtility::StringToWstring(vectorStructStateInfo[i-1]->strCodecName).c_str());
					if (vectorStructStateInfo[i - 1]->iMediaType == 0)
					{
						std::wstring strSolution;
						strSolution.append(up_utility::Integer::ToWString(vectorStructStateInfo[i - 1]->iSendOrReceiveFrameWidth).c_str());
						strSolution.append(L"*");
						strSolution.append(up_utility::Integer::ToWString(vectorStructStateInfo[i - 1]->iSendOrReceiveFrameHeight).c_str());
						pSolution->SetText(strSolution.c_str());
						pFrameRate->SetText(up_utility::Integer::ToWString(vectorStructStateInfo[i - 1]->iSendOrReceiveFrameRate).c_str());
					}
					else
					{
						pSolution->SetText(L"--");
						pFrameRate->SetText(L"--");
					}
					
					pBitrate->SetText(up_utility::Integer::ToWString(vectorStructStateInfo[i-1]->iSendOrReceiveBitrate).c_str());
					pLostPacketPercent->SetText(up_utility::Integer::ToWString(vectorStructStateInfo[i-1]->iSendOrReceiveLostPacketPercent).c_str());

					delete vectorStructStateInfo[i-1];
				}
				else
				{
					//清空
					pUserId->SetText(_T("--"));
					pName->SetText(_T("--"));
					pCodec->SetText(_T("--"));
					pSolution->SetText(_T("--"));
					pFrameRate->SetText(_T("--"));
					pBitrate->SetText(_T("--"));
					pLostPacketPercent->SetText(_T("--"));
				}
			}
		}
		vectorStructStateInfo.clear();
	}
}

void StateInfoWnd::UpdateTotalStateInfo(int iTotalSendBitrate, int iTotalReceiveBitrate, int iAudioRtt, int iVideoRtt)
{
	CLabelUI* pTotalSendBitrate = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("total_send_bitrate")));
	CLabelUI* pSendRtt = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("send_rtt")));
	CLabelUI* pTotalReceiveBitrate = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("total_receive_bitrate")));
	if (pTotalSendBitrate && pSendRtt && pTotalReceiveBitrate)
	{
		pTotalSendBitrate->SetText(up_utility::Integer::ToWString(iTotalSendBitrate).c_str());
		pTotalReceiveBitrate->SetText(up_utility::Integer::ToWString(iTotalReceiveBitrate).c_str());
		std::wstring strRtt = up_utility::Integer::ToWString(iAudioRtt);
		strRtt.append(L"/");
		strRtt.append(up_utility::Integer::ToWString(iVideoRtt));
		pSendRtt->SetText(strRtt.c_str());
	}
}
