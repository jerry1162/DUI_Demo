#include "stdafx.h"
#include "DUI_Lable.h"


DUI_Lable::DUI_Lable()
{
	m_TextColor = Color::White;
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
	Ret = ControlBase::MsgProc(uMsg, wParam, lParam);

	if (Ret == -1)
	{
		return TRUE;
	}

	switch (uMsg)
	{
	default:
		break;
	}

	return Ret;
}

VOID DUI_Lable::Draw()
{
	m_MemDC->graphics->Clear(Color::Transparent);
	SolidBrush br(m_TextColor);
	//m_Text->color = &m_TextColor;
	switch (m_CurState)
	{
	case Normal:
		//m_Text->color = &m_TextColor;
		m_Text->color->SetValue(Color::White);
		break;
	case HighLight:
		m_Text->color->SetValue(Color::Blue);
		break;
	case Pushed:
		m_Text->color->SetValue(Color::Red);
		break;
	default:
		break;
	}
	DrawShadowText(m_MemDC->graphics, 5, m_Text, Color::Black, Color::MakeARGB(100, 50, 50, 50));
	//m_MemDC->graphics->DrawString(m_Text->string->GetBuffer(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &br);

	ControlBase::Draw();
}
