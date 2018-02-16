#pragma once
#include "DUI_Window.h"
#include "Functions.h"
/*
enum DUI_Status
{
	S_Normal = 1,
	S_HighLight,
	S_Pushed,
	S_Focus,
	S_Disabled,
	//S_Hide,
	S_Invalid = -1
};*/
class DUI_Window;

//typedef VOID  (CALLBACK *TIMERPROC)(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);

class DUI_ControlBase
{
public:
	friend class DUI_Window;
	DUI_ControlBase();
	virtual ~DUI_ControlBase();
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE, BOOL bForceUpdate = TRUE);  //�ı�����Ĭ��Ϊ�ؼ�����.
	virtual BOOL Destroy();
	INT GetID();
	VOID SetText(LPTSTR Text = L"");
	LPCWSTR GetText();
	VOID SetAlpha(BYTE Alpha);
	VOID Move(REAL Left = NULL, REAL Top = NULL);
	BOOL Existence();//���ظ�����Ƿ���ڣ����Ƿ��Ѿ�������
	VOID SetVisiable(BOOL bVisiable);
	BOOL GetVisiable();
	VOID Update(INT bForce = -1);
	LRESULT SendMessage(INT ID, UINT Msg, WPARAM wParam, LPARAM lParam);//��ָ���ؼ�������Ϣ��ֱ��ͨ�������ڵ���Ϣ����������������Ϣѭ��
	VOID SetPrompt(LPTSTR Text);
	REAL GetX();
	REAL GetY();
	REAL GetWidth();
	REAL GetHeight();
	MSGPROC SetMsgProc(MSGPROC Proc);
	BOOL HasState(INT bHasState = -1);//-1��ʾ��ȡ״̬
	BOOL MoveWithMouse(INT b = -1);//-1��ʾ��ȡ״̬
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
	
	//�ؼ�����
	GdipString* m_Text;
	LPTSTR m_Prompt;
	BOOL m_bVisialbe;
	DUI_Status m_CurState;
	BYTE m_Alpha;
	BOOL m_bInAnimating;
	BYTE m_AnimateAlpha;
	DUI_Status m_PrevState;
	TIMERPROC m_pAnimateProc;
	MSGPROC m_MsgProc;
	MemDC* m_AnimatePrevDC;//������ʼ��DC����
	BOOL m_bHasState;
	BOOL m_bAutoUpdate;//�ڴ�����ʾǰ�Ŀؼ�Ĭ��Ϊ�٣����ɴ�������Ϊ�棬����Ĭ��Ϊ�档
	BOOL m_bMoveWithMouse;//����갴��ʱ����������ƶ��� ע�⣬�ƶ�ʱ�޷�������������Ϣ��

	//��Ϣ��Ӧ����
	BOOL OnUpdate(WPARAM wParam, LPARAM lParam);//lParam��ʾ�Ƿ���µ�������
};

