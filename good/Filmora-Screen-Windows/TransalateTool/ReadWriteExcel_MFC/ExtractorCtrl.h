#pragma once
#include <string>
#include <vector>
#include <map>

class CMyProgressCtrl;
class CWnd;
class CExtractorCtrl: public CObject
{
public:
	enum DuplicateFlag
	{
        Flag_None = 0, //不存在重复
		Flag_Exsit    //存在重复
	};
	CExtractorCtrl(CWnd* pParentWnd);
	~CExtractorCtrl();
public:
	void DoExtract(std::wstring strProjectPath, std::wstring strResultPath, std::wstring strFilter);
	vector<string> getTsFileByLanguage(string strLanguageType);
	void SetExtractMode(ExtractType type){ m_CurExtractType = type; };
	void SetIsRemoveDuplicate(bool bIsRemoveDuplicate){ m_IsRemoveDuplicate = bIsRemoveDuplicate; };


private:
	void InitData();
	std::string getTsFileType(wstring strFileName);
	void RestoreParams();//还原参数
	void ClearResultFile();
	void ExtractSingleFile(wstring strLanguangeType, wstring strFilePath);
	void DivideTsArray(); //将所有的ts文件按照语言类型做拆分
	void AddExtractInfo(CString strText); //将当前已经提取的信息加入
	bool IsDuplicate(CString strKeyText);
	bool SetCurNodeState(string strNodeText);

private:
	//内部逻辑的字符串使用wstring 方便使用
	std::wstring m_ExtractProjectPath;
	std::wstring m_ExtractResultPath;
	std::wstring m_ExtractFilter;

	vector<string> m_AllEnTsFile; //当前项目下所有的英文版本的ts文件集合
	vector<string> m_AllTsFile; //所有的ts文件的集合
	CMyProgressCtrl* progress;
	ExtractType m_CurExtractType; //当前的提取类型
	bool  m_bIsFullExtract; //是否提取所有待翻译的文本
	CWnd* m_ParentWnd;
	bool m_IsExtracted; //是否曾经被提取过
	map<wstring,wstring> m_AllResultFileMap;//当前所有的翻译结果文件所在的位置（key: 语言类型，value: 文件所在的路径）
	map<wstring, vector<wstring>> m_AllTsFileMap;
	vector<wstring>  m_AllLanguageType;
	map<CString, DuplicateFlag>  m_AllHaveExtractMap; //当前已经提取的文本
	bool m_IsRemoveDuplicate; //提取文本的时候是否使用去重模式
};

