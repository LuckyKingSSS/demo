#include "stdafx.h"
#include "ExcelUtil.h"
#include "StringUtils.h"


CExcelUtil::CExcelUtil()
{
}


CExcelUtil::~CExcelUtil()
{
}

void CExcelUtil::PreLoadSheet()
{
	CRange used_range;
	used_range = m_sheet.get_UsedRange();
	VARIANT ret_ary = used_range.get_Value2();
	if (!(ret_ary.vt & VT_ARRAY))
	{
		return;
	}
	ole_safe_array_.Clear();
	ole_safe_array_.Attach(ret_ary);
}

CString CExcelUtil::GetCell(long iRow, long iColumn)
{
	COleVariant vResult;
	//字符串
	if (already_preload_ == FALSE)
	{
		m_range.AttachDispatch(m_sheet.get_Cells());
		m_range.AttachDispatch(m_range.get_Item(COleVariant((long)iRow), COleVariant((long)iColumn)).pdispVal, true);
		vResult = m_range.get_Value2();
	}
	//如果数据依据预先加载了
	else
	{
		long read_address[2];
		VARIANT val;
		read_address[0] = iRow;
		read_address[1] = iColumn;
		ole_safe_array_.GetElement(read_address, &val);
		vResult = val;
	}

	CString str;
	if (vResult.vt == VT_BSTR)//字符串
	{
		str = vResult.bstrVal;
	}
	else if (vResult.vt == VT_INT)
	{
		str.Format(_T("%d"), vResult.pintVal);
	}
	else if (vResult.vt == VT_R8)     //8字节的数字
	{
		str.Format(_T("%0.0f"), vResult.dblVal);
		//str.Format("%.0f",vResult.dblVal);
		//str.Format("%1f",vResult.fltVal);
	}
	else if (vResult.vt == VT_DATE)    //时间格式
	{
		SYSTEMTIME st;
		VariantTimeToSystemTime(vResult.date, &st);
		CTime tm(st);
		str = tm.Format(_T("%Y-%m-%d"));

	}
	else if (vResult.vt == VT_EMPTY)   //单元格空的
	{
		str = _T("");
	}

	m_range.ReleaseDispatch();
	TRACE(L"the raw string is:  %s\n", str);
	return str;
}

int CExcelUtil::GetColumnCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_sheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Columns(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

int CExcelUtil::GetRowCount()
{
	CRange range;
	CRange usedRange;
	usedRange.AttachDispatch(m_sheet.get_UsedRange(), true);
	range.AttachDispatch(usedRange.get_Rows(), true);
	int count = range.get_Count();
	usedRange.ReleaseDispatch();
	range.ReleaseDispatch();
	return count;
}

void CExcelUtil::QuitApp()
{
	m_ExcelApp.Quit();
	m_sheets.DetachDispatch();
	m_books.DetachDispatch();
	m_ExcelApp.DetachDispatch();
}

void CExcelUtil::ReadExcelFile(string strFilePath, map<CString, CString>& resultMap)
{
	LPDISPATCH lpDisp = NULL;
	if (CStringUtils::getFileName(strFilePath).find("~") != string::npos)
	{
		//对于已经打开的excel产生的临时excel文件做过滤
		return;
	}
	if (!m_ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("启动Excel服务器失败!"));
		return;
	}
	/*判断当前Excel的版本*/
	CString strExcelVersion = m_ExcelApp.get_Version();
	int iStart = 0;
	strExcelVersion = strExcelVersion.Tokenize(_T("."), iStart);
	if (_T("11") == strExcelVersion)
	{
		TRACE("*************:%s\n", _T("当前Excel的版本是2003。"));
	}
	else if (_T("12") == strExcelVersion)
	{
		TRACE("*************:%s\n", _T("当前Excel的版本是2007。"));
	}
	else
	{
		TRACE("*************:%s\n", _T("当前Excel的版本是其他版本"));
	}
	m_ExcelApp.put_Visible(FALSE);
	m_ExcelApp.put_UserControl(FALSE);
	m_books.AttachDispatch(m_ExcelApp.get_Workbooks());
	try
	{
		/*打开指定的工作簿*/
		CString strPathTmp(strFilePath.c_str());
		lpDisp = m_books.Open(strPathTmp,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing, vtMissing,
			vtMissing, vtMissing, vtMissing, vtMissing);
		m_book.AttachDispatch(lpDisp);
	}
	catch (...)
	{
		AfxMessageBox(L"open excel failed");
		//std::cout << "open excel failed" << endl;
		return;
	}
	m_sheets.AttachDispatch(m_book.get_Worksheets());
	lpDisp = m_book.get_ActiveSheet();
	//得到当前活跃sheet  
	//如果有单元格正处于编辑状态中，此操作不能返回，会一直等待  
	m_sheet.AttachDispatch(lpDisp);
	// VARIANT varRead = m_range.get_Value2();
	//PreLoadSheet();
	//读取excel的内容
	int iRowNum = GetRowCount();
	int iColumNum = GetColumnCount();
	for (int itera = 1; itera <= iRowNum; ++itera)
	{
		CString strKey, strValue;
		for (int j = 1; j <= iColumNum; ++j)
		{
			//cout << GetCell(itera, j);
			if (j == 1)
			{
				strKey = GetCell(itera, j);
			}
			if (j == 2)
			{
				strValue = GetCell(itera, j);
				strKey = strKey.Trim();
				strValue = strValue.Trim();
				if (!strValue.IsEmpty())
				{
					resultMap.insert(make_pair(strKey, strValue));
				}
			}
		}
	}
	//关闭excel
	QuitApp();
}

