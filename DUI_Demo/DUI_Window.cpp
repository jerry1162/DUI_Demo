#include "stdafx.h"
#include "DUI_Window.h"
#include "Resource.h"
#include "DUI_ImageButton.h"
//class DUI_ControlBase;
//class DUI_ControlBase;
//class DUI_ImageButton;
//DUI_Window* DUI_Window::m_pThis;
//LONG DUI_Window::PrevWndProc;

//template <typename T>//产生一个代理函数
//LPVOID  GetCallBackAddr(LPVOID pThis, T MethodAddr)
//{
//	const unsigned char BlockCode[] = {
//		0x8B, 0x44, 0x24, 0x10,			//	mov         eax,dword ptr [esp+10h]
//		0x8B, 0x4C, 0x24, 0x0C,			//	mov         ecx,dword ptr [esp+0Ch]
//		0x8B, 0x54, 0x24, 0x08,			//	mov         edx,dword ptr [esp+8]
//		0x50,							//	push        eax
//		0x8B, 0x44, 0x24, 0x08,			//	mov         eax,dword ptr [esp+8]
//		0x51,							//	push        ecx
//		0xB9, 0x00, 0x00, 0x00, 0x00,	//	mov         ecx,0 （类的this指针）
//		0x52,							//	push        edx
//		0x50,							//	push        eax
//		0x51,							//	push		ecx
//		0xE8, 0x00,0x00,0x00,0x00,	//	call        CWndProc::WndProc
//		0xC2, 0x10, 0x00				//	ret         10h
//	};
//
//	size_t CodeBytes = sizeof(BlockCode);
//	LPVOID  Block = VirtualAlloc(nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
//	memcpy(Block, BlockCode, CodeBytes);
//	unsigned char * bBlock = (unsigned char *)Block;
//	*PLONG32(&bBlock[19]) = LONG32(pThis);
//	unsigned char* p = bBlock + 27;
//	__asm
//	{
//		mov eax, MethodAddr
//		sub eax, 4
//		mov edi, p
//		sub eax, edi
//		mov[edi], eax
//	}
//	return (LPVOID)Block;
//}
//
////释放代理函数
//void FreeCallBackAddr(LPVOID wndProc)
//{
//	VirtualFree(wndProc, 4096, MEM_RELEASE);
//}

DUI_Window::DUI_Window()
{

	m_hWnd = NULL;
	m_Parent = nullptr;
	m_MemDC = nullptr;
	m_Graphics = nullptr;
	m_WndRect = nullptr;
	m_ClientRect = nullptr;

	m_Title = nullptr;
	m_BkgColor = nullptr;
	m_Icon = nullptr;
	m_IconRect = nullptr;
	m_BkgImg = nullptr;
	m_SysBtnPic = nullptr;
	m_Sizeable = FALSE;
	m_bMouseDown = FALSE;
	m_LastHoverCtrlID = INVALID_CONTROLID;

	m_Controls = nullptr;
	m_CaptureCtrlID = INVALID_CONTROLID;
	m_FocusCtrlID = INVALID_CONTROLID;
	m_CurCDR = CDR_Normal;
	
	PrevWndProc = NULL;
	m_WndProc = (WNDPROC)GetCallBackAddr(this, &DUI_Window::WndProc);
	m_SysBtnClickProcAddr = (CLICKPROC)GetCallBackAddr(this, &DUI_Window::SysBtnClick);
	m_Alpha = 0;//(SHADOWWIDTH == 0 ? 0 : 255);
	m_bDebug = FALSE;
	m_bAutoUpdate = TRUE;

	for (int i = 0; i < BT_None; i++)
	{
		m_SysBtn[i] = nullptr;
	}
	m_SysBtnCnt = 0;
	m_Prompt = nullptr;
}

DUI_Window::~DUI_Window()
{
	Destroy();
}

