#include "stdafx.h"
#include "DUI_Window.h"
#include "Resource.h"

DUI_Window* DUI_Window::m_pThis;
LONG DUI_Window::PrevWndProc;

DUI_Window::DUI_Window()
{
	m_pThis = this;
	PrevWndProc = NULL;
	m_CaptureControlIndex = NULL;
}

DUI_Window::~DUI_Window()
{
	Destroy();
}

LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Point ptMouse;
	int Res = 0;
	if (m_pThis->OnControl(uMsg, wParam, lParam)) return TRUE;
	switch (uMsg)
	{
	case WM_CREATE:
		m_pThis->DrawWnd();
		break;
	case WM_LBUTTONDOWN:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = m_pThis->OnLButtonDown(wParam, &ptMouse);
		break;
	case WM_LBUTTONUP:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = m_pThis->OnLButtonUp(wParam, &ptMouse);
		break;
	case WM_PAINT:
		Res = m_pThis->OnPaint(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = m_pThis->OnMouseMove(wParam,&ptMouse);
		break;
	case WM_SIZE:
		Res = m_pThis->OnSize(wParam, lParam);
	case WM_UPDATE:
		Res = m_pThis->OnUpdate((RectF*)wParam, lParam);
		break;
	case WM_SETCURSOR:
		Res = m_pThis->OnSetCursor(wParam, lParam);
		break;
	default:
		Res = 0;
		break;
	}
	if (Res)
	{
		return TRUE;
	}
	else
	{
		return CallWindowProc(WNDPROC(PrevWndProc), hwnd, uMsg, wParam, lParam);
	}
	
}

VOID DUI_Window::DrawWnd()
{
	//DrawShadow(m_MemDC->graphics, m_ClientRect, 5);
	SolidBrush Brush(*m_BkgColor);
	if (m_BkgImg != nullptr)
	{
		m_MemDC->graphics->DrawImage(m_BkgImg, m_ClientRect->GetLeft(),
			m_ClientRect->GetTop(),m_ClientRect->Width,m_WndRect->Height);
	}
	else
	{
		m_MemDC->graphics->FillRectangle(&Brush, m_ClientRect->GetLeft(),
			m_ClientRect->GetTop(), m_ClientRect->Width, m_ClientRect->Height);
	}

	//画边框
	GraphicsPath* pPath;
	pPath = new GraphicsPath;
	RectF BorderRect;
	BorderRect.X = 0;
	BorderRect.Y = 0;
	BorderRect.Width = m_ClientRect->Width - 1;
	BorderRect.Height = m_ClientRect->Height - 1;
	if (m_BorderStyle.Mode == BM_Normal)
	{
		pPath->AddRectangle(BorderRect);
	}
	else
	{
		DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 5);
		DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 8);
	}
	if (m_BorderStyle.Color == NULL)
	{
		m_BorderStyle.Color = Color::MakeARGB(150, 50, 50, 50);
	}
	Color* color = new Color(m_BorderStyle.Color);
	Pen pen(*color, 1);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete pPath;

	if (m_BorderStyle.DoubleBorder)
	{
		color->SetValue(Color::MakeARGB(color->GetA(), 255 - color->GetR(), 255 - color->GetG(), 255 - color->GetB()));//Color::MakeARGB(150, 200, 200, 200)
		pen.SetColor(*color);
		pPath = new GraphicsPath;
		BorderRect.X = 1;
		BorderRect.Y = 1;
		BorderRect.Width = m_ClientRect->Width - 3;
		BorderRect.Height = m_ClientRect->Height - 3;
		if (m_BorderStyle.Mode == BM_Normal)
		{
			pPath->AddRectangle(BorderRect);
		}
		else
		{
			DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 3);
			DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 6);
		}

		m_MemDC->graphics->DrawPath(&pen, pPath);
		delete pPath;
		//画边框结束
	}
	delete color;


	if (m_Icon != NULL)
	{
		m_MemDC->graphics->DrawImage(m_Icon, 3, 3, m_Icon->GetWidth(), m_Icon->GetHeight());
	}

	Brush.SetColor(*m_Title->color);
	DrawShadowText(m_MemDC->graphics, 5, m_Title, Color::Black,
		Color::MakeARGB(100, 50, 50, 50));
}

