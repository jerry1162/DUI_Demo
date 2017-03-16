#pragma once
#include "ControlBase.h"
class DUI_Lable :public ControlBase
{
public:
	DUI_Lable();
	virtual ~DUI_Lable();
	VOID SetTextColor(Color color);
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw();

	Color m_TextColor;
};