void CExcelUtil::WriteExcelCell(CString strCellPos, CString strValue, pCellFormatInfo pTextFormat /*= nullptr*/)
{
    m_range = m_sheet.get_Range(_variant_t(strCellPos.GetBuffer()), vtMissing);
	strCellPos.ReleaseBuffer();
	VARIANT  varWrite = (_variant_t)(strValue.GetBuffer());
	strValue.ReleaseBuffer();
	//设置字体颜色
	if (pTextFormat != NULL)
	{
		InteriorPtr interior = m_range.get_Interior();
		interior->PutColor(pTextFormat->m_TextColor);
		//设置字体大小和style
		Excel::FontPtr cellFont = m_range.get_Font();
		//cellFont->PutBackground(RGB(0,0,0));
		//cellFont->PutName(pTextFormat->m_FontName.c_str());
		cellFont->PutFontStyle(pTextFormat->m_FontStyle.c_str());
		cellFont->PutSize(pTextFormat->m_FontSize);
		//释放内存
		delete pTextFormat;
		pTextFormat = NULL;
	}
	m_range.put_Value2(varWrite);
}

void CExcelUtil::WriteExcelRangeCell(CString strStartCellPos, CString strEndCellpos, void ** pDataArray, pCellFormatInfo pTextFormat /*= nullptr*/)
{

}

BOOL CExcelUtil::CreateApp()
{
	if (!m_ExcelApp.CreateDispatch(_T("Excel.Application"), NULL))
	{
		return FALSE;
	}
	m_ExcelApp.put_Visible(FALSE);
	return TRUE;
}

