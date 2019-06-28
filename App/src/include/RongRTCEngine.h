#ifndef RONG_RTC_ENGINE
#define RONG_RTC_ENGINE

#ifdef RONG_RTC_ENGINE_DLL
#define RONG_RTC_API __declspec(dllexport)
#else
#define RONG_RTC_API __declspec(dllimport)
#endif

//������
enum RongRTCStreamType 
{
	RongRTCStreamType_Audio = 0x01, 	//��Ƶ��
	RongRTCStreamType_Video = 0x02, 	//��Ƶ��
	RongRTCStreamType_AudioVideo = 0x03, 	//����Ƶ��
};

//����Ƶ�������ؼ�ֵ
enum RongRTCStreamParamKey
{
	RongRTCStreamParamKey_VideoSolution = 1, //��Ƶ�ֱ���, value��ӦRongRTCVideoSolution
	RongRTCStreamParamKey_VideoFps, //��Ƶ֡��, value��ӦRongRTCVideoFPS
	RongRTCStreamParamKey_VideoSimulcast, //��Ƶ��С��, valueΪtrue/false
	RongRTCStreamParamKey_SRTP, //������, valueΪtrue/false
};

//��Ƶ�ֱ���
enum RongRTCVideoSolution
{
	RongRTCVideoSolution_256_144 = 1, 	//�ֱ���256*144
	RongRTCVideoSolution_320_240,
	RongRTCVideoSolution_480_360 ,
	RongRTCVideoSolution_640_360,
	RongRTCVideoSolution_640_480,
	RongRTCVideoSolution_720_480,
	RongRTCVideoSolution_1280_720,
	RongRTCVideoSolution_1920_1080,
};

//��Ƶ֡��
enum RongRTCVideoFPS
{
	RongRTCVideoFPS_5 = 1, 	//ÿ��5֡
	RongRTCVideoFPS_10 = 2,
	RongRTCVideoFPS_15 = 3,
	RongRTCVideoFPS_20 = 4,
	RongRTCVideoFPS_25 = 5,
	RongRTCVideoFPS_30 = 6,
};

//������
enum RongRTCCode
{
	//�ɹ�
	RongRTCCodeOK = 0,
	//��������������û����ڷ������
	RongRTCCodeSignalServerUserNotExist = 40001,
	//������������ط����ڲ�����
	RongRTCCodeSignalServerInternalError = 40002,
	//������������ط��䲻����
	RongRTCCodeSignalServerRoomNotExist = 40003,
	//��������������û�IDΪ�մ���
	RongRTCCodeSignalServerUserIdIsEmpty = 40004,
	//��������������ظ�������ͬ�������
	RongRTCCodeSignalServerReJoin = 40005,
	//������������ز�������
	RongRTCCodeSignalServerParamInvalid = 40006,
	//��ȡ���������Tokenʧ��
	RongRTCCodeSignalServerGetTokenFailed = 40007,
	//δ����������(IM)������
	RongRTCCodeSignalServerNotConnect = 50000,
	//��������
	RongRTCCodeParameterError = 50001,
	//������ͬ�������
	RongRTCCodeJoinToSameRoom = 50002,
	//�Ѿ����뷿��
	RongRTCCodeHaveJoinedRoom = 50003,
	//���䲻���ڻ�������
	RongRTCCodeRoomNotExist = 50004,
	//����Json����ʽ����
	RongRTCCodeParameterJsonError = 50005,
	//SDK�Ѿ���ʼ����
	RongRTCCodeSDKHaveInitialized = 50006,
	//SDKδ��ʼ��
	RongRTCCodeSDKHaveNotInitialized = 50007,
	//��Ƶ���ݸ�ʽ�ͳ��Ȳ�ƥ��
	RongRTCCodeVideoDataTypeNotMatchDataSize = 50008,
	//����������ʱ
	RongRTCCodeRoomPingTimeOut = 50009,
	//HTTP ����ʱ
	RongRTCCodeHttpTimeoutError = 5010,
	//HTTP ��Ӧ���󣨺� 500��404��405 �ȴ���
	RongRTCCodeHttpResponseError = 50011,
	//HTTP ������󣨺����粻�ɴ����δ��Ϊ������������
	RongRTCCodeHttpRequestError = 50012,
	//�����ظ���(������������б�����һ�����Է������򲻻ᱨ�������)
	RongRTCCodePublishDuplicateStream = 50020,
	//ȡ�����������ڵ���(���ȡ�����������б�����һ������ȡ���������򲻻ᱨ�������)
	RongRTCCodeUnPublishNotExistStream = 50024,
	//Mute���������ڻ���״̬δ��
	RongRTCCodeMuteNotExistStream = 50025,
	//��������ͷʧ�ܣ��޿�������ͷ
	RongRTCCodeOpenCameraFailed = 50026,
	//������������Ϣʧ��
	RongRTCCodeCreateLocalStreamInfoFailed = 50027,
	//���ñ�������Ϣʧ��
	RongRTCCodeSetLocalStreamInfoFailed = 50028,
	//����Զ������Ϣʧ��
	RongRTCCodeSetRemoteStreamInfoFailed = 50029,
};

