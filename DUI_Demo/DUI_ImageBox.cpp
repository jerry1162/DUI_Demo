#include "stdafx.h"
#include "DUI_ImageBox.h"


DUI_ImageBox::DUI_ImageBox()
{
	m_Image = nullptr;
}


DUI_ImageBox::~DUI_ImageBox()
{
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
}

BOOL DUI_ImageBox::SetImagePath(LPTSTR Path)
{
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	m_Image = new Image(Path);
	if (m_Image != nullptr)
	{
		Update();
	}
	return m_Image != nullptr;
}

BOOL DUI_ImageBox::SetImage(Image* pImage)
{
	if (m_Image != pImage)
	{
		if (m_bInAnimating)
		{
			EndAnimate();
		}

		StartAnimate();
	}
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	m_Image = pImage;
	if (m_Image != nullptr)
	{
		//Update();
	}
	return m_Image != nullptr;
}

LRESULT DUI_ImageBox::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(uMsg, wParam, lParam);

	if (Ret == -1)
	{
		return TRUE;
	}
	if (uMsg == CM_STATECHANGED)
	{
		uMsg = uMsg;
	}

	return Ret;
}

VOID DUI_ImageBox::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		Pen pen(Color::MakeARGB(200, 255, 255, 255));
		m_MemDC->graphics->DrawRectangle(&pen, 0, 0, (INT)m_Rect->Width - 1, (INT)m_Rect->Height - 1);
		if (m_Image != nullptr)
		{
			m_MemDC->graphics->DrawImage(m_Image, 0, 0);
		}
		DUI_ControlBase::Draw();
	}
}
