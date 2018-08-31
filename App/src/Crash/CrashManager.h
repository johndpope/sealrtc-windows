#ifndef CRASH_MANAGER_H
#define CRASH_MANAGER_H

#include <list>
#include <string>
#include "UpHttp.h"
#include "UpThreadHandler.h"
#include "Base.h"


class CCrashManager : public CBase,  public Runable
{
public:
	static void CreateInstance();
	static CCrashManager* GetInstance();
	static void DestoryInstance();

public:
	CCrashManager();
	~CCrashManager();
public:
	void UploadingDumpFile();
	void Run();
	void FindDumpFile();

	UpThreadHandler* GetTheadHandle(){return m_ThreadHandler;}
	void RemoveFirstDumpFile(bool bDelete);
private:
	UpThreadHandler* m_ThreadHandler;
	UpHttp* m_HttpHandle;
	std::list<std::string> m_listDumpFile;
	std::string m_strUrl;
	static CCrashManager* m_pCCrashManager;
};

class HttpRequestCrash : public IUpHttpRequest
{
public:
	void OnHttpRequestSuccess(const char* headers, int headerLen, const char* body, int bodyLen);
	void OnHttpRequestFaild(int error);
};

#endif // CRASH_MANAGER_H