BOOL DUI_Window::InitDUIWnd(HWND hWnd, LPCWSTR Title, BOOL bSizable)
{
	if (m_hWnd != NULL)
	{
		return FALSE;
	}
	m_hWnd = hWnd;
	m_Sizeable = bSizable;
	m_Graphics = new Gdiplus::Graphics(hWnd);
	if (m_Graphics == NULL)
	{
		delete m_Graphics;
		return FALSE;
	}
	//设置默认的边框风格
	m_BorderStyle.Mode = BM_RoundRect;
	m_BorderStyle.Color = Color::MakeARGB(150, 50, 50, 50);
	m_BorderStyle.DoubleBorder = TRUE;

	//处理标题结构
	m_Title = new GdipString;
	m_Title->string = new CString;
	m_Title->string->SetString(Title);  ///////////////////////////

	FontFamily fm(L"新宋体");
	m_Title->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Title->format = new StringFormat;
	m_Title->format->GenericDefault();
	m_Title->color = new Color(Color::White);
	//TitleRect的处理在OnSize里;

	if (m_BkgColor == NULL)
	{
		m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	}
	m_Controls = new vector<ControlBase*>;
	PrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, LONG(DUI_Window::WndProc));
	if (PrevWndProc == NULL)
	{
		TRACE(L"子类化失败");
		Destroy();
		return FALSE;
	}
	OnSize(NULL, NULL);
	return TRUE;
}

BOOL DUI_Window::Create(HWND hWnd, LPCWSTR Title, LPCWSTR Icon, ARGB BkgColor,
	BOOL bSizeable)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}

	if (Icon != NULL)
	{
		m_Icon = new Image(Icon);
	}

	m_BkgColor = new Color(BkgColor);
	return InitDUIWnd(hWnd, Title,bSizeable);
}

BOOL DUI_Window::Create(HWND hWnd, LPCWSTR Title, LPCWSTR Icon, LPCWSTR BackgrdPic,
	BOOL bSizeable)
{

	if (hWnd == NULL)
	{
		return FALSE;
	}

	if (Icon != NULL)
	{
		m_Icon = new Image(Icon);
	}

	if (BackgrdPic != NULL)
	{
		m_BkgImg = new Image(BackgrdPic);
	}
	else
	{
		m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	}
	return InitDUIWnd(hWnd, Title,bSizeable);
}

BOOL DUI_Window::Destroy()
{
	SetWindowLong(m_hWnd, GWL_WNDPROC, PrevWndProc);
	delete m_MemDC;
	delete m_WndRect;
	delete m_ClientRect;

	delete m_Title->color;
	delete m_Title->font;
	delete m_Title->format;
	delete m_Title->string;

	delete m_Title;
	delete m_BkgColor;

	delete m_Icon;
	delete m_BkgImg;
	m_pThis = nullptr;
	m_hWnd = NULL;
	return TRUE;
}

BOOL DUI_Window::SetBkgPic(LPCWSTR BackgrdPic)
{
	if (BackgrdPic == NULL || m_hWnd == NULL)
	{
		return FALSE;
	}
	delete m_BkgImg;
	m_BkgImg = new Image(BackgrdPic);
	OnUpdate(NULL, TRUE);
	return TRUE;
}

BOOL DUI_Window::SetBkgColor(ARGB BackgrdColor)
{
	if (BackgrdColor == NULL)
	{
		return FALSE;
	}
	delete m_BkgImg;
	m_BkgImg = NULL;
	delete m_BkgColor;
	m_BkgColor = new Color(BackgrdColor);
	OnUpdate(NULL, TRUE);
	return 0;
}

BOOL DUI_Window::SetTitle(LPCWSTR Title)
{
	m_Title->string->SetString(Title);
	OnUpdate(m_Title->rect, TRUE);
	return TRUE;
}

BOOL DUI_Window::SetBorderStyle(BorderStyle bs)
{
	if (m_BorderStyle == bs)
	{
		return TRUE;
	}
	m_BorderStyle = bs;
	OnSize(NULL, NULL);//OnUpdate(NULL, TRUE);
	return TRUE;
}

BOOL DUI_Window::SetSizeable(BOOL bSizeable)
{
	if (bSizeable!=m_Sizeable)
	{
		m_Sizeable = bSizeable;
	}
	return TRUE;
}

BOOL DUI_Window::GetSizeable()
{
	return m_Sizeable;
}

INT DUI_Window::FindControlByID(INT ID)
{
	for (vector<ControlBase*>::iterator it = m_Controls->begin(); it != m_Controls->end(); ++it)
	{
		if ((*it)->m_bVisialbe == FALSE) continue;
		if ((*it)->m_ID == ID)
		{
			return it - m_Controls->begin();
		}
	}
	return -1;
}

INT DUI_Window::ScreenToClient(Point * pt)
{
	POINT p;
	p = { pt->X ,pt->Y };
	int ret = ::ScreenToClient(m_hWnd, &p);
	pt->X = p.x;
	pt->Y = p.y;
	return 0;
}

BOOL DUI_Window::GetCursorPos(Point * pt)
{
	BOOL ret = FALSE;
	POINT ptTemp;
	ret = ::GetCursorPos(&ptTemp);
	pt->X = ptTemp.x;
	pt->Y = ptTemp.y;
	ScreenToClient(pt);
	return ret;
}

