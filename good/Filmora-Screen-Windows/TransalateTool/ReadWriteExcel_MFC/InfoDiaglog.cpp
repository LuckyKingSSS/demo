// InfoDiaglog.cpp : 实现文件   Ampliar/Diminuir a janela de pré-visualização
//

#include "stdafx.h"
#include <iostream>
#include "ReadWriteExcel_MFC.h"
#include "InfoDiaglog.h"
#include "afxdialogex.h"


// CInfoDiaglog 对话框

IMPLEMENT_DYNAMIC(CInfoDiaglog, CDialogEx)

CInfoDiaglog::CInfoDiaglog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInfoDiaglog::IDD, pParent)
{

}

CInfoDiaglog::~CInfoDiaglog()
{
}

void CInfoDiaglog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInfoDiaglog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInfoDiaglog::OnBnClickedOk)
	ON_MESSAGE(WM_UNMATCH_TEXT, &CInfoDiaglog::OnCloseInfoDlg)
END_MESSAGE_MAP()


// CInfoDiaglog 消息处理程序


void CInfoDiaglog::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CButton* checkbox = (CButton*)GetDlgItem(IDC_CHECKBOX);
	if (checkbox->GetCheck())
	{
        //使用notepad打开文本文件
		std::wstring strCmdLine = L"notepad.exe ";
		strCmdLine.append(m_UnMatchFilePath.GetString());
		std::string strCmd;
		WStringToString(strCmdLine, strCmd);
		system(strCmd.c_str());
	}
	CDialogEx::OnOK();
}

LRESULT CInfoDiaglog::OnCloseInfoDlg(WPARAM wParam, LPARAM lParam)
{
	CString* msg = (CString*)lParam;
	m_InfoText = *(CString*)wParam;
	m_UnMatchFilePath = *msg;
	GetDlgItem(IDC_LOG_TEXT)->SetWindowText(m_InfoText);
	UpdateData(FALSE);
	return 0;
}

BOOL CInfoDiaglog::WStringToString(const std::wstring &wstr, std::string &str)
{
	int nLen = (int)wstr.length();
	DWORD num = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), -1, NULL, 0, NULL, 0);
	str.resize(num + 1, ' ');
	int nResult = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)wstr.c_str(), nLen, (LPSTR)str.c_str(), num, NULL, NULL);
	if (nResult == 0)
	{
		return FALSE;
	}
	return TRUE;

	/*size_t   i;
	unsigned len = wstr.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs_s(&i, p, len, wstr.c_str(), len);
	str = p;
	delete[] p;
	return TRUE;*/
}

BOOL CInfoDiaglog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
