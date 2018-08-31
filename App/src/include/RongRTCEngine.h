#pragma once

#define PC_DLL_EXPORTS

#ifdef PC_DLL_EXPORTS
#define PC_DLL_API __declspec(dllexport)
#else
#define PC_DLL_API __declspec(dllimport)
#endif
class IVideoRender;

//控制动作
enum PC_DLL_API ControlAction : int
{
	//打开
	ControlAction_Open = 0x01,
	//关闭
	ControlAction_Close = 0x02,
};

//被控制的媒体类型
enum PC_DLL_API ControlMediaType : int
{
	//视频
	ControlMediaType_Video = 0x01,
	//音频
	ControlMediaType_Audio = 0x02,
	//音视频
	ControlMediaType_AV = 0x03,
};

//UserRoleType
enum PC_DLL_API UserRoleType : int
{
	//正常身份用户
	UserRoleType_Normal = 1,
	//观察者身份用户
	UserRoleType_Observer = 2,
	//主持人身份用户
	UserRoleType_Host = 3,
};

//UserMediaType
enum PC_DLL_API UserMediaType : int
{
	//音频
	UserMediaType_Audio = 0,
	//音视频
	UserMediaType_Audio_Video = 1,
	//视频
	UserMediaType_Video = 2,
	//无音视频
	UserMediaType_None = 3,
};

//操作类型
enum PC_DLL_API OperationType : int
{
	//接受
	OperationType_Accept = 0x01,
	//拒绝
	OperationType_Refuse= 0x02,
	//忙碌
	OperationType_Busy = 0x04,
};

//接口回调函数
class PC_DLL_API RongRTCEngineObserver
{

public:
	//JoinChannelFailedType
	enum JoinChannelFailedType : int
	{
		//加入房间失败
		JoinChannelFailedType_Failed = 0,
		//加入房间被拒绝
		JoinChannelFailedType_Reject = 1,
	};

public:
	/*
	函数名：OnJoinChannelOK
	参数：enumUserRoleType，加入房间成功时返回用户身份
	返回值：
	说明：当本地调用者加入房间成功时，会回调通知加入房间成功
	*/
	virtual void OnJoinChannelOK(UserRoleType enumUserRoleType) = 0;

	/*
	函数名：OnJoinChannelFailed
	参数：enumJoinChannelFailedType，加入房间失败类型
	返回值：
	说明：当本地调用者加入房间失败时，会回调通知加入房间失败
	*/
	virtual void OnJoinChannelFailed(JoinChannelFailedType enumJoinChannelFailedType) = 0;

	/*
	函数名：OnLeaveChannelOK
	参数：
	返回值：
	说明：当本地调用者退出房间成功时，会回调通知退出房间成功
	*/
	virtual void OnLeaveChannelOK() = 0;

	/*
	函数名：OnDisconnectWhitChannel
	参数：
	返回值：
	说明：当与房间断开连接时，会回调通知
	*/
	virtual void OnDisconnectWhitChannel() = 0;

	/*
	函数名：OnStartLocalAudioVideoOK
	参数：enumUserMediaType，用户媒体类型
	返回值：IVideoRender*类型绘制接口对象
	说明：当本地调用者开启本地音视频，通知调用者开启本地音视频成功，需要创建本地视频绘制对象，并通过返回值返回给Engine
	*/
	virtual IVideoRender* OnStartLocalAudioVideoOK(UserMediaType enumUserMediaType) = 0;

	/*
	函数名：OnNotifyLocalAudioVideoStop
	参数：
	返回值：
	说明：当本地调用者退出房间，通知调用者Engine已经停止本地音视频，调用者可以进行视频绘制对象资源释放
	*/
	virtual void OnNotifyLocalAudioVideoStop() = 0;

	/*
	函数名：OnNotifyUserVideoCreated
	参数：strUserID，加入房间后，已经创建视频流的用户ID；enumUserMediaType，用户媒体类型
	返回值：IVideoRender*类型绘制接口对象
	说明：当远端用户加入房间后创建了视频传输流，如果用户类型enumUserRoleType为UserRoleType_Observer（观察者），
			不需要调用者创建IVideoRender*类型的绘制对象
	*/
	virtual IVideoRender* OnNotifyUserVideoCreated(const char* strUserID, UserMediaType enumUserMediaType = UserMediaType_None) = 0;

