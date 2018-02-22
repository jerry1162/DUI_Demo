#include "stdafx.h"
#include "DUI_ControlBase.h"

DUI_ControlBase::DUI_ControlBase()
{
	m_Parent = nullptr;
	m_Rect = nullptr;
	m_LogicRect = nullptr;
	m_VRect = nullptr;
	m_LogicVRect = nullptr;
	m_MemDC = nullptr;
	m_Alpha = 255;
	m_CurState = S_Normal;
	m_bAnimating = FALSE;
	m_Text = nullptr;
	m_bVisialbe = TRUE;
	m_AnimateAlpha = 0;
	m_PrevState = S_Invalid;
	m_bHasState = FALSE;
	m_AnimatePrevDC = nullptr;
	m_pAnimateProc = (TIMERPROC)GetCallBackAddr(this, &DUI_ControlBase::AnimateProc);
	m_Prompt = nullptr;
	m_MsgProc = nullptr;
	m_bAutoUpdate = FALSE;
	m_bMoveWithMouse = FALSE;
	m_bMouseDown = FALSE;
	m_ParentWnd = nullptr;
	m_ptMouseDown = nullptr;
	m_bCanShowOnNCRgn = FALSE;
	m_CursorName = nullptr;
	m_LastHCursor = NULL;
}


DUI_ControlBase::~DUI_ControlBase()
{
	Destroy();
}

