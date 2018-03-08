#pragma once
#include "DUI_ControlBase.h"
class DUI_ImageBox : public DUI_ControlBase
{
public:
	DUI_ImageBox();
	virtual ~DUI_ImageBox();
	BOOL SetImagePath(LPTSTR Path, BOOL bFitSize = TRUE);
	BOOL SetImage(Image* pImage, BOOL bFitSize = TRUE);
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual VOID Draw(DUI_Status s = (DUI_Status)-1) override;
	Image* m_Image;
};

