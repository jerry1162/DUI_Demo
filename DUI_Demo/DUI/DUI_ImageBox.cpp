#include "stdafx.h"
#include "DUI_ImageBox.h"


DUI_ImageBox::DUI_ImageBox()
{
	m_Image = nullptr;
	m_ScrollBar[0] = nullptr;
	m_ScrollBar[1] = nullptr;
}


DUI_ImageBox::~DUI_ImageBox()
{
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	SafeDelete(m_ScrollBar[0]);
	SafeDelete(m_ScrollBar[1]);
	Destroy();
}

BOOL DUI_ImageBox::SetImagePath(LPTSTR Path, BOOL bFitSize)
{
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	m_Image = new Image(Path);

	/*if (m_Image != nullptr)
	{
		if (bFitSize)
		{
			Size((REAL)m_Image->GetWidth(), (REAL)m_Image->GetHeight());
		}
		//Update();
	}
	return m_Image != nullptr;*/
	return SetImage(m_Image);
}

BOOL DUI_ImageBox::SetImage(Image* pImage, BOOL bFitSize)
{
	BOOL WidthOffset = pImage->GetWidth() != m_Rect->Width;
	BOOL HeightOffset = pImage->GetHeight() != m_Rect->Height;
	if (WidthOffset || HeightOffset)
	{
		if (bFitSize && pImage)
		{
			Size((REAL)pImage->GetWidth(), (REAL)pImage->GetHeight());
		}
		else if (WidthOffset && !HeightOffset)
		{
			SetOffset(0, 0, (REAL)pImage->GetWidth());
			m_ScrollBar[1]->SetVisiable(TRUE);
			m_ScrollBar[1]->UpdateFrame();
			m_ScrollBar[0]->SetVisiable(FALSE);
		}
		else if (HeightOffset && !WidthOffset)
		{
			SetOffset(0, 0, -1, (REAL)pImage->GetHeight());
			m_ScrollBar[0]->SetVisiable(TRUE);
			m_ScrollBar[0]->UpdateFrame();
			m_ScrollBar[1]->SetVisiable(FALSE);
		}
		else if (WidthOffset && HeightOffset)
		{
			SetOffset(0, 0, (REAL)pImage->GetWidth(), (REAL)pImage->GetHeight());
			m_ScrollBar[0]->SetVisiable(TRUE);
			m_ScrollBar[0]->UpdateFrame();
			m_ScrollBar[1]->SetVisiable(TRUE);
			m_ScrollBar[1]->UpdateFrame();
		}
	}
	else
	{
		if (m_ScrollBar[0]->GetVisiable())
		{
			m_ScrollBar[0]->SetVisiable(FALSE);
		}
		if (m_ScrollBar[1]->GetVisiable())
		{
			m_ScrollBar[1]->SetVisiable(FALSE);
		}
	}

	if (m_Image != pImage)
	{
		StartAnimate();
	}
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	m_Image = pImage;
// 	if (m_Image != nullptr)
// 	{
// 		Update();
// 	}
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

	switch (uMsg)
	{
	case CM_SIZE:
		if (m_ScrollBar[0] == nullptr)
		{
			m_ScrollBar[0] = new DUI_ScrollBar;
			m_ScrollBar[0]->Create(this, m_Rect->Width - 15, 2, 12, m_Rect->Height - 3);
			m_ScrollBar[0]->Bind(this);
			m_ScrollBar[0]->SetAutoHideAdjustBtn(TRUE);
			if (m_OffsetRect->Width == 0)
			{
				m_ScrollBar[0]->SetVisiable(FALSE);
			}
			else
			{
				m_ScrollBar[0]->SetVisiable(TRUE);
			}
		}
		else
		{
			m_ScrollBar[0]->Move(m_Rect->Width - 15, 2);
		}

		if (m_ScrollBar[1] == nullptr)
		{
			m_ScrollBar[1] = new DUI_ScrollBar;
			m_ScrollBar[1]->Create(this, 0, m_Rect->Height - 13, m_Rect->Width - 3, 12);
			m_ScrollBar[1]->SetOrientation(HORIZONTAL);
			m_ScrollBar[1]->Bind(this);
			m_ScrollBar[1]->SetAutoHideAdjustBtn(TRUE);
			if (m_OffsetRect->Height == 0)
			{
				m_ScrollBar[1]->SetVisiable(FALSE);
			}
			else
			{
				m_ScrollBar[1]->SetVisiable(TRUE);
			}
		}
		else
		{
			m_ScrollBar[1]->Move(2, m_Rect->Height - 15);
		}
		break;
	case WM_MOUSEWHEEL:
		SHORT Delta = -(SHORT)HIWORD(wParam) / 120;//上滑正，下滑负
		SetOffset(0, m_OffsetRect->Y + (REAL)Delta);
		m_ScrollBar[0]->SetPos(m_ScrollBar[0]->MapPos(m_OffsetRect->Y));
		Update();
		break;
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
		Pen pen(Color::MakeARGB(192, 128, 128, 128));

		if (m_bOffset)
		{
			m_MemDC->graphics->FillRectangle(&br, (INT)m_OffsetRect->X, (INT)m_OffsetRect->Y, (INT)Rt->Width, (INT)Rt->Height);
		} 
		else
		{
			m_MemDC->graphics->FillRectangle(&br, 0, 0, (INT)Rt->Width, (INT)Rt->Height);
		}

		if (m_Image != nullptr)
		{
			m_MemDC->graphics->DrawImage(m_Image, 0, 0);
		}
		if (m_bOffset)
		{
			m_MemDC->graphics->DrawRectangle(&pen, (INT)m_OffsetRect->X, (INT)m_OffsetRect->Y, (INT)Rt->Width - 1, (INT)Rt->Height - 1);
		}
		else
		{
			m_MemDC->graphics->DrawRectangle(&pen, 0, 0, (INT)Rt->Width - 1, (INT)Rt->Height - 1);
		}
		DUI_ControlBase::Draw();
	}
}
