// BaseView.h

#pragma once

#ifdef WIN32
	#include "W32View.h"
	#define L32View W32View
#else
	#include "L32View.h"
#endif

#include "ImageView.h"


class BaseView : public L32View
{
public:
	BaseView()
	{
	}
	
	~BaseView()
	{
	}
	
public:	
	HVIEW Create(const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		L32View::Create(rect, dwStyle, dwExStyle);
		
		return m_hWnd;
	}
	
	void Show(BOOL bShow = TRUE)
	{
		L32View::Show(bShow);
	}

};
