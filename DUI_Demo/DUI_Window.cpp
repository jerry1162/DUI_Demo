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
	case WM_CREATE:
		m_pThis->DrawWnd();
		break;
	case WM_LBUTTONDOWN:
		m_pThis->OnLButtonDown(wParam, lParam);
		break;
	case WM_PAINT:
		m_pThis->OnPaint(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		m_pThis->OnMouseMove(wParam,lParam);
		break;
	case WM_SIZE:
		m_pThis->OnSize(wParam, lParam);
	case WM_UPDATE:
		m_pThis->DrawWnd();
		break;
	default:
		break;
	}

	return CallWindowProc(WNDPROC(PrevWndProc), hwnd, uMsg, wParam, lParam);
}

VOID DUI_Window::DrawWnd()
{
	//DrawShadow(m_MemDC->graphics, m_ClientRect, 5);
	SolidBrush Brush(*m_BackColor);
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
	//画边框结束

	if (m_Icon != NULL)
	{
		m_MemDC->graphics->DrawImage(m_Icon, 3, 3, m_Icon->GetWidth(), m_Icon->GetHeight());
	}

	Brush.SetColor(*m_Title->color);
	DrawShadowText(m_MemDC->graphics, 5, m_Title, Color::Black, Color::Black);
}

BOOL DUI_Window::Create(HWND hWnd, LPCWSTR Title, LPCWSTR Icon, LPCWSTR BackgrdPic,
	BOOL bSizeable)
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

	m_Title = new GdipString;
	m_Title->string = new CString;
	m_Title->string->SetString(Title);  ///////////////////////////

	FontFamily fm(L"新宋体");
	m_Title->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Title->format = new StringFormat;
	m_Title->format->GenericDefault();
	m_Title->color = new Color(Color::White);
	//TitleRect的处理在OnSize里;
	if (Icon != NULL)
	{
		m_Icon = new Image(Icon);
		/*HICON hIcon;
		hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MAIN));
		Bitmap* icon = new Bitmap(hIcon);
		icon->GetWidth();*/
		//ImageFromIDResource(IDI_ICON_MAIN, RT_GROUP_ICON, m_Icon);
	}
	m_BackColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	
	if (BackgrdPic != NULL)
	{
		m_BkgImg = new Image(BackgrdPic);
	}
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
	delete m_BkgImg;
	return TRUE;
}

BOOL DUI_Window::ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR); // type
	if (!hRsrc)
		return FALSE;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream
	pImg = new Image(pstm);
	// pImg = Gdiplus::Image::FromStream(pstm);
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
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
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 10;
	tme.hwndTrack = m_hWnd;
	TrackMouseEvent(&tme);

	//Point pt;
	//pt.X = LOWORD(lParam);
	//pt.Y = HIWORD(lParam);
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

	if (m_Icon != nullptr)
	{
		m_Title->rect = new RectF(m_ClientRect->GetLeft() + m_Icon->GetWidth() + 4,
			m_ClientRect->GetTop() + 5, m_ClientRect->Width - m_Icon->GetWidth() - 4, TITLEBARHEIGHT);
	}
	else
	{
		m_Title->rect = new RectF(m_ClientRect->GetLeft() + 2, m_ClientRect->GetTop() + 4
			, m_ClientRect->Width, TITLEBARHEIGHT);
	}
	SendMessage(m_hWnd, WM_UPDATE, NULL, NULL);
}

VOID DUI_Window::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);
	HDC hDC = m_Graphics->GetHDC();
	m_MemDC->BitBlt(hDC, 0, 0, m_WndRect->Width, m_WndRect->Height,
		0, 0, SRCCOPY);
	m_Graphics->ReleaseHDC(hDC);
	EndPaint(m_hWnd, &ps);
}
