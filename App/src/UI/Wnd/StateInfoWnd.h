#ifndef STATE_INFO_WND_H
#define STATE_INFO_WND_H

struct StructStateInfo
{
	StructStateInfo() :iMediaType(0), iRtt(0), iSendOrReceiveType(0), iSendOrReceiveBit(0), iSendOrReceiveBitrate(0), iSendOrReceiveFrameRate(0),
		iSendOrReceiveFrameWidth(0), iSendOrReceiveFrameHeight(0), iSendOrReceivePackets(0), iSendOrReceiveLostPackets(0), iSendOrReceiveLostPacketPercent(0) {}
	std::string strId;
	int iMediaType; //0 视频 1 音频
	std::string strCodecName;
	int iRtt;
	int iSendOrReceiveType; //0 发送 1 接受
	int iSendOrReceiveBit;
	int iSendOrReceiveBitrate; // kbps
	int iSendOrReceiveFrameRate;
	int iSendOrReceiveFrameWidth;
	int iSendOrReceiveFrameHeight;
	int iSendOrReceivePackets;
	int iSendOrReceiveLostPackets;
	int iSendOrReceiveLostPacketPercent;
};

class StateInfoWnd : public CBaseWnd
{
public:
	StateInfoWnd(void);
	virtual ~StateInfoWnd(void);

    DECL_NOTIFY_MSG()
    DECL_APPWND_CONTROL()

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual void OnFinalMessage(HWND hWnd);	
	virtual LPCTSTR GetWindowClassName() const;
	virtual void PreCreateWindowInit(const CStdStringPtrMap& aInitData);
	
public:
	void OnWinInit(TNotifyUI& msg);

private:
	void InitControl();
	void PaserStateInfo(std::string strJson);
	void UpdateStateInfo(std::vector<StructStateInfo*> vectorStructStateInfo);
	void UpdateTotalStateInfo(int iTotalSendBitrate, int iTotalReceiveBitrate, int iAudioRtt, int iVideoRtt);

};

#endif