#pragma once
#include "DUI_ControlBase.h"
class DUI_ProgressBar :
	public DUI_ControlBase
{
public:
	DUI_ProgressBar();
	virtual ~DUI_ProgressBar();
	VOID SetCurPos(REAL Pos);
	virtual BOOL Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE);
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;
	virtual VOID CALLBACK AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime) override;
	REAL m_CurPos;
	REAL m_AnimCurPos;
	Image* m_pImg;
};

