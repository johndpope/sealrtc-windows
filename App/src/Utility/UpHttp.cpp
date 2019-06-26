#include "UpHttp.h"
#include "UpStringUtility.h"

std::list<UpHttp*> UpHttp::m_HttpList;
UpLock UpHttp::m_Lock;

static VOID CALLBACK HttpRequestCallback(HINTERNET hInternet,DWORD_PTR dwContext,DWORD dwInternetStatus,PVOID lpvStatusInformation,DWORD dwStatusInformationLength)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPVOID Header = NULL;

	UpHttp* myHttp = (UpHttp*)dwContext;
	if(!myHttp)
	{
		return;
	}

	switch(dwInternetStatus)
	{
	case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
		{
			WinHttpReceiveResponse( myHttp->GetRequest(), NULL);
		}
		break;
	case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
		{
			WinHttpQueryHeaders( myHttp->GetRequest(), 
											WINHTTP_QUERY_RAW_HEADERS_CRLF,
											WINHTTP_HEADER_NAME_BY_INDEX, NULL,
											&dwSize, WINHTTP_NO_HEADER_INDEX);

			if( GetLastError( ) == ERROR_INSUFFICIENT_BUFFER )
			{
				Header = new WCHAR[dwSize/sizeof(WCHAR)];
				BOOL results = WinHttpQueryHeaders( myHttp->GetRequest(),
																		WINHTTP_QUERY_RAW_HEADERS_CRLF,
																		WINHTTP_HEADER_NAME_BY_INDEX,
																		Header, &dwSize,
																		WINHTTP_NO_HEADER_INDEX);
				myHttp->mHeaders = UpStringUtility::WcharToChar((LPCWSTR)Header);
				myHttp->mHeadersLength = dwSize;
				delete []Header;
				Header = NULL;
			}

			DWORD size = 0;
			WinHttpQueryDataAvailable( myHttp->GetRequest(), &size);
		}
		break;
	case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		{
			int bufsize = *((int*)lpvStatusInformation);
			if (bufsize > 0)
			{
				char* tempbuf = (char*)malloc(bufsize+1);
				memset(tempbuf, 0, bufsize+1);
				if(WinHttpReadData( myHttp->GetRequest(), (LPVOID)tempbuf, bufsize, &dwDownloaded))
				{
					if (myHttp->mBody)
					{
						myHttp->mBody = (char*)realloc(myHttp->mBody, myHttp->mBodyLength + bufsize + 1);
						memcpy(myHttp->mBody + myHttp->mBodyLength, tempbuf, bufsize);
						myHttp->mBodyLength += bufsize;
					}
					else
					{
						myHttp->mBody = (char*)malloc(bufsize+1);
						memcpy(myHttp->mBody, tempbuf, bufsize);
						myHttp->mBodyLength += bufsize;
					}
				}
				free(tempbuf);
				tempbuf = NULL;

				DWORD size = 0;
				WinHttpQueryDataAvailable( myHttp->GetRequest(), &size);
			}
			else
			{
				if(myHttp->GetHttpRequest())
				{
					myHttp->GetHttpRequest()->OnHttpRequestSuccess(myHttp->mHeaders, myHttp->mHeadersLength, myHttp->mBody, myHttp->mBodyLength);
				}
				//myHttp->ClearData();
			}
		}
		break;
	case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
		{
			WINHTTP_ASYNC_RESULT* rt = (WINHTTP_ASYNC_RESULT*)lpvStatusInformation;
			if (UpHttp::IsExistHttp(myHttp))
			{
				if(myHttp->GetHttpRequest())
				{
					int err = GetLastError();
					myHttp->GetHttpRequest()->OnHttpRequestFaild((int)rt->dwError);

				}
			}
		}
		break;
	default:
		break;
	}
}
	
PCCERT_CONTEXT UpHttp::GetCert(std::string path,WCHAR *pwd)
{
	if (path == "")
	{
		return NULL;
	}
	CRYPT_DATA_BLOB blob;
	memset(&blob, 0, sizeof(blob));

	FILE* fin = NULL;
	errno_t err = fopen_s(&fin, path.c_str(),"rb"); 
	if (!fin)
	{
		return NULL;
	}

	fseek(fin,0,SEEK_END);
	blob.cbData = ftell(fin);
	fseek(fin,0,SEEK_SET);

	blob.pbData = (BYTE *)malloc(blob.cbData);

	fread(blob.pbData, 1, blob.cbData, fin);
	fclose(fin);

	WCHAR *ppwd = pwd;

	HCERTSTORE hCertStore = NULL;
	hCertStore = PFXImportCertStore(&blob, ppwd, CRYPT_EXPORTABLE);
	PCCERT_CONTEXT pCertContext = NULL;
	if (hCertStore)
	{
		pCertContext = CertFindCertificateInStore(hCertStore, X509_ASN_ENCODING|PKCS_7_ASN_ENCODING, 0, 
			CERT_FIND_ANY, NULL, NULL);
	}
	
	free(blob.pbData);

	return pCertContext;
}

