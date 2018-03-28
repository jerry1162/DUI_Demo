#pragma once
#include "DUI_Object.h"
#include "DUI_Window.h"

class DUI_Window;

class DUI_ControlBase :public DUI_Object
{
public:
	friend class DUI_Window;
	friend class DUI_ScrollBar;
	DUI_ControlBase();
	virtual ~DUI_ControlBase();
	virtual BOOL Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE, BOOL bForceUpdate = TRUE);  //�ı�����Ĭ��Ϊ�ؼ�����.
	virtual BOOL IsPtInCtrl(PointF* pt);//ע�⣺�˴�����Ϊ����ڴ��ڵ�����

	//�麯��ʵ��
	virtual BOOL Destroy() override;
	virtual ObjType GetObjType() override;
	virtual REAL GetX() override;
	virtual REAL GetY() override;
	virtual REAL GetWidth() override;
	virtual REAL GetHeight() override;
	virtual BOOL GetCursorPos(PointF* pt) override;
	virtual REAL GetMarginTop() override;
	virtual RectF* GetClientRect() override;

	VOID SetText(LPTSTR Text = L"");
	LPCWSTR GetText();
	VOID SetAlpha(BYTE Alpha);
	VOID Move(REAL Left = NULL, REAL Top = NULL);
	VOID Size(REAL Width = -1, REAL Height = -1); //-1��ʾ���ֲ���
	BOOL Existence();//���ظ�����Ƿ���ڣ����Ƿ��Ѿ�������
	VOID SetVisiable(BOOL bVisiable);
	BOOL GetVisiable();
	VOID Update(INT bForce = -1);
	//LRESULT SendMessage(INT ID, UINT Msg, WPARAM wParam, LPARAM lParam);//��ָ���ؼ�������Ϣ��ֱ��ͨ�������ڵ���Ϣ����������������Ϣѭ��
	VOID SetPrompt(LPTSTR Text);
	MSGPROC SetMsgProc(MSGPROC Proc);
	BOOL HasState(INT bHasState = -1);//-1��ʾ��ȡ״̬
	BOOL MoveWithMouse(INT b = -1);//-1��ʾ��ȡ״̬
	VOID SendMsgToChild(UINT uMsg, WPARAM wParam, LPARAM lParam);
	DUI_Window* GetParentWnd();
	VOID ClientToWnd(PointF* ptClient); //���������������ת��Ϊ����ڴ��ڵ�����
	VOID WndToClient(PointF* ptWnd);
	VOID SetbShowOnNCRgn(BOOL bShow);
	BOOL GetbShowOnNCRgn();
	LPTSTR SetCursor(LPTSTR CursorName);
	LPTSTR GetCursor();
	BOOL SetParent(DUI_Object* Parent);
	BOOL SetMinMaxInfo(MINMAXSIZE MinMaxInfo);//MinSize��ΪNULL��ȡĬ����С��Max��ΪNULL��Ϊ���������
	RectF* GetOffsetRect();
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual BOOL Create(DUI_Object* Parent);
	DUI_Object* m_Parent;
	DUI_Window* m_ParentWnd;
	RectF* m_LogicRect;//����ڸ�������߼�����
	RectF* m_Rect;//����ڴ��ڵ���ʵ����
	RectF* m_LogicVRect;
	RectF* m_VRect;
	RectF* m_OffsetRect;
	MemDC* m_MemDC;
	
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1);
	VOID ChangeState(DUI_Status s);
	DUI_Status GetState();
	BOOL StartAnimate(UINT uElapse = 30, TIMERPROC pCallBack = nullptr);
	BOOL EndAnimate();
	virtual VOID CALLBACK AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	VOID AddCtrl();//����this���뵽������
	BOOL SetOffset(REAL X = 0, REAL Y = 0, REAL Width = -1, REAL Height = -1);
	//�ؼ�����
	GdipString* m_Text;
	LPTSTR m_Prompt;
	BOOL m_bVisialbe;
	DUI_Status m_CurState;
	BYTE m_Alpha;
	BOOL m_bAnimating;
	//BYTE m_AnimateAlpha;
	DUI_Status m_PrevState;
	TIMERPROC m_pAnimateProc;
	MSGPROC m_MsgProc;
	//MemDC* m_AnimatePrevDC;//������ʼ��DC����
	BOOL m_bHasState;
	BOOL m_bAutoUpdate;//�ڴ�����ʾǰ�Ŀؼ�Ĭ��Ϊ�٣����ɴ�������Ϊ�棬����Ĭ��Ϊ�档
	BOOL m_bMoveWithMouse;//����갴��ʱ����������ƶ��� ע�⣬�ƶ�ʱ�޷�������������Ϣ��
	BOOL m_bMouseDown;
	PointF* m_ptMouseDown;
	BOOL m_bCanShowOnNCRgn;
	LPTSTR m_CursorName;
	MINMAXSIZE m_MinMaxInfo;
	BOOL m_bOffset;
	AnimArg* m_AnimArg;

	//��Ϣ��Ӧ����
	BOOL OnUpdate(WPARAM wParam, LPARAM lParam);//lParam��ʾ�Ƿ���µ�������
	BOOL OnCalcRect(WPARAM wParam = NULL, LPARAM lParam = NULL);
};

