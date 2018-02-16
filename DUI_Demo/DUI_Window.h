#pragma once
#include "Def.h"
#include "Functions.h"
//C:\Users\Jerry\Desktop\VCProject\ResDataBase\ResDataBase
#include "C:\\Users\\Jerry\\Desktop\\VCProject\\ResDataBase\\ResDataBase\\RDBManager.h"
#include "DUI_ControlBase.h"
#include "DUI_Prompt.h"
#include <vector>
using std::vector;

/*
//定义窗口基本属性
#define TITLEBARHEIGHT 20
#define MINWNDWIDTH 200
#define ICONSIZE_X 16
#define ICONSIZE_Y 16
#define SHADOWWIDTH 0 //暂时不使用阴影
#define INVALID_CONTROLID -1
//#define INVALID_CONTROLINDEX -1
#define DEF_BORDERWHIDTH 5
#define GAP_ICON_TITLE 5
#define GAP_TITLE_SYSBTN 2
#define ICONOFFSET_X 3
#define ICONOFFSET_Y 3
#define SYSBTN_X 39
#define SYSBTN_Y 19

//自定义消息
//说明当窗口接收到hWnd为非有效窗口句柄时，表明此消息为发送给控件的消息，hWnd值为控件ID
#define WM_UPDATE WM_USER + 1 //对窗口来说，wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上   对控件来说，wParam是指控件ID，lParam相同
#define WM_MOUSEGETIN WM_USER + 2 //表示鼠标进入控件，注意，此消息与WM_MOUSEMOVE不同，只在鼠标进入控件时发送，鼠标在控件上移动时不会发送此消息
#define WM_STATECHANGED WM_USER + 3//当控件状态发生变化时，收到此消息，wParam是指控件ID，lParam为上一个状态 返回0表示不需要在ChangeState里调用Update.
#define CM_CLICKED WM_USER + 4//按钮控件的按下事件 wParam是指控件ID，lParam为Extra信息
#define CM_SHOW WM_USER + 5//组件显示或隐藏 wParam是指控件ID，lParam表示显示状态
	#define CS_SHOW 1
	#define CS_HIDE 0
typedef LRESULT(CALLBACK *MSGPROC) (VOID* pThis,UINT uMsg, WPARAM ID, LPARAM ExtraInfo);
//typedef LRESULT(CALLBACK *CLICKPROC) (VOID* pThis,UINT uMsg, WPARAM ID, LONG Extra);

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
	BT_Maxmize,
	BT_Minimize,
	BT_None
};

struct AnimArg
{
	INT Arg_1;
	INT Arg_2;
	MemDC* pDC1;
	MemDC* pDC2;
	AnimArg()
	{
		Arg_1 = 0;
		Arg_2 = 0;
		pDC1 = nullptr;
		pDC2 = nullptr;
	}
};

//typedef BOOL(CALLBACK *WNDANIMPROC)(AnimArg* pArg);
*/
class DUI_Window
{
public:
	friend class DUI_ControlBase;
	friend class DUI_ImageButton;
	DUI_Window();
	~DUI_Window();
	BOOL Create(HWND hWnd, LPTSTR Title = L"", LPTSTR Icon = NULL,
		LPTSTR BackgrdPic = NULL, BOOL bSizeable = FALSE);
	BOOL Create(HWND hWnd, LPTSTR Title = L"", LPTSTR Icon = NULL,
		ARGB BkgColor = Color::MakeARGB(255, 240, 240, 240), BOOL bSizeable = FALSE);
	BOOL Create(INT Width = 380, INT Height = 250, DUI_Window* Parent = nullptr, LPTSTR Title = L"", LPTSTR Icon = NULL);//请在用此方法创建窗口后手动调用DoModel()进入消息循环。
	VOID DoModel();
	BOOL Destroy();
	
