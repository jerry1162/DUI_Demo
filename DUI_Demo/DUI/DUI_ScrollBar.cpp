#include "stdafx.h"
#include "DUI_ScrollBar.h"


DUI_ScrollBar::DUI_ScrollBar()
{
	m_Orientation = VERTICAL;
	m_Ratio = 1;
	m_ScrollRect = new RectF;
	m_BindObj = nullptr;
	m_bDownInScroll = FALSE;
	m_LineLen = 3;
	m_PageLen = 10;
	m_rcNinePatch[VERTICAL] = new Rect(1, 2, 2, 1);
	m_rcNinePatch[HORIZONTAL] = new Rect(1, 0, 0, 1);
	m_MouseInBtn = 0;
	m_bAutoHideAdjBtn = FALSE;
}


DUI_ScrollBar::~DUI_ScrollBar()
{
	SafeDelete(m_ScrollRect);
	SafeDelete(m_rcNinePatch[0]);
	SafeDelete(m_rcNinePatch[1]);
}

VOID DUI_ScrollBar::SetOrientation(Orientation o)
{
	if (m_Orientation != o)
	{
		m_Orientation = o;
		if (m_Orientation == VERTICAL)
		{
			m_ScrollRect->X = 1;
			m_ScrollRect->Y = m_AdjBtnSize;
		}
		else
		{
			m_ScrollRect->X = m_AdjBtnSize;
			m_ScrollRect->Y = 1;
		}
	}
}

REAL DUI_ScrollBar::MapPos(REAL Pos)
{
	return Pos * m_Ratio + m_AdjBtnSize;
}

BOOL DUI_ScrollBar::SetPos(REAL Pos)
{
	if (m_Orientation == VERTICAL)
	{
		if (Pos + m_ScrollRect->Height + m_AdjBtnSize < m_Rect->Height && Pos >= m_AdjBtnSize)
		{
			m_ScrollRect->Y = Pos;
			Update();
			return TRUE;
		}
		else if (Pos + m_ScrollRect->Height + m_AdjBtnSize >= m_Rect->Height)
		{
			m_ScrollRect->Y = m_Rect->Height - m_ScrollRect->Height - m_AdjBtnSize;
			Update();
			return FALSE;
		}
		else
		{
			m_ScrollRect->Y = m_AdjBtnSize;
			Update();
			return FALSE;
		}
	}
	else
	{
		if (Pos + m_ScrollRect->Width + m_AdjBtnSize < m_Rect->Width && Pos >= m_AdjBtnSize)
		{
			m_ScrollRect->X = Pos;
			Update();
			return TRUE;
		}
		else if (Pos + m_ScrollRect->Width + m_AdjBtnSize >= m_Rect->Width)
		{
			m_ScrollRect->X = m_Rect->Width - m_ScrollRect->Width - m_AdjBtnSize;
			Update();
			return FALSE;
		}
		else
		{
			m_ScrollRect->X = m_AdjBtnSize;
			Update();
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
			m_ScrollRect->X = 1;
			m_ScrollRect->Y = m_AdjBtnSize;
		}
		else
		{
			m_ScrollRect->X = m_AdjBtnSize;
			m_ScrollRect->Y = 1;
		}
		UpdateFrame();
	}
}

VOID DUI_ScrollBar::UpdateFrame()
{
	if (m_BindObj != nullptr)
	{
		if (m_Orientation == VERTICAL)
		{
			m_ScrollRect->Width = m_Rect->Width - 2;
			m_ScrollRect->Height = (m_Rect->Height - 2 * m_AdjBtnSize)*m_BindObj->m_Rect->Height / m_BindObj->m_OffsetRect->Height;
			m_Ratio = (m_Rect->Height - 2 * m_AdjBtnSize) / m_BindObj->m_OffsetRect->Height;
		}
		else
		{
			m_ScrollRect->Height = m_Rect->Height - 2;
			m_ScrollRect->Width = (m_Rect->Width - 2 * m_AdjBtnSize)*m_BindObj->m_Rect->Width / m_BindObj->m_OffsetRect->Width;
			m_Ratio = (m_Rect->Width - 2 * m_AdjBtnSize) / m_BindObj->m_OffsetRect->Width;
		}
		Update();
	}
}

VOID DUI_ScrollBar::SetAutoHideAdjustBtn(BOOL val)
{
	if (m_bAutoHideAdjBtn != val)
	{
		m_bAutoHideAdjBtn = val;
		Update();
	}
}

