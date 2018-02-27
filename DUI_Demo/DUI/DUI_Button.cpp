#include "stdafx.h"
#include "DUI_Button.h"


DUI_Button::DUI_Button()
{
	m_ClickProc = nullptr;
	m_bMouseDown = FALSE;
	HasState(TRUE);
}


DUI_Button::~DUI_Button()
{
}

LONG DUI_Button::GetExtraInfo()
{
	return m_ExtraInfo;
}

LONG DUI_Button::SetExtraInfo(LONG Etr)
{
	LONG last = m_ExtraInfo;
	m_ExtraInfo = Etr;
	return last;
}

LRESULT DUI_Button::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_ControlBase::MsgProc(m_ID,uMsg, wParam, lParam);
	//////////////////////////////////////////////////////////////////////////

	if (Ret == -1)
	{
		return TRUE;
	}

	Point pt;
	switch (uMsg)
	{
	case CM_STATECHANGED:
		if (m_bAnimating)
		{
			EndAnimate();
		}
		if (m_CurState != S_Pushed)
		{
			StartAnimate();
			Ret = 1;
		}
		break;
	default:
		break;
	}
	return Ret;
}

VOID DUI_Button::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		//static BOOL TextDown = FALSE;
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		SolidBrush bkgBrush(Color::MakeARGB(255, 255, 255, 255));
		//LinearGradientBrush brush(PointF(0.0, 0.0), PointF(0.0, m_Rect->Height), Color(255, 229, 236, 226), Color(255, 217, 219, 202));
		LinearGradientBrush brush(RectF(0.0, 0.0, m_LogicRect->Width, m_LogicRect->Height), Color(215 * m_Alpha / 255, 255, 255, 255), Color(80 * m_Alpha / 255, 255, 255, 255), LinearGradientModeVertical);

		//brush.SetWrapMode(WrapModeTile);
		Pen Border(Color::MakeARGB(230 * m_Alpha / 255, 50, 50, 50), 1);
		LinearGradientBrush* brush1;
		GraphicsPath* pPath;
		pPath = new GraphicsPath;
		DrawPathRoundRect(pPath, 0, 0, m_LogicRect->Width - 1, m_LogicRect->Height - 1, 6);
		switch (s)
		{
		case S_Normal:
			//Border.SetColor(Color::MakeARGB(255, 154, 156, 156));//97, 95, 88   129, 123, 105
			Border.SetColor(Color::MakeARGB(255, 128, 128, 128));
			//bkgBrush.SetColor(Color::MakeARGB(255, 237, 235, 228));
			break;
		case S_HighLight:
			//Border.SetColor(Color::MakeARGB(255, 18, 176, 255));
			//bkgBrush.SetColor(Color::MakeARGB(255, 160, 200, 255));
			//Border.SetColor(Color::MakeARGB(255, 1, 136, 255));//136
			Border.SetColor(Color::MakeARGB(230, 128, 128, 128));
			brush1 = new LinearGradientBrush(RectF(0.0, 0.0, m_LogicRect->Width, m_LogicRect->Height), Color(215 * m_Alpha / 255, 255, 255, 255), Color(80 * m_Alpha / 255, 255, 255, 255), LinearGradientModeVertical);
			//m_MemDC->graphics->FillRectangle(brush1, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
			m_MemDC->graphics->FillPath(brush1, pPath);
			delete brush1;
			brush.SetLinearColors(Color(80 * m_Alpha / 255, 147, 221, 234), Color(215 * m_Alpha / 255, 147, 221, 234));//222,243,255
			//brush.SetLinearColors(Color(255, 222, 243, 255), Color(255, 222, 243, 255));
			//brush.SetLinearColors(Color(206, 237, 248), Color(165, 226, 238));
			break;
		case S_Focus:
			//Border.SetColor(Color::MakeARGB(200, 0, 0, 230));
			//bkgBrush.SetColor(Color::MakeARGB(255, 200, 200, 255));

			Border.SetColor(Color::MakeARGB(255, 41, 157, 254));
			brush.SetLinearColors(Color(236, 246, 254), Color(243, 248, 252));
			break;
		case S_Pushed:
			//Border.SetColor(Color::MakeARGB(200, 97, 95, 88));
			//bkgBrush.SetColor(Color::MakeARGB(255, 150, 180, 200));
			Border.SetColor(Color::MakeARGB(255, 41, 157, 254));
			brush.SetLinearColors(Color(100 * m_Alpha / 255, 255, 255, 255), Color(215 * m_Alpha / 255, 255, 255, 255));
			//m_Text->rect->Offset(PointF(1, 1));
			//TextDown = TRUE;
			//brush.SetLinearColors(Color(236, 246, 254), Color(243, 248, 252));
			break;
		case S_Disabled:
			Border.SetColor(Color::Gray);
			bkgBrush.SetColor(Color::Gray);
			break;
		default:
			break;
		}
		// 	m_MemDC->graphics->FillRectangle(&brush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
		// 	m_MemDC->graphics->DrawRectangle(&Border, RectF(0, 0, m_Rect->Width - 1, m_Rect->Height - 1));
		// 	Border.SetColor(Color(100, 255, 255, 255));
		// 	m_MemDC->graphics->DrawRectangle(&Border, RectF(1, 1, m_Rect->Width - 3, m_Rect->Height - 3));


		m_MemDC->graphics->FillPath(&brush, pPath);
		m_MemDC->graphics->DrawPath(&Border, pPath);
		delete pPath;

		if (s == S_HighLight)
		{
			pPath = new GraphicsPath;
			Border.SetColor(Color(171, 236, 254));
			DrawPathRoundRect(pPath, 1, 1, m_LogicRect->Width - 3, m_LogicRect->Height - 3, 3);
			m_MemDC->graphics->DrawPath(&Border, pPath);
			delete pPath;
		}

		bkgBrush.SetColor(*m_Text->color);

		//if (s != S_Pushed&&TextDown)
 	//	{
 	//		m_Text->rect->Offset(PointF(-1, -1));
 	//		TextDown = FALSE;
 	//	}
		INT offset = (m_bMouseDown) ? 1 : 0;
		m_MemDC->graphics->DrawString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, RectF(m_Text->rect->X + offset, m_Text->rect->Y + offset, m_Text->rect->Width, m_Text->rect->Height), m_Text->format, &bkgBrush);

		DUI_ControlBase::Draw();
	}
}

//VOID DUI_Button::AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
//{
//	static DUI_Status PrevState = S_Invalid;
//	static BYTE Alpha;
//	if (message != WM_TIMER)
//	{
//		PrevState = (DUI_Status)message;
//		Alpha = m_Alpha;
//	}
//	if (Alpha != 0)
//	{
//		Alpha -= 30;
//		if (Alpha < 30)
//		{
//			Alpha = 0;
//			return;
//		}
//		MemDC* pTmpDC = new MemDC((INT)m_Rect->Width, (INT)m_Rect->Height);
//		Draw(PrevState);
//		m_MemDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, Alpha);
//		m_MemDC->Clear();
//		Draw();
//		m_MemDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 255 - Alpha);
//		m_MemDC->Clear();
//		pTmpDC->AlphaBlend(m_MemDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 255);
//		delete pTmpDC;
//		OnUpdate(TRUE, TRUE);
//		//TRACE("%d\n", message);
//	}
//	else
//	{
//		PrevState = S_Invalid;
//		EndAnimate();
//	}
//	return;
//}
