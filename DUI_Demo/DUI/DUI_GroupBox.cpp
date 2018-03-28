#include "stdafx.h"
#include "DUI_GroupBox.h"


DUI_GroupBox::DUI_GroupBox()
{
}


DUI_GroupBox::~DUI_GroupBox()
{
	Destroy();
}

RectF * DUI_GroupBox::GetClientRect()
{
	RectF* PCRect = m_Parent->GetClientRect();
	RectF* cRect = new RectF(m_Rect->X + 1, m_Rect->Y + m_Text->rect->Height, m_Rect->Width - 2, m_Rect->Height - m_Text->rect->Height - 1);
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

REAL DUI_GroupBox::GetMarginTop()
{
	return 15.0;
}

LRESULT DUI_GroupBox::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	{
		m_Text->color->SetValue(Color::White);

		SafeDelete(m_Text->format);
		m_Text->format = new StringFormat(StringFormat::GenericTypographic());
		m_Text->format->SetAlignment(StringAlignmentCenter);
		m_Text->format->SetLineAlignment(StringAlignmentCenter);
		m_Text->format->SetTrimming(StringTrimmingEllipsisCharacter);
		m_Text->format->SetFormatFlags(StringFormatFlagsNoWrap);
	}
		break;
	case CM_SIZE:
	{
		RectF TextRect;
		m_MemDC->graphics->MeasureString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &TextRect);
		m_Text->rect->Height = TextRect.Height + 0.1F;
	}
		break;
	default:
		break;
	}

	return Ret;
}

VOID DUI_GroupBox::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		RectF* Rt = m_LogicRect;


		SolidBrush br(Color::MakeARGB(150, 255, 255, 255));
		m_MemDC->graphics->FillRectangle(&br, 0, 5, (INT)Rt->Width, (INT)Rt->Height - 5);

		RectF TextRect;
		m_MemDC->graphics->MeasureString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &TextRect);

		m_MemDC->graphics->SetClip(TextRect, CombineModeExclude);

		Pen pen(Color::MakeARGB(100, 0, 0, 0));
		m_MemDC->graphics->DrawRectangle(&pen, 0, 5, (INT)Rt->Width - 1, (INT)Rt->Height - 5 - 1);

		m_MemDC->graphics->ResetClip();
		//m_MemDC->graphics->DrawRectangle(&pen, TextRect);
		DrawBorderedText(m_MemDC->graphics, m_Text);

		DUI_ControlBase::Draw();
	}
}
