	
////////////////////////////////////////////////////////////////////////////
// BaseSubView.h
//

#pragma once


#ifdef WIN32
	#include "W32SubView.h"
	#define L32SubView W32SubView
#else
	#include "L32SubView.h"
#endif


class BaseSubView : public L32SubView  
{
public:
	BaseSubView()
	{
	}
	
	~ BaseSubView()
	{
	}

	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = 0)
	{
		HSUBVIEW hView = L32SubView::Create(parent, rect, dwStyle, dwExStyle, nID);
		return hView;
	}
	
	void Show(BOOL bShow = TRUE)
	{
		L32SubView::Show(bShow);
	}

};