BOOL DUI_ControlBase::Create(DUI_Object* Parent, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable, BOOL bForceUpdate)
{
	if (!Parent->CanBeParent())
	{
		return FALSE;
	}
	m_ID = NewID();
	m_Parent = Parent;

	if (Parent->GetObjType() == ObjType_Wnd)
	{
		m_ParentWnd = (DUI_Window*)m_Parent;
	}
	else
	{
		m_ParentWnd = ((DUI_ControlBase*)m_Parent)->m_ParentWnd;
	}
	m_LogicRect = new RectF(Left, Top + m_Parent->GetMarginTop(), Width, Height);
	m_MemDC = new MemDC;
	m_MemDC->Create((INT)m_LogicRect->Width, (INT)m_LogicRect->Height);
	//m_MemDC->graphics->SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
	m_bVisialbe = bVisiable;
	OnCalcRect();

	m_Text = new GdipString;
	m_Text->string = new CString;
	m_Text->string->SetString(Text);

	FontFamily fm(L"新宋体");
	m_Text->font = new Gdiplus::Font(&fm, 13, FontStyleRegular, UnitWorld);
	m_Text->format = new StringFormat/*(StringFormat::GenericTypographic())*/;
	m_Text->format->SetAlignment(StringAlignmentCenter);
	m_Text->format->SetLineAlignment(StringAlignmentCenter);
	m_Text->color = new Color(254, 0, 0, 0);
	m_Text->rect = new RectF(0, 0, Width, Height);

	AddCtrl();
	
	if (m_ParentWnd->IsWindowInited())
	{
		m_bAutoUpdate = TRUE;
	}

	MsgProc(m_ID, CM_CREATE, m_ID, NULL);

	Update(bForceUpdate);
	return TRUE;
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
	StartPauseDebug();
	//PauseWhen(m_VRect->IsEmptyArea() && m_ID == 8)
	if (!m_VRect->IsEmptyArea())
	{
		// 	RectF* rect = m_Rect->Clone();
		// 	if (m_Parent->GetObjType() == ObjType_Ctrl)
		// 	{
		// 		rect->Intersect(*((DUI_ControlBase*)m_Parent)->m_Rect);
		// 	}
		// 	m_ParentWnd->m_MemDC->SelectRectClipRgn(rect);
		RectF* ClipRect = nullptr;
		if (!m_bCanShowOnNCRgn)
		{
			ClipRect = m_Parent->GetClientRect();
			if (ClipRect != nullptr)
			{
				m_ParentWnd->m_MemDC->SelectRectClipRgn(ClipRect);
			}
		}

		m_ParentWnd->m_BkgDC->BitBlt(m_ParentWnd->m_MemDC, (INT)m_VRect->X, (INT)m_VRect->Y, (INT)m_VRect->Width, (INT)m_VRect->Height);
		ExcuteAfterPaused(m_ParentWnd->Flush());
		RectF* rfRect = nullptr;
		for (auto it = m_ParentWnd->m_Controls->begin(); it != m_ParentWnd->m_Controls->end(); it++)
		{
			if (!(*it)->m_bVisialbe || (*it) == this || (*it)->m_Parent == this)
			{
				continue;
			}
			rfRect = m_VRect->Clone();
			if (!(*it)->m_VRect->IsEmptyArea() && rfRect->Intersect(*(*it)->m_VRect))
			{
				if (!(*it)->m_bCanShowOnNCRgn)
				{
					RectF* iClipRect = nullptr;
					iClipRect = (*it)->m_Parent->GetClientRect();
					if (iClipRect != nullptr)
					{
						if (ClipRect != nullptr)
						{
							if (iClipRect->Intersect(*ClipRect))
							{
								m_ParentWnd->m_MemDC->SelectRectClipRgn(iClipRect);
							}
						}
						else
						{
							m_ParentWnd->m_MemDC->SelectRectClipRgn(iClipRect);
						}
						delete iClipRect;
					}
				}

				(*it)->m_MemDC->AlphaBlend(m_ParentWnd->m_MemDC->GetMemDC(), (int)rfRect->GetLeft(), (int)rfRect->GetTop(), (int)rfRect->Width, (int)rfRect->Height, INT(rfRect->X - (*it)->m_VRect->X), INT(rfRect->Y - (*it)->m_VRect->Y), (int)rfRect->Width, (int)rfRect->Height, (*it)->m_Alpha);
				ExcuteAfterPaused(m_ParentWnd->Flush());
				if (!m_bCanShowOnNCRgn)
				{
					m_ParentWnd->m_MemDC->SelectRectClipRgn(ClipRect);
				}
				//m_ParentWnd->Flush();
				//(*it)->m_MemDC->AlphaBlend(m_ParentWnd->m_MemDC, (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
				// 			m_ParentWnd->m_MemDC->SelectRectClipRgn(rect);
				// 			delete tmpRect;
				//m_ParentWnd->Flush();
			}
			delete rfRect;
		}
		delete ClipRect;
		m_ParentWnd->m_MemDC->SelectClipRgn();
		// 	delete rect;
		// 	m_ParentWnd->m_MemDC->SelectClipRgn();
	}
	m_LogicRect->X = Left;
	m_LogicRect->Y = Top;
	OnCalcRect();
	SendMsgToChild(CM_CALCRECT, NULL, NULL);
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
		MsgProc(m_ID, CM_SHOW, m_ID, m_bVisialbe);
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

VOID DUI_ControlBase::Update(INT bForce)
{
	if (m_bAutoUpdate && bForce == -1)
	{
		bForce = TRUE;
	}
	if (bForce==TRUE && m_ParentWnd->IsWindowInited())
	{
		OnUpdate(NULL, TRUE);
	}
	else
	{
		OnUpdate(NULL, FALSE);
	}
}

VOID DUI_ControlBase::SetPrompt(LPTSTR Text)
{
	m_Prompt = Text;
}

REAL DUI_ControlBase::GetX()
{
	if (m_Parent->GetObjType() == ObjType_Wnd)
	{
		return m_Rect->X;
	}
	else
	{
		return m_Rect->X - m_Parent->GetX();
	}
}

REAL DUI_ControlBase::GetY()
{
	if (m_Parent->GetObjType() == ObjType_Wnd)
	{
		return m_Rect->Y;
	}
	else
	{
		return m_Rect->Y - m_Parent->GetY();
	}
}

REAL DUI_ControlBase::GetWidth()
{
	return m_Rect->Width;
}

REAL DUI_ControlBase::GetHeight()
{
	return m_Rect->Height;
}

MSGPROC DUI_ControlBase::SetMsgProc(MSGPROC Proc)
{
	MSGPROC PrevProc = m_MsgProc;
	m_MsgProc = Proc;
	return PrevProc;
}

BOOL DUI_ControlBase::HasState(INT bHasState)
{
	if (m_bHasState == -1)
	{
		return m_bHasState;
	}
	m_bHasState = bHasState;
	return TRUE;
}

BOOL DUI_ControlBase::MoveWithMouse(INT b)
{
	if (b == -1)
	{
		return m_bMoveWithMouse;
	}
	m_bMoveWithMouse = b;
	return TRUE;
}

VOID DUI_ControlBase::SendMsgToChild(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!CanBeParent())
	{
		return;
	}
	for (auto it = m_ParentWnd->m_Controls->begin(); it != m_ParentWnd->m_Controls->end(); ++it)
	{
		if ((*it)->m_Parent == this)
		{
			(*it)->MsgProc((*it)->GetID(), uMsg, wParam, lParam);
		}
	}
}

