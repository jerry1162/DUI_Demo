#pragma once
#include "stdafx.h"
#include "Functions.h"

struct GdipString
{
	CString* string;
	Gdiplus::Font* font;
	StringFormat* format;
};


class DUI_Window
{
public:
	DUI_Window();
	DUI_Window(HWND hWnd);
	~DUI_Window();
	BOOL Create(HWND hWnd);
	BOOL Destroy();
private:
	static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HWND m_hWnd;
	MemDC* m_MemDC;
	Gdiplus::Graphics* m_Graphics;
	Gdiplus::RectF m_WndRect;
	GdipString* m_Caption;
	static DUI_Window* m_pCurObj;
	static LONG PrevWndProc;
};