BOOL DUI_Window::OnMouseMove(WPARAM wParam, Point* ptMouse)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 10;
	tme.hwndTrack = m_hWnd;
	TrackMouseEvent(&tme);

	static INT LastHoverID = INVALID_CONTROLID;
	BOOL bFound = FALSE;
	GetCursorPos(ptMouse);
	for (vector<ControlBase*>::iterator it = m_Controls->begin(); it != m_Controls->end(); ++it)
	{
		if ((*it)->m_bVisialbe == FALSE) continue;
		if (PtInRect((*it)->m_Rect, ptMouse))
		{
			bFound = TRUE;
			if (LastHoverID != (*it)->m_ID)
			{
				if (LastHoverID != INVALID_CONTROLID)
				{
					m_Controls->at(FindControlByID(LastHoverID))->MsgProc(WM_MOUSELEAVE, wParam, NULL);
				}
				(*it)->MsgProc(WM_MOUSEGETIN, NULL, NULL);
				LastHoverID = (*it)->m_ID;
			}
			
			break;
		}
	}
	if (!bFound)
	{
		if (LastHoverID != INVALID_CONTROLID)
		{
			m_Controls->at(FindControlByID(LastHoverID))->MsgProc(WM_MOUSELEAVE, wParam, NULL);
			LastHoverID = INVALID_CONTROLID;
		}
		
	}
	return TRUE;
}

BOOL DUI_Window::OnLButtonDown(WPARAM wParam, Point* ptMouse)
{
	if (PtInRect(m_Title->rect, ptMouse) && m_CurCDR == CDR_Normal) //标题栏移动
	{
		SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
	else if (m_Sizeable && m_CurCDR != CDR_Normal)
	{
		switch (m_CurCDR)
		{
		case CDR_NWSE:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, NULL);
			break;
		case CDR_NS:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, NULL);
			break;
		case CDR_NESW:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, NULL);
			break;
		case CDR_WE:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, NULL);
			break;
		case CDR_NWSE2:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, NULL);
			break;
		case CDR_NS2:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, NULL);
			break;
		case CDR_NESW2:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, NULL);
			break;
		case CDR_WE2:
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, NULL);
			break;
		}
	}
	return TRUE;
}

BOOL DUI_Window::OnLButtonUp(WPARAM wParam, Point * ptMouse)
{
	return TRUE;
}

BOOL DUI_Window::OnSize(WPARAM wParam, LPARAM lParam)
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	m_WndRect = new Gdiplus::RectF(0, 0, rect.right - rect.left,
		rect.bottom - rect.top);
	if (m_WndRect->Height < TITLEBARHEIGHT)
	{
		m_WndRect->Height = TITLEBARHEIGHT;
		SetWindowPos(m_hWnd, NULL, 0, 0, m_WndRect->Width, m_WndRect->Height,
			SWP_NOZORDER | SWP_NOMOVE);
	}
	m_ClientRect = new Gdiplus::RectF(SHADOWWIDTH, SHADOWWIDTH,
		rect.right - rect.left - 2 * SHADOWWIDTH, rect.bottom - rect.top - 2 * SHADOWWIDTH);
	if (m_MemDC==nullptr)
	{
		m_MemDC = new MemDC;
		m_MemDC->Create(m_WndRect->Width, m_WndRect->Height);
	}
	else
	{
		m_MemDC->ReSize(m_WndRect->Width, m_WndRect->Height);
	}

	HRGN hRgn;
	if (m_BorderStyle.Mode == BM_RoundRect)
	{
		hRgn = CreateRoundRectRgn(0, 0, m_WndRect->Width, m_WndRect->Height, 5, 5);
	}
	else
	{
		hRgn = CreateRectRgn(0, 0, m_WndRect->Width, m_WndRect->Height);
	}
	SetWindowRgn(m_hWnd, hRgn, FALSE);
	DeleteObject(hRgn);


	if (m_Icon != nullptr)
	{
		m_Title->rect = new RectF(m_ClientRect->GetLeft() + m_Icon->GetWidth() + 8,
			m_ClientRect->GetTop() + 5, m_ClientRect->Width - m_Icon->GetWidth() - 4, TITLEBARHEIGHT);
	}
	else
	{
		m_Title->rect = new RectF(m_ClientRect->GetLeft() + 2, m_ClientRect->GetTop() + 4
			, m_ClientRect->Width, TITLEBARHEIGHT);
	}
	OnUpdate();
	return TRUE;
}

BOOL DUI_Window::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);
	HDC hDC = m_Graphics->GetHDC();
	m_MemDC->BitBlt(hDC, 0, 0, m_WndRect->Width, m_WndRect->Height,
		0, 0, SRCCOPY);
	m_Graphics->ReleaseHDC(hDC);
	EndPaint(m_hWnd, &ps);
	return TRUE;
}

