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
	m_MemDC->Create((int)m_Rect->Width, (int)m_Rect->Height);
	m_bVisialbe = bVisiable;

	m_Text = new GdipString;
	m_Text = new GdipString;
	m_Text->string = new CString;
	m_Text->string->SetString(Text);

	FontFamily fm(L"新宋体");
	m_Text->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Text->format = new StringFormat;
	m_Text->format->SetAlignment(StringAlignmentCenter);
	m_Text->format->SetLineAlignment(StringAlignmentCenter);
	//m_Text->format->GenericDefault();
	m_Text->color = new Color(Color::Black);
	m_Text->rect = new RectF(0, 0, Width, Height);
	OnUpdate(NULL, NULL);

	m_Index = Window->m_Controls->size();
	Window->m_Controls->push_back(this);
	return TRUE;
}

INT ControlBase::GetID()
{
	return m_ID;
}

VOID ControlBase::SetText(LPCWSTR Text)
{
// 	m_Text = new GdipString;
// 	m_Text = new GdipString;
// 	m_Text->string = new CString;
// 	m_Text->string->SetString(Text);
// 
// 	FontFamily fm(L"新宋体");
// 	m_Text->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
// 	m_Text->format = new StringFormat;
// 	m_Text->format->SetAlignment(StringAlignmentCenter);
// 	m_Text->format->SetLineAlignment(StringAlignmentCenter);
// 	//m_Text->format->GenericDefault();
// 	m_Text->color = new Color(Color::Black);
// 	m_Text->rect = new RectF(0, 0, Width, Height);
	m_Text->string->SetString(Text);
	OnUpdate(NULL, TRUE);
}

LRESULT ControlBase::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bMouseDown = FALSE;
	static int i = 0;
	switch (uMsg)
	{
	case WM_UPDATE:
		OnUpdate(wParam, lParam);
		break;
	case WM_LBUTTONUP:
		if (bMouseDown!=FALSE)
		{
			bMouseDown = FALSE;
			m_CurState = HighLight;
			m_Text->rect->Offset(PointF(-1, -1));
			OnUpdate(NULL, TRUE);
		}
		i += 1;
		if ((i % 2) == 0)
		{
			m_Parent->SetTitle(L"OK");
			m_Parent->SetBorderStyle({ BM_RoundRect,NULL,TRUE });
			SetText(L"Button");
		}
		else
		{
			m_Parent->SetTitle(L"Clicked");
			m_Parent->SetBorderStyle({ BM_Normal,Color::MakeARGB(125,0,0,0),FALSE });
			SetText(L"Clicked");
		}
		m_Parent->SetSizeable(!m_Parent->GetSizeable());
		break;
	case WM_LBUTTONDOWN:
		if (bMouseDown != TRUE)
		{
			bMouseDown = TRUE;
			m_CurState = Pushed;
			m_Text->rect->Offset(PointF(1, 1));
			OnUpdate(NULL, TRUE);
		}
		break;
	case WM_MOUSEMOVE:
// 		if (bMouseDown == FALSE && m_CurState != HighLight)
// 		{
// 			m_CurState = HighLight;
// 			//m_Parent->m_HoverControlID = m_ID;
// 			OnUpdate(NULL, TRUE);
// 		}
		break;
	case WM_MOUSELEAVE:
// 		if (m_CurState != Normal)
// 		{
			m_CurState = Normal;
			OnUpdate(NULL, TRUE);
//		}
		break;
	case WM_MOUSEGETIN:
		m_CurState = HighLight;
		OnUpdate(NULL, TRUE);
		break;
	}
	return LRESULT();
}

VOID ControlBase::Draw()
{
	SolidBrush bkgBrush(Color::White);
	Pen Border(Color::Black, 1);
	switch (m_CurState)
	{
	case Normal:
		Border.SetColor(Color::Black);
		bkgBrush.SetColor(Color::White);
		break;
	case HighLight:
		Border.SetColor(Color::Blue);
		bkgBrush.SetColor(Color::MakeARGB(255, 160, 200, 255));
		break;
	case Pushed:
		Border.SetColor(Color::Red);
		bkgBrush.SetColor(Color::MakeARGB(255, 150, 180, 200));
		break;
	case Disabled:
		Border.SetColor(Color::Gray);
		bkgBrush.SetColor(Color::Gray);
		break;
	default:
		break;
	}
	m_MemDC->graphics->FillRectangle(&bkgBrush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
	m_MemDC->graphics->DrawRectangle(&Border, RectF(0, 0, m_Rect->Width - 1, m_Rect->Height - 1));
	bkgBrush.SetColor(*m_Text->color);
	m_MemDC->graphics->DrawString(m_Text->string->GetBuffer(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &bkgBrush);
}

BOOL ControlBase::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	Draw();
	m_MemDC->BitBlt(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, SRCCOPY);
	if (lParam == TRUE)
	{
		HDC hDC = m_Parent->m_Graphics->GetHDC();
		m_Parent->m_MemDC->BitBlt(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), SRCCOPY);
		//m_Parent->OnUpdate(m_Rect, TRUE);
		m_Parent->m_Graphics->ReleaseHDC(hDC);
	}
	return TRUE;
}
