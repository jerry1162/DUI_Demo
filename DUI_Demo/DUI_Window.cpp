#include "stdafx.h"
#include "DUI_Window.h"
#include "Resource.h"

DUI_Window* DUI_Window::m_pThis;
LONG DUI_Window::PrevWndProc;

DUI_Window::DUI_Window()
{
	m_pThis = this;
	PrevWndProc = NULL;
}

DUI_Window::DUI_Window(HWND hWnd)
{
	m_pThis =this;
	PrevWndProc = NULL;
	Create(hWnd);
}


DUI_Window::~DUI_Window()
{
	Destroy();
}

LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		//MessageBeep(MB_ICONINFORMATION);
		m_pThis->OnLButtonDown(wParam, lParam);
		break;
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hwnd, &ps);
		m_pThis->DrawWnd();
		EndPaint(hwnd, &ps);
	case WM_MOUSEMOVE:
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 10;
		tme.hwndTrack = hwnd;
		TrackMouseEvent(&tme);
		m_pThis->OnMouseMove(wParam,lParam);
	case WM_SIZE:
		m_pThis->OnSize(wParam, lParam);
	default:
		break;
	}

	return CallWindowProc(WNDPROC(PrevWndProc),hwnd,uMsg,wParam,lParam);
}

VOID DUI_Window::DrawWnd()
{
	//DrawShadow(m_MemDC->graphics, m_ClientRect, 5);
	SolidBrush Brush(*m_BackColor);
	if (m_Background != nullptr)
	{
		m_MemDC->graphics->DrawImage(m_Background, m_ClientRect->GetLeft(),
			m_ClientRect->GetTop(),m_ClientRect->Width,m_WndRect->Height);
	}
	else
	{
		m_MemDC->graphics->FillRectangle(&Brush, m_ClientRect->GetLeft(),
			m_ClientRect->GetTop(), m_ClientRect->Width, m_ClientRect->Height);
	}

	//»­±ß¿ò
	GraphicsPath* pPath;
	pPath = new GraphicsPath;
	DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 5);
	DrawPathRoundRect(pPath, 0, 0, m_ClientRect->Width - 2, m_ClientRect->Height - 2, 8);
	Color* color = new Color(Color::MakeARGB(150, 20, 20, 20));
	Pen pen(*color, 1);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete pPath;

	color->SetValue(Color::MakeARGB(150, 255, 255, 255));
	pen.SetColor(*color);
	pPath = new GraphicsPath;
	DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 3);
	DrawPathRoundRect(pPath, 1, 1, m_ClientRect->Width - 4, m_ClientRect->Height - 4, 6);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete color;
	delete pPath;
	//»­±ß¿ò½áÊø

	//m_MemDC->graphics->DrawImage(m_Icon, 1, 1, m_Icon->GetWidth(), m_Icon->GetHeight());

	Brush.SetColor(*m_Title->color);
	//LPWSTR str = L"UI DEMO";
	//m_MemDC->graphics->DrawString(m_Title->string->GetBuffer(), m_Title->string->GetLength(),
	//	m_Title->font, *m_Title->rect, m_Title->format, &Brush);
	//	
	DrawShadowText(m_MemDC->graphics, 5, m_Title, Color::Black, Color::Black);
	

	HDC hDC = m_Graphics->GetHDC();
	m_MemDC->BitBlt(hDC,0,0,m_WndRect->Width,m_WndRect->Height,0,0,SRCCOPY);
	m_Graphics->ReleaseHDC(hDC);
}

BOOL DUI_Window::Create(HWND hWnd, LPCWSTR Backgrd)
{
	if (hWnd == NULL)
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

	OnSize(NULL, NULL);

	m_Title = new GdipString;
	m_Title->string = new CString;
	m_Title->string->SetString(L"DirectUI");  ///////////////////////////
	//GetWindowText(hWnd, m_Title->string->, 255);


	FontFamily fm(L"ÐÂËÎÌå");
	m_Title->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Title->format = new StringFormat;
	m_Title->format->GenericDefault();
	m_Title->color = new Color(Color::White);
	m_BackColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	m_Title->rect = new RectF(m_ClientRect->GetLeft() + 20, m_ClientRect->GetTop() + 4
		, m_ClientRect->Width, TITLEBARHEIGHT);

	//ImageFromIDResource(IDI_ICON_MAIN, RT_GROUP_ICON,m_Icon);
	//ASSERT(m_Icon != nullptr);
	
	if (Backgrd != NULL)
	{
		m_Background = new Image(Backgrd);
	}

	PrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, LONG(DUI_Window::WndProc));
	if (PrevWndProc == NULL)
	{
		TRACE(L"×ÓÀà»¯Ê§°Ü");
		Destroy();
		return FALSE;
	}
	return TRUE;
}

BOOL DUI_Window::Destroy()
{
	delete m_MemDC;
	delete m_WndRect;
	delete m_ClientRect;

	delete m_Title->color;
	delete m_Title->font;
	delete m_Title->format;
	delete m_Title->string;

	delete m_Title;
	delete m_BackColor;

	delete m_Icon;
	delete m_Background;

	delete m_pThis;
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

VOID DUI_Window::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	Point pt;
	pt.X = LOWORD(lParam);
	pt.Y = HIWORD(lParam);
}

VOID DUI_Window::OnLButtonDown(WPARAM wParam, LPARAM lParam)
{
	Point pt;
	pt.X = LOWORD(lParam);
	pt.Y = HIWORD(lParam);

	if (PtInRect(m_Title->rect, &pt))
	{
		SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
	}
}

VOID DUI_Window::OnSize(WPARAM wParam, LPARAM lParam)
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
}
