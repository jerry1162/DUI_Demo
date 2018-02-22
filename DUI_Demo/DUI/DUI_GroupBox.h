#pragma once
#include "DUI_ControlBase.h"
class DUI_GroupBox : public DUI_ControlBase
{
public:
	DUI_GroupBox();
	virtual ~DUI_GroupBox();
	virtual BOOL CanBeParent() override;
	virtual REAL GetMarginTop() override;
	virtual RectF* GetClientRect() override;
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;
};

