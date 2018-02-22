#pragma once
#include "DUI_ControlBase.h"
#include "DUI_Button.h"
#include "DUI_RadioGroup.h"
class DUI_Radio : public DUI_Button
{
	friend class DUI_RadioGroup;
public:
	DUI_Radio();
	virtual ~DUI_Radio() override;
	virtual BOOL Create(DUI_Window* Window, DUI_RadioGroup* Group, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text = L"", BOOL bVisiable = TRUE);//注：会自动调整文本矩形
	virtual BOOL Destroy() override;
	BOOL ChangeGroup(DUI_RadioGroup* Group);
private:
	Image* pImg;
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s) override;
	VOID Select(BOOL Select);
	BOOL m_bChecked;
	DUI_RadioGroup* m_Group;
};

