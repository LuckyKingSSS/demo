// ImageView.h

#pragma once

#include "BaseSubView.h"


class ImageView : public BaseSubView  
{
public:
	ImageView()
	{
	}
	
	~ImageView()
	{
	}
	
public:	
	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		HSUBVIEW hView = BaseSubView::Create(parent, rect, dwStyle, dwExStyle);

		return hView;

	}

	
	BEGIN_SUB_MESSAGE(ImageView, BaseSubView)
		ON_W32MESSAGE(WM_PAINT, OnPaint)
		ON_W32MESSAGE(WM_SIZE, OnSize)
		ON_W32MESSAGE(WM_MOUSEMOVE, OnMouseMove)
	END_SUB_MESSAGE()

	
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		//ReSize();

		//printf("OnSize %d %d\r\n", LOWORD(lParam), HIWORD(lParam));

		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
//		glViewport(0, 0, w, h );

		printf("ImageView::OnSize (%dx%d)\r\n", w, h);

		
		return 0;
	}
		
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//	printf("ImageView::OnPaint\r\n");
		
		HDC hDC = (HDC)wParam;
		VRECT rect = { 0 };
		GetClientRect(m_hWnd, &rect);

		
		FillSolidRect(hDC, &rect, 0x5000FF00);
		StrokeRect(hDC, &rect, 0xFF00FF00);
		
		
		return 0;
	}
		
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf("ImageView::OnMouseMove (%dx%d)\r\n", LOWORD(lParam), HIWORD(lParam));
		return 0;
	}


};