LRESULT DUI_Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Point ptMouse;
	int Res = 0;
	if (IsWindow(hwnd))
	{
		if (OnControl(INVALID_CONTROLID, uMsg, wParam, lParam)) return TRUE;
	}
	else
	{
		return OnControl((INT)hwnd, uMsg, wParam, lParam);
	}
	
	switch (uMsg)
	{
	case WM_CREATE:
		/*m_pThis->*/DrawWnd();
		break;
	case WM_LBUTTONDOWN:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = /*m_pThis->*/OnLButtonDown(wParam, &ptMouse);
		break;
	case WM_LBUTTONUP:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = /*m_pThis->*/OnLButtonUp(wParam, &ptMouse);
		break;
	case WM_PAINT:
		Res = /*m_pThis->*/OnPaint(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = /*m_pThis->*/OnMouseMove(wParam,&ptMouse);
		break;
	case WM_SIZE:
		Res = /*m_pThis->*/OnSize(wParam, lParam);
	case WM_UPDATE:
		Res = /*m_pThis->*/OnUpdate(wParam, lParam);
		break;
	case WM_SETCURSOR:
		Res = /*m_pThis->*/OnSetCursor(wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = /*m_pThis->*/OnMouseLeave(wParam, &ptMouse);
		break;
	case WM_SETTEXT:
		SetTitle((LPTSTR)lParam, FALSE);
		break;
	case WM_ERASEBKGND:
		Res = 1;
		break;
	case WM_GETMINMAXINFO:
		Res = OnGetMinMaxInfo(wParam, lParam);
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
	m_MemDC->Clear();
	if (SHADOWWIDTH != 0)
	{
		m_MemDC->graphics->Clear(Color::Transparent);
		DrawShadow(m_MemDC->graphics, m_ClientRect, 5);
	}
	SolidBrush Brush(*m_BkgColor);
	if (m_BkgImg != nullptr)
	{
		m_MemDC->graphics->DrawImage(m_BkgImg, m_WndRect->GetLeft() + SHADOWWIDTH,
			m_WndRect->GetTop() + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH*2, m_WndRect->Height - SHADOWWIDTH*2);
	}
	else
	{
		m_MemDC->graphics->FillRectangle(&Brush, m_WndRect->GetLeft() + SHADOWWIDTH, m_WndRect->GetTop() + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH*2, m_WndRect->Height - SHADOWWIDTH*2);
	}

	//画边框
	GraphicsPath* pPath;
	pPath = new GraphicsPath;
	RectF BorderRect;
	BorderRect.X = m_WndRect->GetLeft() + SHADOWWIDTH;
	BorderRect.Y = m_WndRect->GetTop() + SHADOWWIDTH;
	BorderRect.Width = m_WndRect->Width - SHADOWWIDTH * 2 - 1;
	BorderRect.Height = m_WndRect->Height - SHADOWWIDTH * 2 - 1;
	if (m_BorderStyle.Mode == BM_Normal)
	{
		pPath->AddRectangle(BorderRect);
	}
	else
	{
		DrawPathRoundRect(pPath, BorderRect.GetLeft(), BorderRect.GetTop(), BorderRect.Width - 1, BorderRect.Height - 1, 5);
		DrawPathRoundRect(pPath, BorderRect.GetLeft(), BorderRect.GetTop(), BorderRect.Width - 1, BorderRect.Height - 1, 8);
	}
	if (m_BorderStyle.Color == NULL)
	{
		m_BorderStyle.Color = Color::MakeARGB(150, 0, 0, 0);
	}
	Color* color = new Color(m_BorderStyle.Color);
	Pen pen(*color, 1);
	m_MemDC->graphics->DrawPath(&pen, pPath);
	delete pPath;

	if (m_BorderStyle.DoubleBorder)
	{
		color->SetValue(Color::MakeARGB(255 - color->GetA(), 255 - color->GetR(), 255 - color->GetG(), 255 - color->GetB()));//Color::MakeARGB(150, 200, 200, 200)
		pen.SetColor(*color);
		pPath = new GraphicsPath;
		BorderRect.X = BorderRect.X + 1;
		BorderRect.Y = BorderRect.Y + 1;
		BorderRect.Width = BorderRect.Width - 3;
		BorderRect.Height = BorderRect.Height - 3;
		if (m_BorderStyle.Mode == BM_Normal)
		{
			pPath->AddRectangle(BorderRect);
		}
		else
		{
			DrawPathRoundRect(pPath, BorderRect.X, BorderRect.Y, BorderRect.Width, BorderRect.Height, 3);
			DrawPathRoundRect(pPath, BorderRect.X, BorderRect.Y, BorderRect.Width, BorderRect.Height, 6);
		}

		m_MemDC->graphics->DrawPath(&pen, pPath);
		delete pPath;
		//画边框结束
	}
	delete color;

	//m_MemDC->graphics->DrawImage(m_SysBtn, (INT)0, 0);
	//m_MemDC->graphics->DrawImage(m_SysBtnPic, *m_SysBtn_Close.rect, (REAL)((m_SysBtn_Close.status - 1)*SYSBTN_X), (REAL)0, (REAL)(m_SysBtn_Close.rect->Width), (REAL)(m_SysBtn_Close.rect->Height), UnitPixel);


	if (m_Icon != NULL)
	{
		m_MemDC->graphics->DrawImage(m_Icon, *m_IconRect);
	}

	Brush.SetColor(*m_Title->color);
	DrawShadowText(m_MemDC->graphics, 5, m_Title, Color::Black,Color::MakeARGB(100, 50, 50, 50));

	//调试模式，把所有矩形的边框画出来
	if (m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(200, 100, 100, 100), 1);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_WndRect);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_ClientRect);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_Title->rect);
		m_MemDC->graphics->DrawRectangle(&BorderPen, *m_IconRect);
		//m_MemDC->graphics->DrawRectangle(&BorderPen, *m_SysBtn_Close.rect);
	}
}

