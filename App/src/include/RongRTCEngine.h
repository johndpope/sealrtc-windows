#ifndef RONG_RTC_ENGINE
#define RONG_RTC_ENGINE

#ifdef RONG_RTC_ENGINE_DLL
#define RONG_RTC_API __declspec(dllexport)
#else
#define RONG_RTC_API __declspec(dllimport)
#endif

//流类型
enum RongRTCStreamType 
{
	RongRTCStreamType_Audio = 0x01, 	//音频流
	RongRTCStreamType_Video = 0x02, 	//视频流
	RongRTCStreamType_AudioVideo = 0x03, 	//音视频流
};

//音视频流参数关键值
enum RongRTCStreamParamKey
{
	RongRTCStreamParamKey_VideoSolution = 1, //视频分辨率, value对应RongRTCVideoSolution
	RongRTCStreamParamKey_VideoFps, //视频帧率, value对应RongRTCVideoFPS
	RongRTCStreamParamKey_VideoSimulcast, //视频大小流, value为true/false
	RongRTCStreamParamKey_SRTP, //流加密, value为true/false
};

//视频分辨率
enum RongRTCVideoSolution
{
	RongRTCVideoSolution_256_144 = 1, 	//分辨率256*144
	RongRTCVideoSolution_320_240,
	RongRTCVideoSolution_480_360 ,
	RongRTCVideoSolution_640_360,
	RongRTCVideoSolution_640_480,
	RongRTCVideoSolution_720_480,
	RongRTCVideoSolution_1280_720,
	RongRTCVideoSolution_1920_1080,
};

//视频帧率
enum RongRTCVideoFPS
{
	RongRTCVideoFPS_5 = 1, 	//每秒5帧
	RongRTCVideoFPS_10 = 2,
	RongRTCVideoFPS_15 = 3,
	RongRTCVideoFPS_20 = 4,
	RongRTCVideoFPS_25 = 5,
	RongRTCVideoFPS_30 = 6,
};

//错误码
enum RongRTCCode
{
	//成功
	RongRTCCodeOK = 0,
	//信令服务器返回用户不在房间错误
	RongRTCCodeSignalServerUserNotExist = 40001,
	//信令服务器返回服务内部错误
	RongRTCCodeSignalServerInternalError = 40002,
	//信令服务器返回房间不存在
	RongRTCCodeSignalServerRoomNotExist = 40003,
	//信令服务器返回用户ID为空错误
	RongRTCCodeSignalServerUserIdIsEmpty = 40004,
	//信令服务器返回重复加入相同房间错误
	RongRTCCodeSignalServerReJoin = 40005,
	//信令服务器返回参数错误
	RongRTCCodeSignalServerParamInvalid = 40006,
	//获取信令服务器Token失败
	RongRTCCodeSignalServerGetTokenFailed = 40007,
	//未连接上信令(IM)服务器
	RongRTCCodeSignalServerNotConnect = 50000,
	//参数错误
	RongRTCCodeParameterError = 50001,
	//加入相同房间错误
	RongRTCCodeJoinToSameRoom = 50002,
	//已经加入房间
	RongRTCCodeHaveJoinedRoom = 50003,
	//房间不存在或已销毁
	RongRTCCodeRoomNotExist = 50004,
	//参数Json串格式错误
	RongRTCCodeParameterJsonError = 50005,
	//SDK已经初始化过
	RongRTCCodeSDKHaveInitialized = 50006,
	//SDK未初始化
	RongRTCCodeSDKHaveNotInitialized = 50007,
	//视频数据格式和长度不匹配
	RongRTCCodeVideoDataTypeNotMatchDataSize = 50008,
	//房间心跳超时
	RongRTCCodeRoomPingTimeOut = 50009,
	//HTTP 请求超时
	RongRTCCodeHttpTimeoutError = 5010,
	//HTTP 响应错误（含 500，404，405 等错误）
	RongRTCCodeHttpResponseError = 50011,
	//HTTP 请求错误（含网络不可达，请求未能为能正常发出）
	RongRTCCodeHttpRequestError = 50012,
	//发布重复流(如果发布的流列表中有一个可以发布，则不会报这个错误)
	RongRTCCodePublishDuplicateStream = 50020,
	//取消发布不存在的流(如果取消发布的流列表中有一个可以取消发布，则不会报这个错误)
	RongRTCCodeUnPublishNotExistStream = 50024,
	//Mute的流不存在或者状态未变
	RongRTCCodeMuteNotExistStream = 50025,
	//开启摄像头失败，无可用摄像头
	RongRTCCodeOpenCameraFailed = 50026,
	//创建本地流信息失败
	RongRTCCodeCreateLocalStreamInfoFailed = 50027,
	//设置本地流信息失败
	RongRTCCodeSetLocalStreamInfoFailed = 50028,
	//设置远端流信息失败
	RongRTCCodeSetRemoteStreamInfoFailed = 50029,
};

