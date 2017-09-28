#pragma once


// CInfoDiaglog 对话框
#define WM_UNMATCH_TEXT (WM_USER+100)
class CInfoDiaglog : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoDiaglog)

public:
	CInfoDiaglog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInfoDiaglog();
	BOOL WStringToString(const std::wstring &wstr, std::string &str);

// 对话框数据
	enum { IDD = IDD_INFO_DIAOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnCloseInfoDlg(WPARAM wParam, LPARAM lParam);

private:
	CString m_UnMatchFilePath;
	CString m_InfoText;
public:
	virtual BOOL OnInitDialog();
};