BOOL CExcelUtil::CreateWorkbooks()
{
	if (FALSE == CreateApp())
	{
		return FALSE;
	}
	m_books = m_ExcelApp.get_Workbooks();
	if (!m_ExcelApp.m_lpDispatch)
	{
		AfxMessageBox(L"WorkBooks创建失败!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}

BOOL CExcelUtil::CreateWorkbook()
{
	if (!m_books.m_lpDispatch)
	{
		AfxMessageBox(L"WorkBooks为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_book = m_books.Add(vOptional);
	if (!m_book.m_lpDispatch)
	{
		AfxMessageBox(L"WorkBook为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}

BOOL CExcelUtil::CreateWorksheets()
{
	if (!m_book.m_lpDispatch)
	{
		AfxMessageBox(L"WorkBook为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	m_sheets = m_book.get_Sheets();
	if (!m_sheets.m_lpDispatch)
	{
		AfxMessageBox(L"WorkSheets为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}

BOOL CExcelUtil::CreateSheet(short index)
{
	if (!m_sheets.m_lpDispatch)
	{
		AfxMessageBox(L"WorkSheets为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	m_sheet = m_sheets.get_Item(COleVariant(index));
	if (!m_sheet.m_lpDispatch)
	{
		AfxMessageBox(L"WorkSheet为空!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}

BOOL CExcelUtil::Create(short index)
{
	if (CreateWorkbooks() == FALSE)
	{
		return FALSE;
	}
	if (CreateWorkbook() == FALSE)
	{
		return FALSE;
	}
	if (CreateWorksheets() == FALSE)
	{
		return FALSE;
	}
	if (CreateSheet(index) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

void CExcelUtil::ShowApp()
{
	m_ExcelApp.put_Visible(TRUE);
}

void CExcelUtil::HideApp()
{
	m_ExcelApp.put_Visible(FALSE);
}

BOOL CExcelUtil::SaveWorkbook()
{
	if (!m_book.m_lpDispatch)
	{
		AfxMessageBox(L"Book获取失败!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	m_book.Save();
	return TRUE;
}

BOOL CExcelUtil::SaveWorkbookAs(CString fileName)
{
	if (!m_book.m_lpDispatch)
	{
		AfxMessageBox(L"Book获取失败!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	COleVariant vTrue((short)TRUE),
		vFalse((short)FALSE),
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant vFileName((fileName));

	m_book.SaveAs(
		vFileName,    //VARIANT* FileName  
		vOptional,    //VARIANT* FileFormat  
		vOptional,    //VARIANT* LockComments  
		vOptional,    //VARIANT* Password  
		vOptional,    //VARIANT* AddToRecentFiles  
		vOptional,    //VARIANT* WritePassword  
		0,    //VARIANT* ReadOnlyRecommended  
		vOptional,    //VARIANT* EmbedTrueTypeFonts  
		vOptional,    //VARIANT* SaveNativePictureFormat  
		vOptional,    //VARIANT* SaveFormsData  
		vOptional,    //VARIANT* SaveAsAOCELetter  
		vOptional    //VARIANT* ReadOnlyRecommended  
		/*                vOptional,    //VARIANT* EmbedTrueTypeFonts
		vOptional,    //VARIANT* SaveNativePictureFormat
		vOptional,    //VARIANT* SaveFormsData
		vOptional    //VARIANT* SaveAsAOCELetter*/
		);
	return    TRUE;
}

BOOL CExcelUtil::CloseWorkbook()
{
	COleVariant vTrue((short)TRUE),
		vFalse((short)FALSE),
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	m_book.Close(vFalse,    // SaveChanges.  
		vTrue,            // OriginalFormat.  
		vFalse            // RouteDocument.  
		);
	m_book = m_ExcelApp.get_ActiveWorkbook();
	if (!m_book.m_lpDispatch)
	{
		AfxMessageBox(L"Book获取失败!", MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if (CreateSheet(1) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

void CExcelUtil::CloseApp()
{
	SaveWorkbook();
	COleVariant vTrue((short)TRUE),
		vFalse((short)FALSE),
		vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	m_ExcelApp.Quit();
	if (m_range.m_lpDispatch)
		m_range.ReleaseDispatch();
	if (m_sheet.m_lpDispatch)
		m_sheet.ReleaseDispatch();
	if (m_sheets.m_lpDispatch)
		m_sheets.ReleaseDispatch();
	if (m_book.m_lpDispatch)
		m_book.ReleaseDispatch();
	if (m_books.m_lpDispatch)
		m_books.ReleaseDispatch();
	if (m_ExcelApp.m_lpDispatch)
		m_ExcelApp.ReleaseDispatch();
}
