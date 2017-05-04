#pragma once
#include "DUI_Window.h"
#include "Functions.h"
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
class DUI_Window;

//typedef VOID  (CALLBACK *TIMERPROC)(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

class DUI_ControlBase
{
public:
	friend class DUI_Window;
	DUI_ControlBase();
	virtual ~DUI_ControlBase();
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE);  //文本矩形默认为控件矩形.
	virtual BOOL Destroy();
	INT GetID();
	VOID SetText(LPTSTR Text = L"");
	LPCWSTR GetText();
	VOID SetAlpha(BYTE Alpha);
	VOID Move(REAL Left = NULL, REAL Top = NULL);
	BOOL Existence();//返回该组件是否存在，即是否已经被创建
	VOID SetVisiable(BOOL bVisiable);
	BOOL GetVisiable();
	VOID Update(BOOL bForce = FALSE);
	LRESULT SendMessage(INT ID, UINT Msg, WPARAM wParam, LPARAM lParam);//向指定控件发送消息，直接通过父窗口的消息处理函数，而不是消息循环
	VOID SetPrompt(LPTSTR Text);
	BOOL m_bAutoUpdate;//是否在改变组件时自动刷新，默认为真，若需要连续的多次更改则可以设置为假然后手动刷新
protected:
	INT m_ID;
	//INT m_Index;
	DUI_Window* m_Parent;
	RectF* m_Rect;
	MemDC* m_MemDC;
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1);
	VOID ChangeState(DUI_Status s);
	DUI_Status GetState();
	BOOL GetCursorPos(Point* pt);
	BOOL StartAnimate(TIMERPROC pCallBack = nullptr, UINT uElapse = 30);
	BOOL EndAnimate();
	virtual VOID CALLBACK AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	//static ControlBase* m_This;
	
	//控件属性
	GdipString* m_Text;
	LPTSTR m_Prompt;
	BOOL m_bVisialbe;
	DUI_Status m_CurState;
	BYTE m_Alpha;
	BOOL m_bInAnimating;
	BYTE m_AnimateAlpha;
	DUI_Status m_PrevState;
	TIMERPROC m_pAnimateProc;
	MemDC* m_AnimatePrevDC;//动画开始的DC内容
	//消息响应函数
	BOOL OnUpdate(WPARAM wParam, LPARAM lParam);//lParam表示是否更新到窗口上
};

