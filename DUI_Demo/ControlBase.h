#pragma once
#include "DUI_Window.h"
#include "Functions.h"
class DUI_Window;

class ControlBase
{
public:
	friend class DUI_Window;
	ControlBase();
	~ControlBase();
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE);  //文本矩形默认为控件矩形.
protected:
	INT m_ID;
	INT m_Index;
	DUI_Window* m_Parent;
	RectF* m_Rect;
	MemDC* m_MemDC;
	LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	VOID Draw();
	//static ControlBase* m_This;
	
	//控件属性
	GdipString* m_Text;
	BOOL m_bVisialbe;

	//消息响应函数
	BOOL OnUpdate(WPARAM wParam, LPARAM lParam);//lParam表示是否更新到窗口上

	
};

