
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//#import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\OFFICE12\\MSO.DLL" \
//	rename("RGB", "MSORGB") \
//	rename("DocumentProperties", "MSODocumentProperties")
//using namespace Office;
//#import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\VBA\VBA6\\VBE6EXT.OLB"
//
//using namespace VBIDE;
//#import "E:\\Program Files (x86)\\Microsoft Office\\Office12\\EXCEL.EXE" \
//	rename("DialogBox", "ExcelDialogBox") \
//	rename("RGB", "ExcelRGB") \
//	rename("CopyFile", "ExcelCopyFile") \
//	rename("ReplaceText", "ExcelReplaceText") \
//	no_auto_exclude
//using namespace Excel;




#if 1
#import "C:\\Program Files (x86)\\Common Files\\microsoft shared\\OFFICE14\\MSO.DLL" \
	rename("RGB", "MSORGB") \
	rename("DocumentProperties", "MSODocumentProperties")
using namespace Office;
#import "C:\\Program Files (x86)\\Common Files\\microsoft shared\\VBA\\VBA6\\VBE6EXT.OLB"

using namespace VBIDE;
#import "C:\\Program Files (x86)\\Microsoft Office\\Office14\\EXCEL.EXE" \
	rename("DialogBox", "ExcelDialogBox") \
	rename("RGB", "ExcelRGB") \
	rename("CopyFile", "ExcelCopyFile") \
	rename("ReplaceText", "ExcelReplaceText") \
	no_auto_exclude
using namespace Excel;
#else if 0

#import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\OFFICE12\\MSO.DLL" \
	rename("RGB", "MSORGB") \
	rename("DocumentProperties", "MSODocumentProperties")
using namespace Office;
#import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\VBA\VBA6\\VBE6EXT.OLB"

using namespace VBIDE;
#import "E:\\Program Files (x86)\\Microsoft Office\\Office12\\EXCEL.EXE" \
	rename("DialogBox", "ExcelDialogBox") \
	rename("RGB", "ExcelRGB") \
	rename("CopyFile", "ExcelCopyFile") \
	rename("ReplaceText", "ExcelReplaceText") \
	no_auto_exclude
using namespace Excel;
#endif


//使用gdi+处理gif图片
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")


enum AppType
{
	Type_Translator,   //替换器
	Type_Extrctor      //文本提取器
};

enum ExtractType
{
   Type_Default = 0,      //默认提取模式
   Type_ExtractAll,       //提取所有
   Type_ExtractUnfinished,//提取状态为Unfinished状态
};

//字体大小标志
typedef struct
{
	long lWidth;
	long lHeight;
}FontFlag;

#define  CONFIG_FILENAME L"UserParam.xml"