UpHttp::UpHttp(void)
{
	mSession = NULL;
	mConnect = NULL;
	mRequest = NULL;
	mRecvThread = NULL;
	mHeaders = NULL;
	mBody = NULL;
	mHeadersLength = 0;
	mBodyLength = 0;
	mHttpRequest = NULL;
	mData = NULL;

	mSession = WinHttpOpen(  L"",
										WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
										WINHTTP_NO_PROXY_NAME,
										WINHTTP_NO_PROXY_BYPASS, 
										WINHTTP_FLAG_ASYNC);

	UpHttp::m_Lock.Lock();
	UpHttp::m_HttpList.push_back(this);
	UpHttp::m_Lock.Unlock();
}

UpHttp::~UpHttp(void)
{
	ClearData();

	if(mSession)
	{
		WinHttpCloseHandle(mSession);
		mSession = NULL;
	}

	UpHttp::m_Lock.Lock();
	for (std::list<UpHttp*>::iterator it = UpHttp::m_HttpList.begin(); it != UpHttp::m_HttpList.end(); ++it)
	{
		if (*it == this)
		{
			UpHttp::m_HttpList.remove(this);
			break;
		}
	}
	UpHttp::m_Lock.Unlock();
}

BOOL UpHttp::SendHttpRequest(IUpHttpRequest* request, const char* url, const char* data, DWORD datasize, std::wstring* pHttpHeader)
{
	BOOL re = FALSE;

	if(!url)
	{
		delete request;
		return re;
	}

	ClearData();

	mHttpRequest = request;
	if(data)
	{
		re = HttpPost(url, data, datasize ,pHttpHeader);
	}
	else
	{
		re = HttpGet(url);
	}
	return re;
}

BOOL UpHttp::SendHttpsRequest(IUpHttpRequest* request, const char* url, const char* data, DWORD datasize, std::wstring* pHttpHeader, std::string certpath, WCHAR *pwd)
{

	BOOL re = FALSE;

	if(!url)
	{
		delete request;
		return re;
	}

	ClearData();

	mHttpRequest = request;
	if(data)
	{
		re = HttpsPost(url, data, datasize,pHttpHeader,certpath,pwd);
	}
	else
	{
		re = HttpsGet(url,certpath,pwd);
	}
	return re;
}

void UpHttp::CancelRequest()
{
	ClearData();
}

BOOL UpHttp::HttpGet(const char* url)
{
	BOOL results = FALSE;

	if(!url)
	{
		return results;
	}

	LPWSTR wstr = UpStringUtility::CharToWchar(url);

	URL_COMPONENTSW uc = {0};
	uc.dwStructSize = sizeof(uc);

	const DWORD BUF_LEN = 256;

	WCHAR host[BUF_LEN] = {0};
	uc.lpszHostName = host;
	uc.dwHostNameLength = BUF_LEN;
	WCHAR path[BUF_LEN] = {0};
	uc.lpszUrlPath = path;
	uc.dwUrlPathLength = BUF_LEN;

	BOOL ret = WinHttpCrackUrl(wstr, 0, ICU_ESCAPE, &uc);

	delete []wstr;
	wstr = NULL;

	if(!ret)
	{
		return results;
	}

	if (mSession)
	{	
		mConnect = WinHttpConnect( mSession, uc.lpszHostName,
									uc.nPort, 0);
	}

	if (mConnect)
	{
		mRequest = WinHttpOpenRequest( mConnect, L"GET", uc.lpszUrlPath,
														NULL, WINHTTP_NO_REFERER,
														WINHTTP_DEFAULT_ACCEPT_TYPES,
														0);
	}

	if (mRequest)
	{
		UpHttp* ptr =  this;
		BOOL re = WinHttpSetOption (mRequest,WINHTTP_OPTION_CONTEXT_VALUE, &ptr, sizeof(this));

		WINHTTP_STATUS_CALLBACK isCallback = WinHttpSetStatusCallback(mRequest,
																											(WINHTTP_STATUS_CALLBACK)HttpRequestCallback,
																											WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 
																											NULL);

		results = WinHttpSendRequest( mRequest,
													WINHTTP_NO_ADDITIONAL_HEADERS,
													0, WINHTTP_NO_REQUEST_DATA, 0,
													0, 0);
	}

	//if (results)
	//{
	//	results = WinHttpReceiveResponse( mRequest, NULL);
	//}

	return results;
}

