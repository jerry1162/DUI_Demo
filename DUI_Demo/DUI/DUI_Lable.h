#pragma once
#include "DUI_ControlBase.h"
class DUI_Lable :public DUI_ControlBase
{
public:
	DUI_Lable();
	virtual ~DUI_Lable() override;
	VOID SetTextColor(Color color);
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;

	Color m_TextColor;
};

