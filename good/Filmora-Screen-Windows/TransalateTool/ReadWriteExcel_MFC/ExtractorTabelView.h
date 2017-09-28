#pragma once
#include <string>
#include <vector>
#include <map>
#include "Cache.h"

using namespace std;

// CExtractorTabelView 对话框
class CMyProgressCtrl;
class CExtractorCtrl;
class CExtractorTabelView : public CDialogEx
{
	DECLARE_DYNAMIC(CExtractorTabelView)

public:
	CExtractorTabelView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExtractorTabelView();

// 对话框数据
	enum { IDD = IDD_EXTRACTORVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetResultpathBtn();
	afx_msg void OnBnClickedSetProjectpathBtn();
	afx_msg void OnBnClickedExtractor();
	afx_msg void OnEnChangeExtractfilter();
private:
	void InitData();
	bool preChcek();
	//加载缓存数据
	void LoadCacheData(pExtactCacheInfo info);
	void SaveCache();//保存缓存
private:
	CString m_ResultPath;
	CString m_ProjectPath;
	CString m_FilterValue;
	bool m_IsExtractAll; //是否提取多有的字符
	CExtractorCtrl* m_ExtractorDataCtrl;
public:
	virtual BOOL OnInitDialog();
	BOOL m_ExtractCheckbox;
	CCache* m_Cache;
	BOOL m_bIsExtractUnfinished; //是否仅仅提取unfinished状态的文本
	afx_msg void OnBnClickedExtractUnfinished();
	afx_msg void OnBnClickedExtractAll();
	BOOL m_bIsRemoveDuplicate; //是否对提取的内容去重
};