BOOL UpHttp::HttpsGet(const char*  url, std::string certpath, WCHAR *pwd)
{
	BOOL results = FALSE;

	if(!url)
	{
		return results;
	}

	LPWSTR wstr = UpStringUtility::CharToWchar(url);

	URL_COMPONENTSW uc = {0};
	uc.dwStructSize = sizeof(uc);

	const DWORD BUF_LEN = 256;

	WCHAR host[BUF_LEN] = {0};
	uc.lpszHostName = host;
	uc.dwHostNameLength = BUF_LEN;
	WCHAR path[BUF_LEN] = {0};
	uc.lpszUrlPath = path;
	uc.dwUrlPathLength = BUF_LEN;

	BOOL ret = WinHttpCrackUrl(wstr, 0, ICU_ESCAPE, &uc);

	delete []wstr;
	wstr = NULL;

	if(!ret)
	{
		return results;
	}

	if (mSession)
	{	
		mConnect = WinHttpConnect( mSession, uc.lpszHostName,uc.nPort, 0);
	}

	if (mConnect)
	{
		mRequest = WinHttpOpenRequest( mConnect, L"GET", uc.lpszUrlPath,
														NULL, WINHTTP_NO_REFERER,
														WINHTTP_DEFAULT_ACCEPT_TYPES,
														WINHTTP_FLAG_SECURE);//WINHTTP_FLAG_SECURE
	}

	if (mRequest)
	{
		UpHttp* ptr =  this;
		BOOL re = WinHttpSetOption(mRequest, WINHTTP_OPTION_CONTEXT_VALUE, &ptr, sizeof(this));

		DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
									SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
									SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
									SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

		re = WinHttpSetOption(mRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

		if (certpath.empty() ==false && pwd != NULL)
		{
			PCCERT_CONTEXT pContext = GetCert(certpath, pwd);
			BOOL r_cer = WinHttpSetOption(mRequest, WINHTTP_OPTION_CLIENT_CERT_CONTEXT, (void *)pContext, sizeof(CERT_CONTEXT));
			if (r_cer == FALSE)
			{
				return FALSE;
			}
		}
		
		WINHTTP_STATUS_CALLBACK isCallback = WinHttpSetStatusCallback(mRequest,
																											(WINHTTP_STATUS_CALLBACK)HttpRequestCallback,
																											WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 
																											NULL);

		
		results = WinHttpSendRequest( mRequest,
													WINHTTP_NO_ADDITIONAL_HEADERS,
													0, WINHTTP_NO_REQUEST_DATA, 0,
													0, 0);
		int err = GetLastError();
	}

	return results;
}

BOOL UpHttp::HttpPost(const char*  url, const void* data, DWORD size, std::wstring* pHttpHeader)
{
	BOOL results = FALSE;

	if(!url)
	{
		return results;
	}

	LPWSTR wstr = UpStringUtility::CharToWchar(url);

	URL_COMPONENTSW uc = {0};
	uc.dwStructSize = sizeof(uc);

	const DWORD BUF_LEN = 256;

	WCHAR host[BUF_LEN] = {0};
	uc.lpszHostName = host;
	uc.dwHostNameLength = BUF_LEN;
	WCHAR path[BUF_LEN] = {0};
	uc.lpszUrlPath = path;
	uc.dwUrlPathLength = BUF_LEN;

	BOOL ret = WinHttpCrackUrl(wstr, 0, ICU_ESCAPE, &uc);

	delete wstr;
	wstr = NULL;

	if(!ret)
	{
		return results;
	}

	if (mSession)
	{
		mConnect = WinHttpConnect( mSession, uc.lpszHostName,uc.nPort, 0);
	}

	if (mConnect)
	{
		mRequest = WinHttpOpenRequest(mConnect, L"POST", 
														uc.lpszUrlPath, 
														NULL, WINHTTP_NO_REFERER, 
														WINHTTP_DEFAULT_ACCEPT_TYPES,
														0);
	}

	if (mRequest) 
	{
		UpHttp* ptr =  this;
		BOOL re = WinHttpSetOption (mRequest,WINHTTP_OPTION_CONTEXT_VALUE, &ptr, sizeof(this));

		WINHTTP_STATUS_CALLBACK isCallback = WinHttpSetStatusCallback(mRequest,
																											(WINHTTP_STATUS_CALLBACK)HttpRequestCallback,
																											WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 
																											NULL);

		if (pHttpHeader)
		{
			WinHttpAddRequestHeaders(mRequest, pHttpHeader->c_str(), pHttpHeader->size(), WINHTTP_ADDREQ_FLAG_ADD);
		}

		mData = new std::string();
		mData->append((const char*)data, size);

		results = WinHttpSendRequest(mRequest, 
													WINHTTP_NO_ADDITIONAL_HEADERS,
													0, const_cast<void*>((void*)mData->c_str()), size, 
													size, 0);
	}
	return results;
}

BOOL UpHttp::HttpsPost(const char*  url, const void* data, DWORD size, std::wstring* pHttpHeader, std::string certpath,WCHAR *pwd)
{
	BOOL results = FALSE;

	if(!url)
	{
		return results;
	}

	LPWSTR wstr = UpStringUtility::CharToWchar(url);

	URL_COMPONENTSW uc = {0};
	uc.dwStructSize = sizeof(uc);

	const DWORD BUF_LEN = 256;

	WCHAR host[BUF_LEN] = {0};
	uc.lpszHostName = host;
	uc.dwHostNameLength = BUF_LEN;
	WCHAR path[BUF_LEN] = {0};
	uc.lpszUrlPath = path;
	uc.dwUrlPathLength = BUF_LEN;

	BOOL ret = WinHttpCrackUrl(wstr, 0, ICU_ESCAPE, &uc);

	delete wstr;
	wstr = NULL;

	if(!ret)
	{
		return results;
	}

	if (mSession)
	{
		mConnect = WinHttpConnect( mSession, uc.lpszHostName, uc.nPort, 0);
	}

	if (mConnect)
	{
		mRequest = WinHttpOpenRequest(mConnect, L"POST", uc.lpszUrlPath, NULL, 
														WINHTTP_NO_REFERER, 
														WINHTTP_DEFAULT_ACCEPT_TYPES, 
														WINHTTP_FLAG_SECURE);
	}

	if (mRequest) 
	{
		UpHttp* ptr =  this;
		BOOL re = WinHttpSetOption (mRequest,WINHTTP_OPTION_CONTEXT_VALUE, &ptr, sizeof(this));

		DWORD dwFlags = SECURITY_FLAG_IGNORE_UNKNOWN_CA |
			SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
			SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
			SECURITY_FLAG_IGNORE_CERT_DATE_INVALID;

		re = WinHttpSetOption(mRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags));

		if (certpath.empty() == false && pwd != NULL)
		{
			PCCERT_CONTEXT pContext = GetCert(certpath, pwd);
			BOOL r_cer = WinHttpSetOption(mRequest, WINHTTP_OPTION_CLIENT_CERT_CONTEXT, (void *)pContext, sizeof(CERT_CONTEXT));
			if (r_cer == FALSE)
			{
				return FALSE;
			}
		}

		WINHTTP_STATUS_CALLBACK isCallback = WinHttpSetStatusCallback(mRequest,
																											(WINHTTP_STATUS_CALLBACK)HttpRequestCallback,
																											WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, NULL);

		if (pHttpHeader)
		{
			WinHttpAddRequestHeaders(mRequest, pHttpHeader->c_str(), pHttpHeader->size(), WINHTTP_ADDREQ_FLAG_ADD);
		}

		mData = new std::string();
		mData->append((const char*)data, size);

		results = WinHttpSendRequest(mRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, const_cast<void*>((void*)mData->c_str()), size,
			size, 0);
	}

	//if (results)
	//{
	//	results = WinHttpReceiveResponse( mRequest, NULL);
	//}

	return results;
}

