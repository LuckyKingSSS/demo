// ToolMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ReadWriteExcel_MFC.h"
#include "ToolMainDlg.h"
#include "afxdialogex.h"


// CToolMainDlg 对话框

IMPLEMENT_DYNAMIC(CToolMainDlg, CDialogEx)

CToolMainDlg::CToolMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CToolMainDlg::IDD, pParent)
{

}

CToolMainDlg::~CToolMainDlg()
{
}

void CToolMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TABCtl, m_TableCtl);
}


BEGIN_MESSAGE_MAP(CToolMainDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABCtl, &CToolMainDlg::OnTcnSelchangeTabctl)
END_MESSAGE_MAP()


// CToolMainDlg 消息处理程序


BOOL CToolMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_TableCtl.InsertItem(0, L"Translate");
	m_TableCtl.InsertItem(1, L"Extractor");

	m_ExtractorView.Create(IDD_EXTRACTORVIEW, GetDlgItem(IDC_TABCtl));
	m_TranslateView.Create(IDD_TRANSLATEVIEW, GetDlgItem(IDC_TABCtl));

	//获得IDC_TABTEST客户区大小 
	CRect rs;
	m_TableCtl.GetClientRect(&rs);
	//调整子对话框在父窗口中的位置 
	rs.top += 20;
	//rs.bottom -= 60;
	//rs.left += 10;
	//rs.right -= 2;

	//设置子对话框尺寸并移动到指定位置 
	m_ExtractorView.MoveWindow(&rs);
	m_TranslateView.MoveWindow(&rs);
	//分别设置隐藏和显示 
	m_TranslateView.ShowWindow(true);
	m_ExtractorView.ShowWindow(false);
	//设置默认的选项卡 
	m_TableCtl.SetCurSel(0);

	//修改主程序大小
	SetWindowPos(NULL, 0, 0, 650, 400, SWP_NOZORDER | SWP_NOMOVE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}


void CToolMainDlg::OnTcnSelchangeTabctl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	int iCurSel = m_TableCtl.GetCurSel();
	switch (iCurSel)
	{
	case 0:
		m_TranslateView.ShowWindow(true);
		m_ExtractorView.ShowWindow(false);
		break;
	case 1:
		m_TranslateView.ShowWindow(false);
		m_ExtractorView.ShowWindow(true);
		break;
	default:
		break;
	}
}
