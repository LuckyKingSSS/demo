// W32View.h

#pragma once

#include "W32Wnd.h"
#include "ViewCommon.h"

//extern HINSTANCE g_hInstance;

class W32View : public W32Wnd  
{
public:
	W32View()
		: m_bLDown(FALSE)
	{
	}
	
	~W32View()
	{
	}

	ATOM W32ViewRegisterClass(HINSTANCE hInstance)
	{
		WNDCLASSEX wcex = { 0 };

		wcex.cbSize = sizeof(WNDCLASSEX); 

		wcex.style			= CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc	= (WNDPROC)W32View::W32ViewWindowProc;
		wcex.cbClsExtra		= 0;
		wcex.cbWndExtra		= 0;
		wcex.hInstance		= hInstance;
		wcex.hIcon			= 0;
		wcex.hCursor		=  LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground	= (HBRUSH)(0);
//		wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
		wcex.lpszMenuName	= 0;
		wcex.lpszClassName	= _T("Win32 View");

		return RegisterClassEx(&wcex);
	}
	
public:	
	HVIEW Create(const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0)
	{
		W32ViewRegisterClass(0);

		if (0 == dwStyle)
		{
			dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		}

		RECT rc = { rect.x, rect.y, rect.x + rect.cx, rect.y + rect.cy };
		W32Wnd::Create(0, rc, _T("Win32 View"), _T("Win32 Main View"), dwStyle, dwExStyle, 0, 0);

		return m_hWnd;
	}

	HSUBVIEW Create(HVIEW parent, const VRECT& rect, DWORD dwStyle = 0, DWORD dwExStyle = 0, UINT nID = 0)
	{
		W32ViewRegisterClass(0);

		if (0 == dwStyle)
		{
			dwStyle = WS_CLIPCHILDREN;
		}

		dwStyle |= WS_CHILD;
		RECT rc = { rect.x, rect.y, rect.x + rect.cx, rect.y + rect.cy };

		W32Wnd::Create(parent, rc, _T("Win32 View"), _T("Win32 Sub View"), dwStyle, dwExStyle, nID, 0);

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

	static LRESULT CALLBACK W32ViewWindowProc(
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

		W32View * pWnd =  (W32View *)::GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pWnd)
		{
			return pWnd->W32DefWindowProc(Msg, wParam, lParam);
		}

		return ::DefWindowProc(hWnd, Msg, wParam, lParam);
		
	}
	
protected:

	LRESULT W32DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
	{
		BOOL bRet = FALSE;
		if (WM_PAINT == message)
		{
			LRESULT lRes = 0;
			PAINTSTRUCT ps;
			HDC hDC = BeginPaint(m_hWnd, &ps);

			wParam = (WPARAM)hDC;
			bRet = ProcessWindowMessage(m_hWnd, message, wParam, lParam, lRes, 0);
			if (!bRet)
			{
				VRECT rect = { 0 };
				GetClientRect(m_hWnd, &rect);

				
				FillSolidRect(hDC, &rect, 0x00C0C0C0);
				//StrokeRect(hDC, &rect, 0x0000FF00);

			//	bRet = TRUE;
			}

			EndPaint(m_hWnd, &ps);

			return 0;
		}

		if (!bRet)
		{
			LRESULT lRes = 0;
			UINT msg = message;
			switch(message)
			{
			case WM_LBUTTONDOWN:
				m_bLDown = TRUE;
				SetCapture(m_hWnd);
				break;

			case WM_LBUTTONUP:
				ReleaseCapture();
				m_bLDown = FALSE;
				break;

			case WM_MOUSEMOVE:
				if ((MK_LBUTTON & wParam) && m_bLDown)
					msg = WM_MOUSEDRAG;
				break;


			}
			BOOL bRet = ProcessWindowMessage(m_hWnd, msg, wParam, lParam, lRes, 0);
			if (bRet)
			{
				return lRes;
			}
		}

		if (W32Wnd::W32DefWindowProc == m_pSuper)
			return ::DefWindowProc(m_hWnd, message, wParam, lParam);

		if (m_pSuper)
			return CallWindowProc(m_pSuper, m_hWnd, message, wParam, lParam);

		return ::DefWindowProc(m_hWnd, message, wParam, lParam);
	}

	BOOL	m_bLDown;

};