//��Ƶ���ݸ�ʽ
enum RongRTCVideoDataType 
{
	RongRTCVideoDataType_I420 = 1, //��Ƶ���ݸ�ʽ I420
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

//��Ƶ֡
struct RongRTCVideoFrame
{
	int iWidth; //���
	int iHeight; //�߶�
	RongRTCVideoDataType enumVideoDataType; //��Ƶ��������
	unsigned char* pVideoData; //��Ƶ�����ڴ���ʼ��ַ
	int iDataSize; //��Ƶ�����ڴ��С
};

//������(IM)����������״̬
enum RongRTCConnectionStatus 
{
	RongRTCConnectionStatus_Connected = 0,
	RongRTCConnectionStatus_Unconnected = 1,
};

//��������
enum RongRTCRoomType
{
	RongRTCRoomType_Normal = 0, //��������
	RongRTCRoomType_Live = 1, //ֱ������
};

class RongRTCEventListener
{
public:
	/*
	����: ֪ͨ�û�������ʱ�뷿��ʧȥ����
	*/
	virtual void NotifyDisconnectWithRoom() = 0;
	/*
	����: ֪ͨ�������������״̬�仯
	����: out_statusΪ�������������״̬, ���RongRTCConnectionStatus����
	*/
	virtual void NotifyConnectionStatusChanged(RongRTCConnectionStatus out_status) = 0;
	/*
	����: ֪ͨ�յ���������Ϣ
	����: out_object_nameΪ��Ϣ����, out_contentΪ��Ϣ����
	*/
	virtual void NotifyReceivedMessage(const char* out_object_name, const char* out_content) = 0;
	/*
	����: ֪ͨԶ���û����뷿��
	����: out_user_idΪ�û�ID
	*/
	virtual void NotifyRemoteUserJoinedRoom(const char* out_user_id) = 0;
	/*
	����: ֪ͨԶ���û��뿪����
	����: out_user_idΪ�û�ID
	*/
	virtual void NotifyRemoteUserLeftRoom(const char* out_user_id) = 0;
	/*
	����: ֪ͨԶ���û�����
	����: out_user_idΪ�û�ID
	*/
	virtual void NotifyRemoteUserOffLine(const char* out_user_id) = 0;
	/*
	����: ֪ͨ�û���Զ���û�������
	����: out_user_streams_json ��ʽΪ [{"userid":[{"mediaType":***, "msid":"***", "state":***, "tag": "***","features":["simulcast"]}]}],
	         useridΪ�û�ID, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��, stateΪ��״̬, 0����ر�, 1������, tagΪ����ǩ, featuresΪ������,
			 �������simulcast�����֧�ִ�С��,
			 ����
			 [
			    {"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","state":1,"tag":"RongCloudRTC"},
								     {"mediaType":1,"msid":"11223344_RongCloudRTC","state":0,"tag":"RongCloudRTC","features":["simulcast"]}]},
			 ],
			 11223344�û���������TagΪRongCloudRTC����Ƶ������TagΪRongCloudRTC����Ƶ��, ״̬Ϊ�ر�״̬, ֧�ִ�С��;
	*/
	virtual void NotifyRemoteUserPublishedStreams(const char* out_user_streams_json) = 0;
	/*
	����: ֪ͨ�û���Զ���û�ȡ��������
	����: out_user_streams_json ��ʽΪ [{"userid":[{"mediaType":***, "msid":"***", "tag": "***"}]}],
			useridΪ�û�ID, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��, tagΪ����ǩ, 
			����
			[{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","tag":"RongCloudRTC"}]}],
			11223344�û�ȡ����������TagΪRongCloudRTC����Ƶ��
	*/
	virtual void NotifyRemoteUserUnPublishedStreams(const char* out_user_streams_json) = 0;
	/*
	����: ֪ͨ�û���Զ���û������״̬
	����: out_user_streams_json ��ʽΪ [{"userid":[{"mediaType":***, "msid":"***", "tag": "***", "state": ***}]}],
			useridΪ�û�ID, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��, tagΪ����ǩ, stateΪ��ǰ��״̬, 0����ر�, 1������
			����
			[{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","tag":"RongCloudRTC","state":0}]}],
			11223344�û��ر�����TagΪRongCloudRTC����Ƶ��
	*/
	virtual void NotifyRemoteUserChangedStreamsState(const char* out_user_streams_json) = 0;
	/*
	����: ֪ͨ�û���Ƶ������, ��������Ĭ����Ƶ����Զ����Ƶ������
	����: out_stream_idΪ��ID, out_video_frameΪ��Ƶһ֡����
	ע��: ��Ƶ����Ϊ32λ��ARGB����
	*/
	virtual void NotifyRenderVideoData(const char* out_stream_id, RongRTCVideoFrame out_video_frame) = 0;
	/*
	����: ֪ͨ�û��ɼ���Ĭ����Ƶ������, ���Խ�����Ƶ����Ԥ�������
	����: out_video_frameΪ��Ƶһ֡����
	ע��: ��Ƶ����ΪI420��ʽ����
	*/
	virtual void NotifyPreprocessingDefaultVideoData(RongRTCVideoFrame out_video_frame) = 0;
};

//���д�����Ļص�����
class RongRTCCallBackOneParam
{
public:
	virtual void OnCallBack(int out_code) = 0;
};

//���д�����ͷ���ֵjson���Ļص�����
class RongRTCCallBackTwoParam
{
public:
	virtual void OnCallBack(int out_code, const char* out_ret) = 0;
};

class RONG_RTC_API RongRTCEngine
{
public:
	/*
	����: �����Ƿ����ñ�����־
	����: in_enableΪ�Ƿ����ñ�����־, ��Ϊtrueʱ, ��������,  in_log_dir������־Ŀ¼(��Ҫ��дȨ��), in_log_fileΪ��־�ļ���
			��Ϊfalseʱ, ����ر�, in_log_dir��Ч
	*/
	static int SetLogEnabled(bool in_enable, const char* in_log_dir, const char* in_log_file);
	/*
	����: ��ʼ��SDK
	����: in_appkeyΪ�����ƹ��������AppKey, in_navi_urlΪ���Ƶ�����ַ, in_listenerΪSDK֪ͨ��ص�ʵ��
	*/
	static int InitWithAppKey(const char* in_appkey, const char* in_navi_url, RongRTCEventListener* in_listener);
	/*
	����: ��������(IM)������
	����: in_tokenΪ��������(IM)�������֤Token, �����ȡ��ʽ���������ƹ����鿴
	*/
	static int ConnectWithToken(const char* in_token);
	/*
	����: ����SDK����, ֵ����Ϊ����
	����: in_stream_keyΪ���ö�Ӧ��Keyֵ, ���RongRTCStreamParamKey����, in_valueΪKey��Ӧ��ҪΪ����ֵ��Value
	*/
	static int SetIntStreamParam(RongRTCStreamParamKey in_stream_key, int in_value);
	/*
	����: ����SDK����, ֵ����Ϊ����
	����: in_stream_keyΪ���ö�Ӧ��Keyֵ, ���RongRTCStreamParamKey����, in_valueΪKey��Ӧ��ҪΪ����ֵ��Value
	*/
	static int SetBoolStreamParam(RongRTCStreamParamKey in_stream_key, bool in_value);
	/*
	����: ���뷿��
	����: in_room_idΪ����ID, in_room_typeΪ��������, ���RongRTCRoomType����, in_callbackΪ��ɻص�, �ص������ڶ�������Ϊ��ǰ
			�������û�������Ϣ, ��ʽΪ [{"userid":[{"mediaType":***, "msid":"***", "state":***, "tag": "***","features":["simulcast"]}]}],
			useridΪ�û�ID, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��, stateΪ��״̬, 0����ر�, 1������, tagΪ����ǩ, featuresΪ������,
			�������simulcast�����֧�ִ�С��
			����
			[
				{"11223344":[{"mediaType":0,"msid":"11223344_RongCloudRTC","state":1,"tag":"RongCloudRTC"},
									 {"mediaType":1,"msid":"11223344_RongCloudRTC","state":0,"tag":"RongCloudRTC","features":["simulcast"]}]},
				{"44556677":[{"mediaType":0,"msid":"44556677_RongCloudRTC","state":1,"tag":"RongCloudRTC"}]},
				{"55667788":[{"mediaType":1,"msid":"55667788_RongCloudRTC","state":1,"tag":"RongCloudRTC"}]},
				{"99887766":""}
			]
			11223344�û���������TagΪRongCloudRTC����Ƶ������TagΪRongCloudRTC����Ƶ��, ״̬Ϊ�ر�״̬, ֧�ִ�С��;
			44556677�û���������TagΪRongCloudRTC����Ƶ��; 55667788�û���������TagΪRongCloudRTC����Ƶ��, ��֧�ִ�С��;
			99887766�û�û�з��κ���
	*/
	static int JoinRoom(const char* in_room_id, RongRTCRoomType in_room_type, RongRTCCallBackTwoParam* in_callback);
	/*
	����: �뿪����
	����: in_callbackΪ��ɻص�
	*/
	static int LeaveRoom(RongRTCCallBackOneParam* in_callback);
	/*
	����: ���÷����Զ�������ֵ, ���in_object_name��Ϊ����in_content��Ϊ��, ������������ֵ��ͬʱ�������ڷ�����Ϣ
	����: in_keyΪ��ֵ, in_valueΪֵ, in_object_nameΪ��������ֵʱ���ӵ���Ϣ����, in_contentΪ��������ʱ���ӵ���Ϣ����, in_callbackΪ��ɻص�
	*/
	static int SetRoomAttributeValue(const char* in_key, const char* in_value, const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
	/*
	����: ɾ�������Զ�������ֵ, ���in_object_name��Ϊ����in_content��Ϊ��, ����ɾ������ֵ��ͬʱ�������ڷ�����Ϣ
	����: in_keysΪ��ֵjson��, ��ʽΪ["key","key"], ���� ["11223344","33445566"],
			in_object_nameΪ��������ֵʱ���ӵ���Ϣ����, in_contentΪ��������ʱ���ӵ���Ϣ����, in_callbackΪ��ɻص�
	*/
	static int DeleteRoomAttributeValue(const char* in_keys, const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
	/*
	����: ��ȡ�����Զ�������ֵ
	����: in_keysΪ��ֵjson��, ��ʽΪ["key","key"], ���� ["11223344","33445566"], ���in_keysΪ���ַ���, ��in_keys = ""ʱ, 
			��ȡ����key��Ӧ��value, in_callbackΪ��ɻص�, �ص������ڶ�������Ϊkey��Ӧ��valueֵ,
			��ʽΪ [{"key":"value"},{"key":"value"}], ���� [{"11223344":"abcdefg"},{"33445566":"gfedcba"}]
	*/
	static int GetRoomAttributeValue(const char* in_keys, RongRTCCallBackTwoParam* in_callback);
	/*
	����: ������
	����: in_subscribe_streamΪ��������json��, ��ʽΪ 
			[{"userid": [{"tag":"***","mediaType":***,"simulcast":***},{"tag":"***","mediaType":***}]}],
			useridΪ�û�ID, tagΪ����ǩ, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��, simulcastΪ��Ƶ��С����־, 1�Ǵ���, 2��С��,
			����
			[
				{"11223344": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":1},{"tag":"RongCloudRTC","mediaType":1}]},
				{"55667788": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":2},{"tag":"RongCloudRTC","mediaType":1}]},
				{"99887766": [{"tag":"RongCloudRTC","mediaType":0,"simulcast":2}]}
			]
			in_callbackΪ��ɻص�
	*/
	static int SubscribeStream(const char* in_subscribe_stream, RongRTCCallBackOneParam* in_callback);
	/*
	����: ȡ��������
	����: in_subscribe_streamΪȡ����������json��, ��ʽΪ [{"userid": [{"tag":"***","mediaType":***},{"tag":"***","mediaType":***}]}],
			useridΪ�û�ID, tagΪ����ǩ, mediaTypeΪ������, 0����Ƶ��, 1����Ƶ��,
			����:
			[
				{"11223344": [{"tag":"RongCloudRTC","mediaType":0},{"tag":"RongCloudRTC","mediaType":1}]},
				{"55667788": [{"tag":"RongCloudRTC","mediaType":0},{"tag":"RongCloudRTC","mediaType":1}]},
				{"99887766": [{"tag":"RongCloudRTC","mediaType":0}]}
			],
			in_callbackΪ��ɻص�
	*/
	static int UnSubscribeStream(const char* in_unsubscribe_stream, RongRTCCallBackOneParam* in_callback);
	/*
	����: ����Ĭ����
	����: in_publish_stream_typeΪ����Ĭ����(����ͷ�ɼ�����Ƶ������˷�ɼ�����Ƶ��)������, ���RongRTCStreamType����,
			in_callbackΪ��ɻص�
	*/
	static int PublishDefaultStream(RongRTCStreamType in_publish_stream_type, RongRTCCallBackOneParam* in_callback);
	/*
	����: ȡ������Ĭ����
	����: in_publish_stream_typeΪȡ������Ĭ����(����ͷ�ɼ�����Ƶ������˷�ɼ�����Ƶ��)������, ���RongRTCStreamType����,
			in_callbackΪ��ɻص�
	*/
	static int UnPublishDefaultStream(RongRTCStreamType in_unpublish_stream_type, RongRTCCallBackOneParam* in_callback);
	/*
	����: �򿪻��߹رձ���Ĭ����Ƶ��
	����: in_enableΪ�Ƿ�ر�, trueΪ��, falseΪ�ر�, in_callbackΪ��ɻص�
	*/
	static int SetLocalDefaultAudioStreamEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	����: �򿪻��߹رձ���Ĭ����Ƶ��
	����: in_enableΪ�Ƿ�ر�, trueΪ��, falseΪ�ر�, in_callbackΪ��ɻص�
	*/
	static int SetLocalDefaultVideoStreamEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	����: �򿪻��߹ر�������
	����: in_enableΪ�Ƿ�ر�, trueΪ��, falseΪ�رգ�in_callbackΪ��ɻص�
	*/
	static int SetSpeakerEnabled(bool in_enable, RongRTCCallBackOneParam* in_callback);
	/*
	����: ����SDK������Ļ������
	����: in_callbackΪ��ɻص�
	*/
	static int PublishScreenShareStream(RongRTCCallBackOneParam* in_callback);
	/*
	����: ȡ������SDK������Ļ������
	����: in_callbackΪ��ɻص�
	*/
	static int UnPublishScreenShareStream(RongRTCCallBackOneParam* in_callback);
	/*
	����: �����Զ�����Ƶ��
	����: in_publish_streamΪ�����Զ�������json��, ��ʽΪ [{"tag":"***", "resolution":"***"}]
			tagΪ����ǩ, resolutionΪ��Ƶ���ֱ���, ��ʽΪ ���X�߶�,
			����: [{"tag":"Video1","resolution":"640X480"},{"tag":"Video2","resolution":"640X480"}],
			in_callbackΪ��ɻص�
	*/
	static int PublishCustomizeStream(const char* in_publish_stream, RongRTCCallBackOneParam* in_callback);
	/*
	����: ȡ�������Զ�����Ƶ��
	����: in_publish_streamΪȡ�������Զ�������json��, ��ʽΪ [{"tag":"***"}],
			tagΪ����ǩ, ����: [{"tag":"Video1"},{"tag":"Video2"}], in_callbackΪ��ɻص�
	*/
	static int UnPublishCustomizeStream(const char* in_unpublish_stream, RongRTCCallBackOneParam* in_callback);
	/*
	����: �����Զ�����Ƶ������
	����: in_tagΪ�Զ�����Ƶ��������ǩ, in_video_frameΪһ֡��Ƶ������, ���RongRTCVideoFrame����
	*/
	static int SendCustomizeStreamVideoFrame(const char* in_tag, RongRTCVideoFrame in_video_frame);
	/*
	����: �򷿼��ڷ�����Ϣ
	����: in_object_nameΪ��Ϣ����, in_contentΪ��Ϣ����, in_callbackΪ��ɻص�
	*/
	static int SendRTCMessage(const char* in_object_name, const char* in_content, RongRTCCallBackOneParam* in_callback);
};

#endif

