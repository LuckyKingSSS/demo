	
////////////////////////////////////////////////////////////////////////////
// W32Wnd.h
//==========================================================================
// Created by 苦丁茶 (2004-10-01)
//
// 功能： 模拟MFC和WTL对Windows窗口框架的封装，提供消息映射机制。
// 参考资料：MFC、WTL消息映射机制
// 消息映射用法举例：
// BEGIN_MESSAGE(W32Button, W32Wnd)
//		ON_W32MESSAGE(WM_LBUTTONDOWN, OnLButtonDown)
//		ON_W32MESSAGE(WM_LBUTTONUP, OnLButtonUP)
//		ON_W32MESSAGE(WM_ERASEBKGND, OnEraseBkgnd)
// END_MESSAGE()
// LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
// {
//		return 0;
// }
// 若要WM_CREATE通知，在注册窗口类时请将窗口过程函数指定为 W32Wnd::W32DefWindowProc
// 消息映射具体用法请参考W32Button.h所实现的支持自画Button类。
//==========================================================================

#if !defined(_W32WND_H__INCLUDED_)
#define _W32WND_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class W32Wnd  
{
public:
	HWND m_hWnd;

	~W32Wnd()
	{
		if (m_pSuper)
		{
			UnsubclassWindow();
		}

		if (::IsWindow(m_hWnd))
		{
			::DestroyWindow(m_hWnd);
		}

		m_hWnd = NULL;
	}

	W32Wnd()
	: m_hWnd(NULL)
	, m_pSuper(NULL)
	{
	}

	HWND Create(HWND hWndParent, const RECT& rcPos, LPCTSTR lpstrWndClass, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			UINT nID = 0, LPVOID lpCreateParam = NULL)
	{
		HWND hWnd = ::CreateWindowEx(dwExStyle, lpstrWndClass, szWindowName,
			dwStyle, rcPos.left, rcPos.top, rcPos.right - rcPos.left,
			rcPos.bottom - rcPos.top, hWndParent, (HMENU)nID,
			::GetModuleHandle(NULL), this);

		return hWnd;
	}

	BOOL SubclassWindow(HWND hWnd)
	{
		if (m_hWnd)
			return FALSE;

		::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);
		m_pSuper = (WNDPROC)::SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)((WNDPROC)W32DefWindowProc));
		m_hWnd = hWnd;

		return TRUE;
	}

	HWND UnsubclassWindow()
	{
		if (::IsWindow(m_hWnd) && m_pSuper)
		{
			::SetWindowLongPtr(m_hWnd, GWLP_WNDPROC, (LONG_PTR)((WNDPROC)m_pSuper));
		}
		
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		m_pSuper = NULL;

		return hWnd;
	}

	static BOOL ModifyStyle(HWND hWnd, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		DWORD dwStyle = ::GetWindowLongPtr(hWnd, nStyleOffset);
		DWORD dwNewStyle = (dwStyle & ~dwRemove) | dwAdd;
		if (dwStyle == dwNewStyle)
			return FALSE;

		::SetWindowLongPtr(hWnd, nStyleOffset, (LONG_PTR)dwNewStyle);
		if (nFlags != 0)
		{
			::SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
				SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | nFlags);
		}
		return TRUE;
	}

	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 )
	{
		return ModifyStyle(m_hWnd, GWL_STYLE, dwRemove, dwAdd, nFlags);
	}
	
	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 )
	{
		return ModifyStyle(m_hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
	}

	static LRESULT CALLBACK W32DefWindowProc(
		HWND hWnd,      // handle to window
		UINT Msg,       // message identifier
		WPARAM wParam,  // first message parameter
		LPARAM lParam   // second message parameter
		)
	{
		if (WM_CREATE == Msg)
		{
			LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
			W32Wnd* pWnd = (W32Wnd*)lpcs->lpCreateParams;
			::SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWnd);
			pWnd->m_hWnd = hWnd;
		}

		W32Wnd * pWnd =  (W32Wnd *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pWnd)
		{
			return pWnd->W32DefWindowProc(Msg, wParam, lParam);
		}

		return ::DefWindowProc(hWnd, Msg, wParam, lParam);;
	}