	BOOL SetBkgPic(LPTSTR BackgrdPic = NULL);
	BOOL SetBkgColor(ARGB BackgrdColor = NULL);
	BOOL SetTitle(LPTSTR Title, BOOL bInner = TRUE);//第二个参数用于区别是否是内部调用，默认为真，用于处理外部程序用消息设置窗口标题的情况
	BOOL SetBorderStyle(BorderStyle bs);
	BOOL SetSizeable(BOOL bSizeable);
	BOOL GetSizeable();
	DUI_ControlBase* FindControlByID(INT ID, _Out_ INT* Index = nullptr);
	VOID SetDebugMode(BOOL bDebug);
	VOID Update(BOOL bForce = TRUE);
	DUI_Prompt* GetWndPrompt();
	INT ScreenToClient(Point* pt);
	BOOL GetCursorPos(Point* pt);
	RDBManager* GetRDBMgr();
	VOID AddControl(DUI_ControlBase* pControl);
	BOOL IsWindowInited();
	MSGPROC SetMsgProc(MSGPROC Proc);

	HWND m_hWnd;
private:
	//static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM ID, LPARAM lParam);
	VOID DrawWndBkg();
	DUI_Window* m_Parent;
	HDC m_hDC;
	MemDC* m_MemDC;
	MemDC* m_BkgDC;
	Gdiplus::Graphics* m_Graphics;
	Gdiplus::RectF* m_WndRect;
	Gdiplus::RectF* m_ClientRect;

	LONG PrevWndProc;
	WNDPROC m_WndProc;

	BOOL InitDUIWnd(HWND hWnd, LPTSTR Title = L"", BOOL bSizable = FALSE);
	VOID InitRes();
	BOOL OnControl(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam);//返回Ture表示消息不需要继续传递 ID为INVALID_CONTROLID(-1)则表示此消息被发送给所有控件

	//MsgProcFuncs
	BOOL OnMouseMove(WPARAM wParam,Point* ptMouse);
	BOOL OnLButtonDown(WPARAM wParam, Point* ptMouse);
	BOOL OnLButtonUp(WPARAM wParam, Point* ptMouse);
	BOOL OnMouseLeave(WPARAM wParam, Point* ptMouse);
	BOOL OnSize(WPARAM wParam, LPARAM lParam);
	BOOL OnMove(WPARAM wParam, LPARAM lParam);
	BOOL OnPaint(WPARAM wParam, LPARAM lParam);
	BOOL OnUpdate(WPARAM wParam, BOOL bUpdate);//wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上  //现已取消局部更新
	BOOL OnSetCursor(WPARAM wParam, LPARAM lParam);
	BOOL OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
	BOOL OnTimer(WPARAM wParam, LPARAM lParam);
	BOOL OnShowWindow(WPARAM wParam, LPARAM lParam);
	BOOL OnDropFiles(WPARAM wParam, LPARAM lParam);
	BOOL OnWndInited(WPARAM wParam, LPARAM lParam);
	//BOOL OnWindowPosChanged(WPARAM wParam, LPARAM lParam);

	BYTE m_Alpha;
	BOOL m_bInited;//当处理完WM_SHOWWINDOW后，此标识为真
	GdipString* m_Title;
	Color* m_BkgColor;
	Image* m_Icon;
	RectF* m_IconRect;
	Image* m_BkgImg;
	Image* m_SysBtnPic;
	BOOL m_Sizeable;
	BorderStyle m_BorderStyle;
	DUI_ImageButton* m_SysBtn[4];
	MSGPROC m_SysBtnClickProcAddr;
	BOOL CALLBACK SysBtnClick(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);
	INT m_SysBtnCnt;
	
	BOOL m_bMouseDown;
	BOOL m_bDebug;

	DUI_Prompt* m_Prompt;
	vector<DUI_ControlBase*>* m_Controls;
	INT  m_CaptureCtrlID;
	INT m_FocusCtrlID;
	INT m_LastHoverCtrlID;
	CursorDirection m_CurCDR;

	RDBManager* m_pRdbMgr;
	AnimArg* m_pAnimArg;
	MSGPROC m_MsgProc;
	//WNDANIMPROC m_lpfnAnimProc;
	BOOL WndAnimProc(AnimArg* pArg);
	BOOL m_bAnimate;
};