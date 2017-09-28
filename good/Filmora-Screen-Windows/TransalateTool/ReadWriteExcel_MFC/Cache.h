#pragma once
#include <string>
using namespace std;

typedef struct tagCacheInfo
{
	string strExcelPath;
	string strProjectPath;
	string strFilter;
	string strIsForceReplace;
}TraslateCacheInfo, *pTranslateCacheInfo;

typedef	 struct tagExtactCacheInfo
{
	string strResultPath;
	string strProjectPath;
	string strFilter;
	string bIsExtractAll;
}ExtactCacheInfo, *pExtactCacheInfo;


class CXMLUtil;
class CCache
{
public:
	CCache(wstring strCachePath);
	~CCache();
public:
	bool SaveTranslateCache(string strExcelPath,string strProjectPath,string strFilter,bool bIsForceReplace);
	bool SaveExtractCache(string strResultPath, string strProjectPath, string strFilter, bool bIsExtractAll);
	pTranslateCacheInfo GetTranslateCache();
	pExtactCacheInfo  GetExtractCache();

	
private:
	void LoadXmlData(wstring wstrPath);

private:
	CXMLUtil* m_XmlDoc;
	wstring m_CachePath;
};

