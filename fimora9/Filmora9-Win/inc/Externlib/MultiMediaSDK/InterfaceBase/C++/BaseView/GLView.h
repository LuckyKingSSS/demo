// GLView.h

#pragma once

#include "BaseSubView.h"

#include <gl.h>
#include <glu.h>

#ifdef WIN32


#else
	#include <agl.h>
	typedef AGLContext HGLRC;  

#endif
	
#ifdef WIN32
	#define SWAP_BUFFERS SwapBuffers(hDC);
#else
	#define SWAP_BUFFERS aglSwapBuffers(m_hRC);
#endif


#ifndef WIN32
 
OSStatus MyAGLReportError (void)
{

    GLenum err = aglGetError();
    if (AGL_NO_ERROR != err) { 
        char errStr[256];
        sprintf (errStr, "AGL: %s",(char *) aglErrorString(err));
        printf (errStr);
    }
    if (err == AGL_NO_ERROR)
        return noErr;
    else
        return (OSStatus) err;
}

#endif

class GLView : public BaseSubView  
{
protected:
	HVIEW m_hParent;
    HGLRC  m_hRC;
	
public:
	GLView()
		: m_hParent(0)
		, m_hRC(0)
	{
	}
	
	~GLView()
	{
#ifdef WIN32
		::wglMakeCurrent(NULL, NULL);
		::wglDeleteContext(m_hRC);
#endif
	
		m_hParent = 0;
	}
	
#ifdef WIN32
	OSStatus SetupGLPixelFormat(HVIEW window, HSUBVIEW hView)   
	{   
		OSStatus err = 0;

		HDC hDC = GetDC(hView);
		PIXELFORMATDESCRIPTOR   pfd={   
			sizeof(PIXELFORMATDESCRIPTOR),   
				1,   
				PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,   
				PFD_TYPE_RGBA,   
				24,   
				0,0,0,0,0,0,   
				0,0,0,0,0,0,0,   
				32,   
				0,0,   
				PFD_MAIN_PLANE,   
				0,0,0,0   
		};   
		
		int   pixelformat = ChoosePixelFormat(hDC, &pfd);   
		SetPixelFormat(hDC, pixelformat, &pfd);   

		m_hRC = wglCreateContext(hDC);   
		wglMakeCurrent(hDC, m_hRC);   

		ReleaseDC(hView, hDC);
		
		return   err;   
	}   
#else    
		
	OSStatus SetupGLPixelFormat (HVIEW window, HSUBVIEW hView)
	{
		OSStatus err = 0;

		GLint attributes[] =  { AGL_RGBA,
							AGL_DOUBLEBUFFER, 
							AGL_DEPTH_SIZE, 24, 
							AGL_NONE };
		AGLPixelFormat myAGLPixelFormat;
	 
		myAGLPixelFormat = aglChoosePixelFormat (NULL, 0, attributes);
		err = MyAGLReportError ();
		if (myAGLPixelFormat) {
			m_hRC = aglCreateContext (myAGLPixelFormat, NULL);
			err = MyAGLReportError ();
		}
		if (! aglSetDrawable (m_hRC, GetWindowPort (window)))
				err = MyAGLReportError ();
		if (!aglSetCurrentContext (m_hRC))
				err = MyAGLReportError ();
				

		// Set the drawable to that of the window we are in
		//aglSetDrawable(m_hRC, GetWindowPort(window));


		// Get the bounds of the root view, and the view to draw into (root & view are these views)
		HIRect viewFrame, rootBounds;
		//::HIViewGetBounds(root, &rootBounds);
		::HIViewGetBounds(hView, &viewFrame);

		// Convert the view's coordinates to that of the root view
		//::HIViewConvertRect(&viewFrame, view, root);


		// Setup the buffer rectangle. Left/Width/Height transfer directly,
		// Bottom is in terms of the bottom of the window
		GLint bufferRect[] =
		{
			// Left
			viewFrame.origin.x,
			// Bottom (inverted, 0=Bottom)
			0, // rootBounds.size.height - (viewFrame.origin.y + viewFrame.size.height),
			// Width
			viewFrame.size.width,
			// Height
			viewFrame.size.height
		};


		// Set and enable AGL_BUFFER_RECT
		aglSetInteger(m_hRC, AGL_BUFFER_RECT, bufferRect);
		aglEnable(m_hRC, AGL_BUFFER_RECT);		

		return err;
	}
#endif	
	
public:	
	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		m_hParent = parent;
		HSUBVIEW hView = BaseSubView::Create(parent, rect, dwStyle, dwExStyle);
		
