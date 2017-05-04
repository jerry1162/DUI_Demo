#include "stdafx.h"
#include "DUI_ControlBase.h"

//ControlBase* ControlBase::m_This;

DUI_ControlBase::DUI_ControlBase()
{

	m_ID = NULL;
	m_Parent = nullptr;
	m_Rect = nullptr;
	m_MemDC = nullptr;
	m_Alpha = 255;
	m_CurState = S_Normal;
	m_bInAnimating = FALSE;
	m_Text = nullptr;
	m_bVisialbe = TRUE;
	m_AnimateAlpha = 0;
	m_PrevState = S_Invalid;
	m_bAutoUpdate = TRUE;
	m_AnimatePrevDC = nullptr;
	m_pAnimateProc = (TIMERPROC)GetCallBackAddr(this, &DUI_ControlBase::AnimateProc);
	m_Prompt = nullptr;
}


DUI_ControlBase::~DUI_ControlBase()
{
	Destroy();
}

BOOL DUI_ControlBase::Create(DUI_Window* Window, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable)
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

	//m_Index = Window->m_Controls->size();
	Window->m_Controls->push_back(this);

	if (m_bAutoUpdate)
	{
		Update();
	}
	//Window->OnUpdate(NULL, TRUE);
	return TRUE;
}

INT DUI_ControlBase::GetID()
{
	return m_ID;
}

VOID DUI_ControlBase::SetText(LPTSTR Text)
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
	Update();
}

LPCWSTR DUI_ControlBase::GetText()
{
	return m_Text->string->GetString();
}

VOID DUI_ControlBase::SetAlpha(BYTE Alpha)
{
	m_Alpha = Alpha;
	Update();
}

VOID DUI_ControlBase::Move(REAL Left, REAL Top)
{
	m_Rect->X = Left;
	m_Rect->Y = Top;
	Update();
}

BOOL DUI_ControlBase::Existence()
{
	return m_ID != NULL;
}

VOID DUI_ControlBase::SetVisiable(BOOL bVisiable)
{
	if (m_bVisialbe != bVisiable)
	{
		SendMessage(m_ID, CM_SHOW, m_ID, m_bVisialbe);
		m_bVisialbe = bVisiable;
		//if (m_bVisialbe)
		//{
		//	ChangeState(S_Normal);
		//}
		//else
		//{
		//	ChangeState(S_Hide);
		//}
		//Update();
	}
	 
}

BOOL DUI_ControlBase::GetVisiable()
{
	return m_bVisialbe;
}

VOID DUI_ControlBase::Update(BOOL bForce)
{
	if (bForce)
	{
		OnUpdate(NULL, TRUE);
		return;
	}

	if (m_bAutoUpdate)
	{
		OnUpdate(NULL, TRUE);
	}
	else
	{
		OnUpdate(NULL, FALSE);
	}
}

LRESULT DUI_ControlBase::SendMessage(INT ID, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	return m_Parent->WndProc((HWND)ID, Msg, wParam, lParam);
}

VOID DUI_ControlBase::SetPrompt(LPTSTR Text)
{
	m_Prompt = Text;
}

BOOL DUI_ControlBase::Destroy()
{
	if (Existence())
	{
		if (m_bInAnimating)
		{
			EndAnimate();
		}
		if (m_Rect != nullptr)
		{
			delete m_Rect;
			m_Rect = nullptr;
		}

		if (m_MemDC != nullptr)
		{
			delete m_MemDC;
			m_MemDC = nullptr;
		}

		if (m_Text != nullptr)
		{
			delete m_Text->string;
			m_Text->string = nullptr;
			delete m_Text->font;
			m_Text->font = nullptr;
			delete m_Text->format;
			m_Text->format = nullptr;
			delete m_Text->rect;
			m_Text->rect = nullptr;
			delete m_Text->color;
			m_Text->color = nullptr;
			delete m_Text;
			m_Text = nullptr;
		}
		for (auto it = m_Parent->m_Controls->begin(); it != m_Parent->m_Controls->end(); it++)
		{
			if ((*it)->m_ID == m_ID)
			{
				m_Parent->m_Controls->erase(it);
				m_Parent->Update();
				break;
			}
		}
		m_ID = NULL;
		m_Parent = nullptr;
		m_bVisialbe = FALSE;
		FreeCallBackAddr(m_pAnimateProc);
		m_pAnimateProc = nullptr;
		return TRUE;
	}
	return FALSE;
}

LRESULT DUI_ControlBase::MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			//Update();

			//m_Parent->GetCursorPos(&ptMouse);
			//m_Rect->X = (REAL)ptMouse.X - ptMouseDown.X;
			//m_Rect->Y = (REAL)ptMouse.Y - ptMouseDown.Y;
			//Update();
		}
		break;
	case WM_MOUSEGETIN:
		if (m_Prompt != _T(""))
		{
			m_Parent->GetWndPrompt()->Create(m_Parent->m_hWnd, m_Prompt, m_Rect);
		}
		break;
	}
	if (IsMouseMsg(uMsg))
	{
		if (m_CurState == S_Disabled)
		{
			return -1;
		}
		return TRUE;
	}
	return LRESULT();
}

