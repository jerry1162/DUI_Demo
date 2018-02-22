#include "stdafx.h"
#include "DUI_ProgressBar.h"


DUI_ProgressBar::DUI_ProgressBar()
{
	m_CurPos = 0;
	m_AnimCurPos = 0;
	m_pImg = nullptr;
}


DUI_ProgressBar::~DUI_ProgressBar()
{
	delete m_pImg;
}

VOID DUI_ProgressBar::SetCurPos(REAL Pos)
{
	m_CurPos = Pos;
	Update();
}

BOOL DUI_ProgressBar::Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable)
{
	auto Ret = DUI_ControlBase::Create(Parent, Left, Top, Width, Height, Text, bVisiable, FALSE);
	if (Ret)
	{
		ResItem* lpItem = m_ParentWnd->GetRDBMgr()->GetItemByName(_T("ProgressAnim_Pic"));
		if (!lpItem)
		{
			MessageBox(m_ParentWnd->GetHWND(), _T("×ÊÔ´¼ÓÔØÊ§°Ü"), _T("´íÎó:"), MB_ICONINFORMATION);
			return FALSE;
		}
		m_pImg = ImageFromBin(lpItem->lpData, lpItem->uSize);
		m_AnimCurPos = -m_LogicRect->Width;
		StartAnimate();//nullptr, 20
	}
	return Ret;
}

LRESULT DUI_ProgressBar::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(m_ID, uMsg, wParam, lParam);
	if (Ret == -1)
	{
		return TRUE;
	}
	return Ret;
}

VOID DUI_ProgressBar::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		RectF* Rt = m_LogicRect;
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		SolidBrush br(Color(150, 255, 255, 255));
		m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, Rt->Width - 1, Rt->Height - 1);
		Pen pen(Color(80, 0, 0, 0));
		m_MemDC->graphics->DrawRectangle(&pen, 0.0, 0.0, Rt->Width - 1, Rt->Height - 1);
		br.SetColor(Color(0, 136, 255));
		m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_CurPos, Rt->Height);
		m_MemDC->graphics->DrawImage(m_pImg, m_AnimCurPos, Rt->Height - 24, 90.0, 24.0);
		pen.SetColor(Color(80, 255, 255, 255));
		m_MemDC->graphics->DrawRectangle(&pen, 1.0, 1.0, m_CurPos - 3, Rt->Height - 3);

		pen.SetColor(Color(150, 128, 128, 128));
		m_MemDC->graphics->DrawRectangle(&pen, 0.0, 0.0, m_CurPos - 1, Rt->Height - 1);

		*m_Text->color = Color::White;
		DrawShadowText(m_MemDC->graphics, m_Text);
		DUI_ControlBase::Draw();
	}
}

VOID DUI_ProgressBar::AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	m_AnimCurPos += 5;
	if (m_AnimCurPos > m_LogicRect->Width)
	{
		m_AnimCurPos = -m_LogicRect->Width;
	}
	Draw(S_Invalid);
	Update();
}
