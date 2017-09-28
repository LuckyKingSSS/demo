// W32SubView.h

#pragma once

#include "W32View.h"
#include "ViewCommon.h"

#define	BEGIN_SUB_MESSAGE	BEGIN_MESSAGE
#define	END_SUB_MESSAGE		END_MESSAGE


class W32SubView : public W32View  
{
public:
	W32SubView()
	{
	}
	
	~ W32SubView()
	{
	}
	
public:	
	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = 0)
	{

		W32View::Create(parent, rect, dwStyle, dwExStyle, nID);

		Show();

		return m_hWnd;
	}
	
	void Show(BOOL bShow = TRUE)
	{
		if (bShow)
			ShowWindow(m_hWnd, TRUE);
		else
			ShowWindow(m_hWnd, FALSE);
	}

	void MoveWindow(int x, int y, int cx = -1, int cy = -1)
	{
		if (-1 == cx || -1 == cy)
		{
			SetWindowPos(m_hWnd, 0, x, y, cx, cy, SWP_NOSIZE | SWP_NOZORDER);
		}
		else
		{
			SetWindowPos(m_hWnd, 0, x, y, cx, cy, SWP_NOZORDER);
		}
	}

	void ResizeWindow(int cx, int cy)
	{
		SetWindowPos(m_hWnd, 0, 0, 0, cx, cy, SWP_NOMOVE | SWP_NOZORDER);
	}
	

};


