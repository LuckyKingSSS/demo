///////////////////////////////////////////////////////////////////////////
// DrawBorder.h
//==========================================================================
// 功能    ： 画窗口边框，界面贴图用，只需要一张缩略轮廓图
// 模块	   ： PSB项目WS_Log.dll模块 
// 创建时间： 2006-09-04
// 创建者  ： 杨长元
// 修订    ： 
//==========================================================================

#pragma once

// 画位图边框
// pDstRect - 目标矩形
// pBorderSize - 边框大小 right的值表示右边框尺寸 bottom的值表示下边框尺寸
// lpbi - 位图数据信息头指针
// pBits - 位图数据数据块指针 为0表示位图数据和信息头为一整体
// pSrcRect - 位图数据源矩形位置，为0表示使用最边上部分
// uBmpID - 位图资源ID
// hResModule - 位图资源所在的模块句柄
BOOL __stdcall DrawBorderToWnd (HDC hDC, RECT * pDstRect, RECT * pBorderSize,	LPBITMAPINFO lpbi, BYTE* pBits = 0, RECT* pSrcRect = 0);
BOOL __stdcall DrawBorderToWndFromResource (HDC hDC, RECT * pDstRect, RECT * pBorderSize,	UINT uBmpID, HMODULE hResModule = 0, RECT* pSrcRect = 0);