	/*
	函数名：OnNotifyUserJoinChannel
	参数：strUserID，加入房间的用户ID；strName，用户名；enumUserRoleType，用户角色类型（正常，观察者，主持人）；enumUserMediaType，用户媒体类型
	返回值：IVideoRender*类型绘制接口对象
	说明：当有用户加入房间，其他入会人员会收到此通知
	*/
	virtual void OnNotifyUserJoinChannel(const char* strUserID, const char* strName, UserRoleType enumUserRoleType, UserMediaType enumUserMediaType = UserMediaType_None) = 0;

	/*
	函数名：OnNotifyUserLeaveChannel
	参数：strUserID，离开房间的用户ID；bIsObserver，是否为观察者
	返回值：
	说明：当远端用户离开房间，调用者通知会收到此通知
	*/
	virtual void OnNotifyUserLeaveChannel(const char* strUserID, bool bIsObserver = false) = 0;

	/*
	函数名：OnNotifyUserVideoDestroyed
	参数：strUserID，离开房间的用户ID
	返回值：
	说明：当远端用户离开房间后销毁视频流
	*/
	virtual void OnNotifyUserVideoDestroyed(const char* strUserID) = 0;

	/*
	函数名：OnUserMediaChanged
	参数：strUserID，媒体类型改变的用户ID；enumUserMediaType，用户媒体类型改变之后的媒体类型
	返回值：
	说明：当远端用户改变媒体类型，通知其他入会人员
	*/
	virtual void OnUserMediaChanged(const char* strUserID, UserMediaType enumUserMediaType) = 0;

	/*
	函数名：OnUserOpenOrCloseScreenShare
	参数：strUserID，打开或者关闭屏幕共享的用户ID；bOpen，true为打开屏幕共享，false为关闭屏幕共享
	返回值：
	说明：当用户打开或者关闭屏幕共享，通知其他入会人员
	*/
	virtual void OnUserOpenOrCloseScreenShare(const char* strUserID, bool bOpen) = 0;

	/*
	函数名：OnUserCreateWB
	参数：strUserID，创建白板的用户ID
	返回值：
	说明：当用户创建白板，通知其他入会人员
	*/
	virtual void OnUserCreateWB(const char* strUserID) = 0;

	/*
	函数名：OnGetEWBServiceUrl()
	参数：strUrl，白板Url
	返回值：
	说明：当调用GetEWBServiceUrl()接口，获取白板地址成功，通知调用者
	*/
	virtual void OnGetEWBServiceUrl(const char* strUrl) = 0;

	/*
	函数名：OnQueryEWBServiceUrl()
	参数：strUrl，白板Url
	返回值：
	说明：当调用QueryEWBServiceUrl()接口，查询白板是否创建，如果存在，则返回白板地址，如果失败，则返回空，通知调用者
	*/
	virtual void OnQueryEWBServiceUrl(const char* strUrl) = 0;

	/*
	函数名：OnGetSendRecvStatisticsData
	参数：iSendLostPacketPercent，发送丢包率，iReceiveLostPacketPercent，接受丢包率
	返回值：
	说明：返回Engine发送和接受的丢包率的百分比的整数值，例如30%丢包返回整数30
	*/
	virtual void OnGetSendRecvStatisticsData(unsigned __int64 iSendLostPacketPercent, unsigned __int64 iReceiveLostPacketPercent) = 0;

	/*
	函数名：OnNotifyUserAudioLevel
	参数：strJson，当前入会人员用户ID，声音等级的json字符串
	返回值：
	说明：声音等级大于0，表示当前正在说话
	*/
	virtual void OnNotifyUserAudioLevel(const char* strJson) = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		1.观察者请求发言调用ObserverRequestBecomeNormalUser()，收到OnObserverRequestBecomeNormalUser()反馈接口调用是否成功；
		2.主持人收到请求通知OnNotifyObserverRequestBecomeNormalUser()；
		3.主持人调用接口HostDoObserverRequestBecomeNormalUser()对发言请求进行处理，收到OnHostDoObserverRequestBecomeNormalUser()反馈接口调用是否成功；
		4.当主持人同意请求发言时，请求发言的观察者与其他与会人员都会收到通知OnNotifyHostDoObserverRequestBecomeNormalUser()，
		   当主持人拒绝请求发言时，只有请求发言的观察者会收到通知OnNotifyHostDoObserverRequestBecomeNormalUser()。
	*/
	/*
	函数名：OnObserverRequestBecomeNormalUser
	参数：bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：当有观察者身份的用户调用接口ObserverRequestBecomeNormalUser()，观察者会收到通知
	*/
	virtual void OnObserverRequestBecomeNormalUser(bool bSuccessed) = 0;

