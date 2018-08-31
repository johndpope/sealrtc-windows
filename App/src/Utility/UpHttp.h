#pragma once
#include <windows.h>
#include <winhttp.h>
#include <wincrypt.h>
#include <string>
#include <list>
#include "UpLock.h"

class IUpHttpRequest
{
public:
	virtual ~IUpHttpRequest(){}
public:
	virtual void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen) = 0;
	virtual void OnHttpRequestFaild(int error) = 0;
};

class UpHttp : public UpLock
{
public:
	UpHttp(void);
	~UpHttp(void);

public:
	BOOL SendHttpRequest(IUpHttpRequest* request, const char* url, const char* data, DWORD datasize, std::wstring* pHttpHeader = NULL);
	BOOL SendHttpsRequest(IUpHttpRequest* request, const char* url, const char* data, DWORD datasize,std::wstring* pHttpHeader = NULL, std::string certpath="",WCHAR *pwd=L"");
	void CancelRequest();

private:
	BOOL HttpGet(const char*  url);
	BOOL HttpsGet(const char*  url, std::string certpath="",WCHAR *pwd=L"");
	BOOL HttpPost(const char*  url, const void* data, DWORD size, std::wstring* pHttpHeader = NULL);
	BOOL HttpsPost(const char*  url, const void* data, DWORD size, std::wstring* pHttpHeader = NULL, std::string certpath="",WCHAR *pwd=L"");
	PCCERT_CONTEXT GetCert(std::string path="",WCHAR *pwd=L"");

public:
	void ClearData();
	HINTERNET GetSession(){return mSession;}
	HINTERNET GetConnect(){return mConnect;}
	HINTERNET GetRequest(){return mRequest;}
	IUpHttpRequest* GetHttpRequest(){return mHttpRequest;}

public:
	static bool IsExistHttp(UpHttp* pHttp);
	static UpLock GetLock() { return m_Lock; }
public:
	static std::list<UpHttp*> m_HttpList;
	static UpLock m_Lock;

	char* mHeaders;
	char* mBody;
	std::string* mData;
	int mHeadersLength;
	int mBodyLength;
private:
	HINTERNET mSession;
	HINTERNET mConnect;
	HINTERNET mRequest;
	HANDLE mRecvThread;
	IUpHttpRequest* mHttpRequest;

};
