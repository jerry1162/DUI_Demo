#include "stdafx.h"
#include "DUI_ImageBox.h"


DUI_ImageBox::DUI_ImageBox()
{
	m_Image = nullptr;
	m_ScrollBar = nullptr;
}


DUI_ImageBox::~DUI_ImageBox()
{
	if (m_Image != nullptr)
	{
		delete m_Image;
	}
	SafeDelete(m_ScrollBar);
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
	if (pImage->GetWidth() != m_Rect->Width || pImage->GetHeight() != m_Rect->Height)
	{
		if (bFitSize && pImage)
		{
			Size((REAL)pImage->GetWidth(), (REAL)pImage->GetHeight());
		}
		else
		{
			SetOffset(0, 0, (REAL)pImage->GetWidth(), (REAL)pImage->GetHeight());
			m_ScrollBar->SetVisiable(TRUE);
			m_ScrollBar->UpdateFrame();
			//m_ScrollBar->SetRatio(m_Rect->Width / m_OffsetRect->Width);
		}
	}
	else
	{
		if (m_ScrollBar->GetVisiable())
		{
			m_ScrollBar->SetVisiable(FALSE);
		}
	}

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
		if (m_ScrollBar == nullptr)
		{
			m_ScrollBar = new DUI_ScrollBar;
			m_ScrollBar->Create(this, m_Rect->Width - 10, 0, 10, m_Rect->Height);
			m_ScrollBar->Bind(this);
			if (m_OffsetRect->Width == 0 && m_OffsetRect->Height == 0)
			{
				m_ScrollBar->SetVisiable(FALSE);
			}
			else
			{
				m_ScrollBar->SetVisiable(TRUE);
			}
		}
		else
		{
			m_ScrollBar->Move(m_Rect->Width - 10, 0);
		}
		break;
	case WM_MOUSEWHEEL:
		SHORT Delta = -(SHORT)HIWORD(wParam) / 120;//上滑正，下滑负
		SetOffset(0, m_OffsetRect->Y + (REAL)Delta);
		m_ScrollBar->SetPos(m_OffsetRect->Y);
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
