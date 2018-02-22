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
		if (m_bAnimating)
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

LRESULT DUI_ImageBox::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(m_ID, uMsg, wParam, lParam);

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
		RectF* Rt = m_LogicRect;
		SolidBrush br(Color::MakeARGB(150, 255, 255, 255));
		m_MemDC->graphics->FillRectangle(&br, 0, 0, (INT)Rt->Width, (INT)Rt->Height);
		Pen pen(Color::MakeARGB(200, 255, 255, 255));
		m_MemDC->graphics->DrawRectangle(&pen, 0, 0, (INT)Rt->Width - 1, (INT)Rt->Height - 1);
		if (m_Image != nullptr)
		{
			m_MemDC->graphics->DrawImage(m_Image, 0, 0);
		}
		DUI_ControlBase::Draw();
	}
}
