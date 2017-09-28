//
// 定义发布预览窗口接口
//

#pragma once



#ifdef WIN32

	typedef HWND HSUBVIEW;
	typedef HWND HVIEW;
#else
	typedef void * HSUBVIEW;
	typedef WindowRef HVIEW;
	
#endif

#include "YUVImage.h"

typedef void *HEXPORTVIEWER;


#ifdef	__cplusplus
extern "C" {
#endif

//创建窗口，hParent——父窗口，rcPos——窗口区域
HEXPORTVIEWER __stdcall EVCreate(HVIEW hParentWnd, LPRECT prcPos);

//销毁窗口
void __stdcall EVDestroy(HEXPORTVIEWER hViewer);

//显示设备无关位图
int __stdcall EVShowDib(HEXPORTVIEWER hViewer, const LPBITMAPINFO lpbi, const LPBYTE pBits = 0);

//显示YUV图像
int __stdcall EVShowImage(HEXPORTVIEWER hViewer, const LPBITMAPINFO lpbi, const LPBYTE pBits = 0);

//显示YUVImage图像
int __stdcall EVShowYUVImage(HEXPORTVIEWER hViewer, const YUVImage * pImage);

// 取得预览窗口HWND句柄
HSUBVIEW __stdcall EVGetHwnd(HEXPORTVIEWER hViewer);


#ifdef	__cplusplus
}
#endif