//视频数据格式
enum RongRTCVideoDataType 
{
	RongRTCVideoDataType_I420 = 1, //视频数据格式 I420
	RongRTCVideoDataType_IYUV,
	RongRTCVideoDataType_RGB24,
	RongRTCVideoDataType_ABGR,
	RongRTCVideoDataType_ARGB,
	RongRTCVideoDataType_ARGB4444,
	RongRTCVideoDataType_RGB565,
	RongRTCVideoDataType_ARGB1555,
	RongRTCVideoDataType_YUY2,
	RongRTCVideoDataType_YV12,
	RongRTCVideoDataType_UYVY,
	RongRTCVideoDataType_MJPG,
	RongRTCVideoDataType_NV21,
	RongRTCVideoDataType_NV12,
	RongRTCVideoDataType_BGRA,
};

//视频帧
struct RongRTCVideoFrame
{
	int iWidth; //宽度
	int iHeight; //高度
	RongRTCVideoDataType enumVideoDataType; //视频数据类型
	unsigned char* pVideoData; //视频数据内存起始地址
	int iDataSize; //视频数据内存大小
};

//与信令(IM)服务器连接状态
enum RongRTCConnectionStatus 
{
	RongRTCConnectionStatus_Connected = 0,
	RongRTCConnectionStatus_Unconnected = 1,
};

//房间类型
enum RongRTCRoomType
{
	RongRTCRoomType_Normal = 0, //正常房间
	RongRTCRoomType_Live = 1, //直播房间
};

