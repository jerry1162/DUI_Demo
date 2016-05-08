#pragma once
#include "Functions.h"
#include "ControlBase.h"
#include <vector>
using std::vector;

//定义窗口基本属性
#define TITLEBARHEIGHT 20
#define ICONSIZE_X 16
#define ICONSIZE_Y 16
#define SHADOWWIDTH 0//4 暂时不使用阴影

//自定义消息
#define WM_UPDATE WM_USER + 1 //wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上

class DUI_Window
{
public:
	friend class ControlBase;
	DUI_Window();
	~DUI_Window();
	BOOL Create(HWND hWnd, LPCWSTR Title = L"", LPCWSTR Icon = NULL,
		LPCWSTR BackgrdPic = NULL, BOOL bSizeable = FALSE);
	BOOL Create(HWND hWnd, LPCWSTR Title = L"", LPCWSTR Icon = NULL,
		ARGB BkgColor = Color::MakeARGB(255, 240, 240, 240), BOOL bSizeable = FALSE);
	BOOL Destroy();
	
	BOOL SetBkgPic(LPCWSTR BackgrdPic = NULL);
	BOOL SetBkgColor(ARGB BackgrdColor = NULL);
	BOOL SetTitle(LPCWSTR Title);
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

	BOOL InitDUIWnd(HWND hWnd, LPCWSTR Title = L"");
	BOOL OnControl(UINT uMsg, WPARAM wParam, LPARAM lParam);//返回Ture表示消息不需要继续传递
	INT ScreenToClient(Point* pt);
	BOOL GetCursorPos(Point* pt);

	//MsgProcFuncs
	BOOL OnMouseMove(WPARAM wParam,Point* ptMouse);
	BOOL OnLButtonDown(WPARAM wParam, Point* ptMouse);
	BOOL OnLButtonUp(WPARAM wParam, Point* ptMouse);
	BOOL OnSize(WPARAM wParam, LPARAM lParam);
	BOOL OnPaint(WPARAM wParam, LPARAM lParam);
	BOOL OnUpdate(RectF* Rect = NULL, BOOL bUpdate = FALSE);//wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上


	GdipString* m_Title;
	Color* m_BkgColor;
	Image* m_Icon;
	Image* m_BkgImg;

	vector<ControlBase*>* m_Controls;
	INT m_CaptureControlIndex;
};