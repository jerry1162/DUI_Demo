#include "stdafx.h"
#include "DUI_Lable.h"


DUI_Lable::DUI_Lable()
{
	m_TextColor = Color::White;
	//m_pAnimateProc = (TIMERPROC)GetCallBackAddr(this, &DUI_Lable::AnimateProc);
	HasState(TRUE);
}


DUI_Lable::~DUI_Lable()
{
}

VOID DUI_Lable::SetTextColor(Color color)
{
	return VOID();
}

LRESULT DUI_Lable::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(uMsg, wParam, lParam);

	if (Ret == -1)
	{
		return TRUE;
	}

	switch (uMsg)
	{
	case CM_STATECHANGED:
		if (m_bInAnimating)
		{
			EndAnimate();
		}
		
		StartAnimate();
		Ret = 1;
		break;
	case CM_SHOW:
		if (m_bInAnimating)
		{
			EndAnimate();
		}
		StartAnimate();
		//Ret = 1;
		break;
	default:
		break;
	}

	return Ret;
}

VOID DUI_Lable::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		SolidBrush br(/*m_TextColor*/Color::Black);
		//m_Text->color = &m_TextColor;
		switch (s)
		{
		case S_Normal:
			//m_Text->color = &m_TextColor;
			m_Text->color->SetValue(Color::White);
			break;
		case S_HighLight:
			m_Text->color->SetValue(Color::Blue);
			break;
		case S_Pushed:
			m_Text->color->SetValue(Color::Red);
			break;
		case S_Focus:
			m_Text->color->SetValue(Color::Gray);
			break;
		default:
			break;
		}
		DrawShadowText(m_MemDC->graphics, 5, m_Text, Color::Black, Color::MakeARGB(100, 50, 50, 50));
		//br.SetColor(m_TextColor);
		//m_MemDC->graphics->DrawString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &br);

		DUI_ControlBase::Draw();
	}
}