
// mac carbon events (zhaojian added)

#ifndef __CARBON_EVENT_H__
#define __CARBON_EVENT_H__

#include <Carbon/Carbon.h>
	
typedef unsigned int UINT;
typedef UINT WPARAM;
typedef UINT LPARAM;
typedef WindowRef HWND;	
typedef UInt32 ThreadID;
										   
inline UINT PostMessage( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
#ifdef __OBJC__
;
#else
{
	EventRef event = NULL;	

	OSStatus statusCreate = CreateEvent( NULL, Msg, 'kind', 0, kEventAttributeNone, &event ); // Create your own event					   			       
	OSStatus statusSet = SetEventParameter( event, 'WPAM', 'UINT', sizeof( WPARAM ), &wParam );	
	statusSet = SetEventParameter( event, 'LPAM', 'UINT', sizeof( LPARAM ), &lParam );
		
	// Send your own event to the target	
#ifndef	NO_EVENTQUEUE	
	OSStatus statusSend = PostEventToQueue( GetMainEventQueue(), event, kEventPriorityStandard );  //the event you post will not be processed until it is pulled from the queue and dispatched to the appropriate event target

#else	
	OSStatus statusSend = SendEventToEventTarget( event, GetWindowEventTarget( hWnd ) );  //compands immediately

#endif
	
	/*ThreadID id;
	GetCurrentThread( &id );
	printf("in CarbonEvent PostMessage, thread ID:%d\r\n", id );*/
	
	if( event )
	{
		ReleaseEvent( event );
	}
		
	if( statusCreate || statusSet || statusSend )
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
#endif
#endif