#include "UpFile.h"
#include <windows.h>
#include <WinBase.h>
#include <direct.h>
#include "UpStringUtility.h"

UpFile::UpFile(const char* fileName, bool isOnlyReady)
{
	int err = 0;
	if (IsFileExisted(fileName))
	{
		if (isOnlyReady)
		{
			file = ::CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		else
		{
			file = ::CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}

	}
	else
	{
		std::string dir = GetPath(fileName);
		if (!IsDirExisted(dir.c_str()))
		{
			CreatePath(dir.c_str());
		}
		file = ::CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

UpFile::UpFile(const wchar_t* fileName, bool isOnlyReady)
{
	int err = 0;
	if (IsFileExistedW(fileName))
	{
		if (isOnlyReady)
		{
			file = ::CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}
		else
		{
			file = ::CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		}

	}
	else
	{
		std::wstring dir = GetPathW(fileName);
		if (!IsDirExistedW(dir.c_str()))
		{
			CreatePathW(dir.c_str());
		}
		file = ::CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
}

UpFile::~UpFile(void)
{
	CloseHandle(file);
}

__int64 UpFile::FileSize()
{
	__int64 size = 0;
	LARGE_INTEGER ret = { 0 };
	bool isOK = ::GetFileSizeEx(file, &ret) != 0;
	if (isOK)
	{
		size = ((__int64)ret.HighPart << 32) | ret.LowPart;
	}
	return size;
}

int UpFile::Read(char* buf, int num)
{
	DWORD size;
	int ret = ::ReadFile(file, buf, num, &size, NULL);
	return ret;
}

int UpFile::Write(const char* buf, int num)
{
	DWORD size;

	return ::WriteFile(file, buf, num, &size, NULL);
}

bool UpFile::Seek(int postion)
{
	DWORD  ret = ::SetFilePointer(file, postion, NULL, FILE_BEGIN);
	if (ret != INVALID_SET_FILE_POINTER)
	{
		return true;
	}
	else
	{
		return false;
	}
}

std::string* UpFile::ReadFileToString(std::wstring filepath)
{
	UpFile* file = new UpFile(filepath.c_str());
	int filesize = (int)file->FileSize();
	char* buf = new char[filesize + 1];
	file->Read(buf, filesize);
	std::string *data = new std::string;
	data->append((char*)buf, filesize);

	delete[]buf;
	buf = NULL;
	delete file;
	file = NULL;

	return data;
}

void UpFile::ReleaseFileString(std::string* fileString)
{
	delete fileString;
}


bool UpFile::CreatePath(const char* filePath)
{
	int ret = 0;
	char *ptr = NULL;
	int i = 0;
	char path[256];
	do
	{
		ptr = strchr((char *)filePath + i, '\\');
		if (ptr)
		{
			strncpy_s(path, filePath, ptr - filePath);
			path[ptr - filePath] = '\0';
			if (!(strlen(path) == 2 && path[1] == ':') && !IsDirExisted((const char *)path))
			{
				ret = ::_mkdir(path);
			}
			i = ptr - filePath + 1;
		}
	} while (ptr);

	return !ret;
}

bool UpFile::CreatePathW(const wchar_t* filePath)
{
	char* path = UpStringUtility::WcharToChar(filePath);
	bool ret = CreatePath(path);
	delete path;
	path = NULL;
	return ret;
}

bool UpFile::IsDirExisted(const char* dirName)
{
	bool isExisted = false;
	DWORD dwAttr = GetFileAttributesA(dirName);
	if (dwAttr != -1 && (dwAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		isExisted = true;
	}
	return isExisted;
}
bool UpFile::IsDirExistedW(const wchar_t* dirName)
{
	bool isExisted = false;
	DWORD dwAttr = GetFileAttributesW(dirName);
	if (dwAttr != -1 && (dwAttr&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		isExisted = true;
	}
	return isExisted;
}

std::string UpFile::GetPath(const char * fileName)
{
	std::string ret;
	if (fileName == NULL)
	{
		return ret;
	}
	std::string dir;
	dir.append(fileName);
	int index = dir.find_last_of('\\');
	if (index != -1)
	{
		if (index == dir.length() - 1)
		{
			return dir;
		}
		else
		{
			std::string ret = dir.substr(0, index + 1);
			return ret;
		}
	}
	return ret;
}

std::wstring UpFile::GetPathW(const wchar_t* fileName)
{
	std::wstring ret;
	if (fileName == NULL)
	{
		return ret;
	}
	std::wstring dir;
	dir.append(fileName);
	int index = dir.find_last_of('\\');
	if (index != -1)
	{
		if (index == dir.length() - 1)
		{
			return dir;
		}
		else
		{
			std::wstring ret = dir.substr(0, index + 1);
			return ret;
		}
	}
	return ret;

}

bool UpFile::IsFileExisted(const char* fileName)
{
	bool isExisted = false;
	DWORD dwAttr = GetFileAttributesA(fileName);   
	if (dwAttr!=-1&&(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
	{
		isExisted = true;
	}
	return isExisted; 
}

bool UpFile::IsFileExistedW(const wchar_t* fileName)
{
	bool isExisted = false;
	DWORD dwAttr = GetFileAttributesW(fileName);   
	if (dwAttr!=-1&&(dwAttr&FILE_ATTRIBUTE_DIRECTORY)==0)
	{
		isExisted = true;
	}
	return isExisted; 
}

bool UpFile::DeleteExistFile(const char* fileName)
{
	bool ret = !::DeleteFileA(fileName);
	return ret;
}

bool UpFile::DeleteExistFileW(const wchar_t* fileName)
{
	bool ret = !::DeleteFileW(fileName);
	return ret;
}

void UpFile::OpenURL(std::wstring url, bool use_ie)
{
	if (use_ie)
	{
		ShellExecute(NULL, L"Open", L"IEXPLORE", url.c_str(), NULL, SW_SHOW);
	}
	else
	{
		HINSTANCE Histance = ShellExecute(NULL, L"Open", url.c_str(), NULL, NULL, SW_SHOW);
		if ((int)Histance == SE_ERR_NOASSOC)
		{
			WCHAR sysDir[MAX_PATH];
			::GetSystemDirectory(sysDir, MAX_PATH);
			std::wstring wsRundll, wsParam;
			wsRundll.append(sysDir);
			wsRundll.append(L"\\rundll32.exe");
			wsParam.append(sysDir);
			wsParam.append(L"\\shell32.dll,OpenAs_RunDLL ");
			wsParam.append(url);
			ShellExecute(NULL, L"Open", wsRundll.c_str(), wsParam.c_str(), NULL, SW_SHOW);
		}
	}
}