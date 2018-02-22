#pragma once
#include "DUI_Object.h"
#include "DUI_Window.h"

class DUI_Window;

class DUI_ControlBase :public DUI_Object
{
public:
	friend class DUI_Window;
	DUI_ControlBase();
	virtual ~DUI_ControlBase();
	virtual BOOL Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE, BOOL bForceUpdate = TRUE);  //文本矩形默认为控件矩形.
	virtual BOOL IsPtInCtrl(Point* pt);//注意：此处坐标为相对于窗口的坐标

	//虚函数实现
	virtual BOOL Destroy() override;
	virtual ObjType GetObjType() override;
	virtual REAL GetX() override;
	virtual REAL GetY() override;
	virtual REAL GetWidth() override;
	virtual REAL GetHeight() override;
	virtual BOOL GetCursorPos(Point* pt) override;


	VOID SetText(LPTSTR Text = L"");
	LPCWSTR GetText();
	VOID SetAlpha(BYTE Alpha);
	VOID Move(REAL Left = NULL, REAL Top = NULL);
	VOID Size(REAL Width = -1, REAL Height = -1); //-1表示保持不变
	BOOL Existence();//返回该组件是否存在，即是否已经被创建
	VOID SetVisiable(BOOL bVisiable);
	BOOL GetVisiable();
	VOID Update(INT bForce = -1);
	//LRESULT SendMessage(INT ID, UINT Msg, WPARAM wParam, LPARAM lParam);//向指定控件发送消息，直接通过父窗口的消息处理函数，而不是消息循环
	VOID SetPrompt(LPTSTR Text);
	MSGPROC SetMsgProc(MSGPROC Proc);
	BOOL HasState(INT bHasState = -1);//-1表示获取状态
	BOOL MoveWithMouse(INT b = -1);//-1表示获取状态
	VOID SendMsgToChild(UINT uMsg, WPARAM wParam, LPARAM lParam);
	DUI_Window* GetParentWnd();
	VOID ClientToWnd(Point* ptClient); //将相对于自身的左边转换为相对于窗口的坐标
	VOID WndToClient(Point* ptWnd);
	VOID SetbShowOnNCRgn(BOOL bShow);
	BOOL GetbShowOnNCRgn();
	LPTSTR SetCursor(LPTSTR CursorName);
	LPTSTR GetCursor();
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	DUI_Object* m_Parent;
	DUI_Window* m_ParentWnd;
	RectF* m_LogicRect;//相对于父组件的逻辑坐标
	RectF* m_Rect;//相对于窗口的真实坐标
	RectF* m_LogicVRect;
	RectF* m_VRect;
	MemDC* m_MemDC;
	
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1);
	VOID ChangeState(DUI_Status s);
	DUI_Status GetState();
	BOOL StartAnimate(TIMERPROC pCallBack = nullptr, UINT uElapse = 30);
	BOOL EndAnimate();
	virtual VOID CALLBACK AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	VOID AddCtrl();//处理将this加入到窗口中
	//控件属性
	GdipString* m_Text;
	LPTSTR m_Prompt;
	BOOL m_bVisialbe;
	DUI_Status m_CurState;
	BYTE m_Alpha;
	BOOL m_bAnimating;
	BYTE m_AnimateAlpha;
	DUI_Status m_PrevState;
	TIMERPROC m_pAnimateProc;
	MSGPROC m_MsgProc;
	MemDC* m_AnimatePrevDC;//动画开始的DC内容
	BOOL m_bHasState;
	BOOL m_bAutoUpdate;//在窗口显示前的控件默认为假，后由窗口设置为真，其余默认为真。
	BOOL m_bMoveWithMouse;//当鼠标按下时，跟随鼠标移动。 注意，移动时无法完成其他鼠标消息。
	BOOL m_bMouseDown;
	Point* m_ptMouseDown;
	BOOL m_bCanShowOnNCRgn;
	LPTSTR m_CursorName;
	HCURSOR m_LastHCursor;

	//消息响应函数
	BOOL OnUpdate(WPARAM wParam, LPARAM lParam);//lParam表示是否更新到窗口上
	BOOL OnCalcRect(WPARAM wParam = NULL, LPARAM lParam = NULL);
};

