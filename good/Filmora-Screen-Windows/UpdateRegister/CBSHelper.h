#pragma once

#include <Unknwn.h>

// 提交检测的类型
enum CBSRegCheckType
{
	// 启动时检测
	rcCheckOnStart = 0,
	// 注册时检测
	rcCheckOnRegist = 1,
	// 频繁检查
	rcCheckcontinually = 2
};

enum CBSRegCheckResult
{
	// 正常用户，正常提示注册成功
	crOK,
	// 警告用户，在弹出注册成功提示之前弹出警告提示，警告提示后提示注册成功
	crWarning,
	// 网络连接错误
	crError,
	// 非法用户，清空注册信息，并弹出禁用提示，用户可以退出程序，也可继续注册
	crQuit,
	// 退款用户，由产品决定是否让用户继续使用
	crRefund
};

typedef void(__stdcall * RegCheckCallBack)(CBSRegCheckResult checkResult, wchar_t* pszMessage);

class ICBSHelper : public IUnknown
{
public:
	virtual void __stdcall Init(int nProdId, wchar_t* pszVersion, wchar_t* pszEmail) = 0;
	virtual bool __stdcall Start() = 0;
	virtual void __stdcall LocalRegister(wchar_t* pszEmail, wchar_t* pszRegCode) = 0;
	virtual void __stdcall SetRegCheckCallBack(RegCheckCallBack pCallBack) = 0;
	virtual void __stdcall RegCheck(wchar_t* pszEmail, wchar_t* pszLang, CBSRegCheckType checkType) = 0;
};

typedef void(__stdcall * CreateCBSHelper)(ICBSHelper* &pCBSHelper);