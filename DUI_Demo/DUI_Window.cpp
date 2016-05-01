#include "stdafx.h"
#include "DUI_Window.h"

DUI_Window* DUI_Window::m_pCurObj;
LONG DUI_Window::PrevWndProc;

DUI_Window::DUI_Window()
{
	m_pCurObj = this;
	PrevWndProc = NULL;
}

DUI_Window::DUI_Window(HWND hWnd)
{
	m_pCurObj =this;
	PrevWndProc = NULL;
	Create(hWnd);
}


DUI_Window::~DUI_Window()
{

}

LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		MessageBeep(MB_ICONINFORMATION);
		break;
	default:
		break;
	}

	return CallWindowProc(WNDPROC(PrevWndProc),hwnd,uMsg,wParam,lParam);
}

BOOL DUI_Window::Create(HWND hWnd)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}
	m_hWnd = hWnd;
	m_Graphics = new Gdiplus::Graphics(hWnd);
	if (m_Graphics == NULL)
	{
		delete m_Graphics;
		return FALSE;
	}
	RECT rect;
	GetWindowRect(hWnd, &rect);
	m_WndRect.X = rect.left;
	m_WndRect.Y = rect.top;
	m_WndRect.Width = rect.right - rect.left;
	m_WndRect.Height = rect.bottom - rect.top;
	m_MemDC = new MemDC;
	if (m_MemDC->Create(m_WndRect.Width, m_WndRect.Height) != TRUE)
	{
		delete m_Graphics;
		delete m_MemDC;
		return FALSE;
	}
	LPTSTR  szTitle=L"UIDemo\0";
	//GetWindowText(hWnd, szTitle, 255);
	m_Caption = new GdipString;
	m_Caption->string = new CString;
	*(m_Caption->string) = szTitle;
	m_Caption->format->GenericDefault();
	PrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, LONG(DUI_Window::WndProc));
	if (PrevWndProc == NULL)
	{
		TRACE(L"×ÓÀà»¯Ê§°Ü");
		Destroy();
		return FALSE;
	}
	return TRUE;
}

BOOL DUI_Window::Destroy()
{
	delete m_MemDC;
	delete m_Caption;
	delete m_pCurObj;
	return 0;
}
