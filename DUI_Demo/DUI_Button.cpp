#include "stdafx.h"
#include "DUI_Button.h"


DUI_Button::DUI_Button()
{
}


DUI_Button::~DUI_Button()
{
}

VOID DUI_Button::OnClick()
{
	static int i = 0;
	//m_Parent->m_bDebug = !m_Parent->m_bDebug;
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
}

LRESULT DUI_Button::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = ControlBase::MsgProc(uMsg, wParam, lParam);
	//////////////////////////////////////////////////////////////////////////

	if (Ret == -1)
	{
		return TRUE;
	}

	static BOOL bMouseDown = FALSE;
	Point pt;
	switch (uMsg)
	{
	case WM_LBUTTONUP:
		if (bMouseDown != FALSE)
		{
			bMouseDown = FALSE;
			m_Text->rect->Offset(PointF(-1, -1));
			OnUpdate(NULL, TRUE);
		}
		m_Parent->GetCursorPos(&pt);
		if (m_Rect->Contains(pt.X, pt.Y))
		{
			OnClick();
		}
		break;
	case WM_LBUTTONDOWN:
		if (bMouseDown != TRUE)
		{
			bMouseDown = TRUE;
			m_Text->rect->Offset(PointF(1, 1));
			OnUpdate(NULL, TRUE);
		}
		break;
	default:
		break;
	}
	return Ret;
}

VOID DUI_Button::Draw()
{
	m_MemDC->Clear();
	SolidBrush bkgBrush(Color::MakeARGB(255, 255, 255, 255));
	LinearGradientBrush brush(PointF(0.0, 0.0), PointF(0.0, m_Rect->Height), Color(255, 229, 236, 226), Color(255, 217, 219, 202));
	
	//brush.SetWrapMode(WrapModeTileFlipX);
	Pen Border(Color::MakeARGB(230, 50, 50, 50), 1);

	GraphicsPath* pPath;
	switch (m_CurState)
	{
	case Normal:
		Border.SetColor(Color::MakeARGB(255, 154, 156, 156));//97, 95, 88   129, 123, 105
		bkgBrush.SetColor(Color::MakeARGB(255, 237, 235, 228));
		break;
	case HighLight:
		//Border.SetColor(Color::MakeARGB(255, 18, 176, 255));
		//bkgBrush.SetColor(Color::MakeARGB(255, 160, 200, 255));
		Border.SetColor(Color::MakeARGB(255, 1, 136, 255));//136
		//brush.SetLinearColors(Color(206, 237, 248), Color(165, 226, 238));
		//brush.SetLinearColors(Color(206, 237, 248), Color(165, 226, 238));

		break;
	case Focus:
		//Border.SetColor(Color::MakeARGB(200, 0, 0, 230));
		//bkgBrush.SetColor(Color::MakeARGB(255, 200, 200, 255));

		Border.SetColor(Color::MakeARGB(255, 41, 157, 254));
		brush.SetLinearColors(Color(236, 246, 254), Color(243, 248, 252));
		break;
	case Pushed:
		//Border.SetColor(Color::MakeARGB(200, 97, 95, 88));
		//bkgBrush.SetColor(Color::MakeARGB(255, 150, 180, 200));
		Border.SetColor(Color::MakeARGB(255, 41, 157, 254));
		brush.SetLinearColors(Color(236, 246, 254), Color(243, 248, 252));
		break;
	case Disabled:
		Border.SetColor(Color::Gray);
		bkgBrush.SetColor(Color::Gray);
		break;
	default:
		break;
	}
	//m_MemDC->graphics->FillRectangle(&bkgBrush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
	//m_MemDC->graphics->DrawRectangle(&Border, RectF(0, 0, m_Rect->Width - 1, m_Rect->Height - 1));
	pPath = new GraphicsPath;
	DrawPathRoundRect(pPath, 0, 0, m_Rect->Width - 1, m_Rect->Height - 1, 6);
	m_MemDC->graphics->FillPath(&brush, pPath);
	m_MemDC->graphics->DrawPath(&Border, pPath);
	delete pPath;

	if (m_CurState == HighLight)
	{
		pPath = new GraphicsPath;
		Border.SetColor(Color(171, 236, 254));
		DrawPathRoundRect(pPath, 1, 1, m_Rect->Width - 3, m_Rect->Height - 3, 3);
		m_MemDC->graphics->DrawPath(&Border, pPath);
		delete pPath;
	}

	bkgBrush.SetColor(*m_Text->color);
	m_MemDC->graphics->DrawString(m_Text->string->GetBuffer(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &bkgBrush);

	ControlBase::Draw();
}