BOOL DUI_Window::InitDUIWnd(HWND hWnd, LPTSTR Title, BOOL bSizable)
{
	if (m_hWnd != NULL)
	{
		return FALSE;
	}
	m_hWnd = hWnd;
	m_Sizeable = bSizable;
	//m_WndRect = new RectF;
	m_Graphics = new Gdiplus::Graphics(hWnd);
	//m_MemDC = new MemDC;
	if (m_Graphics == NULL)
	{
		
		return FALSE;
	}
	//设置默认的边框风格
	m_BorderStyle.Mode = BM_RoundRect;
	m_BorderStyle.Color = Color::MakeARGB(150, 50, 50, 50);
	m_BorderStyle.DoubleBorder = TRUE;

	//处理标题结构
	m_Title = new GdipString;
	m_Title->string = new CString;
	//SetTitle(Title);
	//LPWSTR str = new WCHAR[255];
	//GetWindowText(m_hWnd, str, 255);
	//m_Title->string->SetString(Title);  ///////////////////////////

	FontFamily fm(L"新宋体");
	m_Title->font = new Gdiplus::Font(&fm, 10, FontStyleRegular, UnitPoint);
	m_Title->format = new StringFormat;
	m_Title->format->GenericDefault();
	m_Title->color = new Color(Color::White);
	m_Title->rect = new RectF;
	//TitleRect的处理在OnSize里;

	//初始化控制按钮信息
	//m_LastBtn = BT_None;
	m_SysBtnPic = ImageFromIDResource(IDB_SYSBTN_3, _T("PNG"));
	//ASSERT(m_SysBtnPic != NULL);
	//m_SysBtn_Close.rect = new RectF;//在OnSize中处理

	if (m_BkgColor == NULL)
	{
		m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	}

	if (m_Alpha != 0)
	{
		LONG style = GetWindowLong(m_hWnd, GWL_EXSTYLE);
		style |= WS_EX_LAYERED;
		SetWindowLong(m_hWnd, GWL_EXSTYLE, style);
	}
	m_Prompt = new DUI_Prompt;

	PrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, (LONG)m_WndProc);
	if (PrevWndProc == NULL)
	{
		//TRACE(L"子类化失败");
		Destroy();
		return FALSE;
	}
	m_Controls = new vector<DUI_ControlBase*>;
	OnSize(NULL, NULL);

	SetTitle(Title);
	return TRUE;
}

BOOL DUI_Window::Create(HWND hWnd, LPTSTR Title, LPTSTR Icon, ARGB BkgColor,
	BOOL bSizeable)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}

	if (Icon != NULL)
	{
		m_Icon = new Image(Icon);
		//m_IconRect = new RectF;
		//IconRect的处理在OnSize中
	}

	m_BkgColor = new Color(BkgColor);
	return InitDUIWnd(hWnd, Title,bSizeable);
}

BOOL DUI_Window::Create(INT Width, INT Height, DUI_Window* Parent, LPTSTR Title, LPTSTR Icon)
{
	if (Icon != NULL)
	{
		m_Icon = (Bitmap*)ImageFromIDResource(IDI_ICON_MAIN, RT_ICON);
	}
	m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	m_Parent = Parent;
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindowW((LPCWSTR)GetDefaultWndClass(), Title, WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP,
		(cx - Width) / 2, (cy - Height) / 2, Width, Height, m_Parent->m_hWnd, nullptr, GetModuleHandle(nullptr), nullptr);
	if (hWnd == NULL)
	{
		return FALSE;
	}
	ShowWindow(hWnd, SW_SHOW); 
	UpdateWindow(hWnd);
	//return InitDUIWnd(hWnd, Title, NULL);
	return Create(hWnd, Title, L"", L"", TRUE);
}

BOOL DUI_Window::Create(HWND hWnd, LPTSTR Title, LPTSTR Icon, LPTSTR BackgrdPic,
	BOOL bSizeable)
{

	if (hWnd == NULL)
	{
		return FALSE;
	}

	if (Icon != NULL)
	{
		//m_Icon = new Image(Icon);
		m_Icon = ImageFromIDResource(IDI_ICON_MAIN, RT_ICON);
		//ASSERT(m_Icon != NULL);
		//m_IconRect = new RectF;
		//IconRect的处理在OnSize中
	}

	if (BackgrdPic != NULL)
	{
		//m_BkgImg = new Image(BackgrdPic);
		m_BkgImg = ImageFromIDResource(IDB_BKGPIC, _T("JPG"));
		//ASSERT(m_BkgImg != NULL);
	}
	else
	{
		m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	}
	return InitDUIWnd(hWnd, Title,bSizeable);
}

