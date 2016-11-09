#pragma once
#include "DUI_Window.h"
#include "Functions.h"
class DUI_Window;
enum State
{
	Normal = 1,
	HighLight,
	Pushed,
	Disabled
};
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
protected:
	INT m_ID;
	INT m_Index;
	DUI_Window* m_Parent;
	RectF* m_Rect;
	MemDC* m_MemDC;
	LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID Draw();
	//static ControlBase* m_This;
	
	//�ؼ�����
	GdipString* m_Text;
	BOOL m_bVisialbe;
	State m_CurState;
	//��Ϣ��Ӧ����
	BOOL OnUpdate(WPARAM wParam = NULL, LPARAM lParam = NULL);//lParam��ʾ�Ƿ���µ�������

	
};
