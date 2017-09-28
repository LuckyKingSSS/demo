// OutputControlUI.h

#pragma once

#include "EncodeParam.h"

typedef void* HOUTPUTCTRL;
typedef void* HMENUDATA;
typedef void* HTITLELIST;

HOUTPUTCTRL __stdcall OCCreateCtrl();

void __stdcall OCDestroyCtrl(HOUTPUTCTRL hCtrl);

void __stdcall OCRegisterMsgHwnd(HOUTPUTCTRL hCtrl, HWND hMsgWnd);

void __stdcall OCSetMenuData(HOUTPUTCTRL hCtrl, HMENUDATA hMenuData);

void __stdcall OCSetTitleList(HOUTPUTCTRL hCtrl, HTITLELIST hTitleList);

void __stdcall OCSetTempPath(HOUTPUTCTRL hCtrl, const wchar_t* szTempPath);

void __stdcall OCSetDvdFolder(HOUTPUTCTRL hCtrl, const wchar_t* szDvdFolder);

void __stdcall OCSetEncParam(HOUTPUTCTRL hCtrl, const EncodeParam* pEncParam);

BOOL __stdcall OCStartProcess(HOUTPUTCTRL hCtrl);

DWORD __stdcall OCWait(HOUTPUTCTRL hCtrl, UINT uMilliSecondsTimeOut = 0xFFFFFFFF);

int __stdcall OCPause(HOUTPUTCTRL hCtrl);

int __stdcall OCResume(HOUTPUTCTRL hCtrl);

int __stdcall OCAbort(HOUTPUTCTRL hCtrl, WPARAM wParam = 0, LPARAM lParam = 0);

void __stdcall OCInitEncParam(EncodeParam* pEncParam);