class RongRTCEventListener
{
public:
	/*
	功能: 通知用户心跳超时与房间失去连接
	*/
	virtual void NotifyDisconnectWithRoom() = 0;
	/*
	功能: 通知信令服务器连接状态变化
	参数: out_status为信令服务器连接状态, 详见RongRTCConnectionStatus定义
	*/
	virtual void NotifyConnectionStatusChanged(RongRTCConnectionStatus out_status) = 0;
	/*
	功能: 通知收到房间内消息
	参数: out_object_name为消息名字, out_content为消息内容
	*/
	virtual void NotifyReceivedMessage(const char* out_object_name, const char* out_content) = 0;
	/*
	功能: 通知远端用户加入房间
	参数: out_user_id为用户ID
	*/
	virtual void NotifyRemoteUserJoinedRoom(const char* out_user_id) = 0;
	/*
	功能: 通知远端用户离开房间
	参数: out_user_id为用户ID
	*/
	virtual void NotifyRemoteUserLeftRoom(const char* out_user_id) = 0;
	/*
	功能: 通知远端用户离线
	参数: out_user_id为用户ID
	*/
	virtual void NotifyRemoteUserOffLine(const char* out_user_id) = 0;
	/*
	功能: 通知用户有远端用户发布流
	参数: out_user_streams_json 格式为 [{"userid":[{"mediaType":***, "msid":"***", "state":***, "tag": "***","features":["simulcast"]}]}],
	         userid为用户ID, mediaType为流类型, 0是音频流, 1是视频流, state为流状态, 0代表关闭, 1代表开启, tag为流标签, features为流特征,
			 如果包含simulcast则代表支持大小流,
			 例子
			 [
			    {"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","state":1,"tag":"RongCloudRTC"},
								     {"mediaType":1,"msid":"11223344_RongCloudRTC","state":0,"tag":"RongCloudRTC","features":["simulcast"]}]},
			 ],
			 11223344用户发布了流Tag为RongCloudRTC的音频流和流Tag为RongCloudRTC的视频流, 状态为关闭状态, 支持大小流;
	*/
	virtual void NotifyRemoteUserPublishedStreams(const char* out_user_streams_json) = 0;
	/*
	功能: 通知用户有远端用户取消发布流
	参数: out_user_streams_json 格式为 [{"userid":[{"mediaType":***, "msid":"***", "tag": "***"}]}],
			userid为用户ID, mediaType为流类型, 0是音频流, 1是视频流, tag为流标签, 
			例子
			[{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","tag":"RongCloudRTC"}]}],
			11223344用户取消发布了流Tag为RongCloudRTC的音频流
	*/
	virtual void NotifyRemoteUserUnPublishedStreams(const char* out_user_streams_json) = 0;
	/*
	功能: 通知用户有远端用户变更流状态
	参数: out_user_streams_json 格式为 [{"userid":[{"mediaType":***, "msid":"***", "tag": "***", "state": ***}]}],
			userid为用户ID, mediaType为流类型, 0是音频流, 1是视频流, tag为流标签, state为当前流状态, 0代表关闭, 1代表开启
			例子
			[{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","tag":"RongCloudRTC","state":0}]}],
			11223344用户关闭了流Tag为RongCloudRTC的音频流
	*/
	virtual void NotifyRemoteUserChangedStreamsState(const char* out_user_streams_json) = 0;
	/*
	功能: 通知用户视频流数据, 包括本地默认视频流和远端视频流数据
	参数: out_stream_id为流ID, out_video_frame为视频一帧数据
	注释: 视频数据为32位的ARGB数据
	*/
	virtual void NotifyRenderVideoData(const char* out_stream_id, RongRTCVideoFrame out_video_frame) = 0;
	/*
	功能: 通知用户采集到默认视频流数据, 可以进行视频数据预处理操作
	参数: out_video_frame为视频一帧数据
	注释: 视频数据为I420格式数据
	*/
	virtual void NotifyPreprocessingDefaultVideoData(RongRTCVideoFrame out_video_frame) = 0;
};

//带有错误码的回调函数
class RongRTCCallBackOneParam
{
public:
	virtual void OnCallBack(int out_code) = 0;
};

//带有错误码和返回值json串的回调函数
class RongRTCCallBackTwoParam
{
public:
	virtual void OnCallBack(int out_code, const char* out_ret) = 0;
};

