#pragma once
#include "DUI_ControlBase.h"
class DUI_Lable :public DUI_ControlBase
{
public:
	DUI_Lable();
	virtual ~DUI_Lable() override;
	VOID SetTextColor(Color color);
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s) override;

	Color m_TextColor;
};