VOID DUI_Window::DoModel()
{
	if (m_hWnd != NULL)
	{
		if (m_Parent != nullptr)
		{
			EnableWindow(m_Parent->m_hWnd, FALSE);
		}
		MessageLoop();
		if (m_Parent != nullptr)
		{
			EnableWindow(m_Parent->m_hWnd, TRUE);
			SetForegroundWindow(m_Parent->m_hWnd);
		}
	}
}

BOOL DUI_Window::Destroy()
{
	if (m_hWnd != NULL)
	{
		SetWindowLong(m_hWnd, GWL_WNDPROC, PrevWndProc);
		FreeCallBackAddr(m_WndProc);
		FreeCallBackAddr(m_SysBtnClickProcAddr);
		delete m_MemDC;
		delete m_WndRect;
		delete m_ClientRect;

		delete m_Title->color;
		delete m_Title->rect;
		delete m_Title->font;
		delete m_Title->format;
		delete m_Title->string;

		delete m_Title;
		delete m_BkgColor;

		delete m_Icon;
		delete m_BkgImg;
		delete m_Prompt;
		m_hWnd = NULL;
		return TRUE;
	}
	return FALSE;
}

BOOL DUI_Window::SetBkgPic(LPTSTR BackgrdPic)
{
	if (BackgrdPic == NULL || m_hWnd == NULL)
	{
		return FALSE;
	}
	delete m_BkgImg;
	m_BkgImg = new Image(BackgrdPic);
	Update();
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
	Update();
	return 0;
}

BOOL DUI_Window::SetTitle(LPTSTR Title, BOOL bInner)
{
	//LPWSTR str = new WCHAR[255];
	//GetWindowText(m_hWnd, str, 255);
	m_Title->string->SetString(Title);
	if (bInner)
	{
		SendMessage(m_hWnd, WM_SETTEXT, NULL, (LPARAM)Title);
	}
	Update();
	return TRUE;
}

BOOL DUI_Window::SetBorderStyle(BorderStyle bs)
{
	if (m_BorderStyle == bs)
	{
		return TRUE;
	}
	m_BorderStyle = bs;
	Update();
	return TRUE;
}

BOOL DUI_Window::SetSizeable(BOOL bSizeable)
{
	if (bSizeable != m_Sizeable)
	{
		m_Sizeable = bSizeable;
		OnSize(NULL, NULL);
	}
	return TRUE;
}

BOOL DUI_Window::GetSizeable()
{
	return m_Sizeable;
}

DUI_ControlBase* DUI_Window::FindControlByID(INT ID, _Out_ INT* Index)
{
	if (ID == INVALID_CONTROLID)
	{
		return nullptr;
	}
	for (vector<DUI_ControlBase*>::iterator it = m_Controls->begin(); it != m_Controls->end(); ++it)
	{
		//if ((*it)->m_bVisialbe == FALSE) continue;
		if ((*it)->m_ID == ID)
		{
			if (Index != nullptr)
			{
				*Index = it - m_Controls->begin();
			}
			return *it;
		}
	}
	return nullptr;
}

VOID DUI_Window::SetDebugMode(BOOL bDebug)
{
	m_bDebug = bDebug;
	Update();
}

VOID DUI_Window::Update(BOOL bForce)
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

DUI_Prompt* DUI_Window::GetWndPrompt()
{
	return m_Prompt;
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

	//if (PtInRect(m_SysBtn_Close.rect, ptMouse))
	//{
	//	m_LastBtn = BT_Close;
	//	if (m_SysBtn_Close.status == S_Normal)
	//	{
	//		m_SysBtn_Close.status = S_HighLight;
	//		OnUpdate();
	//	}
	//}
	//else
	//{
	//	m_LastBtn = BT_None;
	//	if (m_SysBtn_Close.status != S_Normal)
	//	{
	//		m_SysBtn_Close.status = S_Normal;
	//		OnUpdate();
	//	}
	//}
	return TRUE;
}