protected:

	LRESULT W32DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bRet = ProcessWindowMessage(m_hWnd, message, wParam, lParam, lRes, 0);
		if (bRet)
		{
			return lRes;
		}

		if (W32Wnd::W32DefWindowProc == m_pSuper)
			return ::DefWindowProc(m_hWnd, message, wParam, lParam);

		if (m_pSuper)
			return CallWindowProc(m_pSuper, m_hWnd, message, wParam, lParam);

		return ::DefWindowProc(m_hWnd, message, wParam, lParam);
	}

	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
	{ 
		return FALSE; 
	}

	WNDPROC	m_pSuper;

};



/////////////////////////////////////////////////////////////////////////////
// Message map

#define BEGIN_MESSAGE(theClass, baseClass) \
public: \
virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
{ \
	BOOL br = ProcessMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	if (!br) \
		return baseClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	return TRUE; \
} \
BOOL ProcessMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
{ \
	BOOL bHandled = TRUE; \
	hWnd; \
	uMsg; \
	wParam; \
	lParam; \
	lResult; \
	bHandled; \
	switch(dwMsgMapID) \
	{ \
	case 0:

#define ALT_MSG(msgMapID) \
		break; \
	case msgMapID:

#define ON_W32MESSAGE(msg, func) \
if(uMsg == msg) \
{ \
	bHandled = TRUE; \
	lResult = func(uMsg, wParam, lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_MESSAGE_RANGE(msgFirst, msgLast, func) \
if(uMsg >= msgFirst && uMsg <= msgLast) \
{ \
	bHandled = TRUE; \
	lResult = func(uMsg, wParam, lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32COMMAND(id, code, func) \
if(uMsg == WM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_ID(id, func) \
if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_CODE(code, func) \
if(uMsg == WM_COMMAND && code == HIWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32COMMAND_RANGE(idFirst, idLast, func) \
if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32NOTIFY(id, cd, func) \
if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code) \
{ \
	bHandled = TRUE; \
	lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_NOTIFY_IDR(id, func) \
if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
{ \
	bHandled = TRUE; \
	lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_NOTIFY_CODE(cd, func) \
if(uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
{ \
	bHandled = TRUE; \
	lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32NOTIFY_RANGE(idFirst, idLast, func) \
if(uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
{ \
	bHandled = TRUE; \
	lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define CHAIN_MSG(theChainClass) \
{ \
	if(theChainClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
		return TRUE; \
}

#define CHAIN_MSG_MEMBER(theChainMember) \
{ \
	if(theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
		return TRUE; \
}

#define CHAIN_MSG_ALT(theChainClass, msgMapID) \
{ \
	if(theChainClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
		return TRUE; \
}

#define CHAIN_MSG_ALT_MEMBER(theChainMember, msgMapID) \
{ \
	if(theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
		return TRUE; \
}

#define CHAIN_MSG_DYNAMIC(dynaChainID) \
{ \
	if(CDynamicChain::CallChain(dynaChainID, hWnd, uMsg, wParam, lParam, lResult)) \
		return TRUE; \
}

#define END_MESSAGE() \
		break; \
	default: \
		break; \
	} \
	return FALSE; \
}


#define REFLECT_NOTIFICATIONS_EX() \
{ \
	bHandled = TRUE; \
	lResult = ReflectNotifications(uMsg, wParam, lParam, bHandled); \
}

#define OWNERDRAW_NOTIFICATIONS(message, wParam, lParam) \
{ \
	if (WM_DRAWITEM == message) \
		::SendMessage(((LPDRAWITEMSTRUCT)lParam)->hwndItem, OCM__BASE + message, wParam, lParam); \
}


#ifdef __AFX_H__


#define ON_WM_DRAWCHILDITEM() \
{ WM_DRAWITEM, 0, 0, 0, AfxSig_vww, \
	(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT))&OnDrawChildItem },
//#define OWNERDRAW_NOTIFICATIONS_MFC() \
// { WM_DRAWITEM, 0, 0, 0, AfxSig_vww, \
//(AFX_PMSG)(AFX_PMSGW)(void (AFX_MSG_CALL CWnd::*)(UINT, UINT))&::OnOwnerDrawNotify },

#endif // __AFX_H__

#endif // !defined(_W32WND_H__INCLUDED_)
