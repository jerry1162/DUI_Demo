#pragma once
#include "DUI_ControlBase.h"

enum ImgIndex
{
	ScrollBar_V_AdjBtn_Up_1,
	ScrollBar_V_AdjBtn_Up_2,
	ScrollBar_V_AdjBtn_Up_3,
	ScrollBar_V_AdjBtn_Down_1,
	ScrollBar_V_AdjBtn_Down_2,
	ScrollBar_V_AdjBtn_Down_3,
	ScrollBar_V_Body_1,
	ScrollBar_V_Body_2,
	ScrollBar_V_Body_3,
	ScrollBar_H_AdjBtn_Up_1,
	ScrollBar_H_AdjBtn_Up_2,
	ScrollBar_H_AdjBtn_Up_3,
	ScrollBar_H_AdjBtn_Down_1,
	ScrollBar_H_AdjBtn_Down_2,
	ScrollBar_H_AdjBtn_Down_3,
	ScrollBar_H_Body_1,
	ScrollBar_H_Body_2,
	ScrollBar_H_Body_3
};

const LPTSTR ItemName[] = 
{
	_T("ScrollBar_V_AdjBtn_Up_1"),
	_T("ScrollBar_V_AdjBtn_Up_2"),
	_T("ScrollBar_V_AdjBtn_Up_3"),
	_T("ScrollBar_V_AdjBtn_Down_1"),
	_T("ScrollBar_V_AdjBtn_Down_2"),
	_T("ScrollBar_V_AdjBtn_Down_3"),
	_T("ScrollBar_V_Body_1"),
	_T("ScrollBar_V_Body_2"),
	_T("ScrollBar_V_Body_3"),
	_T("ScrollBar_H_AdjBtn_Up_1"),
	_T("ScrollBar_H_AdjBtn_Up_2"),
	_T("ScrollBar_H_AdjBtn_Up_3"),
	_T("ScrollBar_H_AdjBtn_Down_1"),
	_T("ScrollBar_H_AdjBtn_Down_2"),
	_T("ScrollBar_H_AdjBtn_Down_3"),
	_T("ScrollBar_H_Body_1"),
	_T("ScrollBar_H_Body_2"),
	_T("ScrollBar_H_Body_3")
};

class DUI_ScrollBar : public DUI_ControlBase
{
public:
	DUI_ScrollBar();
	virtual ~DUI_ScrollBar();
	VOID SetOrientation(Orientation o);
	REAL MapPos(REAL Pos);//将绑定组件的位置转换为滚动条的位置
	BOOL SetPos(REAL Pos);
	BOOL SetPosAndNortify(REAL Pos);
	REAL GetPos();
	VOID Bind(DUI_ControlBase* Ctrl);
	VOID UpdateFrame();
	VOID SetAutoHideAdjustBtn(BOOL val);
	virtual BOOL IsPtInCtrl(PointF* pt) override;//注意：此处坐标为相对于窗口的坐标
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1) override;
	REAL m_Ratio;
	REAL m_DownPos;
	Orientation m_Orientation;
	RectF* m_ScrollRect;
	DUI_ControlBase* m_BindObj;
	BOOL m_bDownInScroll;
	INT m_MouseInBtn;
	REAL m_AdjBtnSize;
	INT m_LineLen;
	INT m_PageLen;
	Image* pImgs[18];
	Rect* m_rcNinePatch[2];
	BOOL m_bAutoHideAdjBtn;//自动隐藏调节按钮，为真时普通状态隐藏调节按钮（鼠标穿透）
};

