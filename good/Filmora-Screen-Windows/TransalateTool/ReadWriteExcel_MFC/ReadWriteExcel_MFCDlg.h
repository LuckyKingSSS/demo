
// ReadWriteExcel_MFCDlg.h : 头文件
//

#pragma once
#include <map>
#include <list>
#include <vector>
#include ".\excelReader\Application.h"
#include ".\excelReader\Workbooks.h"
#include ".\excelReader\Workbook.h"
#include ".\excelReader\Worksheets.h"
#include ".\excelReader\Worksheet.h"
#include ".\excelReader\Range.h"
#include "tinyxml2.h"
#include "afxwin.h"
#include "Cache.h"
using namespace std;
class CMyProgressCtrl;
class CTranslateCtrl;

#define WM_UNMATCH_TEXT (WM_USER+100)


// CReadWriteExcel_MFCDlg 对话框
class CReadWriteExcel_MFCDlg : public CDialogEx
{
// 构造
public:
	CReadWriteExcel_MFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TRANSLATEVIEW };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedFindsource();
    afx_msg void OnBnClickedSetresultpath();
    afx_msg void OnBnClickedTranslate();

	

	afx_msg void OnClose();
private:
	//初始化成员变量
	void initData();
	//初始化界面
	void initUI();
	//获取特定字体的长度
	FontFlag getStringSize(CString strText);
	//显示特定的tooltip
	void setToolTip();
   //翻译前的预检查
	bool preChcek();
	//加载缓存数据
	void LoadCacheData(pTranslateCacheInfo info);
	//保存当前的缓存数据
	void SaveCache();

private:
    CString m_SourceFilePathName;//存储翻译对应关系的excel文件
    CString m_ResultFilePathName; //需要被翻译文件的绝对路径
	CString        m_FilterValue;       //过滤器的值用户过滤特定的ts文件
	list<CString>  m_AllFilter;         //当前支持的过滤参数
	CButton*         m_ExtractorButton; //提取按钮
	CToolTipCtrl     m_Tooltip;         //显示tooltip
	// excel文件所在文件夹
	CStatic m_SourcePathText;
	// ts文件所在文件夹
	CStatic m_ResultPathText;
public:
	afx_msg void OnChangeFilterbox();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	CTranslateCtrl* m_TranslateCtrl;
	BOOL m_IsReplaceAll;      
	CString m_strExcelPath;
	CString m_strProjectPath;
	CString m_strFilter;
	CCache* m_TranslateCache; //缓存参数
};