DUI_Window * DUI_ControlBase::GetParentWnd()
{
	return m_ParentWnd;
}

VOID DUI_ControlBase::ClientToWnd(Point * ptClient)
{
	ptClient->X += (INT)m_Rect->X;
	ptClient->Y += (INT)m_Rect->Y;
}

VOID DUI_ControlBase::WndToClient(Point * ptWnd)
{
	ptWnd->X -= (INT)m_Rect->X;
	ptWnd->Y -= (INT)m_Rect->Y;
}

VOID DUI_ControlBase::SetbShowOnNCRgn(BOOL bShow)
{
	m_bCanShowOnNCRgn = bShow;
	Update();
}

BOOL DUI_ControlBase::GetbShowOnNCRgn()
{
	return m_bCanShowOnNCRgn;
}

LPTSTR DUI_ControlBase::SetCursor(LPTSTR CursorName)
{
	LPTSTR PrevCur = m_CursorName;
	m_CursorName = CursorName;
	return PrevCur;
}

LPTSTR DUI_ControlBase::GetCursor()
{
	return m_CursorName;
}

BOOL DUI_ControlBase::IsPtInCtrl(Point * pt)
{
	RectF* cRect = m_VRect->Clone();
	if (!m_bCanShowOnNCRgn)
	{
		RectF* pcRect = m_Parent->GetClientRect();
		cRect->Intersect(*pcRect);
		delete pcRect;
	}
	BOOL bInCtrl = PtInRect(cRect, pt);
	delete cRect;
	return bInCtrl;
}

BOOL DUI_ControlBase::Destroy()
{
	if (Existence())
	{
		if (m_bAnimating)
		{
			EndAnimate();
		}
		for (auto it = m_ParentWnd->m_Controls->begin(); it != m_ParentWnd->m_Controls->end(); it++)
		{
			if ((*it)->m_ID == m_ID)
			{
				m_ParentWnd->m_Controls->erase(it);
				m_ParentWnd->Update();
				break;
			}
		}
		
		SafeDelete(m_Rect);
		SafeDelete(m_LogicRect);
		SafeDelete(m_VRect);
		SafeDelete(m_LogicVRect);
		SafeDelete(m_MemDC);

		SafeDelete(m_Text);
		SafeDelete(m_ptMouseDown);
		m_ID = NULL;
		m_Parent = nullptr;
		m_bVisialbe = FALSE;
		FreeCallBackAddr(m_pAnimateProc);
		m_pAnimateProc = nullptr;
		return TRUE;
	}
	return FALSE;
}

ObjType DUI_ControlBase::GetObjType()
{
	return ObjType_Ctrl;
}

