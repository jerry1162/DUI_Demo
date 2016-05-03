#pragma once
#include "stdafx.h"
#include "Functions.h"

//定义窗口基本属性
#define TITLEBARHEIGHT 20
#define ICONSIZE_X 16
#define ICONSIZE_Y 16
#define SHADOWWIDTH 0//4 暂时不使用阴影
#define WM_UPDATE WM_USER + 2

class DUI_Window
{
public:
	DUI_Window();
	DUI_Window(HWND hWnd);
	~DUI_Window();
	BOOL Create(HWND hWnd, LPCWSTR Title = L"", LPCWSTR Icon = NULL,
		LPCWSTR BackgrdPic = NULL, BOOL bSizeable = FALSE);
	BOOL Destroy();
	INT ScreenToClient(Point* pt);
	BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * &pImg);
private:
	static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID DrawWnd();

	HWND m_hWnd;
	MemDC* m_MemDC;
	Gdiplus::Graphics* m_Graphics;
	Gdiplus::RectF* m_WndRect;
	Gdiplus::RectF* m_ClientRect;
	static DUI_Window* m_pThis;
	static LONG PrevWndProc;

	//MsgProcFuncs
	VOID OnMouseMove(WPARAM wParam,LPARAM lParam);
	VOID OnLButtonDown(WPARAM wParam, LPARAM lParam);
	VOID OnSize(WPARAM wParam, LPARAM lParam);
	VOID OnPaint(WPARAM wParam, LPARAM lParam);

	GdipString* m_Title;
	Color* m_BackColor;
	Image* m_Icon;
	Image* m_BkgImg;

};