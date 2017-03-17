#include "stdafx.h"
#include "ControlBase.h"

//ControlBase* ControlBase::m_This;

ControlBase::ControlBase()
{
	//m_This = this;
	m_Alpha = 255;
}


ControlBase::~ControlBase()
{
	delete m_Rect;
	delete m_MemDC;

	delete m_Text->string;
	delete m_Text->font;
	delete m_Text->format;
	delete m_Text->rect;
	delete m_Text->color;
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

	//m_Text = new GdipString;
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
	

	m_Index = Window->m_Controls->size();
	Window->m_Controls->push_back(this);

	m_CurState = Normal;
	OnUpdate(NULL, TRUE);
	//Window->OnUpdate(NULL, TRUE);
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

VOID ControlBase::SetAlpha(BYTE Alpha)
{
	m_Alpha = Alpha;
	OnUpdate(NULL, TRUE);
}

LRESULT ControlBase::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bMouseDown = FALSE;
	static Point ptMouseDown;
	static Point ptLable;
	Point ptMouse;
	switch (uMsg)
	{
	case WM_UPDATE:
		OnUpdate(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		bMouseDown = TRUE;
		GetCursorPos(&ptMouse);
		ptMouseDown.X = ptMouse.X;
		ptMouseDown.Y = ptMouse.Y;
		ptLable.X = (INT)m_Rect->X;
		ptLable.Y = (INT)m_Rect->Y;
		break;
	case WM_LBUTTONUP:
		bMouseDown = FALSE;
		ptMouseDown.X = 0;
		ptMouseDown.Y = 0;
		ptLable.X = 0;
		ptLable.Y = 0;
		break;
	case WM_MOUSEMOVE:
		if (bMouseDown)
		{
			//TRACE("\nlParam %d  %d\n", LOWORD(lParam), HIWORD(lParam));
			//TRACE("\nptMouse %d  %d\n", ptMouseDown.X, ptMouseDown.Y);
			//GetCursorPos(&ptMouse);
			//m_Rect->X = (REAL)ptLable.X + ptMouse.X - ptMouseDown.X;
			//m_Rect->Y = (REAL)ptLable.Y + ptMouse.Y - ptMouseDown.Y;
			//ptLable.X = (INT)m_Rect->X;
			//ptLable.Y = (INT)m_Rect->Y;
			//OnUpdate(NULL, TRUE);
		}
		break;
	}
	if (IsMouseMsg(uMsg))
	{
		if (m_CurState == Disabled)
		{
			return -1;
		}
		return TRUE;
	}
	return LRESULT();
}

VOID ControlBase::Draw()
{
// 	SolidBrush bkgBrush(Color::MakeARGB(255, 255, 255, 255));
// 	Pen Border(Color::MakeARGB(255,0,0,0), 1);
// 	m_MemDC->graphics->FillRectangle(&bkgBrush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
// 	m_MemDC->graphics->DrawRectangle(&Border, RectF(0, 0, m_Rect->Width - 1, m_Rect->Height - 1));
// 
// 	bkgBrush.SetColor(*m_Text->color);
// 	m_MemDC->graphics->DrawString(m_Text->string->GetBuffer(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &bkgBrush);
	//static int i = 0;
	//TRACE("\nHello - %d\n", ++i);
	if (m_Parent->m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(255, 100, 100, 100), 1);
		m_MemDC->graphics->DrawRectangle(&BorderPen, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
	}
}

VOID ControlBase::ChangeStatus(BTNStatus s)
{
	m_CurState = s;
	OnUpdate(NULL, TRUE);
}

BTNStatus ControlBase::GetStatus()
{
	return m_CurState;
}

BOOL ControlBase::GetCursorPos(Point * pt)
{
	BOOL ret;
	ret = m_Parent->GetCursorPos(pt);
	pt->X -= (INT)m_Rect->X;
	pt->Y -= (INT)m_Rect->Y;
	return ret;
}

BOOL ControlBase::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	Draw();
	//m_MemDC->AlphaBlend(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);

	//for (vector<ControlBase*>::iterator it = m_Parent->m_Controls->begin(); it != m_Parent->m_Controls->end(); it++)
	//{
	//	INT i = it - m_Parent->m_Controls->begin();
	//	if (i <= m_Index)
	//	{
	//		continue;
	//	}
	//	if (m_Rect->IntersectsWith(*(*it)->m_Rect))
	//	{
	//		//(*it)->m_MemDC->Clear();
	//		//m_Parent->OnUpdate(NULL, FALSE);
	//		//(*it)->Draw();
	//		(*it)->OnUpdate();
	//		//(*it)->m_MemDC->AlphaBlend(m_Parent->m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
	//	}
	//}

/*	m_MemDC->BitBlt(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, SRCCOPY);*/
	if (lParam == TRUE)
	{
		HDC hDC = m_Parent->m_Graphics->GetHDC();
		if (m_Parent->m_Alpha != 0)
		{
			m_Parent->OnUpdate(m_ID, TRUE);
			//SIZE szWnd;
			//szWnd = { (INT)m_Parent->m_WndRect->Width, (INT)m_Parent->m_WndRect->Height };
			//POINT ptSrc = { 0,0 };
			//BLENDFUNCTION bf;
			//bf.AlphaFormat = AC_SRC_ALPHA;
			//bf.BlendFlags = 0;
			//bf.BlendOp = 0;
			//bf.SourceConstantAlpha = m_Parent->m_Alpha;
			//UpdateLayeredWindow(m_Parent->m_hWnd, NULL, NULL, &szWnd, m_Parent->m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
		}
		else
		{
			//m_Parent->m_MemDC->BitBlt(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), SRCCOPY);
			m_Parent->m_MemDC->AlphaBlend(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
		}
		m_Parent->m_Graphics->ReleaseHDC(hDC);

	}
	else
	{
		m_MemDC->AlphaBlend(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
	}
	return TRUE;
}
