#pragma once 
#include <map>
#include <string>

class CWnd;
class CMyProgressCtrl;
class CExcelUtil;
class CCache;
class CTranslateCtrl
{
public:
	CTranslateCtrl(CWnd* pParentWnd);
	~CTranslateCtrl();

public:
	void DoTranslate(std::wstring strExcelFilePath, std::wstring strProjectPath, std::wstring strTranslateFilter);
	void SetReplaceType(bool bType){ bIsForcedReplace = bType; };//设置替换模式


private:
	void InitData();
	std::string getTsFileType(std::string strFileName);
	std::string TraslateRawData(std::string strRawData, std::string strType);
	//获取特定路径下的所有ts文件的内容
	void ReadAllExcelFile(wstring strExcelFilePath);
    //获取翻译文件对应的语言类型
	std::string GetTranslateFileLanguage(std::string strLanguageType);
	//翻译特定的ts文件
	void TranslateSingleFile(string strFilePath);
	//过滤部分已经翻译过的文件
	bool FilterTranslateFile(const string &strLangugeType, const std::wstring &strTranslateFilter);
	void SaveUnMatchFile();//保存没有成功替换的文本
	void ReportTranslateResult();//报告替换结果
	void RestoreParams();//再次翻译的时候将系统参数还原
	void GetExcelExtraData();//统计Excel中存在但是在ts文件中不存在文本

private:
	CWnd* m_pParentWnd;
	CMyProgressCtrl* progress;
	CString m_CurrentHandleTsFile;
	CString m_CurrentHandleTsPath;
	list<CString> m_ReplaceList;   //当前具备excel文件的语言类型集合
	vector<string> m_AllExcelFile; //所有的excel文件的集合
	map<std::string, map<CString, CString>> m_AllLanguageMap;
	map<std::string, map<CString, CString>> m_ExtraExcelTexts;//Excel文档相较于Ts文件中多的文本的集合
	multimap<CString, CString> m_UnMatchMap;//未匹配到的字符串的字典
	multimap<string, string> m_UnFinishStateTexts; //当前所有状态不正确的文本的集合
	multimap<string, string>  m_AllVanishTexts;//ts中存在但是在代码中被删除的字符串集合
	bool bIsForcedReplace;
	CExcelUtil* m_ExcelHelper;
	wstring m_UnMatchTextFilePath;
};
