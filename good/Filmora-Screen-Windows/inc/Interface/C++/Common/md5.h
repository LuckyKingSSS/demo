///////////////////////////////////////////////////////////////////////////
// Md5.h
//==========================================================================
// 功能    ： Md5
// 模块	   ： PSB项目WS_Log.dll模块 
// 创建时间： 2006-08-23
// 创建者  ： 
// 修订    ： 
//==========================================================================

#pragma once

int __stdcall GetDataMD5(const BYTE* pBuf, UINT nLength, char szBuf[64]);
int __stdcall GetFileMD5(const char* pFileName, char szBuf[64]);
const char* __stdcall GetFileMD5Ex(const char* pFileName);

// 启动校验和检测
// 返回成功表示已经启动检测
// 请确保pFileName指针一直有效
int __stdcall StartCheckSum(const wchar_t* pFileName); 

int __stdcall StartCheckMD5(const wchar_t* pFileName, const char* pMD5); 

