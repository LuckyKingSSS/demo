	
////////////////////////////////////////////////////////////////////////////
// L32SubView.h
//==========================================================================
//
//==========================================================================


#pragma once

#include "ViewCommon.h"
#include <sys/times.h>
			
#define kSubViewID CFSTR("Base Sub View")			// class ID



class L32SubView  
{
public:
	WindowRef m_hParentWnd;
	HIViewRef m_hWnd;
	CFStringRef m_srClassID; 
	
	HIObjectClassRef m_SubViewClassRef;
	EventHandlerUPP m_EvtHandler;			// main event handler
	EventHandlerRef	m_EventRef;

	DWORD	 m_dwLDownTime;

	//
	OSStatus BaseSubViewRegister()
	{
		OSStatus				err = noErr;
		static HIObjectClassRef	sHIBaseSubWindowClassRef = NULL;

		if ( sHIBaseSubWindowClassRef == NULL )
		{
		
			EventTypeSpec		eventList[] = {
				{ kEventClassHIObject, kEventHIObjectConstruct },
				{ kEventClassHIObject, kEventHIObjectInitialize },
				{ kEventClassHIObject, kEventHIObjectDestruct },

				{ kEventClassControl, kEventControlActivate },
				{ kEventClassControl, kEventControlDeactivate },
				{ kEventClassControl, kEventControlDraw },
				{ kEventClassControl, kEventControlHiliteChanged },
				{ kEventClassControl, kEventControlHitTest },
				{ kEventClassControl, kEventControlInitialize },
				
				{ kEventClassControl, kEventControlBoundsChanged } ,
				{ kEventClassControl, kEventControlClick } ,
				{ kEventClassControl, kEventControlGetClickActivation } ,
				
				/*
//#if CUSTOM_TRACK
				{ kEventClassControl, kEventControlTrack },
//#endif
				{ kEventClassControl, kEventControlValueFieldChanged },
				
				{ kEventClassMouse, kEventMouseDown},
				{ kEventClassMouse, kEventMouseUp},
				{ kEventClassMouse, kEventMouseMoved},
				
				{ kEventClassControl, kControlDownButtonPart } ,
				{ kEventClassControl, kControlUpButtonPart } ,
				
				{ kEventClassControl, kEventControlClick } ,
				{ kEventClassControl, kEventControlHit } ,
				{ kEventClassControl, kEventControlInterceptSubviewClick } ,
				{ kEventClassControl, kEventControlGetClickActivation } ,
				{ kEventClassControl, kEventControlSetCursor } ,
				{ kEventClassControl, kEventControlContextualMenuClick } ,
				{ kEventClassControl, kEventControlTrackingAreaEntered } ,
				{ kEventClassControl, kEventControlTrackingAreaExited } ,
				{ kEventClassControl, kEventControlIndicatorMoved } ,
				
				{ kEventClassControl, kEventControlDragEnter } ,
				{ kEventClassControl, kEventControlDragWithin } ,
				{ kEventClassControl, kEventControlDragLeave } ,
				{ kEventClassControl, kEventControlDragReceive } ,
				*/
				};
				
			char szClassID[64] = "Base Sub View";
			sprintf(szClassID, "Base Sub View[0x%x]", this);
			
			m_srClassID = CFStringCreateWithCString(0, szClassID,  kCFStringEncodingASCII);

			err = HIObjectRegisterSubclass(
				//kSubViewID,			// class ID
				m_srClassID,			// class ID
				kHIViewClassID,				// base class ID
				0,						// option bits
				HIBaseSubViewHandler,			// construct proc
				GetEventTypeCount( eventList ),
				eventList,
				(void *)this,						// construct data,
				&m_SubViewClassRef );
		}
		
		return err;
	}

	virtual ~L32SubView()
	{
		Destroy();
	}

	L32SubView()
	: m_hWnd(NULL)
	, m_SubViewClassRef(0)
	, m_hParentWnd(0)
	, m_EvtHandler(0)
	, m_EventRef(0)
	, m_dwLDownTime(0)
	{
		m_srClassID = kSubViewID;
	}
	
	static DWORD timeGetTime()
	{
		tms tm;
		return (times(&tm) * 10);
	}	
	