void UpHttp::ClearData()
{
	if(mRequest)
	{
		WINHTTP_STATUS_CALLBACK isCallback = ::WinHttpSetStatusCallback(mRequest,
																											NULL,
																											WINHTTP_CALLBACK_FLAG_ALL_NOTIFICATIONS, 
																											NULL);
		WinHttpCloseHandle(mRequest);
		mRequest = NULL;
	}
	if(mConnect)
	{
		WinHttpCloseHandle(mConnect);
		mConnect = NULL;
	}

	if(mHeaders)
	{
		delete []mHeaders;
		mHeaders = NULL;
	}
	if(mBody)
	{
		free(mBody);
		mBody = NULL;
	}
	mHeadersLength = 0;
	mBodyLength = 0;

	if( mHttpRequest  )
	{
		delete mHttpRequest;
		mHttpRequest = NULL;
	}
	if (mData)
	{
		delete mData;
		mData = NULL;
	}
}

bool  UpHttp::IsExistHttp(UpHttp* pHttp)
{
	bool bResult = false;
	UpHttp::m_Lock.Lock();
	for (std::list<UpHttp*>::iterator it = m_HttpList.begin(); it != m_HttpList.end(); ++it)
	{
		if (*it == pHttp)
		{
			bResult = true;
			break;
		}
	}
	UpHttp::m_Lock.Unlock();
	return bResult;
}