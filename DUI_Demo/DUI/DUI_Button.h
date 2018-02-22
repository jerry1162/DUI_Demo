#pragma once
#include "DUI_ControlBase.h"
class DUI_Button : public DUI_ControlBase
{
public:
	DUI_Button();
	virtual ~DUI_Button();
	LONG SetExtraInfo(LONG Etr);
	LONG GetExtraInfo();
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s = S_Invalid) override;
	MSGPROC m_ClickProc;
	LONG m_ExtraInfo;
};