	/*
	函数名：OnNotifyObserverRequestBecomeNormalUser
	参数：strUserID，请求发言的用户ID
	返回值：
	说明：当有观察者身份的用户调用接口ObserverRequestBecomeNormalUser()，主持人会收到通知
	*/
	virtual void OnNotifyObserverRequestBecomeNormalUser(const char* strUserID) = 0;

	/*
	函数名：OnHostDoObserverRequestBecomeNormalUser
	参数：strUserId，主持人处理请求发言观察者的ID，bAccept，主持人操作行为，同意或者拒绝，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：当主持人调用HostDoObserverRequestBecomeNormalUser()，主持人会收到通知
	*/
	virtual void OnHostDoObserverRequestBecomeNormalUser(const char* strUserId, bool bAccept, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyHostDoObserverRequestBecomeNormalUser
	参数：strUserID，请求发言的用户ID，enumOperationType，主持人操作行为，同意，拒绝，或者忙碌
	返回值：
	说明：当主持人针对请求进行操作之后，用户收到通知，如果strUserID与自己ID相同，说明自己是观察者，请求发言，收到主持人处理结果
			如果strUserID与自己ID不相同，说明是其他用户（观察者），请求发言，收到主持人对请求发言的处理结果
	*/
	virtual void OnNotifyHostDoObserverRequestBecomeNormalUser(const char* strUserID, OperationType enumEngineOperationType) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		1.主持人邀请观察者发言调用HostRequestUpgradeObserverToNormalUser()，收到OnHostRequestUpgradeObserverToNormalUser()反馈接口调用是否成功；
		2.被邀请观察者收到邀请通知OnNotifyHostRequestUpgradeObserverToNormalUser()；
		3.被邀请观察者调用接口OnObserverDoHostInvite()对邀请进行处理，收到OnObserverDoHostInvite()反馈接口调用是否成功；
		4.当被邀请观察者同意邀请发言时，主持人与其他与会人员都会收到通知OnNotifyObserverDoHostInvite()，
		   当被邀请观察者拒绝邀请发言时，只有主持人会收到通知OnNotifyHostDoObserverRequestSpeak()。
	*/
	/*
	函数名：OnHostRequestUpgradeObserverToNormalUser
	参数：strUserId，被操作观察者的用户ID，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：主持人调用HostRequestUpgradeObserverToNormalUser()邀请观察者发言，主持人会收到通知
	*/
	virtual void OnHostRequestUpgradeObserverToNormalUser(const char* strUserId, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyHostRequestUpgradeObserverToNormalUser
	参数：strHostID，主持人ID，strUserID，被操作用户ID
	返回值：
	说明：主持人调用HostRequestUpgradeObserverToNormalUser()邀请观察者发言，观察者会收到此通知
	*/
	virtual void OnNotifyHostRequestUpgradeObserverToNormalUser(const char* strHostID) = 0;

	/*
	函数名：OnObserverDoHostRequestUpgradeObserverToNormalUser
	参数：strHostId，主持人用户ID，bAccpet，返回用户接受或者拒绝，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：当观察者调用ObserverDoHostRequestUpgradeObserverToNormalUser()接口之后，观察者会收到调用接口结果
	*/
	virtual void OnObserverDoHostRequestUpgradeObserverToNormalUser(const char* strHostId, bool bAccpet, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyObserverDoHostRequestUpgradeObserverToNormalUser
	参数：strUserId，被邀请人员的用户ID，bAccpet，返回用户接受或者拒绝
	返回值：
	说明：当观察者被主持请求升级为正常用户，调用ObserverDoHostRequestUpgradeObserverToNormalUser()处理主持人邀请之后，
			与会人员会收到OnNotifyObserverDoHostRequestUpgradeObserverToNormalUser通知，返回观察者处理邀请的结果
	*/
	virtual void OnNotifyObserverDoHostRequestUpgradeObserverToNormalUser(const char* strUserId, bool bAccpet) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
	1.主持人请求降级正常用户为观察者调用HostRequestDegradeNormalUserToObserver()，收到OnHostRequestDegradeNormalUserToObserver()反馈接口调用是否成功；
	2.被请求正常用户收到请求通知OnNotifyHostRequestDegradeNormalUserToObserver()；
	3.被请求正常用户收到请求，调用接口NoramlUserDoHostRequestDegradeNormalUserToObserver()对请求进行处理，
		收到OnNoramlUserDoHostRequestDegradeNormalUserToObserver()反馈接口调用是否成功；
	4.当被请求用户同意请求时，主持人与其他与会人员都会收到通知OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver()，
		当被请求用户拒绝邀请时，只有主持人会收到通知OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver()。
	*/

	/*
	函数名：OnHostRequestDegradeNormalUserToObserver
	参数：strUserID，被操作用户ID，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：当主持人调用HostRequestDegradeNormalUserToObserver()请求降级正常用户为观察者时，会收到调用结果
	*/
	virtual void OnHostRequestDegradeNormalUserToObserver(const char* strUserID, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyHostRequestDegradeNormalUserToObserver
	参数：strHostID，主持人用户ID
	返回值：
	说明：当用户被请求降为观察者，入会人员会收到此通知
	*/
	virtual void OnNotifyHostRequestDegradeNormalUserToObserver(const char* strHostID) = 0;

	/*
	函数名：OnNoramlUserDoHostRequestDegradeNormalUserToObserver
	参数：strHostID，主持人用户ID，bAccpet，返回用户接受或者拒绝，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：用户调用NoramlUserDoHostRequestDegradeNormalUserToObserver()来处理主持人降级请求，会收到通知
	*/
	virtual void OnNoramlUserDoHostRequestDegradeNormalUserToObserver(const char* strHostID, bool bAccpet, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver
	参数：strUserId，被请求人员的用户ID，bAccpet，返回用户接受或者拒绝
	返回值：
	说明：当正常用户被主持请求升级为观察者，调用正常用户调用NoramlUserDoHostRequestDegradeNormalUserToObserver()处理主持人邀请之后，
			与会人员会收到OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver通知，返回正常用户处理请求的结果
	*/
	virtual void OnNotifyNoramlUserDoHostRequestDegradeNormalUserToObserver(const char* strUserId, bool bAccpet) = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*
		1.主持人通过调用HostRequestControlUserDevice()对与会人员媒体类型（音频打开或关闭，视频打开或关闭）进行操作，收到OnHostRequestControlUserDevice()反馈接口调用是否成功；
		2.被操作的与会人员收到请求操作通知OnNotifyHostRequestControlUserDevice()；
		3.被操作的与会人员调用UserDoHostRequestControlUserDevice()进行确认是否接受操作请求，会收到OnUserDoHostRequestControlUserDevice()反馈接口调用是否成功；
		4.被操作的与会人员同意打开或者关闭时，主持人与其他与会人员都会收到通知OnNotifyUserDoHostRequestControlUserDevice()，
		   被操作的与会人员拒绝打开或者关闭时，主持人会收到通知OnNotifyUserDoHostRequestControlUserDevice()。
	*/
	/*
	函数名：OnHostRequestControlUserDevice
	参数：strUserId，被操作人员的用户ID，enumControlAction，操作类型，关闭或者打开，
			enumControlMediaType，操作的媒体类型，bSuccessed，返回接口调用结果
	返回值：
	说明：当主持人调用接口HostManageMeetingMember()，主持人会收到调用接口的结果
	*/
	virtual void OnHostRequestControlUserDevice(const char* strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyHostRequestControlUserDevice
	参数：strHostId，主持人的用户ID，enumControlAction，操作类型，关闭或者打开，enumControlMediaType，操作的媒体类型
	返回值：
	说明：主持人请求打开关闭音、视频，被请求人会收到请求的通知，被操作人员可以进行同意或者拒绝操作
	*/
	virtual void OnNotifyHostRequestControlUserDevice(const char* strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType) = 0;
	
	/*
	函数名：OnUserDoHostRequestControlUserDevice
	参数：strHostId，主持人的用户ID，enumControlAction，操作类型，关闭或者打开，enumControlMediaType，操作的媒体类型，bAccept，是否同意打开音、视频或者音视频，bSuccessed，返回接口调用结果，enumMediaType，用户现在音视频类型
	返回值：
	说明：当用户调用UserDoHostRequestControlUserDevice()接口，处理主持人请求打开或关闭自己音视频的开关，会收到调用接口的结果
	*/
	virtual void OnUserDoHostRequestControlUserDevice(const char* strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccept, bool bSuccessed, UserMediaType enumMediaType) = 0;

	/*
	函数名：OnNotifyUserDoHostRequestControlUserDevice
	参数：strUserId，被操作人员的用户ID，enumControlAction，操作类型，关闭或者打开
	enumControlMediaType，操作的媒体类型，bAccpet，返回用户接受或者拒绝，enumMediaType，用户现在音视频类型
	返回值：
	说明：当被操作用户调用UserDoHostRequestControlUserDevice()处理主持人请求之后，与会人员会收到OnNotifyUserDoHostRequest()通知，返回被操作人员处理请求的结果
	*/
	virtual void OnNotifyUserDoHostRequestControlUserDevice(const char* strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccpet, UserMediaType enumMediaType) = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	/*
	函数名：OnHostReomveMemberOrObserver
	参数：strUserId，被操作人员的用户ID，bSuccessed，返回操作结果，成功或者失败
	返回值：
	说明：主持人调用HostReomveMemberOrObserver()将用户踢出房间，主持人会收到调用结果
	*/
	virtual void OnHostReomveMemberOrObserver(const char* strUserId, bool bSuccessed) = 0;

	/*
	函数名：OnNotifyHostReomveMemberOrObserver
	参数：strUserID，被操作用户ID
	返回值：
	说明：当被操作用户被踢出房间时，被操作用户会收到此通知
	*/
	virtual void OnNotifyHostReomveMemberOrObserver(const char* strUserID) = 0;

	/*
	函数名：OnGetHostPower
	参数：bSuccessed，返回操作结果
	返回值：
	说明：当有用户调用接口GetHostPower()，会收到通知
	*/
	virtual void OnGetHostPower(bool bSuccessed) = 0;

	/*
	函数名：OnNotifyGetHostPower
	参数：strUserID，请求获取主持人权限的用户ID
	返回值：
	说明：当有用户调用接口GetHostPower()获取主持人权限成功，其他与会人员会收到通知
	*/
	virtual void OnNotifyGetHostPower(const char* strUserID) = 0;

	/*
	函数名：OnGetInviteUrl
	参数：strUrl，邀请链接地址，bSuccessed，获取邀请链接成功还是失败
	返回值：
	说明：当用户调用接口GetInviteUrl()后，会收到该回调，如果bSuccessed为true，则strUrl有效，说明获取邀请链接成功
			如果bSuccessed为false，则strUrl无效，为空，说明获取邀请链接失败
	*/
	virtual void OnGetInviteUrl(const char* strUrl, bool bSuccessed) = 0;
};

//Engine接口返回值类型
enum EngineErrorType : int
{
	//操作成功
	EngineErrorType_Success = 0,
	//操作失败
	EngineErrorType_Failed = 1,
	//没有找到摄像头
	EngineErrorType_CaptureNotFound = 2,
	//Engine没有初始化
	EngineErrorType_EngineNotInit = 3,
	//Engine已经初始化过
	EngineErrorType_EngineHaveInit = 4,
	//参数无效
	EngineErrorType_InvalidArgument = 5,
	//接口无效
	EngineErrorType_InterfaceInvalid = 6,
	//操作太频繁
	EngineErrorType_InterfaceTooFrequentOperation = 6,
};

//Engine接口
class PC_DLL_API RongRTCEngine
{
private:
	RongRTCEngine();
	~RongRTCEngine();

public: 

	//日志级别
	enum EngineLogLevel
	{
		//关闭日志
		Engine_LL_NONE,
		//信息级别日志
		Engine_LL_INFO,
		//警告级别日志
		Engine_LL_WARNING,
		//错误级别日志
		Engine_LL_ERROR,
	};

	/*
	函数名：SetLogEnabled
	参数：enumLogLevel，日志级别，strOutputDir，本地日志输出目录
	返回值：
	说明：日志系统作为查找问题辅助功能，如果需要开启日志功能，需要在Engine初始化之前开启，Engine一次生命周期只能设置一次，
			本地日志功能默认关闭，当参数enumLogLevel为Engine_LL_INFO（信息级别日志 ）、Engine_LL_WARNING（警告级别日志）或者
			Engine_LL_ERROR（错误级别日志）时，日志功能被打开，参数strOutputFile作为输出日志文件而起作用，当enumLogLevel为
			Engine_LL_NONE（关闭日志）时，日志功能被关闭，参数strOutputFile被忽略
	*/
	static EngineErrorType SetLogEnabled(EngineLogLevel enumLogLevel, const char* strOutputFile);


	//视频分辨率和帧率
	enum VideoResolutionProfile : int
	{
		//分辨率256*144，帧率15
		VideoProfile_256_144_15F = 1,
		//分辨率320*240，帧率15
		VideoProfile_320_240_15F = 2,
		//分辨率480*360，帧率15
		VideoProfile_480_360_15F = 3,
		//分辨率640*360，帧率15
		VideoProfile_640_360_15F = 4,
		//分辨率640*480，帧率15
		VideoProfile_640_480_15F = 5,
		//分辨率720*480，帧率15
		VideoProfile_720_480_15F = 6,
		//分辨率1280*720，帧率15
		VideoProfile_1280_720_15F = 7,
		//分辨率1920*1080，帧率15
		VideoProfile_1920_1080_15F = 8,
	};

	/*
	函数名：SelectResolution
	参数：enumVideoResolutionProfile为VideoResolutionProfile类型枚举
	返回值：
	说明：选择分辨率和码率
	*/
	static EngineErrorType SelectResolution(VideoResolutionProfile enumVideoResolutionProfile);

	/*
	函数名：GetSDKVersion
	参数：strSDKVersion，SDK版本号
	返回值：
	说明：获取SDK版本号，版本号长度不大于128个char
	*/
	static EngineErrorType GetSDKVersion(char* strSDKVersion);

	/*
	函数名：Init
	参数：strUserId，用户ID；
			strCmpIp，CMP服务地址；strCmpCrt，Tls模式下的证书；iCmpCrtLen，证书字符串的长度
			pEngineObserver，用户实现RongRTCEngineObserver，作为回调传入Engine
	返回值：
	说明：初始化RongRTCEngine，如果没有设置CMP服务IP地址，则默认阿里云生产地址
	*/
	static EngineErrorType Init(const char* strUserId, const char* strCmpIp, const char* strCmpCrt, int iCmpCrtLen, RongRTCEngineObserver* pEngineObserver);

	/*
	函数名：ReSetServerConfig
	参数：strCmpIp，CMP服务地址；strCmpCrt，Tls模式下的证书；iCmpCrtLen，证书字符串的长度
	返回值：
	说明：重新设置Engine的服务器地址
	*/
	static EngineErrorType ReSetServerConfig(const char* strCmpIp, const char* strCmpCrt, int iCmpCrtLen);

	/*
	函数名：UnInit
	参数：
	返回值：
	说明：释放Engine资源
	*/
	static EngineErrorType UnInit();

	/*以下接口需调用Init之后调用*/

	/*
	函数名：JoinChannel
	参数：strChannelId，房间号；strName，用户名；strToken ，用户令牌（用于验证用户身份的字符串）；bIsObserver ，是否为观察者身份；iMediaType，音视频类型
	返回值：
	说明：加入房间接口，需要调用LogonServer接口回调返回成功之后再调用，加入房间成功通过回调
			RongRTCEngineObserver::OnJoinChannelOK返回，加入房间失败通过回调RongRTCEngineObserver::OnJoinChannelFailed返回，
			当bIsObserver = true时，iMediaType不起作用，当bIsObserver = false，iMediaType暂时不支持UserMediaType_Video（纯视频）和UserMediaType_None（无音视频）
	*/
	static EngineErrorType JoinChannel(const char* strChannelId, const char* strName, const char* strToken, bool bIsObserver = false, UserMediaType iMediaType = UserMediaType_Audio_Video);

	/*
	函数名：GetUserRoleType
	参数：enumUserRoleType，用户角色类型
	返回值：
	说明：用户加入会议室后，可以通过该接口获取用户角色类型
	*/
	static EngineErrorType GetUserRoleType(UserRoleType& enumUserRoleType);

	/*
	函数名：LeaveChannel
	参数：
	返回值：
	说明：离开房间接口，需要调用JoinChannel接口回调返回成功之后再调用，离开房间成功通过回调
	RongRTCEngineObserver::OnLeaveChannelOK返回，离开房间失败通过回调RongRTCEngineObserver::OnLeaveChannelFailed返回
	*/
	static EngineErrorType LeaveChannel();

	/*
	函数名：StartLocalAudioVideo
	参数：
	返回值：
	说明：开启本地音视频，即开启默认摄像头和麦克风
	*/
	static EngineErrorType StartLocalAudioVideo();

	/*
	函数名：StartOrStopScreenShare
	参数：bStart，是否开启屏幕分享，iLeft，iTop，iRight，iBottom，为开启屏幕分享时以屏幕左上角（0，0）坐标为参考点的矩形区域
	返回值：
	说明：当bStart为true时，开启屏幕分享，iLeft，iTop，iRight，iBottom参数有效，但需要满足以下条件：
				1）矩形区域需在屏幕内；
				2）矩形区域最小分辨率为32*32，即iRight - iLeft大于32，iBottom - iTop大于32；
				3）矩形区域最大分辨率为1280*720或者720*1280；
			 当bStart为false时，关闭屏幕分享，且iLeft，iTop，iRight，iBottom参数无效
	*/
	static EngineErrorType StartOrStopScreenShare(bool bStart, int iLeft = 0, int iTop = 0, int iRight = 0, int iBottom = 0);

	/*
	函数名：SetAudioEnabled
	参数：bEnable，当为true时表示打开，当为false时表示关闭
	返回值：
	说明：设置音频是否开启，即是否打开/关闭音频流
	*/
	static EngineErrorType SetAudioEnabled(bool bEnable);

	/*
	函数名：SetVideoEnabled
	参数：bEnable，当为true时表示打开，当为false时表示关闭
	返回值：
	说明：设置视频是否开启，即是否打开/关闭视频流
	*/
	static EngineErrorType SetVideoEnabled(bool bEnable);

	/*
	函数名：SetSpeakerEnabled
	参数：bEnable，当为true时表示打开，当为false时表示关闭
	返回值：
	说明：设置扬声器是否开启，即是否打开/关闭扬声器
	*/
	static EngineErrorType SetSpeakerEnabled(bool bEnable);

	/*
	函数名：SetBeautyEnabled
	参数：bEnable，当为true时表示打开，当为false时表示关闭
	返回值：
	说明：设置美颜功能是否开启，即是否打开/关闭对本地视频进行美颜功能
	*/
	static EngineErrorType SetBeautyEnabled(bool bEnable);

	/*
	函数名：GetEWBServiceUrl
	参数：
	返回值：
	说明：获取白板地址
	*/
	static EngineErrorType GetEWBServiceUrl();

	/*
	函数名：QueryEWBServiceUrl
	参数：
	返回值：
	说明：查询白板地址
	*/
	static EngineErrorType QueryEWBServiceUrl();

	/*
	函数名：ObserverRequestBecomeNormalUser
	参数：
	返回值：
	说明：观察者请求成为正常人员，只有用户为观察身份时，此接口才有效
	*/
	static EngineErrorType ObserverRequestBecomeNormalUser();

	/*
	函数名：HostDoObserverRequestBecomeNormalUser
	参数：strUserId，请求者ID，enumEngineOperationType，主持人回复请求类型
	返回值：
	说明：当观察者请求发言，主持人通过该接口处理，接受，拒绝，或者忙碌
	*/
	static EngineErrorType HostDoObserverRequestBecomeNormalUser(const char* strUserId, OperationType enumEngineOperationType);

	/*
	函数名：HostRequestDegradeNormalUserToObserver
	参数：strUserId，被操作人员的用户ID
	返回值：
	说明：主持人请求将正常用户降级为观察者
	*/
	static EngineErrorType HostRequestDegradeNormalUserToObserver(const char* strUserId);

	/*
	函数名：NoramlUserDoHostRequestDegradeNormalUserToObserver
	参数：strHostId，主持人的用户ID，bAccept，接受请求还是拒绝
	返回值：
	说明：被请求降级的用户处理主持人的降级请求
	*/
	static EngineErrorType NoramlUserDoHostRequestDegradeNormalUserToObserver(const char* strhostId, bool bAccept);

	/*
	函数名：HostRequestUpgradeObserverToNormalUser
	参数：strUserId，被操作观察者的用户ID
	返回值：
	说明：主持人请求观察者升级为正常用户
	*/
	static EngineErrorType HostRequestUpgradeObserverToNormalUser(const char* strUserId);

	/*
	函数名：ObserverDoHostRequestUpgradeObserverToNormalUser
	参数：strUserId，主持人的用户ID，bAccept，是否同意升级为正常用户，打开音视频
	返回值：
	说明：观察者处理主持人邀请发言
	*/
	static EngineErrorType ObserverDoHostRequestUpgradeObserverToNormalUser(const char* strUserId, bool bAccept);

	/*
	函数名：HostReomveUser
	参数：strUserId，被操作人员的用户ID
	返回值：
	说明：主持人将用户踢出房间
	*/
	static EngineErrorType HostReomveUser(const char* strUserId);

	/*
	函数名：HostRequestControlUserDevice
	参数：strUserId，被操作人员的用户ID，enumHostManageAction，操作类型，关闭或者打开，HostManageActionMediaType，操作的媒体类型
	返回值：
	说明：主持人通过该接口对与会人员音视频的开关进行管理
	*/
	static EngineErrorType HostRequestControlUserDevice(const char* strUserId, ControlAction enumControlAction, ControlMediaType enumControlMediaType);

	/*
	函数名：UserDoHostRequestControlUserDevice
	参数：strHostId，主持人的用户ID，enumControlAction，操作类型，关闭或者打开，enumControlMediaType，操作的媒体类型，bAccept，是否同意打开音、视频或者音视频
	返回值：
	说明：用户通过该接口处理主持人打开或关闭音视频的开关进行请求或者观察者处理主持人邀请发言
	*/
	static EngineErrorType UserDoHostRequestControlUserDevice(const char* strHostId, ControlAction enumControlAction, ControlMediaType enumControlMediaType, bool bAccept);

	/*
	函数名：GetHostPower
	参数：
	返回值：
	说明：获取主持人权限
	*/
	static EngineErrorType GetHostPower();

	/*
	函数名：GetInviteUrl
	参数：
	返回值：
	说明：获取邀请链接
	*/
	static EngineErrorType GetInviteUrl();

	/*
	函数名：SelectResolution
	参数：enumVideoResolutionProfile为VideoResolutionProfile类型枚举
	返回值：
	说明：会议中动态改变视频分辨率
	*/
	static EngineErrorType ChangeVideoSize(VideoResolutionProfile enumVideoResolutionProfile);

	/*
	函数名：SetTinyStreamEnabled
	参数：bEnable，当为true时表示打开，当为false时表示关闭
	返回值：
	说明：设置小流功能是否开启，即是否打开/关闭对本地视频的小流功能
	*/
	static EngineErrorType SetTinyStreamEnabled(bool bEnable);

	/*
	函数名：SubscribeStream
	参数：strJson，订阅流信息Json串，
				格式说明， [{"user_id","abc123","stream_type",1},{"user_id","def456","stream_type",2}]
					user_id对应值为用户ID，stream_type对应值为1或者2（1代表大流，2代表小流）
	返回值：
	说明：订阅入会人员大小流
	*/
	static EngineErrorType SubscribeStream(const char* strJson);

	//视频模式
	enum VideoMode : int
	{
		//高清模式
		VideoMode_High_Resolution = 1,
		//流畅模式
		VideoProfile_Smooth = 2,
	};

	/*
	函数名：SetVideoMode
	参数：enumVideoMode，视频模式类型
	返回值：
	说明：设置视频模式，默认为VideoProfile_Smooth，流畅模式
	*/
	static EngineErrorType SetVideoMode(VideoMode enumVideoMode);
};

