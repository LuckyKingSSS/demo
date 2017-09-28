#pragma once
#include "afxcmn.h"
#include "ExtractorTabelView.h"
#include "ReadWriteExcel_MFCDlg.h"


// CToolMainDlg 对话框

class CToolMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CToolMainDlg)

public:
	CToolMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolMainDlg();

// 对话框数据
	enum { IDD = IDD_READWRITEEXCEL_MFC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_TableCtl;
	virtual BOOL OnInitDialog();
private:
	CExtractorTabelView  m_ExtractorView;
	CReadWriteExcel_MFCDlg m_TranslateView;
public:
	afx_msg void OnTcnSelchangeTabctl(NMHDR *pNMHDR, LRESULT *pResult);
};