LRESULT DUI_ControlBase::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Point ptMouse;
	if (m_MsgProc != nullptr)
	{
		if (!m_MsgProc((VOID*)this, uMsg, wParam, lParam))
		{
			return TRUE;
		}
	}
	switch (uMsg)
	{
	case WM_UPDATE:
		OnUpdate(wParam, lParam);
		break;
	case WM_LBUTTONDOWN:
		m_bMouseDown = TRUE;
		GetCursorPos(&ptMouse);
		if (m_ptMouseDown == nullptr)
		{
			m_ptMouseDown = new Point(0, 0);
		}
		m_ptMouseDown->X = ptMouse.X;
		m_ptMouseDown->Y = ptMouse.Y;
		break;
	case WM_LBUTTONUP:
		if (m_bMouseDown)
		{
			m_bMouseDown = FALSE;
			SafeDelete(m_ptMouseDown);
			GetCursorPos(&ptMouse);
			ClientToWnd(&ptMouse);
			if (IsPtInCtrl(&ptMouse))
			{
				MsgProc(m_ID, CM_CLICKED, m_ID, NULL);
			}

		}
		break;
	case WM_MOUSEMOVE:
		if (m_bMouseDown)
		{
			if (m_bMoveWithMouse)
			{
				m_Parent->GetCursorPos(&ptMouse);
				Move((REAL)ptMouse.X - m_ptMouseDown->X, (REAL)ptMouse.Y - m_ptMouseDown->Y);
			}
		}
		break;
	case CM_MOUSEGETIN:
		if (m_CursorName != nullptr)
		{
			m_LastHCursor = m_ParentWnd->SetCursor(m_CursorName);
		}
		else
		{
			m_ParentWnd->SetCursor(IDC_ARROW);
		}
		if (m_Prompt != _T(""))
		{
			m_ParentWnd->GetWndPrompt()->Create(m_ParentWnd->GetHWND(), m_Prompt, m_VRect);
		}
		break;
	case CM_MOUSELEAVE:
		if (m_LastHCursor != NULL)
		{
			m_ParentWnd->SetCursor(m_LastHCursor);
			m_LastHCursor = NULL;
		}
		break;
	case CM_SETCURSOR:
		if (m_CursorName != nullptr)
		{
			m_ParentWnd->SetCursor(m_CursorName);
		}
		break;
	case CM_SETAUTOUPDATE:
		m_bAutoUpdate = (BOOL)lParam;
		break;
	case CM_ADDCTRL:
		if (m_Parent->GetObjType() == ObjType_Wnd)
		{
			m_ParentWnd->AddControl((DUI_ControlBase*)wParam);
		}
		else
		{
			m_Parent->SendMsg(uMsg, wParam, lParam);
		}
		break;
	case CM_CALCRECT:
		OnCalcRect(wParam, lParam);
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
	return FALSE;
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
	if (m_ParentWnd->m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(255, 100, 100, 100), 1);
		m_MemDC->graphics->DrawRectangle(&BorderPen, 0.0, 0.0, m_LogicRect->Width - 1, m_LogicRect->Height - 1);
		//m_MemDC->graphics->DrawRectangle(&BorderPen, *m_Rect);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_Text->rect);
	}
}

VOID DUI_ControlBase::ChangeState(DUI_Status s)
{
	if (!m_bHasState)
	{
		return;
	}
	m_PrevState = m_CurState;
	m_CurState = s;
	if (!MsgProc(m_ID, CM_STATECHANGED, m_ID, m_PrevState))
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
	pt->X -= (INT)m_LogicRect->X;
	pt->Y -= (INT)m_LogicRect->Y;
	return ret;
}