VOID DUI_ControlBase::Draw(DUI_Status s)
{
// 	SolidBrush bkgBrush(Color::MakeARGB(255, 255, 255, 255));
// 	Pen Border(Color::MakeARGB(255,0,0,0), 1);
// 	m_MemDC->graphics->FillRectangle(&bkgBrush, 0.0, 0.0, m_Rect->Width, m_Rect->Height);
// 	m_MemDC->graphics->DrawRectangle(&Border, RectF(0, 0, m_Rect->Width - 1, m_Rect->Height - 1));
// 
// 	bkgBrush.SetColor(*m_Text->color);
// 	m_MemDC->graphics->DrawString(m_Text->string->GetString(), m_Text->string->GetLength(), m_Text->font, *m_Text->rect, m_Text->format, &bkgBrush);
	//static int i = 0;
	//TRACE("\nHello - %d\n", ++i);
	if (m_Parent->m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(255, 100, 100, 100), 1);
		m_MemDC->graphics->DrawRectangle(&BorderPen, 0.0, 0.0, m_Rect->Width - 1, m_Rect->Height - 1);
		//m_MemDC->graphics->DrawRectangle(&BorderPen, *m_Rect);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_Text->rect);
	}
}

VOID DUI_ControlBase::ChangeState(DUI_Status s)
{
	m_PrevState = m_CurState;
	m_CurState = s;
	if (!MsgProc(WM_STATECHANGED, m_ID, m_PrevState))
	{
		Draw();
		Update();
	}
}

DUI_Status DUI_ControlBase::GetState()
{
	return m_CurState;
}

BOOL DUI_ControlBase::GetCursorPos(Point * pt)
{
	BOOL ret;
	ret = m_Parent->GetCursorPos(pt);
	pt->X -= (INT)m_Rect->X;
	pt->Y -= (INT)m_Rect->Y;
	return ret;
}

BOOL DUI_ControlBase::StartAnimate(TIMERPROC pCallBack, UINT uElapse)
{
	if (pCallBack == nullptr)
	{
		pCallBack = m_pAnimateProc;
	}
	auto Ret = SetTimer(m_Parent->m_hWnd, m_ID, uElapse, pCallBack);
	if (Ret != NULL)
	{
		m_bInAnimating = TRUE;
		m_AnimatePrevDC = new MemDC((INT)m_Rect->Width, (INT)m_Rect->Height);
		m_MemDC->BitBlt(m_AnimatePrevDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, SRCCOPY);
		//m_AnimatePrevDC->Clear();

		//Draw();
		pCallBack(m_Parent->m_hWnd, NULL, m_ID, GetTickCount());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL DUI_ControlBase::EndAnimate()
{
	m_bInAnimating = FALSE;
	if (m_AnimatePrevDC != nullptr)
	{
		delete m_AnimatePrevDC;
		m_AnimatePrevDC = nullptr;
	}
	
	//delete m_AnimateDestDC;
	return KillTimer(m_Parent->m_hWnd, m_ID);
}


VOID CALLBACK DUI_ControlBase::AnimateProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (message != WM_TIMER)
	{
		m_AnimateAlpha = m_Alpha;
	}
	if (m_AnimateAlpha != 0)
	{
		m_AnimateAlpha -= 30;
		if (m_AnimateAlpha < 30)
		{
			m_AnimateAlpha = 0;
		}
		MemDC* pTmpDC = new MemDC((INT)m_Rect->Width, (INT)m_Rect->Height);
		//m_MemDC->Clear();
		m_AnimatePrevDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, m_AnimateAlpha);
		Draw();
		m_MemDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 255 - m_AnimateAlpha);
		m_MemDC->Clear();
		pTmpDC->AlphaBlend(m_MemDC->GetMemDC(), 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 0, 0, (INT)m_Rect->Width, (INT)m_Rect->Height, 255);
		delete pTmpDC;
		Update();
	}
	else
	{
		//m_PrevState = S_Invalid;
		EndAnimate();
	}
	return;
}

BOOL DUI_ControlBase::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	if (!m_bInAnimating)
	{
		Draw();
	}

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
		//HDC hDC = m_Parent->m_Graphics->GetHDC();
		m_Parent->OnUpdate(m_ID, TRUE);

		//if (m_Parent->m_Alpha != 0)
		//{
		//	m_Parent->OnUpdate(m_ID, TRUE);
		//	//SIZE szWnd;
		//	//szWnd = { (INT)m_Parent->m_WndRect->Width, (INT)m_Parent->m_WndRect->Height };
		//	//POINT ptSrc = { 0,0 };
		//	//BLENDFUNCTION bf;
		//	//bf.AlphaFormat = AC_SRC_ALPHA;
		//	//bf.BlendFlags = 0;
		//	//bf.BlendOp = 0;
		//	//bf.SourceConstantAlpha = m_Parent->m_Alpha;
		//	//UpdateLayeredWindow(m_Parent->m_hWnd, NULL, NULL, &szWnd, m_Parent->m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
		//}
		//else
		//{
		//	//m_Parent->m_MemDC->BitBlt(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), SRCCOPY);
		//	m_Parent->m_MemDC->AlphaBlend(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
		//}

		//m_Parent->m_Graphics->ReleaseHDC(hDC);

	}
	else
	{
		m_MemDC->AlphaBlend(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
	}
	return TRUE;
}
