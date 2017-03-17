#pragma once
#include "Functions.h"
#include "ControlBase.h"
#include <vector>
using std::vector;

//定义窗口基本属性
#define TITLEBARHEIGHT 20
#define ICONSIZE_X 16
#define ICONSIZE_Y 16
#define SHADOWWIDTH 0 //暂时不使用阴影
#define INVALID_CONTROLID -1
#define INVALID_CONTROLINDEX -1
#define DEF_BORDERWHIDTH 5
#define GAP_ICON_TITLE 5
#define GAP_TITLE_SYSBTN 2
#define ICONOFFSET_X 3
#define ICONOFFSET_Y 3

//自定义消息
#define WM_UPDATE WM_USER + 1 //对窗口来说，wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上   对控件来说，wParam是指控件ID，lParam相同
#define WM_MOUSEGETIN WM_USER + 2 //表示鼠标进入控件，注意，此消息与WM_MOUSEMOVE不同，只在鼠标进入控件时发送，鼠标在控件上移动时不会发送此消息
enum BorderMode
{
	BM_Normal = 1,//普通模式，即矩形边框
	BM_RoundRect
};
struct BorderStyle
{
	BorderMode Mode;
	ARGB Color;
	BOOL DoubleBorder;
	BOOL operator==(BorderStyle bs)
	{
		return (Mode == bs.Mode&&Color == bs.Color&&DoubleBorder == bs.DoubleBorder);
	}
};
enum CursorDirection
{
	CDR_Normal = 0,
	CDR_NWSE,
	CDR_NS,
	CDR_NESW,
	CDR_WE,
	CDR_NWSE2,
	CDR_NS2,
	CDR_NESW2,
	CDR_WE2
};
enum SYSBTNTYPE
{
	BT_Close=1,
	BT_Minimize,
	BT_Maxmize,
	BT_Setting,
	BT_Help,
	BT_None
};
enum BTNStatus
{
	Normal = 1,
	Pushed,
	HighLight,
	Focus,
	Disabled
};
struct SYSBTN
{
	SYSBTNTYPE type;
	RectF* rect;
	BTNStatus status;
};
class DUI_Window
{
public:
	friend class ControlBase;
	friend class DUI_Lable;
	friend class DUI_Button;
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
	BOOL SetBorderStyle(BorderStyle bs);
	BOOL SetSizeable(BOOL bSizeable);
	BOOL GetSizeable();
	INT FindControlByID(INT ID);
	VOID SetDebugMode(BOOL bDebug);
private:
	//static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID DrawWnd();

	HWND m_hWnd;
	MemDC* m_MemDC;
	Gdiplus::Graphics* m_Graphics;
	Gdiplus::RectF* m_WndRect;
	Gdiplus::RectF* m_ClientRect;

	//static DUI_Window* m_pThis;
	//static LONG PrevWndProc;
	LONG PrevWndProc;
	WNDPROC m_WndProc;

	BOOL InitDUIWnd(HWND hWnd, LPCWSTR Title = L"", BOOL bSizable = FALSE);
	BOOL OnControl(UINT uMsg, WPARAM wParam, LPARAM lParam);//返回Ture表示消息不需要继续传递
	INT ScreenToClient(Point* pt);
	BOOL GetCursorPos(Point* pt);

	//MsgProcFuncs
	BOOL OnMouseMove(WPARAM wParam,Point* ptMouse);
	BOOL OnLButtonDown(WPARAM wParam, Point* ptMouse);
	BOOL OnLButtonUp(WPARAM wParam, Point* ptMouse);
	BOOL OnMouseLeave(WPARAM wParam, Point* ptMouse);
	BOOL OnSize(WPARAM wParam, LPARAM lParam);
	BOOL OnPaint(WPARAM wParam, LPARAM lParam);
	BOOL OnUpdate(WPARAM wParam=NULL, BOOL bUpdate = TRUE);//wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上  //现已取消局部更新
	BOOL OnSetCursor(WPARAM wParam, LPARAM lParam);

	BYTE m_Alpha;
	GdipString* m_Title;
	Color* m_BkgColor;
	Image* m_Icon;
	RectF* m_IconRect;
	Image* m_BkgImg;
	Image* m_SysBtn;
	BOOL m_Sizeable;
	BorderStyle m_BorderStyle;
	SYSBTN m_SysBtn_Close;
	SYSBTNTYPE m_LastBtn;
	BOOL m_bMouseDown;
	BOOL m_bDebug;


	vector<ControlBase*>* m_Controls;
	INT  m_CaptureCtrlID;
	INT m_FocusCtrlID;
	//INT m_HoverControlID;
	CursorDirection m_CurCDR;
};