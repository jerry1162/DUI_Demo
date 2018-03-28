#pragma once
#include "DUI_ControlBase.h"
class DUI_ScrollBar : public DUI_ControlBase
{
public:
	DUI_ScrollBar();
	virtual ~DUI_ScrollBar();
	VOID SetOrientation(Orientation o);
	BOOL SetPos(REAL Pos);
	REAL GetPos();
	VOID Bind(DUI_ControlBase* Ctrl);
	VOID UpdateFrame();
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1) override;
	REAL m_Ratio;
	REAL m_DownPos;
	Orientation m_Orientation;
	RectF* m_ScrollRect;
	DUI_ControlBase* m_BindObj;
};

