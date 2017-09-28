#include "stdafx.h"
#include <locale>
#include <codecvt>
#include "StringUtils.h"


CStringUtils::CStringUtils()
{
}


CStringUtils::~CStringUtils()
{
}

BOOL CStringUtils::WStringToString(const std::wstring &wstr, std::string &str)
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

BOOL CStringUtils::StringToWString(const std::string &str, std::wstring &wstr)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
	{
		wstr = std::wstring(L"");
		return FALSE;
	}
	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
	{
		wstr = std::wstring(L"");
		return FALSE;
	}
	MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	wstr = std::wstring(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;
	return TRUE;
}

std::string CStringUtils::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}

std::string CStringUtils::trim(string& s)
{
	const string drop = " ";
	// trim right
	s.erase(s.find_last_not_of(drop) + 1);
	// trim left
	return s.erase(0, s.find_first_not_of(drop));
}

std::wstring CStringUtils::trim(wstring& s)
{
	const wstring drop = L" ";
	// trim right
	s.erase(s.find_last_not_of(drop) + 1);
	// trim left
	return s.erase(0, s.find_first_not_of(drop));
}

string CStringUtils::getFileName(string strFilePath)
{
	strFilePath = trim(strFilePath);
	string strFileName;
	int iIndex = strFilePath.find_last_of("\\");
	strFileName = strFilePath.substr(iIndex + 1);
	return strFileName;
}

std::string CStringUtils::ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8 < wchar_t > ;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}

char* CStringUtils::UnicodeToUtf8(const wchar_t* unicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	return szUtf8;
}

CString CStringUtils::stringToCString(string strInput)
{
	const wchar_t* cInput = CStringUtils::Utf8ToUnicode(strInput.c_str());
	CString strResult(cInput);
	return strResult;
}

const wchar_t* CStringUtils::Utf8ToUnicode(const char* utf8Str)
{
	int len;
	len = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0);
	wchar_t *szUtf8 = new wchar_t[len + 1];   //(wchar_t*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, szUtf8, len);
	return szUtf8;
}

//FontFlag CStringUtils::getStringSize(CString strText)
//{
//	//设置文本框字体  
//	//CFont* ptf = GetDlgItem(IDC_SOURCETEXT)->GetFont(); // 得到原来的字体    
//	//LOGFONT lf;
//	//ptf->GetLogFont(&lf);
//	//FontFlag fontSize;
//	//CSize strSize;
//	//CDC *pDC = GetDC();
//	//strSize = pDC->GetTextExtent(strText);
//	//ReleaseDC(pDC);
//	//fontSize.lWidth = strSize.cx;
//	//fontSize.lHeight = strSize.cy;
//	//return fontSize;
//}

CString CStringUtils::ReplaceEntitySymbols(CString strText)
{
	CString strRsult = strText;
	//替换常用的实体符号
	int iIndex = -1;
	iIndex = strRsult.Find(L"&rdquo;");
	if (iIndex != -1)
	{
		strRsult.Replace(L"&rdquo;", L"\"");
		iIndex = -1;
	}
	iIndex = strRsult.Find(L"&apos;");
	if (iIndex != -1)
	{
		strRsult.Replace(L"&apos;", L"\'");
		iIndex = -1;
	}
	iIndex = strRsult.Find(L"&quot;");
	if (iIndex != -1)
	{
		strRsult.Replace(L"&quot;", L"\"");
		iIndex = -1;
	}
	//对于文本中的特殊符号做特殊处理 + -  =
	if (strRsult.Find(L"+") == 0 || strRsult.Find(L"-") == 0 || strRsult.Find(L"=") == 0)
	{
		strRsult.Format(L"%c%s", L'\'', strRsult);
	}
	return strRsult;
}

std::wstring CStringUtils::RemoveLineBreaks(wstring wstrInput)
{
	int iIndex = -1;
	while ((iIndex = wstrInput.find_first_of(L"\r\n")) != wstring::npos)
	{
		wstrInput.replace(iIndex,2,L"");
	}
	return wstrInput;
}