class RONG_RTC_API RongRTCEngine
{
public:
	/*
	功能: 设置是否启用本地日志
	参数: in_enable为是否启用本地日志, 当为true时, 代表启用,  in_log_dir代表日志目录(需要有写权限), in_log_file为日志文件名
			当为false时, 代表关闭, in_log_dir无效
	*/
	static int SetLogEnabled(bool in_enable, const char* in_log_dir, const char* in_log_file);
	/*
	功能: 初始化SDK
	参数: in_appkey为在融云官网申请的AppKey, in_navi_url为融云导航地址, in_listener为SDK通知类回调实现
	*/
	static int InitWithAppKey(const char* in_appkey, const char* in_navi_url, RongRTCEventListener* in_listener);
	/*
	功能: 连接信令(IM)服务器
	参数: in_token为连接信令(IM)服务的验证Token, 具体获取方式可以在融云官网查看
	*/
	static int ConnectWithToken(const char* in_token);
	/*
	功能: 设置SDK参数, 值类型为整形
	参数: in_stream_key为设置对应的Key值, 详见RongRTCStreamParamKey定义, in_value为Key对应需要为整形值的Value
	*/
	static int SetIntStreamParam(RongRTCStreamParamKey in_stream_key, int in_value);
	/*
	功能: 设置SDK参数, 值类型为布尔
	参数: in_stream_key为设置对应的Key值, 详见RongRTCStreamParamKey定义, in_value为Key对应需要为布尔值的Value
	*/
	static int SetBoolStreamParam(RongRTCStreamParamKey in_stream_key, bool in_value);
	/*
	功能: 加入房间
	参数: in_room_id为房间ID, in_room_type为房间类型, 详见RongRTCRoomType定义, in_callback为完成回调, 回调函数第二个参数为当前
			房间内用户的流信息, 格式为 [{"userid":[{"mediaType":***, "msid":"***", "state":***, "tag": "***","features":["simulcast"]}]}],
			userid为用户ID, mediaType为流类型, 0是音频流, 1是视频流, state为流状态, 0代表关闭, 1代表开启, tag为流标签, features为流特征,
			如果包含simulcast则代表支持大小流
			例子
			[
				{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","state":1,"tag":"RongCloudRTC"},
									 {"mediaType":1,"msid":"11223344_RongCloudRTC","state":0,"tag":"RongCloudRTC","features":["simulcast"]}]},
				{"44556677":[{"mediaType":0,"msid":"44556677_RongCloudRTC","state":1,"tag":"RongCloudRTC"}]},
				{"55667788":[{"mediaType":1,"msid":"55667788_RongCloudRTC","state":1,"tag":"RongCloudRTC"}]},
				{"99887766":""}
			]
			11223344用户发布了流Tag为RongCloudRTC的音频流和流Tag为RongCloudRTC的视频流, 状态为关闭状态, 支持大小流;
			44556677用户发布了流Tag为RongCloudRTC的音频流; 55667788用户发布了流Tag为RongCloudRTC的视频流, 不支持大小流;
			99887766用户没有发任何流
	*/
	static int JoinRoom(const char* in_room_id, RongRTCRoomType in_room_type, RongRTCCallBackTwoParam* in_callback);
	/*
	功能: 离开房间
	参数: in_callback为完成回调
	*/
	static int LeaveRoom(RongRTCCallBackOneParam* in_callback);
	/*
	功能: 设置房间自定义属性值, 如果in_object_name不为空且in_content不为空, 会在设置属性值的同时往房间内发送消息
	参数: in_key为键值, in_value为值, in_object_name为设置属性值时附加的消息名字, in_content为设置属性时附加的消息内容, in_callback为完成回调
	*/
	static int SetRoomAttributeValue(const char* in_key, const char* in_value, const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 删除房间自定义属性值, 如果in_object_name不为空且in_content不为空, 会在删除属性值的同时往房间内发送消息
	参数: in_keys为键值json串, 格式为["key","key"], 例子 ["11223344","33445566"],
			in_object_name为设置属性值时附加的消息名字, in_content为设置属性时附加的消息内容, in_callback为完成回调
	*/
	static int DeleteRoomAttributeValue(const char* in_keys, const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 获取房间自定义属性值
	参数: in_keys为键值json串, 格式为["key","key"], 例子 ["11223344","33445566"], 如果in_keys为空字符串, 即in_keys = ""时, 
			获取所有key对应的value, in_callback为完成回调, 回调函数第二个参数为key对应的value值,
			格式为 [{"key":"value"},{"key":"value"}], 例子 [{"11223344":"abcdefg"},{"33445566":"gfedcba"}]
	*/
	static int GetRoomAttributeValue(const char* in_keys, RongRTCCallBackTwoParam* in_callback);
	/*
	功能: 订阅流
	参数: in_subscribe_stream为订阅流的json串, 格式为 
			[{"userid": [{"tag":"***","mediaType":***,"simulcast":***},{"tag":"***","mediaType":***}]}],
			userid为用户ID, tag为流标签, mediaType为流类型, 0是音频流, 1是视频流, simulcast为视频大小流标志, 1是大流, 2是小流,
			例子
			[
				{"11223344": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":1},{"tag":"RongCloudRTC","mediaType":1}]},
				{"55667788": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":2},{"tag":"RongCloudRTC","mediaType":1}]},
				{"99887766": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":2}]}
			]
			in_callback为完成回调
	*/
	static int SubscribeStream(const char* in_subscribe_stream, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 取消订阅流
	参数: in_subscribe_stream为取消订阅流的json串, 格式为 [{"userid": [{"tag":"***","mediaType":***},{"tag":"***","mediaType":***}]}],
			userid为用户ID, tag为流标签, mediaType为流类型, 0是音频流, 1是视频流,
			例子:
			[
				{"11223344": [{"tag":"RongCloudRTC","mediaType":0},{"tag":"RongCloudRTC","mediaType":1}]},
				{"55667788": [{"tag":"RongCloudRTC","mediaType":0},{"tag":"RongCloudRTC","mediaType":1}]},
				{"99887766": [{"tag":"RongCloudRTC","mediaType":0}]}
			],
			in_callback为完成回调
	*/
	static int UnSubscribeStream(const char* in_unsubscribe_stream, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 发布默认流
	参数: in_publish_stream_type为发布默认流(摄像头采集的视频流，麦克风采集的音频流)的类型, 详见RongRTCStreamType定义,
			in_callback为完成回调
	*/
	static int PublishDefaultStream(RongRTCStreamType in_publish_stream_type, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 取消发布默认流
	参数: in_publish_stream_type为取消发布默认流(摄像头采集的视频流，麦克风采集的音频流)的类型, 详见RongRTCStreamType定义,
			in_callback为完成回调
	*/
	static int UnPublishDefaultStream(RongRTCStreamType in_unpublish_stream_type, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 打开或者关闭本地默认音频流
	参数: in_enable为是否关闭, true为打开, false为关闭, in_callback为完成回调
	*/
	static int SetLocalDefaultAudioStreamEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 打开或者关闭本地默认视频流
	参数: in_enable为是否关闭, true为打开, false为关闭, in_callback为完成回调
	*/
	static int SetLocalDefaultVideoStreamEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 打开或者关闭扬声器
	参数: in_enable为是否关闭, true为打开, false为关闭，in_callback为完成回调
	*/
	static int SetSpeakerEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 发布SDK内置屏幕共享流
	参数: in_callback为完成回调
	*/
	static int PublishScreenShareStream(RongRTCCallBackOneParam* in_callback);
	/*
	功能: 取消发布SDK内置屏幕共享流
	参数: in_callback为完成回调
	*/
	static int UnPublishScreenShareStream(RongRTCCallBackOneParam* in_callback);
	/*
	功能: 发布自定义视频流
	参数: in_publish_stream为发布自定义流的json串, 格式为 [{"tag":"***", "resolution":"***"}]
			tag为流标签, resolution为视频流分辨率, 格式为 宽度X高度,
			例子: [{"tag":"Video1","resolution":"640X480"},{"tag":"Video2","resolution":"640X480"}],
			in_callback为完成回调
	*/
	static int PublishCustomizeStream(const char* in_publish_stream, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 取消发布自定义视频流
	参数: in_publish_stream为取消发布自定义流的json串, 格式为 [{"tag":"***"}],
			tag为流标签, 例子: [{"tag":"Video1"},{"tag":"Video2"}], in_callback为完成回调
	*/
	static int UnPublishCustomizeStream(const char* in_unpublish_stream, RongRTCCallBackOneParam* in_callback);
	/*
	功能: 发送自定义视频流数据
	参数: in_tag为自定义视频流的流标签, in_video_frame为一帧视频流数据, 详见RongRTCVideoFrame定义
	*/
	static int SendCustomizeStreamVideoFrame(const char* in_tag, RongRTCVideoFrame in_video_frame);
	/*
	功能: 向房间内发送消息
	参数: in_object_name为消息名字, in_content为消息内容, in_callback为完成回调
	*/
	static int SendRTCMessage(const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
};

#endif

