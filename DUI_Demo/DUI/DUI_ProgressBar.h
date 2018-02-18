#pragma once
#include "DUI_ControlBase.h"
class DUI_ProgressBar :
	public DUI_ControlBase
{
public:
	DUI_ProgressBar();
	virtual ~DUI_ProgressBar();
	VOID SetCurPos(REAL Pos);
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bVisiable = TRUE);
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s) override;
	virtual VOID CALLBACK AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime);
	REAL m_CurPos;
	REAL m_AnimCurPos;
	Image* m_pImg;
};

