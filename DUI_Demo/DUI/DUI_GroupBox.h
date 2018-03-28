#pragma once
#include "DUI_ControlBase.h"
class DUI_GroupBox : public DUI_ControlBase
{
public:
	DUI_GroupBox();
	virtual ~DUI_GroupBox();
	virtual RectF* GetClientRect() override;
	virtual REAL GetMarginTop() override;
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;
};

