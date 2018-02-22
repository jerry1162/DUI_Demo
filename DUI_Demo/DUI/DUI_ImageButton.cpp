#include "stdafx.h"
#include "DUI_ImageButton.h"


DUI_ImageButton::DUI_ImageButton()
{
	//m_pAnimateProc = (TIMERPROC)GetCallBackAddr(this, &DUI_ImageButton::AnimateProc);
	for (int i = 0; i < 6; i++)
	{
		m_StatusImage[i] = nullptr;
	}
	m_Start = NULL;
	m_bAutoReleaseImg = TRUE;
}


DUI_ImageButton::~DUI_ImageButton()
{
	if (m_bAutoReleaseImg)
	{
		ReleaseImages();
	}
	Destroy();
}

BOOL DUI_ImageButton::SetImages(Image * Normal, Image * HighLight, Image * Pushed, BOOL bUpdate)
{
	if (Normal == nullptr)
	{
		return FALSE;
	}
	else if (m_StatusImage[S_Normal] != Normal && m_StatusImage[S_Normal] != nullptr)
	{
		delete m_StatusImage[S_Normal];
	}
	m_StatusImage[S_Normal] = Normal;

	if (HighLight == nullptr)
	{
		return FALSE;
	}
	else if (m_StatusImage[S_HighLight] != Normal && m_StatusImage[S_HighLight] != nullptr)
	{
		delete m_StatusImage[S_HighLight];
	}
	m_StatusImage[S_HighLight] = HighLight;

	if (Pushed == nullptr)
	{
		return FALSE;
	}
	else if (m_StatusImage[S_Pushed] != Normal && m_StatusImage[S_Pushed] != nullptr)
	{
		delete m_StatusImage[S_Pushed];
	}
	m_StatusImage[S_Pushed] = Pushed;

	m_StatusImage[S_Focus] = m_StatusImage[S_Normal];
	m_StatusImage[S_Disabled] = m_StatusImage[S_Normal];
	if (bUpdate)
	{
		Update();
	}
	return TRUE;
}

BOOL DUI_ImageButton::SetImages(Image * pImg, INT Start, BOOL bUpdate)
{
	if (pImg == nullptr)
	{
		return FALSE;
	}
	if (pImg != m_StatusImage[0] && m_StatusImage[0] != nullptr)
	{
		delete m_StatusImage[0];
	}
	m_StatusImage[0] = pImg;

	m_Start = Start;
	if (bUpdate)
	{
		Update();
	}
	return TRUE;
}

BOOL DUI_ImageButton::Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height, Image * Normal, Image * HighLight, Image * Pushed, LPCWSTR Text, BOOL bVisiable)
{
	BOOL Ret = Ret = SetImages(Normal, HighLight, Pushed, FALSE);
	if (!Ret)
	{
		return Ret;
	}
	Ret = DUI_ControlBase::Create(Parent, Left, Top, Width, Height, Text, bVisiable);
	return Ret;
}

BOOL DUI_ImageButton::Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height, Image * pImg, INT Start, LPCWSTR Text, BOOL bVisiable)
{
	BOOL Ret = Ret = SetImages(pImg, Start, FALSE);
	if (!Ret)
	{
		return Ret;
	}
	Ret = DUI_ControlBase::Create(Parent, Left, Top, Width, Height, Text, bVisiable);
	return Ret;
}

BOOL DUI_ImageButton::ReleaseImages()
{
	for (int i = 0; i <= S_Pushed; i++)
	{
		if (m_StatusImage[i] != nullptr)
		{
			delete m_StatusImage[i];
		}
	}
	return 0;
}

VOID DUI_ImageButton::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		RectF* Rt = m_LogicRect;


		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		Image* pImg;
		if (m_StatusImage[0] == nullptr)
		{
			pImg = m_StatusImage[s];
			//ASSERT(pImg != nullptr);
			m_MemDC->graphics->DrawImage(pImg, 0, 0);
		}
		else
		{
			pImg = m_StatusImage[0];
			if (s == S_Focus || s == S_Disabled)
			{
				s = S_Normal;
			}
			m_MemDC->graphics->DrawImage(pImg, RectF(0.0, 0.0, Rt->Width, Rt->Height), m_Start + (s - 1)*Rt->Width, 0.0, Rt->Width, Rt->Height, UnitPixel);
		}
		if (m_Text != nullptr)
		{
			SolidBrush TextBrush(*m_Text->color);
			INT offset = (m_bMouseDown) ? 1 : 0;
			m_MemDC->graphics->DrawString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, RectF(m_Text->rect->X + offset, m_Text->rect->Y + offset, m_Text->rect->Width, m_Text->rect->Height), m_Text->format, &TextBrush);
			
			//DrawShadowText(m_MemDC->graphics, 5, m_Text, Color::Black, Color::MakeARGB(100, 50, 50, 50));
		}
		DUI_ControlBase::Draw();
	}
	return VOID();
}
