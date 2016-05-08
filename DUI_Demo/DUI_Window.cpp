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
	if (m_pThis->OnControl(uMsg, wParam, lParam)) return TRUE;
	switch (uMsg)
	{
	case WM_CREATE:
		m_pThis->DrawWnd();
		break;
	case WM_LBUTTONDOWN:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		m_pThis->OnLButtonDown(wParam, &ptMouse);
		break;
	case WM_LBUTTONUP:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		m_pThis->OnLButtonUp(wParam, &ptMouse);
		break;
	case WM_PAINT:
		m_pThis->OnPaint(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		m_pThis->OnMouseMove(wParam,&ptMouse);
		break;
	case WM_SIZE:
		m_pThis->OnSize(wParam, lParam);
	case WM_UPDATE:
		m_pThis->OnUpdate((RectF*)wParam, lParam);
		break;
	default:
		break;
	}

	return CallWindowProc(WNDPROC(PrevWndProc), hwnd, uMsg, wParam, lParam);
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
	DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 5);
	DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 8);
	Color* color = new Color(Color::MakeARGB(150, 50, 50, 50));
	Pen pen(*color, 1);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete pPath;

	color->SetValue(Color::MakeARGB(150, 200, 200, 200));
	pen.SetColor(*color);
	pPath = new GraphicsPath;
	DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 3);
	DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 6);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete color;
	delete pPath;
	//画边框结束

	if (m_Icon != NULL)
	{
		m_MemDC->graphics->DrawImage(m_Icon, 3, 3, m_Icon->GetWidth(), m_Icon->GetHeight());
	}

	Brush.SetColor(*m_Title->color);
	DrawShadowText(m_MemDC->graphics, 5, m_Title, Color::Black,
		Color::MakeARGB(100, 50, 50, 50));
}

BOOL DUI_Window::InitDUIWnd(HWND hWnd, LPCWSTR Title)
{
	if (m_hWnd != NULL)
	{
		return FALSE;
	}
	m_hWnd = hWnd;
	m_Graphics = new Gdiplus::Graphics(hWnd);
	if (m_Graphics == NULL)
	{
		delete m_Graphics;
		return FALSE;
	}

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
	return InitDUIWnd(hWnd, Title);
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


	return InitDUIWnd(hWnd, Title);
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
	return TRUE;
}

BOOL DUI_Window::OnLButtonDown(WPARAM wParam, Point* ptMouse)
{
	if (PtInRect(m_Title->rect, ptMouse)) //标题栏移动
	{
		SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
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
	HRGN hRgn = CreateRoundRectRgn(0, 0, m_WndRect->Width, m_WndRect->Height, 5, 5);
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
			m_MemDC->BitBlt(hDC, Rect->GetLeft(), Rect->GetTop(), Rect->Width, Rect->Height,
				Rect->GetLeft(), Rect->GetTop(), SRCCOPY);
		} 
		else
		{
			m_MemDC->BitBlt(hDC, 0, 0, m_WndRect->Width, m_WndRect->Height,
				0, 0, SRCCOPY);
		}
		m_Graphics->ReleaseHDC(hDC);
	}
	return TRUE;
}

BOOL DUI_Window::OnControl(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_Controls->size() == 0)
	{
		return FALSE;
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
	}
	return FALSE;
}