	void Show(BOOL bShow = TRUE)
	{
		if (bShow)
			ShowControl(m_hWnd);
		else
			HideControl(m_hWnd);
	}
	

// ---------------------------------

// Handles global non-system mouse events for GL windows as follows:
// primary button = object tumble (trackball)
// secondary button (or cntl-primary) = pan
// tertiary button (or option-primary) = dolly
// wheel = aperture
	
OSStatus handleWindowMouseEvents (EventHandlerCallRef myHandler, EventRef event)
{
    WindowRef			window = NULL;
//    pRecContext 		pContextInfo = NULL;
	OSStatus			result = eventNotHandledErr;
    UInt32 				kind = GetEventKind (event);
	EventMouseButton	button = 0;
	HIPoint				location = {0.0f, 0.0f};
	UInt32				modifiers = 0;	
	long				wheelDelta = 0;		
	Rect 				rectPort;

	// Mac OS X v10.1 and later
	// should this be front window???
	GetEventParameter(event, kEventParamWindowRef, typeWindowRef, NULL, sizeof(WindowRef), NULL, &window);
//	printf("MouseEvents window = 0x%x Parent = 0x%x\r\n", window, m_hParentWnd);
//	printf("--\r\n");
	if (window != m_hParentWnd)
		return result;
//	printf("---\r\n");

//	if (window)
//		pContextInfo = GetCurrentContextInfo (window);
//	if (!pContextInfo)
//		return result; // not an application GLWindow so do not process (there is an exception)
	GetWindowPortBounds (window, &rectPort);
		
//	result = CallNextEventHandler(myHandler, event);	
//	if (eventNotHandledErr == result) 
	{ // only handle events not already handled (prevents wierd resize interaction)
		if (IsValidControlHandle(m_hWnd))
		{
//		printf("----\r\n");
	
		GetEventParameter(event, kEventParamWindowMouseLocation, typeHIPoint, NULL, sizeof(HIPoint), NULL, &location);	// Mac OS X v10.1 and later
		//printf("(%f, %f).\r\n", location.x, location.y);
		
		Rect crect = { 0 };	
		GetWindowBounds(window, kWindowContentRgn, &crect);
		Rect grect = { 0 };	
		GetWindowBounds(window, kWindowStructureRgn, &grect);
		
		short x = (short)location.x + (grect.left - crect.left) - 0;
		short y = (short)location.y + (grect.top - crect.top) - 0;
	
		HIRect viewFrame;
		HIViewGetFrame (m_hWnd, &viewFrame);
		
		x -= viewFrame.origin.x;
		y -= viewFrame.origin.y;
		
		
//		if ((x >= 0) && (x < viewFrame.size.width) && (y >= 0) && (y < viewFrame.size.height))
		{

		UINT message = 0;
		WPARAM wParam = 0;
		LPARAM lParam = MAKELPARAM(x, y);
			
	
		switch (kind) {
			// start trackball, pan, or dolly
			case kEventMouseDown:
				printf("kEventMouseDown.\r\n");
				{
					VRECT cr = { 0 };
					GetClientRect(m_hWnd, &cr);
					RECT rcBound = { 0, 0, cr.cx, cr.cy };
					POINT pt = { x, y };
					if (PtInRect(&rcBound, pt))
						message = WM_LBUTTONDOWN;
				}
				break;
			case kEventMouseUp:
				//printf("kEventMouseUp.\r\n");
				message = WM_LBUTTONUP;
				break;
			case kEventMouseDragged:
				GetEventParameter(event, kEventParamWindowMouseLocation, typeHIPoint, NULL, sizeof(HIPoint), NULL, &location);	// Mac OS X v10.1 and later
				//printf("kEventMouseDragged (%f, %f).\r\n", location.x, location.y);
				message = WM_MOUSEDRAG;
				break;
			case kEventMouseMoved:
				//printf("kEventMouseMoved.\r\n");
				message = WM_MOUSEMOVE;
				break;
			case kEventMouseWheelMoved:
				//printf("kEventMouseWheelMoved.\r\n");
				break;
		}
		
		if (message > 0)
			L32DefWindowProc(message, wParam, lParam);
		
		}
		}
		
	}
	
	return eventNotHandledErr;
}

// ---------------------------------

// application level event handler
static pascal OSStatus appEvtHndlr (EventHandlerCallRef myHandler, EventRef event, void* userData)
{
//#pragma unused (myHandler, userData)
/*
    OSStatus result = eventNotHandledErr;
    Rect rectPort;
//    pRecContext pContextInfo = NULL;
    WindowRef window = FrontWindow ();
    UInt32 kind = GetEventKind (event);
    HICommand command;

    if (window) { // do we have a window?
        GetWindowPortBounds (window, &rectPort); // get bounds for later inval
        //pContextInfo = GetCurrentContextInfo (window);
    }
*/	

//	printf("-\r\n");

	OSStatus result =  CallNextEventHandler(myHandler, event);	
//	if (eventNotHandledErr != result)
//		return result;
			
//	printf("CallNextEventHandler OK.\r\n");


    UInt32 event_class = GetEventClass (event);
	
	L32SubView* pView = static_cast<L32SubView*>(userData);
	
	switch (event_class) {
		case kEventClassMouse:
//			printf("kEventClassMouse.\r\n");
			pView->handleWindowMouseEvents (myHandler, event);
			break;
	}
	
	return eventNotHandledErr;
}

//public:
	HIViewRef Create(WindowRef parent, const VRECT& rcPos, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = 0)
	{
		BaseSubViewRegister();
		
	//	L32SubView* pView = 0;
	
		HIViewRef hView = 0;
		OSStatus err = HIObjectCreate( m_srClassID, (EventRef)0, (HIObjectRef*)&hView );
//		L32SubView* pView = (L32SubView*)SubView(1, (WPARAM)hView);
		
		if (noErr == err)
		{
			ControlRef root = 0;
			err = GetRootControl( parent, &root );
			HIViewAddSubview( root, m_hWnd );
	
			HIRect		inBounds = CGRectMake(rcPos.x, rcPos.y, rcPos.cx, rcPos.cy);

			HIViewSetFrame( m_hWnd, &inBounds );
			ShowControl( m_hWnd );
		
			// Give it an ID, so we can find it later
			SetControlID( m_hWnd, (ControlID*)&nID );
			
			SetControlAction(m_hWnd, LittleArrowActionProc);
		}
		
		
		EventTypeSpec	list[] = { 
							//	   { kEventClassCommand,  kEventProcessCommand },
							//	   { kEventClassCommand,  kEventCommandUpdateStatus },
								   { kEventClassMouse, kEventMouseDown },// handle trackball functionality globaly because there is only a single user
								   { kEventClassMouse, kEventMouseUp }, 
								   { kEventClassMouse, kEventMouseMoved },
								   { kEventClassMouse, kEventMouseDragged },
								   { kEventClassMouse, kEventMouseWheelMoved } };
		m_EvtHandler = NewEventHandlerUPP(appEvtHndlr);
		err = InstallApplicationEventHandler (m_EvtHandler, GetEventTypeCount (list), list, this, &m_EventRef);
		//err = HIViewInstallEventHandler (m_hWnd, m_EvtHandler, GetEventTypeCount (list), list, this, &m_EventRef);
		
		
		m_hParentWnd = parent;

		return hView;
	}
	
