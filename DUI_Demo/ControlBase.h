#pragma once
#include "DUI_Window.h"
#include "Functions.h"
class DUI_Window;
enum BTNStatus;
class ControlBase
{
public:
	friend class DUI_Window;
	ControlBase();
	~ControlBase();
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE);  //�ı�����Ĭ��Ϊ�ؼ�����.
	INT GetID();
	VOID SetText(LPCWSTR Text = L"");
	VOID SetAlpha(BYTE Alpha);
protected:
	INT m_ID;
	INT m_Index;
	DUI_Window* m_Parent;
	RectF* m_Rect;
	MemDC* m_MemDC;
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw();
	VOID ChangeStatus(BTNStatus s);
	BTNStatus GetStatus();
	BOOL GetCursorPos(Point* pt);
	//static ControlBase* m_This;
	
	//�ؼ�����
	GdipString* m_Text;
	BOOL m_bVisialbe;
	BTNStatus m_CurState;
	BYTE m_Alpha;
	//��Ϣ��Ӧ����
	BOOL OnUpdate(WPARAM wParam = NULL, LPARAM lParam = NULL);//lParam��ʾ�Ƿ���µ�������
};

