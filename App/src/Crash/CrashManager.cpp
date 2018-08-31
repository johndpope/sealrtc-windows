#include "CrashManager.h"
#include "UpFile.h"
#include "UpStringUtility.h"
#include "Platform.h"

CCrashManager* CCrashManager::m_pCCrashManager = NULL;

void CCrashManager::CreateInstance()
{
	if (m_pCCrashManager == NULL)
	{
		m_pCCrashManager = new CCrashManager();
	}
}

CCrashManager* CCrashManager::GetInstance()
{
	return m_pCCrashManager;
}

void CCrashManager::DestoryInstance()
{
	if (m_pCCrashManager)
	{
		delete m_pCCrashManager;
		m_pCCrashManager = NULL;
	}
}

CCrashManager::CCrashManager()
{
	m_ThreadHandler = new UpThreadHandler();
	m_HttpHandle = new UpHttp();
	m_strUrl.append("http://op.blinktalk.io:8000/upload");
}

CCrashManager::~CCrashManager()
{
	if(m_ThreadHandler)
	{
		delete m_ThreadHandler;
		m_ThreadHandler = NULL;
	}
	if(m_HttpHandle)
	{
		delete m_HttpHandle;
		m_HttpHandle = NULL;
	}
}

void CCrashManager::UploadingDumpFile()
{
	FindDumpFile();
	if (!m_ThreadHandler->IsWork())
	{
		m_ThreadHandler->Start("CrashReport");
	}
	m_ThreadHandler->Post(this);
}

void CCrashManager::Run()
{
	if (m_listDumpFile.size() && m_strUrl.empty() == false)
	{
		HttpRequestCrash* pHttpRequestCrash = new HttpRequestCrash();
	
		std::string strDumpFilePath = *(m_listDumpFile.begin());
		if (UpFile::IsFileExisted(strDumpFilePath.c_str()))
		{
			std::wstring strHeader;
			strHeader.append(L"Content-type: multipart/form-data; boundary=-------------------------acebdf13572468\r\n");

			int pos = strDumpFilePath.rfind("\\");
			std::string strFileName = strDumpFilePath.substr(pos+1, strDumpFilePath.length()-pos);

			std::string* pData = UpFile::ReadFileToString(UpStringUtility::StringToWstring(strDumpFilePath));
		   
			std::string strForm;
			strForm.append("---------------------------acebdf13572468\r\n");

			strForm.append("Content-Disposition: form-data; name=\"file\"; filename=\"");
			strForm.append(strFileName);
			strForm.append("\"\r\n");

			//strForm.append("Content-Type: application/vnd.tcpdump.pcap\r\n");
			strForm.append("Content-Type: application/octet-stream\r\n");
			strForm.append("\r\n");
			strForm.append((const char*)pData->c_str(),pData->length());
			strForm.append("\r\n");
			strForm.append("---------------------------acebdf13572468--\r\n");
			strForm.append("\r\n");

			m_HttpHandle->SendHttpRequest(pHttpRequestCrash, m_strUrl.c_str(),strForm.c_str(),strForm.length(),&strHeader);
			UpFile::ReleaseFileString(pData);
		}
	}
	else
	{
		if (m_HttpHandle)
		{
			delete m_HttpHandle;
			m_HttpHandle = NULL;
		}
		m_ThreadHandler->Stop();
	}
}

void CCrashManager::FindDumpFile()
{
	m_listDumpFile.clear();
	std::string strDumpPath = CPlatform::GetCrashPath();
	char szFind[MAX_PATH];
	_snprintf_s(szFind, MAX_PATH, "%s*.*", strDumpPath);

	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind = ::FindFirstFileA(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind) return;
	while(TRUE)
	{
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
		{
			std::string strDumpFile;
			strDumpFile = strDumpPath + FindFileData.cFileName;
			m_listDumpFile.push_back(strDumpFile);
		}
		if(!::FindNextFileA(hFind,&FindFileData)) break;
	}
	::FindClose(hFind);
}

void CCrashManager::RemoveFirstDumpFile(bool bDelete)
{
	if (bDelete)
	{
		std::string strDumpFilePath = *(m_listDumpFile.begin());
		if (UpFile::IsFileExisted(strDumpFilePath.c_str()))
		{
			UpFile::DeleteExistFile(strDumpFilePath.c_str());
		}
	}
	m_listDumpFile.pop_front();
}

void HttpRequestCrash::OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen)
{
	std::string strHeaders;
	strHeaders.append(headers);
	int nPos = strHeaders.find(" ",0);
	if (nPos != -1)
	{
		int nCr = strHeaders.find(" ",nPos+1);
		std::string strValue;
		strValue = strHeaders.substr(nPos+1,nCr - nPos);
		DWORD dwValue = atoi(strValue.c_str());
		bool bDelete = false;
		if (dwValue == 200)
		{
			bDelete = true;
		}
		CCrashManager::GetInstance()->RemoveFirstDumpFile(bDelete);
	}
	
	if(!CCrashManager::GetInstance()->GetTheadHandle()->IsWork())
	{
		CCrashManager::GetInstance()->GetTheadHandle()->Start("CrashReport");
	}
	CCrashManager::GetInstance()->GetTheadHandle()->Post(CCrashManager::GetInstance());
}

void HttpRequestCrash::OnHttpRequestFaild(int error)
{
	CCrashManager::GetInstance()->RemoveFirstDumpFile(false);
	if(!CCrashManager::GetInstance()->GetTheadHandle()->IsWork())
	{
		CCrashManager::GetInstance()->GetTheadHandle()->Start("CrashReport");
	}
	CCrashManager::GetInstance()->GetTheadHandle()->Post(CCrashManager::GetInstance());
}
