#pragma once
#include "ControlBase.h"
class DUI_Button :
	public ControlBase
{
public:
	DUI_Button();
	virtual ~DUI_Button();
	VOID OnClick();
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw();
};

