// ExtractorTabelView.cpp : 实现文件
//
#include "stdafx.h"
#include <string>
#include <vector>
#include <fstream>
#include <io.h>  
#include "tinyxml2.h"
#include "Utils/StringUtils.h"
#include "Utils/XMLUtil.h"
#include "ReadWriteExcel_MFC.h"
#include "ExtractorTabelView.h"
#include "MyProgressCtrl.h"
#include "afxdialogex.h"
#include "Utils/FileFinderHelper.h"
#include "ExtractorCtrl.h"

using namespace std;
using namespace tinyxml2;
#pragma comment(lib,"tinyxml2.lib")

// CExtractorTabelView 对话框

IMPLEMENT_DYNAMIC(CExtractorTabelView, CDialogEx)

CExtractorTabelView::CExtractorTabelView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExtractorTabelView::IDD, pParent)
	, m_ResultPath(_T(""))
	, m_ProjectPath(_T(""))
	, m_FilterValue(_T(""))
	, m_ExtractCheckbox(FALSE)
	, m_bIsExtractUnfinished(FALSE)
	, m_bIsRemoveDuplicate(FALSE)
{
	
}

CExtractorTabelView::~CExtractorTabelView()
{
	delete m_ExtractorDataCtrl;
}

void CExtractorTabelView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RESULT_PATH_EDIT, m_ResultPath);
	DDX_Text(pDX, IDC_PROJECTPATH_EDIT, m_ProjectPath);
	DDX_Text(pDX, IDC_EXTRACTFILTER, m_FilterValue);
	DDX_Check(pDX, IDC_EXTRACT_ALL, m_ExtractCheckbox);
	DDX_Check(pDX, IDC_EXTRACT_UNFINISHED, m_bIsExtractUnfinished);
	DDX_Check(pDX, IDC_CHECK1, m_bIsRemoveDuplicate);
}


BEGIN_MESSAGE_MAP(CExtractorTabelView, CDialogEx)
	ON_BN_CLICKED(ID_SET_RESULTPATH_BTN, &CExtractorTabelView::OnBnClickedSetResultpathBtn)
	ON_BN_CLICKED(ID_SET_PROJECTPATH_BTN, &CExtractorTabelView::OnBnClickedSetProjectpathBtn)
	ON_BN_CLICKED(ID_EXTRACTOR, &CExtractorTabelView::OnBnClickedExtractor)
	ON_EN_CHANGE(IDC_EXTRACTFILTER, &CExtractorTabelView::OnEnChangeExtractfilter)
	ON_BN_CLICKED(IDC_EXTRACT_UNFINISHED, &CExtractorTabelView::OnBnClickedExtractUnfinished)
	ON_BN_CLICKED(IDC_EXTRACT_ALL, &CExtractorTabelView::OnBnClickedExtractAll)
END_MESSAGE_MAP()


// CExtractorTabelView 消息处理程序


void CExtractorTabelView::OnBnClickedSetResultpathBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	CString workingDirectory;
	wchar_t temp[256];
	GetEnvironmentVariable(_T("userprofile"), temp, 256);
	workingDirectory = temp;
	CFolderPickerDialog dlg(workingDirectory, 0, NULL, 0);
	if (dlg.DoModal())
	{
		m_ResultPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CExtractorTabelView::OnBnClickedSetProjectpathBtn()
{
	// TODO:  在此添加控件通知处理程序代码
	CString workingDirectory;
	wchar_t temp[256];
	GetEnvironmentVariable(_T("userprofile"), temp, 256);
	workingDirectory = temp;
	CFolderPickerDialog dlg(workingDirectory, 0, NULL, 0);
	if (dlg.DoModal())
	{
		m_ProjectPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CExtractorTabelView::OnBnClickedExtractor()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	ExtractType curType = Type_Default;
	if (m_bIsExtractUnfinished == TRUE)
	{
		curType = Type_ExtractUnfinished;
	}
	else if (m_ExtractCheckbox == TRUE)
	{
		curType = Type_ExtractAll;
	}
	m_ExtractorDataCtrl->SetExtractMode(curType);
    //设置是否去重
	m_ExtractorDataCtrl->SetIsRemoveDuplicate((m_bIsRemoveDuplicate == TRUE) ? true : false);
	if (!preChcek())
	{
		return;
	}
	m_ExtractorDataCtrl->DoExtract(m_ProjectPath.GetBuffer(), m_ResultPath.GetBuffer(), m_FilterValue.GetBuffer());
	//保存缓存
	SaveCache();
}


void CExtractorTabelView::OnEnChangeExtractfilter()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

void CExtractorTabelView::InitData()
{
	m_IsExtractAll = false;
	m_ExtractorDataCtrl = new CExtractorCtrl(this);
	//获取当前程序所在路径
	wstring strAppPath = CFileFinderHelper::GetAppPath();
	strAppPath = strAppPath + L"\\" + CONFIG_FILENAME;
	m_Cache = new CCache(strAppPath.c_str());
}

BOOL CExtractorTabelView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitData();
	LoadCacheData(m_Cache->GetExtractCache());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

bool CExtractorTabelView::preChcek()
{
	if (m_ProjectPath.IsEmpty() || m_ResultPath.IsEmpty())
	{
		MessageBox(L"请输入结果存放路径和工程所在路径", L"提示", MB_OK);
		return false;
	}
	return true;
}

void CExtractorTabelView::LoadCacheData(pExtactCacheInfo info)
{
	m_ProjectPath = CStringUtils::stringToCString(info->strProjectPath);
	m_ResultPath = CStringUtils::stringToCString(info->strResultPath);
	m_FilterValue = CStringUtils::stringToCString(info->strFilter);
	m_ExtractCheckbox = (CStringUtils::stringToCString(info->bIsExtractAll) == "true") ? TRUE : FALSE;
	UpdateData(FALSE);
	delete info;
}


void CExtractorTabelView::SaveCache()
{
	string strExcelFile;
	string strProjectPathTmp;
	string strFilter;
	CStringUtils::WStringToString(m_ResultPath.GetBuffer(), strExcelFile);
	CStringUtils::WStringToString(m_ProjectPath.GetBuffer(), strProjectPathTmp);
	CStringUtils::WStringToString(m_FilterValue.GetBuffer(), strFilter);
	m_Cache->SaveExtractCache(strExcelFile, strProjectPathTmp, strFilter, (m_ExtractCheckbox == TRUE)?true:false);
}

void CExtractorTabelView::OnBnClickedExtractUnfinished()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_bIsExtractUnfinished == TRUE)
	{
		GetDlgItem(IDC_EXTRACT_ALL)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EXTRACT_ALL))->SetCheck(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EXTRACT_ALL)->EnableWindow(TRUE);
	}
}


void CExtractorTabelView::OnBnClickedExtractAll()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_ExtractCheckbox == TRUE)
	{
		GetDlgItem(IDC_EXTRACT_UNFINISHED)->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EXTRACT_UNFINISHED))->SetCheck(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EXTRACT_UNFINISHED)->EnableWindow(TRUE);
	}
}