BOOL DUI_Window::OnLButtonDown(WPARAM wParam, Point* ptMouse)
{
	if (!m_bMouseDown)
	{
		m_bMouseDown = TRUE;
	}
	//if (PtInRect(m_SysBtn_Close.rect, ptMouse))
	//{
	//	m_SysBtn_Close.status = S_Pushed;
	//	OnUpdate();
	//}
	if (ptMouse->Y <= TITLEBARHEIGHT && m_CurCDR == CDR_Normal) //标题栏移动 PtInRect(m_Title->rect, ptMouse)
	{
		SendMessage(m_hWnd, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
		SendMessage(m_hWnd, WM_LBUTTONUP, wParam, MAKELPARAM(ptMouse->X, ptMouse->Y));
	}
	else if (m_Sizeable && m_CurCDR != CDR_Normal)
	{
		SendMessage(m_hWnd, WM_LBUTTONUP, wParam, MAKELPARAM(ptMouse->X, ptMouse->Y));
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
	if (m_bMouseDown)
	{
		m_bMouseDown = FALSE;
	}
	if (m_CaptureCtrlID == NULL)
	{
		m_CaptureCtrlID = INVALID_CONTROLID;
	}
	//if (PtInRect(m_SysBtn_Close.rect, ptMouse))
	//{
	//	m_SysBtn_Close.status = S_HighLight;
	//	OnUpdate();
	//	SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
	//}
	return TRUE;
}

BOOL DUI_Window::OnMouseLeave(WPARAM wParam, Point * ptMouse)
{
	//if (m_LastBtn != BT_None && !m_bMouseDown)
	//{
	//	m_SysBtn_Close.status = S_Normal;
	//	OnUpdate();
	//}
	return 0;
}

BOOL DUI_Window::OnSize(WPARAM wParam, LPARAM lParam)
{
	if (IsIconic(m_hWnd))
	{
		return FALSE;
	}
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	if (m_WndRect == nullptr)
	{
		m_WndRect = new Gdiplus::RectF;
	}
	m_WndRect->X = 0;
	m_WndRect->Y = 0;
	m_WndRect->Width = REAL(rect.right - rect.left); //LOWORD(lParam);
	m_WndRect->Height = REAL(rect.bottom - rect.top); //HIWORD(lParam);
	if (wParam == 2)
	{
		//wParam = wParam;
	}
	//if (lParam != NULL)
	//{
	//	TRACE("%d,%d\n", LOWORD(lParam), HIWORD(lParam));
	//	m_WndRect->Width = LOWORD(lParam);
	//	m_WndRect->Height = HIWORD(lParam);
	//}

	//if (m_WndRect->Width != LOWORD(lParam))
	//{
	//	int x = LOWORD(lParam);
	//	TRACE("X:%d", x);
	//}
	//if (m_WndRect->Width != HIWORD(lParam))
	//{
	//	int y = HIWORD(lParam);
	//	TRACE("Y:%d", y);
	//}
	if (m_ClientRect == nullptr)
	{
		m_ClientRect = new Gdiplus::RectF;
	}
	m_ClientRect->X = SHADOWWIDTH;
	m_ClientRect->Y = SHADOWWIDTH;
	m_ClientRect->Width = m_WndRect->Width - 2 * SHADOWWIDTH - 2;
	m_ClientRect->Height = m_WndRect->Height - 2 * SHADOWWIDTH - 2;
	if (m_MemDC == nullptr)
	{
		m_MemDC = new MemDC;
		m_MemDC->Create((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	else
	{
		m_MemDC->ReSize((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}

	HRGN hRgn;
	if (m_BorderStyle.Mode == BM_RoundRect)
	{
		hRgn = CreateRoundRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 5, 5);
	}
	else
	{
		hRgn = CreateRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	SetWindowRgn(m_hWnd, hRgn, FALSE);
	DeleteObject(hRgn);

	if (m_IconRect == nullptr)
	{
		if (m_Icon != nullptr)
		{
			m_IconRect = new RectF(0, 0, (REAL)m_Icon->GetWidth(), (REAL)m_Icon->GetHeight());
		}
		else
		{
			m_IconRect = new RectF(0, 0, 0, 0);
		}
		
	}
	m_IconRect->X = m_ClientRect->GetLeft() + ICONOFFSET_X;
	m_IconRect->Y = m_ClientRect->GetTop() + ICONOFFSET_Y;

	//更新控制按钮的位置
	if (m_SysBtn[BT_Close] == nullptr)
	{
		m_SysBtn[BT_Close] = new DUI_ImageButton;
		m_SysBtn[BT_Close]->Create(this, 0, 0, 39, 19, m_SysBtnPic, 0);
		m_SysBtn[BT_Close]->m_bAutoReleaseImg = FALSE;
		m_SysBtn[BT_Close]->SetExtra(BT_Close);
		m_SysBtn[BT_Close]->SetPrompt(_T("关闭"));
		m_SysBtnCnt++;
	}
	if (m_SysBtn[BT_Minimize] == nullptr)
	{
		m_SysBtn[BT_Minimize] = new DUI_ImageButton;
		m_SysBtn[BT_Minimize]->Create(this, 0, 0, 28, SYSBTN_Y, m_SysBtnPic, 285);
		m_SysBtn[BT_Minimize]->m_bAutoReleaseImg = FALSE;
		m_SysBtn[BT_Minimize]->SetExtra(BT_Minimize);
		m_SysBtn[BT_Minimize]->SetPrompt(_T("最小化"));
		SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
		m_SysBtnCnt++;
	}

	if (GetSizeable())
	{
		if (m_SysBtn[BT_Maxmize] == nullptr)
		{
			m_SysBtn[BT_Maxmize] = new DUI_ImageButton;
			m_SysBtn[BT_Maxmize]->Create(this, 0, 0, 28, SYSBTN_Y, m_SysBtnPic, IsZoomed(m_hWnd) ? 201 : 117);
			m_SysBtn[BT_Maxmize]->m_bAutoReleaseImg = FALSE;
			m_SysBtn[BT_Maxmize]->SetExtra(BT_Maxmize);
			SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MAXIMIZEBOX);
			m_SysBtnCnt++;
		}
		m_SysBtn[BT_Maxmize]->SetPrompt(IsZoomed(m_hWnd) ? _T("还原") : _T("最大化"));
		//if (!m_SysBtn[BT_Maxmize]->m_bVisialbe)
		//{
		//	m_SysBtn[BT_Maxmize]->m_bVisialbe = TRUE;
		//}
	}
	else
	{
		if (m_SysBtn[BT_Maxmize] != nullptr)
		{
			//m_SysBtn[BT_Maxmize]->m_bVisialbe = FALSE;
			//m_SysBtn[BT_Maxmize]->Destroy();
			//m_SysBtn[BT_Maxmize] = nullptr;
			delete m_SysBtn[BT_Maxmize];
			m_SysBtn[BT_Maxmize] = nullptr;
		}
	}



	REAL BtnWith = 0;
	for (int i = BT_Close; i < BT_None; i++)
	{
		if (m_SysBtn[i] != nullptr&&m_SysBtn[i]->m_bVisialbe)
		{
			m_SysBtn[i]->SetClickEventHandler(m_SysBtnClickProcAddr);
			BtnWith += m_SysBtn[i]->m_Rect->Width;
			if (m_BorderStyle.DoubleBorder)
			{
				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith - 1, m_ClientRect->GetTop() + 1);
			}
			else
			{
				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith + 1, m_ClientRect->GetTop());
			}
		}
	}

	if (m_Title->rect == nullptr)
	{
		m_Title->rect = new RectF;
	}
	if (m_Icon != nullptr)
	{
		m_Title->rect->X = m_ClientRect->GetLeft() + m_IconRect->GetRight() + GAP_ICON_TITLE;
		m_Title->rect->Y = m_ClientRect->GetTop() +  5;
		m_Title->rect->Width = m_ClientRect->Width - m_IconRect->GetRight() - GAP_ICON_TITLE - BtnWith - GAP_TITLE_SYSBTN;
		m_Title->rect->Height = TITLEBARHEIGHT;
	}
	else
	{
		m_Title->rect->X = m_ClientRect->GetLeft() + 2;
		m_Title->rect->Y = m_ClientRect->GetTop() + 4;
		m_Title->rect->Width = m_ClientRect->Width - BtnWith - 2 - GAP_TITLE_SYSBTN;
		m_Title->rect->Height = TITLEBARHEIGHT;
	}
	return TRUE;
}

BOOL DUI_Window::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(m_hWnd, &ps);
	//HDC hDC = m_Graphics->GetHDC();
	//m_MemDC->BitBlt(hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,0, 0, SRCCOPY);
	Update();
	//m_Graphics->ReleaseHDC(hDC);
	EndPaint(m_hWnd, &ps);
	return TRUE;
}

BOOL DUI_Window::OnUpdate(WPARAM wParam, BOOL bUpdate)
{
	DrawWnd();
	if (wParam == NULL)
	{
		OnControl(INVALID_CONTROLID, WM_UPDATE, wParam, FALSE);
	}
	else
	{
		INT Index;
		DUI_ControlBase* pCtrl = FindControlByID((INT)wParam, &Index);
		if (pCtrl != nullptr)
		{
			INT i = 0;
			for (vector<DUI_ControlBase*>::iterator it = m_Controls->begin(); it != m_Controls->end(); it++)
			{
				i = it - m_Controls->begin();
				if (!(*it)->m_bVisialbe)
				{
					continue;
				}
				if (i <= Index)
				{
					(*it)->m_MemDC->AlphaBlend(m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
					continue;
				}
				if ((*it)->m_Rect->IntersectsWith(*pCtrl->m_Rect))
				{
					(*it)->OnUpdate(NULL, FALSE);
					//(*it)->m_MemDC->AlphaBlend(m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
				}
				else
				{
					(*it)->m_MemDC->AlphaBlend(m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
				}
			}
		}
	}
	if (bUpdate == TRUE)
	{
		HDC hDC = GetDC(m_hWnd);//m_Graphics->GetHDC();
		if (hDC == NULL)
		{
			m_Graphics->GetLastStatus();
			
		}
		else
		{
			hDC = hDC;
		}
		if (m_Alpha != 0)
		{
			SIZE szWnd;
			szWnd = { (INT)m_WndRect->Width, (INT)m_WndRect->Height };
			POINT ptSrc = { 0,0 };
			BLENDFUNCTION bf;
			bf.AlphaFormat = AC_SRC_ALPHA;
			bf.BlendFlags = 0;
			bf.BlendOp = 0;
			bf.SourceConstantAlpha = m_Alpha;
			UpdateLayeredWindow(m_hWnd, hDC, NULL, &szWnd, m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
		}
		//else if (Rect != NULL)
		//{
		//	//m_MemDC->BitBlt(hDC, (INT)Rect->GetLeft(), (INT)Rect->GetTop(), (INT)Rect->Width, (INT)Rect->Height, (INT)Rect->GetLeft(), (INT)Rect->GetTop(), SRCCOPY);
		//	m_MemDC->AlphaBlend(hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,
		//		0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
		//} 
		else
		{
			//m_MemDC->BitBlt(hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,0, 0, SRCCOPY);
			m_MemDC->AlphaBlend(hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
		}
		ReleaseDC(m_hWnd, hDC);//m_Graphics->ReleaseHDC(hDC);
	}
	return TRUE;
}

BOOL DUI_Window::OnSetCursor(WPARAM wParam, LPARAM lParam)
{
	if (m_Sizeable)
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

BOOL DUI_Window::OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam)
{
	MINMAXINFO* pInfo = (MINMAXINFO*)lParam;
	pInfo->ptMinTrackSize.x = MINWNDWIDTH;
	pInfo->ptMinTrackSize.y = TITLEBARHEIGHT + 10;
	pInfo->ptMaxPosition.x = 0;
	pInfo->ptMaxPosition.y = 0;
	//测试发现，窗口最大化时，不需要加上后边的两个size。
	pInfo->ptMaxSize.x = GetSystemMetrics(SM_CXFULLSCREEN) + GetSystemMetrics(SM_CXDLGFRAME);
	// + GetSystemMetrics(SM_CXBORDER)+ GetSystemMetrics(SM_CYBORDER); 
	pInfo->ptMaxSize.y = GetSystemMetrics(SM_CYFULLSCREEN) + GetSystemMetrics(SM_CYCAPTION);
	// + GetSystemMetrics(SM_CYDLGFRAME) + GetSystemMetrics(SM_CYBORDER);
	return TRUE;
}

LRESULT DUI_Window::SysBtnClick(HWND hWnd, UINT uMsg, WPARAM ID, LONG Extra)
{
	DUI_ImageButton* pBtn = (DUI_ImageButton*)hWnd;
	switch (Extra)
	{
	case BT_Close:
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	case BT_Minimize:
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		break;
	case BT_Maxmize:
		if (IsZoomed(m_hWnd))
		{
			pBtn->SetImages(m_SysBtnPic, 117);
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		}
		else
		{
			pBtn->SetImages(m_SysBtnPic, 201);
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			//SendMessage(m_hWnd, WM_SIZE, 1, 1);

		}
		break;
	default:
		break;
	}
	return 0;
}

BOOL DUI_Window::OnControl(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == CM_SHOW)
	{
		uMsg = uMsg;
	}
	if (m_Controls->size() == 0)
	{
		return FALSE;
	}
	Point ptMouse;
	GetCursorPos(&ptMouse);
	if (IsMouseMsg(uMsg))
	{
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
	}
	DUI_ControlBase* pCtrl = nullptr;
	if (ID != INVALID_CONTROLID)//若为发送给指定控件的消息，则直接处理后返回
	{
		pCtrl = FindControlByID(ID);
		if (IsMouseMsg(uMsg))
		{
			lParam = MAKELPARAM(ptMouse.X - pCtrl->m_Rect->GetLeft(),
				ptMouse.Y - pCtrl->m_Rect->GetTop());
		}
		return pCtrl->MsgProc(uMsg, wParam, lParam);
	}

	if (uMsg == WM_UPDATE)
	{
		for (vector<DUI_ControlBase*>::iterator it = m_Controls->begin(); it != m_Controls->end(); ++it)
		{
			if ((*it)->m_bVisialbe == FALSE) continue;
			(*it)->MsgProc(uMsg, wParam, lParam);
		}
	}
	else
	{
		if (m_CaptureCtrlID != INVALID_CONTROLID)
		{
			//if (m_CaptureCtrlID == NULL)
			//{
			//	if (uMsg == WM_LBUTTONUP)
			//	{
			//		bMsDown = FALSE;
			//		m_CaptureCtrlID = INVALID_CONTROLID;
			//		//WndProc(m_hWnd, WM_MOUSEMOVE, wParam, lParam);
			//	}
			//	return FALSE;
			//}
			BOOL bInCtrl = FALSE;
			pCtrl = FindControlByID((INT)m_CaptureCtrlID/*, &Index*/);
			bInCtrl= PtInRect(pCtrl->m_Rect, &ptMouse);
			if (IsMouseMsg(uMsg))
			{
				switch (uMsg)
				{
				case WM_MOUSEMOVE:
					if (m_bMouseDown)
					{
						if (bInCtrl)
						{
							if (m_LastHoverCtrlID == INVALID_CONTROLID)
							{
								m_LastHoverCtrlID = pCtrl->m_ID;
								pCtrl->ChangeState(S_Pushed);
							}
						}
						else
						{
							if (m_LastHoverCtrlID != INVALID_CONTROLID)
							{
								m_LastHoverCtrlID = INVALID_CONTROLID;
								pCtrl->ChangeState(S_Focus);
							}
						}
					}
					//else
					//{
					//	m_Controls->at(Index)->ChangeState(S_HighLight);
					//}
					break;
				case WM_LBUTTONUP:
					if (m_CaptureCtrlID != INVALID_CONTROLID)
					{
						m_bMouseDown = FALSE;
						m_CaptureCtrlID = INVALID_CONTROLID;
						ReleaseCapture();
						if (bInCtrl)
						{
							pCtrl->ChangeState(S_HighLight);
						}
						else
						{
							pCtrl->ChangeState(S_Normal);
							WndProc(m_hWnd, WM_MOUSEMOVE, wParam, lParam);
							//return TRUE;
						}
					}
					break;
				default:
					break;
				}
			}
			if (IsMouseMsg(uMsg))
			{
				lParam = MAKELPARAM(ptMouse.X - pCtrl->m_Rect->GetLeft(), ptMouse.Y - pCtrl->m_Rect->GetTop());
			}
			return pCtrl->MsgProc(uMsg, wParam, lParam);
		}
		BOOL bInCtrl = FALSE, bFound = FALSE,Ret;
		for (vector<DUI_ControlBase*>::reverse_iterator it = m_Controls->rbegin(); it != m_Controls->rend(); ++it)
		{
			if ((*it)->m_bVisialbe == FALSE) continue;
			bInCtrl = PtInRect((*it)->m_Rect, &ptMouse);
			if (bInCtrl)
			{
				bFound = TRUE;
				if (IsMouseMsg(uMsg))
				{
					lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),
						ptMouse.Y - (*it)->m_Rect->GetTop());
					Ret = (*it)->MsgProc(uMsg, wParam, lParam);
				}
			}
			switch (uMsg)
			{
			case WM_MOUSEMOVE:
				if (m_LastHoverCtrlID != (*it)->m_ID && bInCtrl)
				{
					if (m_LastHoverCtrlID != INVALID_CONTROLID)
					{
						FindControlByID(m_LastHoverCtrlID)->MsgProc(WM_MOUSELEAVE, wParam, NULL);
						FindControlByID(m_LastHoverCtrlID)->ChangeState(S_Normal);
					}
					(*it)->MsgProc(WM_MOUSEGETIN, NULL, NULL);
					(*it)->ChangeState(S_HighLight);
					m_LastHoverCtrlID = (*it)->m_ID;
				}
				break;
			case WM_LBUTTONDOWN:
				if (bInCtrl)
				{
					m_bMouseDown = TRUE;
					SetCapture(m_hWnd);
					m_CaptureCtrlID = (*it)->m_ID;
					//m_LastHoverCtrlID = m_CaptureCtrlID;
					(*it)->ChangeState(S_Pushed);
				}
				break;
			default:
				break;
			}
			if (IsMouseMsg(uMsg) && bInCtrl)
			{
				//lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),ptMouse.Y - (*it)->m_Rect->GetTop());
				return Ret;//(*it)->MsgProc(uMsg, wParam, lParam);
			}
		}
		if (!bFound)
		{
			if (uMsg == WM_LBUTTONDOWN)
			{
				m_CaptureCtrlID = INVALID_CONTROLID;//标识在窗口上，而非控件上。
			}
			if (m_LastHoverCtrlID != INVALID_CONTROLID)
			{
				pCtrl = FindControlByID(m_LastHoverCtrlID);
				if (pCtrl == nullptr)
				{
					m_LastHoverCtrlID = INVALID_CONTROLID;
					return FALSE;
				}
				pCtrl->MsgProc(WM_MOUSELEAVE, wParam, NULL);
				pCtrl->ChangeState(S_Normal);
				m_LastHoverCtrlID = INVALID_CONTROLID;
			}

		}
	}
	return FALSE;
}