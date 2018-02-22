#pragma once
#include <vector>
#include <typeinfo>
using namespace std;
using namespace Gdiplus;
#include "Functions.h"
#include "MemDC.h"

enum ObjType
{
	ObjType_Unknown = 0,
	ObjType_Wnd,
	ObjType_Ctrl
};

//定义窗口基本属性
#define TITLEBARHEIGHT (20)
#define MINWNDWIDTH (200)
#define ICONSIZE_X (16)
#define ICONSIZE_Y (16)
#define SHADOWWIDTH (0) //暂时不使用阴影
#define INVALID_CONTROLID (-1)
#define DEF_BORDERWHIDTH (5)
#define GAP_ICON_TITLE (5)
#define GAP_TITLE_SYSBTN (2)
#define ICONOFFSET_X (3)
#define ICONOFFSET_Y (3)
#define SYSBTN_X (39)
#define SYSBTN_Y (19)

//自定义消息
//说明当窗口接收到hWnd为非有效窗口句柄时，表明此消息为发送给控件的消息，hWnd值为控件ID
#define WM_UPDATE (WM_USER + 1) //对窗口来说，wParam是指向需要更新的RectF的指针,为空则刷新整个窗口,lParam表示是否更新到窗口上   对控件来说，wParam是指控件ID，lParam相同
#define WM_WNDINITED (WM_USER + 2) //用于处理窗口初始化完后的一些工作

#define CM_MOUSELEAVE (WM_USER + 99)  //表示鼠标离开控件,wParam为控件ID，lParam未用
#define CM_MOUSEGETIN (WM_USER + 100) //表示鼠标进入控件，注意，此消息与WM_MOUSEMOVE不同，只在鼠标进入控件时发送，鼠标在控件上移动时不会发送此消息
#define CM_STATECHANGED (WM_USER + 101)//当控件状态发生变化时，收到此消息，wParam是指控件ID，lParam为上一个状态 返回0表示不需要在ChangeState里调用Update.
#define CM_CLICKED (WM_USER + 102)//控件的按下事件 wParam是指控件ID，lParam未用
#define CM_SHOW (WM_USER + 103)//组件显示或隐藏 wParam是指控件ID，lParam表示显示状态
	#define CS_SHOW (1)
	#define CS_HIDE (0)
#define CM_SETAUTOUPDATE (WM_USER + 104)//用于设置自动刷新属性 wParam是指控件ID，lParam表示属性值
#define CM_CALCRECT (WM_USER + 105)//用于刷新控件矩形 wParam是控件ID，lParam为NULL
#define CM_ADDCTRL (WM_USER + 106) //用于通知窗口增加了一个组件，wParam为控件指针，lParam未用。


#define RG_ADDRADIO (WM_USER + 107)//当单选框创建时，向其单选组发送此消息 wParam是单选框ID，lParam未用
#define RG_SELECTCHANGE (WM_USER + 108)//当单选框状态改变时，向其单选组发送此消息 wParam是新选中的单选框ID，lParam未用
#define RG_GROUPCHANGE (WM_USER + 109)//当单选框改变所属单选组时，向其单选组发送此消息 wParam是单选框ID，lParam未用

#define CM_CREATE (WM_USER + 109) //当组件创建完毕由ControlBase分发给组件，wParam是控件ID，lParam未用
#define CM_SETCURSOR (WM_USER + 110) //需要设置鼠标指针时发送，wParam是控件ID，lParam未用
#define CM_MOVE (WM_USER + 111) //当组件位置被移动后发送给控件，也就是由Move发送，wParam是控件ID，lParam未用
#define CM_SIZE (WM_USER + 112) //当组件大小被改变后发送给控件，也就是由Size发送，wParam是控件ID，lParam未用
#define CM_PARENTDESTROY (WM_USER + 113) //父组件被销毁前发送给控件，wParam是控件ID，lParam未用





typedef BOOL(CALLBACK *MSGPROC) (VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);//返回值表示此消息是否要继续传递，真则表示继续向下传递。注：用户消息处理会先于对象本身的消息处理。
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
	BT_Close = 1,
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


enum DUI_Status
{
	S_Normal = 1,
	S_HighLight,
	S_Pushed,
	S_Focus,
	S_Disabled,
	//S_Hide,
	S_Invalid = -1
};