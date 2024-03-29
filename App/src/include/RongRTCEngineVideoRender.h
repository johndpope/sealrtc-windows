#pragma once
#include <Windows.h>

//视频绘制接口
class IVideoRender
{
public:

	/*
	函数名：OnFrame
	参数：pBitmapInfo，为BITMAPINFO*类型，描述Bitmap信息，pImage，为unsigned char*类型，Bitmap数据
	返回值：
	说明：本地调用者需要实现该接口类，并通过RongRTCEngineInterface.h中RongRTCEngineObserver::OnStartLocalRender或者RongRTCEngineObserver::OnStartRemoteRender
			的返回值注册到RongRTCEngine中，当有视频数据时会通过该方法返回给本地调用者每一帧数据，本地调用者可以对视频数据进行个性化绘制或者其他操作
	*/
	virtual void OnFrame(BITMAPINFO* pBitmapInfo, unsigned char* pImage) = 0;
};