BOOL DUI_Window::OnUpdate(RectF* Rect, BOOL bUpdate)
{
	DrawWnd();
	if (bUpdate == TRUE)
	{
		HDC hDC = m_Graphics->GetHDC();
		if (Rect != NULL)
		{
			m_MemDC->BitBlt(hDC, Rect->GetLeft(), Rect->GetTop(), Rect->Width, Rect->Height,Rect->GetLeft(), Rect->GetTop(), SRCCOPY);
		} 
		else
		{
			OnControl(WM_UPDATE, NULL, FALSE);
			m_MemDC->BitBlt(hDC, 0, 0, m_WndRect->Width, m_WndRect->Height,
				0, 0, SRCCOPY);
		}
		m_Graphics->ReleaseHDC(hDC);
	}
	return TRUE;
}

BOOL DUI_Window::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	if (m_Sizeable&&m_CaptureControlIndex == NULL)
	{
		//ChangeCursor();
		Point ptMouse;
		GetCursorPos(&ptMouse);
		if (ptMouse.X <= DEF_BORDERWHIDTH && ptMouse.Y <= DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			m_CurCDR = CDR_NWSE;
		}
		else if (ptMouse.X > DEF_BORDERWHIDTH && ptMouse.X < m_ClientRect->Width - DEF_BORDERWHIDTH && ptMouse.Y <= DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			m_CurCDR = CDR_NS;
		}
		else if (ptMouse.X > m_ClientRect->Width - DEF_BORDERWHIDTH && ptMouse.Y <= DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			m_CurCDR = CDR_NESW;
		}
		else if (ptMouse.X <= DEF_BORDERWHIDTH && ptMouse.Y > DEF_BORDERWHIDTH && ptMouse.Y <= m_ClientRect->Height - DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			m_CurCDR = CDR_WE;
		}
		else if (ptMouse.X <= DEF_BORDERWHIDTH && ptMouse.Y > m_ClientRect->Height - DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENESW));
			m_CurCDR = CDR_NESW2;
		}
		else if (ptMouse.X > DEF_BORDERWHIDTH && ptMouse.X <= m_ClientRect->Width - DEF_BORDERWHIDTH && ptMouse.Y > m_ClientRect->Height - DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENS));
			m_CurCDR = CDR_NS2;
		}
		else if (ptMouse.X > m_ClientRect->Width - DEF_BORDERWHIDTH && ptMouse.Y > DEF_BORDERWHIDTH && ptMouse.Y <= m_ClientRect->Height - DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZEWE));
			m_CurCDR = CDR_WE2;
		}
		else if (ptMouse.X > m_ClientRect->Width - DEF_BORDERWHIDTH && ptMouse.Y > m_ClientRect->Height - DEF_BORDERWHIDTH)
		{
			SetCursor(LoadCursor(NULL, IDC_SIZENWSE));
			m_CurCDR = CDR_NWSE2;
		}
		else
		{
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			m_CurCDR = CDR_Normal;
		}
		return 1;
	}
	else
	{
		m_CurCDR = CDR_Normal;
		return 0;
	}
	
}

BOOL DUI_Window::OnControl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_Controls->size() == 0)
	{
		return FALSE;
	}
	if (uMsg == WM_UPDATE)
	{
		INT ID = FindControlByID((INT)wParam);
		if (ID != INVALID_CONTROLID)
		{
			m_Controls->at(ID)->MsgProc(uMsg, wParam, lParam);
		}
		
	}
	Point ptMouse;
	GetCursorPos(&ptMouse);
	if (m_CaptureControlIndex != NULL)
	{
		if (IsMouseMsg(uMsg))
		{
			lParam = MAKELPARAM(ptMouse.X - m_Controls->at(m_CaptureControlIndex)->m_Rect->GetLeft(),
				ptMouse.Y - m_Controls->at(m_CaptureControlIndex)->m_Rect->GetTop());
		}
		return m_Controls->at(m_CaptureControlIndex)->MsgProc(uMsg, wParam, lParam);
	}
	for (vector<ControlBase*>::iterator it = m_Controls->begin();it != m_Controls->end(); ++it)
	{
		if ((*it)->m_bVisialbe == FALSE) continue;
		if (IsMouseMsg(uMsg) && PtInRect((*it)->m_Rect, &ptMouse))
		{
			lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),
			ptMouse.Y - (*it)->m_Rect->GetTop());
			return (*it)->MsgProc(uMsg, wParam, lParam);
		}
		else if(uMsg==WM_UPDATE)
		{
			lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),
				ptMouse.Y - (*it)->m_Rect->GetTop());
			return (*it)->MsgProc(uMsg, wParam, lParam);
		}
	}
	return FALSE;
}