	void Destroy()
	{
		if (m_EventRef)
		{
			RemoveEventHandler(m_EventRef);
			m_EventRef = 0;
		}
		
		if (m_hWnd)
		{
			HIViewRemoveFromSuperview(m_hWnd);
		}
		
		if (m_SubViewClassRef)
		{
			HIObjectUnregisterClass(m_SubViewClassRef);
		}
		
		m_hWnd = NULL;
		m_SubViewClassRef = 0;
		
		m_hParentWnd = 0;
		m_EvtHandler = 0;
		m_EventRef = 0;
	}


	static BOOL ModifyStyle(WindowRef HIViewRef, int nStyleOffset, DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0)
	{
		ChangeWindowAttributes(HIViewRef, dwAdd, dwRemove);
		return TRUE;
	}

//	BOOL ModifyStyle(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 )
//	{
//		return ModifyStyle(m_hWnd, 0, dwRemove, dwAdd, nFlags);
//	}
	
//	BOOL ModifyStyleEx(DWORD dwRemove, DWORD dwAdd, UINT nFlags = 0 )
//	{
//		return ModifyStyle(m_hWnd, GWL_EXSTYLE, dwRemove, dwAdd, nFlags);
//	}

	void MoveWindow(int x, int y, int cx = -1, int cy = -1)
	{
		if (-1 == cx || -1 == cy)
		{
			HIViewSetBoundsOrigin(m_hWnd, (float)x, (float)y);
		}
		else
		{
			HIRect inBounds = CGRectMake(x, y, cx, cy);
			HIViewSetFrame( m_hWnd, &inBounds );
		}
	}

