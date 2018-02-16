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

BOOL DUI_ProgressBar::Create(DUI_Window * Window, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable)
{
	auto Ret = DUI_ControlBase::Create(Window, Left, Top, Width, Height, Text, bVisiable, FALSE);
	if (Ret)
	{
		ResItem* lpItem = m_Parent->GetRDBMgr()->GetItemByName(_T("ProgressAnim_Pic"));
		if (!lpItem)
		{
			MessageBox(m_Parent->m_hWnd, _T("×ÊÔ´¼ÓÔØÊ§°Ü"), _T("´íÎó:"), MB_ICONINFORMATION);
			return FALSE;
		}
		m_pImg = ImageFromBin(lpItem->lpData, lpItem->uSize);
		m_AnimCurPos = -m_Rect->Width;
		StartAnimate();//nullptr, 20
	}
	return Ret;
}

LRESULT DUI_ProgressBar::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(uMsg, wParam, lParam);
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
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		SolidBrush br(Color(150, 255, 255, 255));
		m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
		Pen pen(Color(80, 0, 0, 0));
		m_MemDC->graphics->DrawRectangle(&pen, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
		br.SetColor(Color(0, 136, 255));
		m_MemDC->graphics->FillRectangle(&br, 0.0, 0.0, m_CurPos, m_Rect->Height);
		m_MemDC->graphics->DrawImage(m_pImg, m_AnimCurPos, m_Rect->Height - 24, 90.0, 24.0);
		pen.SetColor(Color(80, 255, 255, 255));
		m_MemDC->graphics->DrawRectangle(&pen, 1.0, 1.0, m_CurPos - 3, m_Rect->Height - 3);

		pen.SetColor(Color(150, 128, 128, 128));
		m_MemDC->graphics->DrawRectangle(&pen, 0.0, 0.0, m_CurPos - 1, m_Rect->Height - 1);

		*m_Text->color = Color::White;
		DrawShadowText(m_MemDC->graphics, 5, m_Text, Color::Black, Color::MakeARGB(100, 50, 50, 50));
		DUI_ControlBase::Draw();
	}
}

VOID DUI_ProgressBar::AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	m_AnimCurPos += 5;
	if (m_AnimCurPos>m_Rect->Width)
	{
		m_AnimCurPos = -m_Rect->Width;
	}
	Draw(S_Invalid);
	Update();
}
