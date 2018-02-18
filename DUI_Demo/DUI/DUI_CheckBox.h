#pragma once
#include "DUI_Button.h"
#define CHECK_SIZE 15
class DUI_CheckBox :
	public DUI_Button
{
public:
	DUI_CheckBox();
	virtual ~DUI_CheckBox();
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		LPCWSTR Text = L"", BOOL bChecked = FALSE, BOOL bVisiable = TRUE);//ע�����Զ������ı�����
	BOOL GetCheckStatus();
	VOID SetCheckStatus(BOOL bCheckd);
	virtual BOOL Destroy();
private:
	Image* pImg;
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s);
	virtual VOID OnClick();
	BOOL m_bChecked;
};