	void ResizeWindow(int cx, int cy)
	{
		HIRect inBounds = CGRectMake(0, 0, cx, cy);
		HIViewGetFrame(m_hWnd, &inBounds);
		inBounds.size.width = cx;
		inBounds.size.height = cy;
		HIViewSetFrame(m_hWnd, &inBounds);
	}


static void LittleArrowActionProc(ControlRef cref, ControlPartCode part)  {
    SInt32 val = GetControl32BitValue(cref);
    SInt32 s = 0;
    GetControlData(cref, 0, kControlLittleArrowsIncrementValueTag,  sizeof(SInt32), &s, nil);
    switch (part) {
        case kControlUpButtonPart:
            SetControl32BitValue(cref, val+s);
            break;
        case kControlDownButtonPart:
            SetControl32BitValue(cref, val-s);
            break;
        };
    };

	static pascal OSStatus HIBaseSubViewHandler(
		EventHandlerCallRef		inCallRef,
		EventRef				inEvent,
		void*					inUserData )
	{
		OSStatus				err = eventNotHandledErr;
		UInt32					eventClass = GetEventClass( inEvent );
		UInt32					eventKind = GetEventKind( inEvent );
	
		UINT message = 0; 
		WPARAM wParam = (WPARAM)inCallRef;
		LPARAM lParam = (LPARAM)inEvent;
		
		L32SubView* pView = static_cast<L32SubView*>(inUserData);
		
		switch(eventClass)
		{
			case kEventClassHIObject:
			switch ( eventKind )
			{
				case kEventHIObjectConstruct:
				{
				
				//	TCarbonEvent		event( inEvent );
					HIObjectRef	object = NULL;
				//	OSStatus	error = event.GetParameter<HIObjectRef>( kEventParamHIObjectInstance,
				//													   typeHIObjectRef, &object );
																	   
					OSStatus error = GetEventParameter( inEvent, kEventParamHIObjectInstance, typeHIObjectRef,
							NULL, sizeof( object ), NULL, &object );
																	   
					pView->m_hWnd = (HIViewRef)object;
					
					error = SetEventParameter(inEvent, kEventParamHIObjectInstance, typeVoidPtr, sizeof( HIObjectRef ), (HIObjectRef)&pView);
					//event.SetParameter<HIObjectRef>( kEventParamHIObjectInstance,
					//		typeVoidPtr, (HIObjectRef)pView ); 

				//	SubView(0, (WPARAM)pView);
					err = noErr;
				}
				break;
				
				case kEventHIObjectDestruct:
					// don't CallNextEventHandler
					//err = HITestViewDestruct( inEvent, data );
				{
					L32SubView* pView = static_cast<L32SubView*>(inUserData);
					if (pView)
						pView->Destroy();
											
				//	SubView(2, (WPARAM)pView);

					
				//	delete pView;
				}
				err = noErr;
				break;
			}
			break;
			
			case kEventClassControl:
			{
				switch ( eventKind )
				{
					case kEventControlDraw:
					{
						L32SubView* pView = static_cast<L32SubView*>(inUserData);
						// L32SubView::HISubViewDraw( inEvent, pView->m_hWnd );

						message = WM_PAINT;
						
						CGContextRef context = 0;
						OSStatus error = GetEventParameter( inEvent, kEventParamCGContextRef, typeCGContextRef,
							NULL, sizeof( CGContextRef ), NULL, &context );
							
						wParam = (WPARAM)context;


					//	err = noErr;
					//	err = 1;
					}
					break;
					
					case kEventControlBoundsChanged:
					{
						Rect currBounds = { 0 };
						OSStatus error = GetEventParameter( inEvent, kEventParamCurrentBounds, typeQDRectangle,
							NULL, sizeof( Rect ), NULL, &currBounds );
							
						message = WM_SIZE;
						lParam = MAKELPARAM((currBounds.right - currBounds.left), (currBounds.bottom - currBounds.top));
					}
					break;
					
					case kEventControlGetClickActivation:
					//	printf("kEventControlGetClickActivation\r\n");
					case kEventControlClick:
					{
						L32SubView* pView = static_cast<L32SubView*>(inUserData);
						
						HIPoint			point;

						OSStatus error = GetEventParameter( inEvent, kEventParamWindowMouseLocation, typeHIPoint,
								NULL, sizeof( point ), NULL, &point );

					//	printf("mouse down (%f, %f).\r\n", point.x, point.y);
						
						Rect crect = { 0 };	
						GetWindowBounds(pView->m_hParentWnd, kWindowContentRgn, &crect);
						Rect grect = { 0 };	
						GetWindowBounds(pView->m_hParentWnd, kWindowStructureRgn, &grect);
						
		
						short x = (short)point.x + (grect.left - crect.left) - 0;
						short y = (short)point.y + (grect.top - crect.top) - 0;
					
						HIRect viewFrame;
						HIViewGetFrame (pView->m_hWnd, &viewFrame);
						
						x -= viewFrame.origin.x;
						y -= viewFrame.origin.y;
						
						message = WM_LBUTTONDOWN;
						
						DWORD dwCurrTime = L32SubView::timeGetTime();
						//printf("%d\r\n", dwCurrTime - pView->m_dwLDownTime);
						if ((dwCurrTime - pView->m_dwLDownTime) <= 200)
						{
							message = WM_LBUTTONDBLCLK;
							pView->m_dwLDownTime = 0;
						}
						else
						{
							pView->m_dwLDownTime = dwCurrTime;
						}
						
						wParam = 0;
						lParam = MAKELPARAM(x, y);
					}
					break;

				}
			}
			break;
			
			case kEventClassMouse:
			{
				switch(eventKind)
				{
					case kEventMouseDown:
					message = WM_LBUTTONDOWN;
					break;
					
					case kEventMouseUp:
					message = WM_LBUTTONUP;
					break;
					
					case kEventMouseMoved:
					message = WM_MOUSEMOVE;
					break;
				}
			}
			break;
		}
		
		if (err && pView)
		{
		//L32SubView* pSubView = static_cast<L32SubView*>(inUserData);
			//UINT message = 0; 
			//WPARAM wParam = (WPARAM)inCallRef;
			//LPARAM lParam = (LPARAM)inEvent;
			err = pView->L32DefWindowProc(message, wParam, lParam);
			
			if (err && WM_PAINT == message)
			{
				HIRect					bounds;
				CGContextRef			context;

				OSStatus error = GetEventParameter( inEvent, kEventParamCGContextRef, typeCGContextRef,
						NULL, sizeof( CGContextRef ), NULL, &context );

				error = HIViewGetBounds( pView->m_hWnd, &bounds );
			
				CGContextSetRGBFillColor( context, 255, 0, 0, 0.25 );
				CGContextSetRGBStrokeColor( context, 255, 0, 0, 1 );
				CGContextFillRect( context, bounds );
				CGContextStrokeRect( context, bounds );
			}
			
		}
		
		if (err)
			err = CallNextEventHandler( inCallRef, inEvent );
		
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

	virtual BOOL ProcessWindowMessage(HIViewRef HIViewRef, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0)
	{ 
		return FALSE; 
	}
	
};




/////////////////////////////////////////////////////////////////////////////
// Message map

#define BEGIN_SUB_MESSAGE(theClass, baseClass) \
public: \
virtual BOOL ProcessWindowMessage(HIViewRef hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
{ \
	BOOL br = ProcessMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	if (!br) \
		return baseClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, dwMsgMapID); \
	return TRUE; \
} \
BOOL ProcessMessage(HIViewRef hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
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
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HIViewRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_ID(id, func) \
if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HIViewRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_COMMAND_CODE(code, func) \
if(uMsg == WM_COMMAND && code == HIWORD(wParam)) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HIViewRef)lParam, bHandled); \
	if(bHandled) \
		return TRUE; \
}

#define ON_W32COMMAND_RANGE(idFirst, idLast, func) \
if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst && LOWORD(wParam) <= idLast) \
{ \
	bHandled = TRUE; \
	lResult = func(HIWORD(wParam), LOWORD(wParam), (HIViewRef)lParam, bHandled); \
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

#define END_SUB_MESSAGE() \
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