BOOL DUI_ScrollBar::IsPtInCtrl(PointF * pt)
{
	if (DUI_ControlBase::IsPtInCtrl(pt))
	{
		if (!m_bAutoHideAdjBtn)
		{
			return TRUE;
		}
		PointF* ptClient = new PointF(*pt);
		WndToClient(ptClient);
		BOOL Ret = TRUE;
		if (m_CurState == S_Normal)
		{
			if (m_Orientation == VERTICAL)
			{
				if (ptClient->Y < m_AdjBtnSize || ptClient->Y > m_Rect->Height - m_AdjBtnSize)
				{
					Ret = FALSE;
				}
			}
			else
			{
				if (ptClient->X < m_AdjBtnSize || ptClient->X > m_Rect->Width - m_AdjBtnSize)
				{
					Ret = FALSE;
				}
			}
		}
		delete ptClient;
		return Ret;
	}
	return FALSE;
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
		m_AdjBtnSize = (REAL)m_ParentWnd->GetRDBMgr()->GetIntValByName(_T("ScrollBar_AdjBtn_Size"));
		ResItem* lpItem;
		for (int i = ScrollBar_V_AdjBtn_Up_1; i <= ScrollBar_H_Body_3; i++)
		{
			lpItem = m_ParentWnd->GetRDBMgr()->GetItemByName(ItemName[i]);
			pImgs[i] = ImageFromBin(lpItem->lpData, lpItem->uSize);
		}
		HasState(TRUE);
		UpdateFrame();
		break;
	case WM_MOUSEWHEEL:
	{
		SHORT Delta = -(SHORT)HIWORD(wParam) / 120;//上滑正，下滑负
		//SetPos(GetPos() + Delta);
	}
		break;
	case CM_STATECHANGED:
		if (m_CurState != S_Pushed && m_PrevState != S_Pushed)
		{
			StartAnimate();
			return 1;
		}
		break;
	case WM_MOUSEMOVE:
	{
		PointF ptMouse;
		GetCursorPos(&ptMouse);
		
		if (m_bMouseDown)
		{
			if (m_Orientation == VERTICAL)
			{
				if (m_bDownInScroll)
				{
					SetPosAndNortify((REAL)ptMouse.Y - m_DownPos);
				}
				else
				{
					SetPosAndNortify((REAL)ptMouse.Y);
				}
			}
			else
			{
				if (m_bDownInScroll)
				{
					SetPosAndNortify((REAL)ptMouse.X - m_DownPos);
				}
				else
				{
					SetPosAndNortify((REAL)ptMouse.X);
				}
			}
		}
		else
		{
			if (m_Orientation == VERTICAL)
			{
				if (ptMouse.Y <= m_AdjBtnSize)
				{
					if (m_MouseInBtn != 1)
					{
						m_MouseInBtn = 1;
						StartAnimate();
						Ret = 1;
					}
				}
				else if (ptMouse.Y >= m_ScrollRect->Y&&ptMouse.Y <= m_ScrollRect->GetBottom())
				{
					if (m_MouseInBtn != 2)
					{
						m_MouseInBtn = 2;
						StartAnimate();
					}
				}
				else if (ptMouse.Y >= m_Rect->Height - m_AdjBtnSize)
				{
					if (m_MouseInBtn != 3)
					{
						m_MouseInBtn = 3;
						StartAnimate();
					}
				}
				else
				{
					if (m_MouseInBtn != 0)
					{
						m_MouseInBtn = 0;
						StartAnimate();
					}
				}
			}
			else
			{
				if (ptMouse.X <= m_AdjBtnSize)
				{
					if (m_MouseInBtn != 1)
					{
						m_MouseInBtn = 1;
						StartAnimate();
					}
				}
				else if (ptMouse.X >= m_ScrollRect->X&&ptMouse.X <= m_ScrollRect->GetRight())
				{
					if (m_MouseInBtn != 2)
					{
						m_MouseInBtn = 2;
						StartAnimate();
					}
				}
				else if (ptMouse.X >= m_Rect->Width - m_AdjBtnSize)
				{
					if (m_MouseInBtn != 3)
					{
						m_MouseInBtn = 3;
						StartAnimate();
					}
				}
				else
				{
					if (m_MouseInBtn != 0)
					{
						m_MouseInBtn = 0;
						StartAnimate();
					}
				}
			}
		}
	}
		break;
	case WM_LBUTTONDOWN:
		m_bDownInScroll = m_ScrollRect->Contains(*m_ptMouseDown);
		if (m_Orientation == VERTICAL)
		{
			m_DownPos = m_ptMouseDown->Y - GetPos();
			if (!m_bDownInScroll)
			{
				if (m_ptMouseDown->Y < m_AdjBtnSize)
				{
					SetPosAndNortify(GetPos() - m_LineLen);
				}
				else if (m_ptMouseDown->Y >= m_AdjBtnSize && m_ptMouseDown->Y < m_ScrollRect->Y)
				{
					SetPosAndNortify(GetPos() - m_PageLen);
				}
				else if (m_ptMouseDown->Y > m_ScrollRect->GetBottom() && m_ptMouseDown->Y < m_Rect->Height - m_AdjBtnSize)
				{
					SetPosAndNortify(GetPos() + m_PageLen);
				}
				else
				{
					SetPosAndNortify(GetPos() + m_LineLen);
				}
			}
		}
		else
		{
			m_DownPos = m_ptMouseDown->X - GetPos();
			if (!m_bDownInScroll)
			{
				if (m_ptMouseDown->X < m_AdjBtnSize)
				{
					SetPosAndNortify(GetPos() - m_LineLen);
				}
				else if (m_ptMouseDown->X >= m_AdjBtnSize && m_ptMouseDown->X < m_ScrollRect->X)
				{
					SetPosAndNortify(GetPos() - m_PageLen);
				}
				else if (m_ptMouseDown->X> m_ScrollRect->GetRight() && m_ptMouseDown->X < m_Rect->Width - m_AdjBtnSize)
				{
					SetPosAndNortify(GetPos() + m_PageLen);
				}
				else
				{
					SetPosAndNortify(GetPos() + m_LineLen);
				}
			}
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
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		switch (s)
		{
		case S_Normal:
			if (m_Orientation == VERTICAL)
			{
				if (!m_bAutoHideAdjBtn)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
			}
			else
			{
				if (!m_bAutoHideAdjBtn)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
			}
			break;
		case S_HighLight:case S_Focus:
			if (m_Orientation == VERTICAL)
			{
				if (m_MouseInBtn == 1)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_2], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else if (m_MouseInBtn == 2)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_2], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else if (m_MouseInBtn == 3)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_2], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
			}
			else
			{
				if (m_MouseInBtn == 1)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_2], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else if (m_MouseInBtn == 2)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_2], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else if (m_MouseInBtn == 3)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_2], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
			}
			break;
		case S_Pushed:
			if (m_Orientation == VERTICAL)
			{
				if (m_MouseInBtn == 1)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_3], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else if (m_MouseInBtn == 2)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_3], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else if (m_MouseInBtn == 3)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_3], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
				else
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Up_1], 1, 0);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_V_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[VERTICAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_V_AdjBtn_Down_1], 1, (INT)(Rt->Height - m_AdjBtnSize));
				}
			}
			else
			{
				if (m_MouseInBtn == 1)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_3], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else if (m_MouseInBtn == 2)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_3], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else if (m_MouseInBtn == 3)
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_3], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
				else
				{
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Up_1], 0, 1);
					DrawImgNinePatch(m_MemDC->graphics, pImgs[ScrollBar_H_Body_1], m_ScrollRect->X, m_ScrollRect->Y, m_ScrollRect->Width - 0, m_ScrollRect->Height - 0, m_rcNinePatch[HORIZONTAL], m_Alpha);
					m_MemDC->graphics->DrawImage(pImgs[ScrollBar_H_AdjBtn_Down_1], (INT)(Rt->Width - m_AdjBtnSize), (INT)(Rt->Height - m_AdjBtnSize) - 1);
				}
			}
			break;
		}
		DUI_ControlBase::Draw();
	}
}

BOOL DUI_ScrollBar::SetPosAndNortify(REAL Pos)
{
	BOOL Ret = SetPos(Pos);
	if (m_BindObj != nullptr)
	{
		if (m_Orientation == VERTICAL)
		{
			CtrlMsgProc(m_BindObj, CM_NORTIFY, MAKEWPARAM(m_ID, NID_SCROLL_Y), (LPARAM)((m_ScrollRect->Y - m_AdjBtnSize) / m_Ratio));
		}
		else
		{
			CtrlMsgProc(m_BindObj, CM_NORTIFY, MAKEWPARAM(m_ID, NID_SCROLL_X), (LPARAM)((m_ScrollRect->X - m_AdjBtnSize) / m_Ratio));
		}
	}

	return Ret;
}
