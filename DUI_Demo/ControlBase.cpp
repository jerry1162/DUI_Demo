#include "stdafx.h"
#include "ControlBase.h"

//ControlBase* ControlBase::m_This;

ControlBase::ControlBase()
{
	//m_This = this;
}


ControlBase::~ControlBase()
{
	delete m_Rect;
	delete m_MemDC;
	delete m_Text;
	//m_This = nullptr;
	m_ID = NULL;
	m_Parent = nullptr;
	m_bVisialbe = FALSE;
}

BOOL ControlBase::Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable)
{
	m_ID = NewID();
	m_Parent = Window;
	m_Rect = new RectF(Left, Top + TITLEBARHEIGHT, Width, Height);
	m_MemDC = new MemDC;
	m_MemDC->Create(m_Rect->Width, m_Rect->Height);
	m_bVisialbe = bVisiable;

	m_Text = new GdipString;
	m_Text = new GdipString;
	m_Text->string = new CString;
	m_Text->string->SetString(Text);

	FontFamily fm(L"Î¢ÈíÑÅºÚ");
	m_Text->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Text->format = new StringFormat;
	m_Text->format->GenericDefault();
	m_Text->color = new Color(Color::Black);
	m_Text->rect = new RectF(Left, Top, Width, Height);
	OnUpdate(NULL, NULL);

	m_Index = Window->m_Controls->size();
	Window->m_Controls->push_back(this);
	return TRUE;
}

LRESULT ControlBase::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_UPDATE:
		OnUpdate(wParam, lParam);
		break;
	case WM_LBUTTONUP:
		m_Parent->SetTitle(L"Clicked");
		break;
	}
	return LRESULT();
}

VOID ControlBase::Draw()
{
	SolidBrush Brush(Color::White);
	m_MemDC->graphics->FillRectangle(&Brush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
}

BOOL ControlBase::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	Draw();
	m_MemDC->BitBlt(m_Parent->m_MemDC->GetMemDC(), m_Rect->GetLeft(), m_Rect->GetTop(),
		m_Rect->Width, m_Rect->Height, 0, 0, SRCCOPY);
	if (lParam == TRUE)
	{
		m_Parent->OnUpdate(m_Rect, TRUE);
	}
	return TRUE;
}