		SetupGLPixelFormat(parent, m_hWnd);

/*
		HDC hDC = GetDC(m_hWnd);

		SetupPixelFormat(hDC);

		m_hRC = wglCreateContext(hDC);   
		wglMakeCurrent(hDC, m_hRC);   

		ReleaseDC(m_hWnd, hDC);

		m_imgBack.Load("d:\\320.bmp");
//		glViewport(0, 0, 420, 240 );

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 1024, 1024, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		

		glEnable(GL_TEXTURE_2D);
//		glShadeModel(GL_FLAT);

*/

		return hView;

	}

	
	BEGIN_SUB_MESSAGE(GLView, BaseSubView)
		ON_W32MESSAGE(WM_PAINT, OnPaint)
		ON_W32MESSAGE(WM_SIZE, OnSize)
		ON_W32MESSAGE(WM_MOUSEMOVE, OnMouseMove)
	END_SUB_MESSAGE()

	
	void SetCurrentContext()
	{
#ifdef WIN32
		HDC hDC = GetDC(m_hWnd);
		wglMakeCurrent(hDC, m_hRC);   
		ReleaseDC(m_hWnd, hDC);
#else
		aglSetCurrentContext(m_hRC);
#endif
	}

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{

		int w = LOWORD(lParam);
		int h = HIWORD(lParam);
		
		if (m_hRC)
		{
			SetCurrentContext();
			glViewport(0, 0, w, h );
		}

		printf("GLView::OnSize (%dx%d)\r\n", w, h);

#ifndef WIN32 
		GLint bufferRect[] =
		{
			0,
			0, 
			w,
			h
		};
		
		// Set and enable AGL_BUFFER_RECT
		aglSetInteger(m_hRC, AGL_BUFFER_RECT, bufferRect);
#endif
		
		return 0;
	}

	void vglLine(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat r1, GLfloat g1, GLfloat b1, 
				 GLfloat x2, GLfloat y2, GLfloat z2, GLfloat r2, GLfloat g2, GLfloat b2)
	{
		glBegin(GL_LINES); 
			glColor3f(r1, g1, b1);   
			glVertex3f(x1, y1, z1);
			glColor3f(r2, g2, b2);   
			glVertex3f(x2, y2, z2); 
		glEnd();   
	}
		
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
	//	printf("GLView::OnPaint\r\n");
	
		SetCurrentContext();
		
		HDC hDC = (HDC)wParam;
		VRECT rect = { 0 };
		GetClientRect(m_hWnd, &rect);

		int cx = rect.cx;
		int cy = rect.cy;

		GLfloat mx = (cx - 1) / (GLfloat)cx;
		GLfloat my = (cy - 1) / (GLfloat)cy;

		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glLoadIdentity();

//		glEnable(GL_LINE_SMOOTH);
//		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
		GLfloat y0 = 0;
		y0 = - (GLfloat)(((GLfloat)cy - 1) / 2) / ((GLfloat)cy / 2);
	
		vglLine(-1.0f,    y0, 0.0f,   1.0f, 1.0f, 1.0f,      mx, y0, 0.0f,   1.0f, 0.0f, 0.0f);
		vglLine(   mx, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,      mx,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f);
		vglLine(   mx,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   -1.0f,    my, 0.0f,   0.0f, 1.0f, 1.0f);
		vglLine(-1.0f,    my, 0.0f,   1.0f, 0.0f, 1.0f,   -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 0.0f);

		vglLine(-0.0f,  0.2, 0.0f,   1.0f, 1.0f, 0.0f,   -0.2f, 0.2f, 0.0f,   0.0f, 1.0f, 1.0f);
		vglLine(-0.2f,  0.2, 0.0f,   1.0f, 1.0f, 1.0f,   -0.3f,-0.2f, 0.0f,   1.0f, 0.0f, 0.0f);
		vglLine(-0.3f,  -0.2, 0.0f,   1.0f, 1.0f, 1.0f,  -0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 1.0f);
		vglLine(-0.1f,  -0.2, 0.0f,   0.0f, 1.0f, 0.0f,  -0.05f, -0.0f, 0.0f,   0.0f, 1.0f, 1.0f);
		vglLine(-0.05f,  -0.0, 0.0f,   1.0f, 1.0f, 1.0f,  -0.25f, -0.0f, 0.0f,   0.0f, 1.0f, 1.0f);

		vglLine( 0.2f,  0.2, 0.0f,   1.0f, 0.0f, 0.0f,  0.1f, -0.2f, 0.0f,   0.0f, 1.0f, 1.0f);
		vglLine( 0.1f, -0.2, 0.0f,   1.0f, 1.0f, 1.0f,  0.3f, -0.2f, 0.0f,   1.0f, 1.0f, 0.0f);


	
		SWAP_BUFFERS;
		
		return 0;
	}
		
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		printf("GLView::OnMouseMove (%dx%d)\r\n", LOWORD(lParam), HIWORD(lParam));
		return 0;
	}



};
