// L32View.h

#pragma once

#include "ViewCommon.h"

class L32View  
{
public:
	HVIEW m_hWnd;  
	
public:
	L32View()
		: m_hWnd(0)
	{
	}
	
	~L32View()
	{
		Destroy();
	}
	
public:	
	HVIEW Create(const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		Rect bounds  = {(short)rect.y, (short)rect.x,
			            (short)(rect.y + rect.cy), (short)(rect.x + rect.cx)};

		CreateNewWindow (
							6, // kDocumentWindowClass, 
							0x1a09001f, // kWindowStandardHandlerAttribute | kWindowResizableAttribute | kWindowStandardDocumentAttributes,
							&bounds,
							&m_hWnd
							);
							
		SetThemeWindowBackground (m_hWnd, kThemeBrushDrawerBackground, true);
		
		EventTypeSpec	windowEventList[] = {
							{ kEventClassWindow, kEventWindowBoundsChanged } ,
							};
		// Install the window event handler
		OSStatus err = InstallWindowEventHandler( m_hWnd, MainWindowEventHandler,
				GetEventTypeCount( windowEventList ), windowEventList, (void*)this, NULL );
		

		return m_hWnd;

	}
	
	void Destroy()
	{
		ReleaseWindow(m_hWnd);
		m_hWnd = 0;
	}
	
	void Show(BOOL bShow = TRUE)
	{
		if (bShow)
			ShowWindow(m_hWnd);
		else
			HideWindow(m_hWnd);
	}

protected:

	// -----------------------------------------------------------------------------
	//	MyWindowEventHandler
	// -----------------------------------------------------------------------------
	//
	static pascal OSStatus MainWindowEventHandler(
		EventHandlerCallRef	inHandlerCallRef,
		EventRef			inEvent,
		void*				inUserData )
	{
		OSStatus			err = eventNotHandledErr;
		UInt32				eventClass = GetEventClass( inEvent );
		UInt32				eventKind = GetEventKind( inEvent );
	//	WindowRef			window = (WindowRef) inUserData;
		
		UINT message = 0; 
		WPARAM wParam = (WPARAM)inHandlerCallRef;
		LPARAM lParam = (LPARAM)inEvent;
		
		L32View* pView = static_cast<L32View*>(inUserData);

		if ( eventClass == kEventClassWindow && eventKind == kEventWindowBoundsChanged )
		{
			Rect currBounds = { 0 };
			OSStatus error = GetEventParameter( inEvent, kEventParamCurrentBounds, typeQDRectangle,
				NULL, sizeof( Rect ), NULL, &currBounds );
				
			message = WM_SIZE;
			lParam = MAKELPARAM((currBounds.right - currBounds.left), (currBounds.bottom - currBounds.top));
			//printf("kEventWindowBoundsChanged\r\n");
		}
		
		
		if (err && pView)
		{
			err = pView->L32DefWindowProc(message, wParam, lParam);
		}		
		
		return err;
	}

protected:

	OSStatus L32DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		OSStatus err = eventNotHandledErr;
		LRESULT lResult = 0;
		BOOL bRet = ProcessWindowMessage(m_hWnd, message, wParam, lParam, lResult, 0);
		if (bRet)
		{
			err = noErr;
		}

		return err;
	}

	virtual BOOL ProcessWindowMessage(WindowRef hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
	{ 
		return FALSE; 
	}

		

};


/////////////////////////////////////////////////////////////////////////////
// Message map


#define BEGIN_MESSAGE(theClass, baseClass) \
public: \
virtual BOOL ProcessWindowMessage(WindowRef hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
{ \
	BOOL br = ProcessMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	if (!br) \
		return baseClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	return TRUE; \
} \
BOOL ProcessMessage(WindowRef hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
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
	lResult = func(HIWORD(wParam), LOWORD(wParam), (WindowRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_ID(id, func) \
if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (WindowRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_CODE(code, func) \
if(uMsg == WM_COMMAND && code == HIWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (WindowRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32COMMAND_RANGE(idFirst, idLast, func) \
if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (WindowRef)lParam, bHandled); \
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