BOOL DUI_ControlBase::StartAnimate(TIMERPROC pCallBack, UINT uElapse)
{
	if (pCallBack == nullptr)
	{
		pCallBack = m_pAnimateProc;
	}
	auto Ret = SetTimer(m_ParentWnd->GetHWND(), m_ID, uElapse, pCallBack);
	if (Ret != NULL)
	{
		m_bAnimating = TRUE;
		m_AnimatePrevDC = new MemDC((INT)m_LogicRect->Width, (INT)m_LogicRect->Height);
		m_MemDC->BitBlt(m_AnimatePrevDC->GetMemDC(), 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 0, 0, SRCCOPY);
		//m_AnimatePrevDC->Clear();

		//Draw();
		pCallBack(m_ParentWnd->GetHWND(), NULL, m_ID, GetTickCount());
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL DUI_ControlBase::EndAnimate()
{
	m_bAnimating = FALSE;
	SafeDelete(m_AnimatePrevDC);
	return KillTimer(m_ParentWnd->GetHWND(), m_ID);
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
		MemDC* pTmpDC = new MemDC((INT)m_LogicRect->Width, (INT)m_LogicRect->Height);
		//m_MemDC->Clear();
		m_AnimatePrevDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, m_AnimateAlpha);
		Draw();
		m_MemDC->AlphaBlend(pTmpDC->GetMemDC(), 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 255 - m_AnimateAlpha);
		m_MemDC->Clear();
		pTmpDC->AlphaBlend(m_MemDC->GetMemDC(), 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 0, 0, (INT)m_LogicRect->Width, (INT)m_LogicRect->Height, 255);
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

VOID DUI_ControlBase::AddCtrl()
{
	if (m_Parent->GetObjType() == ObjType_Wnd)
	{
		m_ParentWnd->AddControl(this);
	}
	else
	{
		m_Parent->SendMsg(CM_ADDCTRL, (WPARAM)this, NULL);
	}
}

BOOL DUI_ControlBase::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	StartPauseDebug();

	if (!m_bAnimating)
	{
		Draw();
	}
	if (m_VRect->IsEmptyArea())
	{
		return TRUE;
	}

	RectF* ClipRect = nullptr;
	if (!m_bCanShowOnNCRgn)
	{
		ClipRect = m_Parent->GetClientRect();
		if (ClipRect != nullptr)
		{
			m_ParentWnd->m_MemDC->SelectRectClipRgn(ClipRect);
		}
	}
	

	m_ParentWnd->m_BkgDC->BitBlt(m_ParentWnd->m_MemDC, (INT)m_VRect->X, (INT)m_VRect->Y, (INT)m_VRect->Width, (INT)m_VRect->Height);

	ExcuteAfterPaused(m_ParentWnd->Flush());

	RectF* rfRect = nullptr;
	for (auto it = m_ParentWnd->m_Controls->begin(); it != m_ParentWnd->m_Controls->end(); it++)
	{
		if (!(*it)->m_bVisialbe)
		{
			continue;
		}
		if (*it == this)
		{
			m_MemDC->AlphaBlend(m_ParentWnd->m_MemDC, (int)m_VRect->GetLeft(), (int)m_VRect->GetTop(), (int)m_VRect->Width, (int)m_VRect->Height, INT(m_LogicVRect->X - m_LogicRect->X), INT(m_LogicVRect->Y - m_LogicRect->Y), (int)m_LogicVRect->Width, (int)m_LogicVRect->Height, (*it)->m_Alpha);
			ExcuteAfterPaused(m_ParentWnd->Flush());
			continue;
		}

		rfRect = m_VRect->Clone();
		if (!(*it)->m_VRect->IsEmptyArea() && rfRect->Intersect(*(*it)->m_VRect))
		{
			RectF* iClipRect = nullptr;
			if (!(*it)->m_bCanShowOnNCRgn && *it != m_Parent)
			{
				iClipRect = (*it)->m_Parent->GetClientRect();
				if (iClipRect != nullptr)
				{
					if (ClipRect != nullptr)
					{
						if (iClipRect->Intersect(*ClipRect))
						{
							m_ParentWnd->m_MemDC->SelectRectClipRgn(iClipRect);
						}
					}
					else
					{
						m_ParentWnd->m_MemDC->SelectRectClipRgn(iClipRect);
					}
					delete iClipRect;
				}
			}

			(*it)->m_MemDC->AlphaBlend(m_ParentWnd->m_MemDC, (int)rfRect->X, (int)rfRect->Y, (int)rfRect->Width, (int)rfRect->Height, INT(rfRect->X - (*it)->m_Rect->X), INT(rfRect->Y - (*it)->m_Rect->Y), (int)rfRect->Width, (int)rfRect->Height, (*it)->m_Alpha);

			m_ParentWnd->m_MemDC->SelectRectClipRgn(ClipRect);
			ExcuteAfterPaused(m_ParentWnd->Flush());
		}
		delete rfRect;

		
	}
	delete ClipRect;
	m_ParentWnd->m_MemDC->SelectClipRgn();
	if (lParam == TRUE && m_ParentWnd->m_bInited)
	{
		if (m_ParentWnd->m_Alpha != 0)
		{
			SIZE szWnd;
			szWnd = { (INT)m_ParentWnd->m_WndRect->Width, (INT)m_ParentWnd->m_WndRect->Height };
			POINT ptSrc = { 0,0 };
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = m_ParentWnd->m_Alpha;
			UpdateLayeredWindow((HWND)m_ParentWnd->m_ID, m_ParentWnd->m_hDC, nullptr, &szWnd, m_ParentWnd->m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
		}
		else
		{
			m_ParentWnd->m_MemDC->AlphaBlend(m_ParentWnd->m_hDC, 0, 0, (INT)m_ParentWnd->m_WndRect->Width, (INT)m_ParentWnd->m_WndRect->Height, 0, 0, (INT)m_ParentWnd->m_WndRect->Width, (INT)m_ParentWnd->m_WndRect->Height, 255);
		}
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

// 	if (lParam == TRUE)
// 	{
// 		//HDC hDC = m_Parent->m_Graphics->GetHDC();
// 		m_Parent->OnUpdate(m_ID, TRUE);
// 
// // 		if (m_Parent->m_Alpha != 0)
// // 		{
// // 			//m_Parent->OnUpdate(m_ID, TRUE);
// // 			SIZE szWnd;
// // 			szWnd = { (INT)m_Parent->m_WndRect->Width, (INT)m_Parent->m_WndRect->Height };
// // 			POINT ptSrc = { 0,0 };
// // 			BLENDFUNCTION bf;
// // 			bf.AlphaFormat = AC_SRC_ALPHA;
// // 			bf.BlendFlags = 0;
// // 			bf.BlendOp = 0;
// // 			bf.SourceConstantAlpha = m_Parent->m_Alpha;
// // 			UpdateLayeredWindow(m_Parent->m_hWnd, NULL, NULL, &szWnd, m_Parent->m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
// // 		}
// // 		else
// // 		{
// // 			//m_Parent->m_MemDC->BitBlt(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), SRCCOPY);
// // 			m_Parent->m_MemDC->AlphaBlend(hDC, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
// // 		}
// // 
// // 		m_Parent->m_Graphics->ReleaseHDC(hDC);
// 
// 	}
// 	else
// 	{
// 		m_Parent->OnUpdate(m_ID, FALSE);
// 		/*
// 		m_Parent->m_MemDC->SelectRectClipRgn((INT)m_Rect->X, (INT)m_Rect->Y, (INT)m_Rect->Width, (INT)m_Rect->Height);
// 		m_Parent->m_BkgDC->AlphaBlend(m_Parent->m_MemDC);
// 
// 		m_MemDC->AlphaBlend(m_Parent->m_MemDC->GetMemDC(), (int)m_Rect->GetLeft(), (int)m_Rect->GetTop(), (int)m_Rect->Width, (int)m_Rect->Height, 0, 0, (int)m_Rect->Width, (int)m_Rect->Height, m_Alpha);
// 
// 		m_Parent->m_MemDC->SelectClipRgn();*/
// 	}

	return TRUE;
}

BOOL DUI_ControlBase::OnCalcRect(WPARAM wParam, LPARAM lParam)
{
	if (m_Rect == nullptr)
	{
		m_Rect = new RectF(0, 0, 0, 0);
	}
	if (m_VRect == nullptr)
	{
		m_VRect = new RectF(0, 0, 0, 0);
	}
	if (m_LogicVRect == nullptr)
	{
		m_LogicVRect = new RectF(0, 0, 0, 0);
	}
	m_Rect->Width = m_LogicRect->Width;
	m_Rect->Height = m_LogicRect->Height;

	if (m_Parent->GetObjType() == ObjType_Wnd)
	{
		m_Rect->X = m_LogicRect->X;
		m_Rect->Y = m_LogicRect->Y;

		RectF* cRect = m_Rect->Clone();
		m_VRect->X = m_Rect->X;
		m_VRect->Y = m_Rect->Y;
		m_VRect->Width = m_Rect->Width;
		m_VRect->Height = m_Rect->Height;

// 		if (!m_bCanShowOnNCRgn)
// 		{
// 			RectF* pcRect = m_Parent->GetClientRect();
// 			if (pcRect != nullptr)
// 			{
// 				if (!m_VRect->Intersect(*pcRect))
// 				{
// 					m_VRect->X = 0;
// 					m_VRect->Y = 0;
// 					m_VRect->Width = 0;
// 					m_VRect->Height = 0;
// 				}
// 				delete pcRect;
// 			}
// 		}

		m_LogicVRect->X = m_VRect->X;
		m_LogicVRect->Y = m_VRect->Y;
		m_LogicVRect->Width = m_VRect->Width;
		m_LogicVRect->Height = m_VRect->Height;
	}
	else
	{
		m_Rect->X = m_LogicRect->X + m_Parent->GetX();
		m_Rect->Y = m_LogicRect->Y + m_Parent->GetY();

		m_VRect->X = m_Rect->X;
		m_VRect->Y = m_Rect->Y;
		m_VRect->Width = m_Rect->Width;
		m_VRect->Height = m_Rect->Height;
// 		if (!m_bCanShowOnNCRgn)
// 		{
// 			RectF* pcRect = m_Parent->GetClientRect();
// 			if (pcRect != nullptr)
// 			{
// 				if (!m_VRect->Intersect(*pcRect))
// 				{
// 					m_VRect->X = 0;
// 					m_VRect->Y = 0;
// 					m_VRect->Width = 0;
// 					m_VRect->Height = 0;
// 				}
// 				delete pcRect;
// 			}
// 		}
		//TRACE("%d\n", m_VRect->IsEmptyArea());

		m_LogicVRect->X = m_VRect->X - m_Parent->GetX();
		m_LogicVRect->Y = m_VRect->Y - m_Parent->GetY();
		m_LogicVRect->Width = m_VRect->Width;
		m_LogicVRect->Height = m_VRect->Height;
	}
	return TRUE;
}
