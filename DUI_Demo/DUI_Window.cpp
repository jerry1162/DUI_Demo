#include "stdafx.h"
#include "DUI_Window.h"

//LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	return LRESULT();
//}
static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return 1;
}
DUI_Window::DUI_Window(HWND hWnd)
{

}


DUI_Window::~DUI_Window()
{

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
	m_MemGraphics.Create(m_WndRect.Width, m_WndRect.Height);
	LPWSTR str= L"";
	GetWindowText(hWnd, str, 256);
	m_Caption->string = str;
	m_Caption->format.GenericDefault();

	m_hWnd = hWnd;
	return 0;
}

BOOL DUI_Window::Destroy()
{

	return 0;
}
