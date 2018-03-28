#pragma once
#include "DUI_ControlBase.h"
class DUI_SizeBox : public DUI_ControlBase
{
public:
	DUI_SizeBox();
	virtual ~DUI_SizeBox();
	virtual RectF* GetClientRect() override;
	virtual BOOL IsPtInCtrl(PointF* pt) override;//注意：此处坐标为相对于窗口的坐标
	BOOL Bind(DUI_ControlBase* pCtrl = nullptr);
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;
	INT GetBoxIndex(PointF * pt);  //客户区坐标
	const REAL m_BoxSize;
	RectF* m_Boxs[8];
	DUI_ControlBase* m_BindCtrl;
	RectF* m_PrevRect;
	INT m_SelectedBoxIndex;
};

