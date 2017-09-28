#pragma once
#include ".\excelReader\Application.h"
#include ".\excelReader\Workbooks.h"
#include ".\excelReader\Workbook.h"
#include ".\excelReader\Worksheets.h"
#include ".\excelReader\Worksheet.h"
#include ".\excelReader\Range.h"
#include <string>
#include <map>

using namespace std;

//单元格格式信息
typedef struct tagCellFormatInfo
{
	DWORD  m_TextColor;   //文字的颜色
	string m_FontName;    //字体名字
	string m_FontStyle;   //字体样式
	int    m_FontSize;    //字体大小
	tagCellFormatInfo(DWORD dTextColor, string strFontName, string strFontStyle, int iFontSize)
	{
		m_TextColor = dTextColor;
		m_FontName = strFontName;
		m_FontStyle = strFontStyle;
		m_FontSize = iFontSize;
	}
}CellFormatInfo, *pCellFormatInfo;

class CExcelUtil
{
public:
	CExcelUtil();
	~CExcelUtil();

public:
//**********************读 写 Excel 操作*************************************  
    /*
    *  功能：读取excel中前两列的内容，并组成map结构 
    /*  string strFilePath: excel 文件所在的路径
    */         
	void ReadExcelFile(string strFilePath, map<CString, CString>& resultMap);
    //在特定位置写入特定的文本信息
	void WriteExcelCell(CString strCellPos, CString strValue, pCellFormatInfo pTextFormat = nullptr);
	//在特定的区域类写文本信息
	void WriteExcelRangeCell(CString strStartCellPos, CString strEndCellpos, void ** pDataArray, pCellFormatInfo pTextFormat = nullptr);



//**********************创建新EXCEL*******************************************  
	//创建App对象
	BOOL CreateApp();
	//创建工作簿集合对象
	BOOL CreateWorkbooks();
	//创建工作簿对象
	BOOL CreateWorkbook();
	//创建工作表集合
	BOOL CreateWorksheets();
	//创建一个工作表
	BOOL CreateWorksheet(short index);
	
	BOOL CreateSheet(short index);
	//创建新的EXCEL应用程序并创建一个新工作簿和工作表  
	BOOL Create(short index);
	void ShowApp(); //显示EXCEL文档  
	void HideApp(); //隐藏EXCEL文档  

//**********************保存文档*********************************************  
	BOOL SaveWorkbook();                   //Excel是以打开形式，保存。  
	BOOL SaveWorkbookAs(CString fileName); //Excel以创建形式，保存。  
	BOOL CloseWorkbook();
	void CloseApp();


private:
	void PreLoadSheet();
	CString GetCell(long iRow, long iColumn);
	int GetColumnCount();
	int GetRowCount();
	void QuitApp();

private:
	CApplication m_ExcelApp;
	CWorkbooks m_books;
	CWorkbook m_book;
	CWorksheets m_sheets;
	CWorksheet m_sheet;
	CRange m_range;
	///是否已经预加载了某个sheet的数据
	BOOL          already_preload_;
	///Create the SAFEARRAY from the VARIANT ret.
	COleSafeArray ole_safe_array_;
	map<wstring, map<wstring, wstring>> m_TranslateRsult;
};

