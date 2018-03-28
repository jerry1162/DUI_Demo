#include "stdafx.h"
#include "DUI_SizeBox.h"


DUI_SizeBox::DUI_SizeBox() :m_BoxSize(7)
{
	for (INT i = 0; i < 8; i++)
	{
		m_Boxs[i] = nullptr;
	}
	m_BindCtrl = nullptr;
	m_PrevRect = nullptr;
	m_SelectedBoxIndex = -1;
}

DUI_SizeBox::~DUI_SizeBox()
{
	if (Existence())
	{
		for (INT i = 0; i < 8; i++)
		{
			SafeDelete(m_Boxs[i])
		}
		Destroy();
	}
}

RectF * DUI_SizeBox::GetClientRect()
{
	RectF* PCRect = m_Parent->GetClientRect();
	RectF* cRect = new RectF(8, 8, m_Rect->Width - 16, m_Rect->Height - 16);
	if (PCRect != nullptr)
	{
		BOOL b = cRect->Intersect(*PCRect);
		delete PCRect;
		if (!b)
		{
			return nullptr;
		}
	}
	return cRect;
}

BOOL DUI_SizeBox::IsPtInCtrl(PointF * pt)
{
	if (DUI_ControlBase::IsPtInCtrl(pt))
	{
		PointF* ptClient = new PointF(*pt);
		WndToClient(ptClient);
		INT Index = GetBoxIndex(ptClient);
		delete ptClient;
		return Index != -1;
	}
	return FALSE;
}

BOOL DUI_SizeBox::Bind(DUI_ControlBase * pCtrl)
{
	if (pCtrl == nullptr || pCtrl == this)
	{
		if (m_bVisialbe != FALSE)
		{
			m_bVisialbe = FALSE;
			Update();
		}
		return FALSE;
	}
	m_bVisialbe = TRUE;
	m_BindCtrl = pCtrl;
	PointF* pt = new PointF(0, 0);
	m_BindCtrl->ClientToWnd(pt);
	Move(pt->X - m_BoxSize - 1, pt->Y - m_BoxSize - 1);
	delete pt;
	Size(m_BindCtrl->GetWidth() + 2 * m_BoxSize + 2, m_BindCtrl->GetHeight() + 2 * m_BoxSize + 2);
	return TRUE;
}

