// GLImageView.h

#pragma once

#include "GLView.h"
#include "DibImage.h"

#ifdef WIN32
	#include <mmsystem.h>
#else
	#include <sys/times.h>
#endif

class GLImageView : public GLView  
{
protected:
	
public:
	GLImageView()
	{
	}
	
	~GLImageView()
	{
	}
	
public:	
	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		HSUBVIEW hView = GLView::Create(parent, rect, dwStyle, dwExStyle);
		
//#ifdef WIN32
//		m_DibImage.Load(_T("D:\\GLTest320.bmp"));
//#else
//		m_DibImage.Load(_T("/Library/Test/GLTest320.bmp"));
//#endif


		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_FLAT);

		printf("GLImageView::Create\r\n");

		return hView;

	}
	
	BOOL ShowImage(LPBITMAPINFO lpbi, LPBYTE pBits = 0)
	{
		if (0 == lpbi)
			return FALSE;
			
		if (0 == pBits)
			pBits = ((LPBYTE)lpbi) + 40;
			
		if (   (lpbi->bmiHeader.biWidth == m_DibImage.Width())
		    && (lpbi->bmiHeader.biHeight == m_DibImage.Height()) 
			&& (lpbi->bmiHeader.biBitCount == m_DibImage.BitCount())
			)
		{
			memcpy(m_DibImage.GetBits(), pBits, m_DibImage.ImageSize());
		}
		else
		{
			m_DibImage.Create(lpbi->bmiHeader.biWidth, lpbi->bmiHeader.biHeight, lpbi->bmiHeader.biBitCount);
			memcpy(m_DibImage.GetBits(), pBits, m_DibImage.ImageSize());
		}
		
		Update();
		
		return TRUE;
	}

	
	BEGIN_SUB_MESSAGE(GLImageView, GLView)
		ON_W32MESSAGE(WM_PAINT, OnPaint)
		ON_W32MESSAGE(WM_SIZE, OnSize)
		ON_W32MESSAGE(WM_MOUSEMOVE, OnMouseMove)
	END_SUB_MESSAGE()

	
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		printf("GLImageView::OnSize (%dx%d)\r\n", w, h);
		
		GLView::OnSize(uMsg, wParam, lParam, bHandled);

		return 0;
	}
		
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
//		printf("GLImageView::OnPaint\r\n");
		
//		GLView::OnPaint(uMsg, wParam, lParam, bHandled);

		if (m_DibImage.GetBits())
			update((HDC)wParam);
		else
			GLView::OnPaint(uMsg, wParam, lParam, bHandled);
		
		return 0;
	}
		
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
/*	
		PAINTSTRUCT ps;
		HDC hDC;
		hDC = BeginPaint(m_hWnd, &ps);

		RECT rect = { 0 };
		GetClientRect(m_hWnd, &rect);

	//	FillSolidRect(hDC, &rect, RGB(0, 0, 0));

		Update(hDC);

		EndPaint(m_hWnd, &ps);

	*/	
		printf("GLImageView::OnMouseMove (%dx%d)\r\n", LOWORD(lParam), HIWORD(lParam));
		return 0;
	}

#ifndef WIN32	
	DWORD timeGetTime()
	{
		tms tm;
		return (times(&tm) * 10);
	}	
#endif

	void Update()
	{
		update(0);
	}

	void update(HDC hDC)
	{
		LPBITMAPINFO lpbi = m_DibImage.GetBitmapInfo();
		if (0 == lpbi)
			return;
	
		DWORD dt = timeGetTime();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glLoadIdentity();
		
		int width = lpbi->bmiHeader.biWidth;
		int height = lpbi->bmiHeader.biHeight;
		
		double iw = 1024.0 / width;
		double ih = 1024.0 / height;
		
//		VRECT vr = { 0 };
//		GetClientRect(m_hWnd, &vr);
//		int cx = vr.cx;
//		int cy = vr.cy;
		
//		GLfloat mx = (cx - 1) / (GLfloat)cx;
//		GLfloat my = (cy - 1) / (GLfloat)cy;
		
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3f(-iw, -ih, 0.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-iw, ih, 0.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(iw, ih, 0.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(iw, -ih, 0.0);
		glEnd();
		
		
		BYTE* pBits = m_DibImage.GetBits();
		glTexSubImage2D(GL_TEXTURE_2D, 0, (1024 - width) / 2, (1024 - height) / 2,
			width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBits);
			
		
		glFlush();
		
		SWAP_BUFFERS;
		
		dt = timeGetTime() - dt;
		printf("GLImageView dispaly %d millisec.\r\n", dt);
		
	}


protected:
	DibImage m_DibImage;


};
