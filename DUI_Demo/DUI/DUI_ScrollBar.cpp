#include "stdafx.h"
#include "DUI_ScrollBar.h"


DUI_ScrollBar::DUI_ScrollBar()
{
	m_Orientation = VERTICAL;
	m_Ratio = 1;
	m_ScrollRect = new RectF;
	m_BindObj = nullptr;
}


DUI_ScrollBar::~DUI_ScrollBar()
{
	SafeDelete(m_ScrollRect);
}
VOID DUI_ScrollBar::SetOrientation(Orientation o)
{
	m_Orientation = o;
}
BOOL DUI_ScrollBar::SetPos(REAL Pos)
{
	if (m_Orientation == VERTICAL)
	{
		if (Pos + m_ScrollRect->Height <= m_Rect->Height && Pos >= 0)
		{
			m_ScrollRect->Y = Pos;
			Update();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		if (Pos + m_ScrollRect->Width <= m_Rect->Width - 0 && Pos >= 0)
		{
			m_ScrollRect->X = Pos;
			Update();
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
}
REAL DUI_ScrollBar::GetPos()
{
	if (m_Orientation == VERTICAL)
	{
		return m_ScrollRect->Y;
	}
	else
	{
		return m_ScrollRect->X;
	}
}

VOID DUI_ScrollBar::Bind(DUI_ControlBase* Ctrl)
{
	if (Ctrl != nullptr)
	{
		m_BindObj = Ctrl;
		if (m_Orientation == VERTICAL)
		{
			m_ScrollRect->Width = m_Rect->Width - 0;
			m_ScrollRect->Height = m_Rect->Height*m_BindObj->m_Rect->Height / m_BindObj->m_OffsetRect->Height;
			m_Ratio = (m_Rect->Height - m_ScrollRect->Height) / (m_BindObj->m_OffsetRect->Height - m_BindObj->m_Rect->Height);
		}
		else
		{
			m_ScrollRect->Height = m_Rect->Height - 2;
			m_ScrollRect->Height = m_Rect->Height*m_BindObj->m_Rect->Width / m_BindObj->m_OffsetRect->Width;
			m_Ratio = (m_Rect->Width - m_ScrollRect->Width) / (m_BindObj->m_OffsetRect->Width - m_BindObj->m_Rect->Width);
		}
	}
}

VOID DUI_ScrollBar::UpdateFrame()
{
	if (m_BindObj != nullptr)
	{
		if (m_Orientation == VERTICAL)
		{
			m_ScrollRect->Width = m_Rect->Width - 0;
			m_ScrollRect->Height = m_Rect->Height*m_BindObj->m_Rect->Height / m_BindObj->m_OffsetRect->Height;
			m_Ratio = (m_Rect->Height - m_ScrollRect->Height) / (m_BindObj->m_OffsetRect->Height - m_BindObj->m_Rect->Height);
		}
		else
		{
			m_ScrollRect->Height = m_Rect->Height - 0;
			m_ScrollRect->Height = m_Rect->Height*m_BindObj->m_Rect->Width / m_BindObj->m_OffsetRect->Width;
			m_Ratio = (m_Rect->Width - m_ScrollRect->Width) / (m_BindObj->m_OffsetRect->Width - m_BindObj->m_Rect->Width);
		}
	}
}

LRESULT DUI_ScrollBar::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(m_ID, uMsg, wParam, lParam);

	if (Ret == -1)
	{
		return TRUE;
	}

	switch (uMsg)
	{
	case CM_CREATE:
		HasState(TRUE);
		m_ScrollRect->X = 0;
		m_ScrollRect->Y = 0;
		break;
	case WM_MOUSEWHEEL:
	{
		SHORT Delta = -(SHORT)HIWORD(wParam) / 120;//上滑正，下滑负
		
		//SetPos(GetPos() + Delta);
	}
		break;
	case CM_STATECHANGED:
		if (m_bAnimating)
		{
			EndAnimate();
		}
		if (m_CurState != S_Pushed && m_PrevState != S_Pushed)
		{
			StartAnimate();
			Ret = 1;
		}
		break;
	case WM_MOUSEMOVE:
	{
		if (m_bMouseDown)
		{
			PointF ptMouse;
			GetCursorPos(&ptMouse);
			if (m_Orientation == VERTICAL)
			{
				SetPos((REAL)ptMouse.Y - m_DownPos);
				if (m_BindObj != nullptr)
				{
					m_BindObj->SetOffset(0, m_ScrollRect->Y / m_Ratio);
					m_BindObj->Update();
				}
			}
			else
			{
				SetPos((REAL)ptMouse.X - m_DownPos);
				if (m_BindObj != nullptr)
				{
					m_BindObj->SetOffset(m_ScrollRect->X / m_Ratio, 0);
					m_BindObj->Update();
				}
			}
		}
	}
		break;
	case WM_LBUTTONDOWN:
		if (m_Orientation == VERTICAL)
		{
			m_DownPos = m_ptMouseDown->Y - GetPos();
		}
		else
		{
			m_DownPos = m_ptMouseDown->X - GetPos();
		}
		
		break;
	case WM_LBUTTONUP:
		m_DownPos = 0;
		break;
	}

	return Ret;
}

VOID DUI_ScrollBar::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		RectF* Rt = m_LogicRect;
		SolidBrush br(Color::Transparent);
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		switch (s)
		{
		case S_Normal:
			//br.SetColor(Color::MakeARGB(50, 255, 255, 255));
			//m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
			br.SetColor(Color::MakeARGB(100, 255, 255, 255));
			m_MemDC->graphics->FillRectangle(&br, m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 1, m_ScrollRect->Height - 1);
			break;
		case S_HighLight:case S_Focus:
			br.SetColor(Color::MakeARGB(50, 255, 255, 255));
			m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
			br.SetColor(Color::MakeARGB(150, 255, 255, 255));
			m_MemDC->graphics->FillRectangle(&br, m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 1, m_ScrollRect->Height - 1);
			break;
		case S_Pushed:
			br.SetColor(Color::MakeARGB(50, 255, 255, 255));
			m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
			if (m_bMouseDown&&m_ScrollRect->Contains(*m_ptMouseDown))
			{

				br.SetColor(Color::MakeARGB(255, 255, 255, 255));
				m_MemDC->graphics->FillRectangle(&br, m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 1, m_ScrollRect->Height - 1);
			}
			else
			{
				br.SetColor(Color::MakeARGB(150, 255, 255, 255));
				m_MemDC->graphics->FillRectangle(&br, m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 1, m_ScrollRect->Height - 1);
			}
			
			break;
		}


		

		DUI_ControlBase::Draw();
	}
}