LRESULT DUI_SizeBox::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == CM_MOUSEGETIN)
	{
		PointF* pt = new PointF;
		GetCursorPos(pt);
		INT Index = GetBoxIndex(pt);
		if (Index != -1)
		{
			switch (Index)
			{
			case SD_LEFT_TOP:
				m_CursorName = IDC_SIZENWSE;
				break;
			case SD_MID_TOP:
				m_CursorName = IDC_SIZENS;
				break;
			case SD_RIGHT_TOP:
				m_CursorName = IDC_SIZENESW;
				break;
			case SD_LEFT_MID:
				m_CursorName = IDC_SIZEWE;
				break;
			case SD_RIGHT_MID:
				m_CursorName = IDC_SIZEWE;
				break;
			case SD_LEFT_BOTTOM:
				m_CursorName = IDC_SIZENESW;
				break;
			case SD_MID_BOTTOM:
				m_CursorName = IDC_SIZENS;
				break;
			case SD_RIGHT_BOTTOM:
				m_CursorName = IDC_SIZENWSE;
				break;
			}
		}
		delete pt;
	}
	else if (uMsg == CM_MOUSELEAVE)
	{
		m_CursorName = nullptr;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		SafeDelete(m_PrevRect);
		m_PrevRect = m_LogicRect->Clone();
		PointF* pt = new PointF((REAL)GET_X_LPARAM(lParam), (REAL)GET_Y_LPARAM(lParam));
		m_SelectedBoxIndex = GetBoxIndex(pt);
		delete pt;
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		SafeDelete(m_PrevRect);
		m_SelectedBoxIndex = -1;
	}
	else if (uMsg == WM_MOUSEMOVE && m_bMouseDown)
	{
		PointF* ptClient = new PointF;
		PointF* ptParent = new PointF;
		if (m_SelectedBoxIndex != -1)
		{
			switch (m_SelectedBoxIndex)
			{
			case SD_LEFT_TOP:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewWidth, NewHeight;
				NewHeight = m_PrevRect->Height + m_PrevRect->Y - (ptParent->Y - m_ptMouseDown->Y);
				NewWidth = m_PrevRect->Width + m_PrevRect->X - (ptParent->X - m_ptMouseDown->X);
				if (NewWidth >= m_MinMaxInfo.MinSize->Width && NewHeight >= m_MinMaxInfo.MinSize->Height)
				{
					Move(ptParent->X - m_ptMouseDown->X, ptParent->Y - m_ptMouseDown->Y);
					Size(m_PrevRect->Width + m_PrevRect->X - m_LogicRect->X, m_PrevRect->Height + m_PrevRect->Y - m_LogicRect->Y);
				}
			}
				break;
			case SD_MID_TOP:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewHeight = m_PrevRect->Height + m_PrevRect->Y - (ptParent->Y - m_ptMouseDown->Y);
				if (NewHeight >= m_MinMaxInfo.MinSize->Height)
				{
					Move(m_PrevRect->X, ptParent->Y - m_ptMouseDown->Y);
					Size(-1, NewHeight);
				}
				
			}
				break;
			case SD_RIGHT_TOP:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewWidth, NewHeight;

				ptClient->X = (REAL)GET_X_LPARAM(lParam);
				ptClient->Y = (REAL)GET_Y_LPARAM(lParam);

				NewHeight = m_PrevRect->Height + m_PrevRect->Y - (ptParent->Y - m_ptMouseDown->Y);
				NewWidth = m_PrevRect->Width + ptClient->X - m_ptMouseDown->X;
				if (NewWidth >= m_MinMaxInfo.MinSize->Width && NewHeight >= m_MinMaxInfo.MinSize->Height)
				{
					Move(m_PrevRect->X, ptParent->Y - m_ptMouseDown->Y);
					Size(NewWidth, NewHeight);
				}
			}
				break;
			case SD_LEFT_MID:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewWidth = m_PrevRect->Width + m_PrevRect->X - (ptParent->X - m_ptMouseDown->X);
				if (NewWidth>=m_MinMaxInfo.MinSize->Width)
				{
					Move(ptParent->X - m_ptMouseDown->X, m_PrevRect->Y);
					Size(NewWidth, -1);
				}
			}
				break;
			case SD_RIGHT_MID:
				ptClient->X = (REAL)GET_X_LPARAM(lParam);
				ptClient->Y = (REAL)GET_Y_LPARAM(lParam);
				Size(m_PrevRect->Width + ptClient->X - m_ptMouseDown->X, -1);
				break;
			case SD_LEFT_BOTTOM:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewWidth, NewHeight;

				NewWidth = m_PrevRect->Width + m_PrevRect->X - (ptParent->X - m_ptMouseDown->X);
				NewHeight = m_PrevRect->Height + (ptParent->Y - m_ptMouseDown->Y) - m_PrevRect->Y;
				if (NewWidth >= m_MinMaxInfo.MinSize->Width && NewHeight >= m_MinMaxInfo.MinSize->Height)
				{
					Move(ptParent->X - m_ptMouseDown->X, m_PrevRect->Y);
					Size(NewWidth, NewHeight);
				}
			}
				break;
			case SD_MID_BOTTOM:
				ptClient->X = (REAL)GET_X_LPARAM(lParam);
				ptClient->Y = (REAL)GET_Y_LPARAM(lParam);
				Size(-1, m_PrevRect->Height + ptClient->Y - m_ptMouseDown->Y);
				break;
			case SD_RIGHT_BOTTOM:
			{
				m_Parent->GetCursorPos(ptParent);
				REAL NewWidth, NewHeight;

				NewWidth = m_PrevRect->Width + (ptParent->X - m_ptMouseDown->X) - m_PrevRect->X;
				NewHeight = m_PrevRect->Height + (ptParent->Y - m_ptMouseDown->Y) - m_PrevRect->Y;
				if (NewWidth >= m_MinMaxInfo.MinSize->Width && NewHeight >= m_MinMaxInfo.MinSize->Height)
				{
					Size(NewWidth, NewHeight);
				}
			}
				break;
			}
		}
		delete ptClient;
		delete ptParent;
		return TRUE;
	}
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(m_ID, uMsg, wParam, lParam);
	if (Ret == -1)
	{
		return TRUE;
	}


	switch (uMsg)
	{
	case CM_CREATE:
	{
		for (INT i = 0; i < 8; i++)
		{
			m_Boxs[i] = new RectF(0, 0, 7, 7);
		}
		Bind(nullptr);
		MINMAXSIZE Info;
		Info.MinSize = new SizeF(m_BoxSize * 3, m_BoxSize * 3);
		SetMinMaxInfo(Info);
	}
		break;
	case CM_SIZE:
		m_Boxs[SD_MID_TOP]->X = (m_LogicRect->Width - m_BoxSize) / 2;

		m_Boxs[SD_RIGHT_TOP]->X = m_LogicRect->Width - m_BoxSize - 1;

		m_Boxs[SD_LEFT_MID]->Y = (m_LogicRect->Height - m_BoxSize) / 2;

		m_Boxs[SD_RIGHT_MID]->X = m_LogicRect->Width - m_BoxSize - 1;
		m_Boxs[SD_RIGHT_MID]->Y = (m_LogicRect->Height - m_BoxSize) / 2;

		m_Boxs[SD_LEFT_BOTTOM]->Y = m_LogicRect->Height - m_BoxSize - 1;

		m_Boxs[SD_MID_BOTTOM]->X = (m_LogicRect->Width - m_BoxSize) / 2;
		m_Boxs[SD_MID_BOTTOM]->Y = m_LogicRect->Height - m_BoxSize - 1;

		m_Boxs[SD_RIGHT_BOTTOM]->X = m_LogicRect->Width - m_BoxSize - 1;
		m_Boxs[SD_RIGHT_BOTTOM]->Y = m_LogicRect->Height - m_BoxSize - 1;
		break;
	}

	return Ret;
}

VOID DUI_SizeBox::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		RectF* Rt = m_LogicRect;


		SolidBrush br(Color::MakeARGB(255, 0, 0, 128));
		Pen pen(Color::MakeARGB(255, 255, 255, 255));
		for (INT i = 0; i < 8; i++)
		{
			m_MemDC->graphics->FillRectangle(&br, *m_Boxs[i]);
			m_MemDC->graphics->DrawRectangle(&pen, *m_Boxs[i]);
		}


		DUI_ControlBase::Draw();
	}
}

INT DUI_SizeBox::GetBoxIndex(PointF * pt)
{
	for (INT i = 0; i < 8; i++)
	{
		if (m_Boxs[i]->Contains((REAL)pt->X, (REAL)pt->Y))
		{
			return i;
		}
	}
	return -1;
}
