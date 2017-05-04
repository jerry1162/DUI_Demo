#pragma once
#include "DUI_Button.h"
class DUI_Window;
//class DUI_ControlBase;
enum DUI_Status;

class DUI_ImageButton : public DUI_Button
{
public:
	DUI_ImageButton();
	virtual ~DUI_ImageButton();
	BOOL SetImages(Image* Normal, Image* HighLight, Image* Pushed, BOOL bUpdate = TRUE);
	BOOL SetImages(Image* pImg, INT Start, BOOL bUpdate = TRUE);
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		Image* Normal, Image* HighLight, Image* Pushed, LPCWSTR Text = L"", BOOL bVisiable = TRUE);
	virtual BOOL Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height,
		Image* pImg, INT Start, LPCWSTR Text = L"", BOOL bVisiable = TRUE);
	virtual BOOL ReleaseImages();
	BOOL m_bAutoReleaseImg;//是否自动释放给定的图片资源，默认为真
protected:
	//virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual VOID Draw(DUI_Status s);
	Image* m_StatusImage[6];
	INT m_Start;
};

