#pragma once
#include "DUI_ControlBase.h"
class DUI_Button : public DUI_ControlBase
{
public:
	DUI_Button();
	virtual ~DUI_Button();
	CLICKPROC SetClickEventHandler(CLICKPROC Proc);//����֮ǰ�ĵ���ʱ�䴦�����
	LONG SetExtra(LONG Etr);
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s = S_Invalid);
	virtual VOID OnClick();
	CLICKPROC m_ClickProc;
	LONG m_ExtraInfo;
	BOOL m_bMouseDown;
};

