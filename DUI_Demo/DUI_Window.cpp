#include "stdafx.h"
#include "DUI_Window.h"



DUI_Window::DUI_Window()
{
	static DUI_Window* m_pCurObj = this;
	static LONG PrevWndProc = NULL;
}

DUI_Window::DUI_Window(HWND hWnd)
{
	static DUI_Window* m_pCurObj =this;
	static LONG PrevWndProc = NULL;
	Create(hWnd);
}


DUI_Window::~DUI_Window()
{

}

LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static DUI_Window* m_pCurObj;
	static LONG PrevWndProc;

	switch (uMsg)
	{
	case WM_CREATE:
		MessageBeep(MB_ICONINFORMATION);
		break;
	default:
		break;
	}

	return CallWindowProc(WNDPROC(PrevWndProc),hwnd,uMsg,wParam,lParam);
}

BOOL DUI_Window::Create(HWND hWnd)
{
	
	m_Graphics = Graphics::FromHWND(m_hWnd);
	RECT rect;
	GetWindowRect(hWnd, &rect);
	m_WndRect.X = rect.left;
	m_WndRect.Y = rect.top;
	m_WndRect.Width = rect.right - rect.left;
	m_WndRect.Height = rect.bottom - rect.top;
	m_MemDC->Create(m_WndRect.Width, m_WndRect.Height);
	LPWSTR str= L"";
	GetWindowText(hWnd, str, 256);
	m_Caption->string = str;
	m_Caption->format.GenericDefault();
	static LONG PrevWndProc;
	PrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, LONG(DUI_Window::WndProc));
	m_hWnd = hWnd;
	return 0;
}

BOOL DUI_Window::Destroy()
{

	return 0;
}
