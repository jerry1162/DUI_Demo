#include "stdafx.h"
#include "DUI_Window.h"
#include "Resource.h"
#include "DUI_ImageButton.h"

DUI_Window::DUI_Window()
{

	m_hWnd = NULL;
	m_Parent = nullptr;
	m_hDC = nullptr;
	m_MemDC = nullptr;
	m_BkgDC = nullptr;
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
	m_SysBtnClickProcAddr = (MSGPROC)GetCallBackAddr(this, &DUI_Window::SysBtnClick);
	m_Alpha = 255;//(SHADOWWIDTH == 0 ? 0 : 255);
	m_bInited = FALSE;
	m_bDebug = FALSE;
	m_MsgProc = nullptr;

	for (int i = 0; i < BT_None; i++)
	{
		m_SysBtn[i] = nullptr;
	}
	m_SysBtnCnt = 0;
	m_Prompt = nullptr;
	m_pRdbMgr = nullptr;
	m_pAnimArg = new AnimArg;
	m_bAnimate = FALSE;

	//m_lpfnAnimProc= (WNDANIMPROC)GetCallBackAddr(this, &DUI_Window::WndAnimProc);
	InitRes();
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
	if (m_MsgProc != nullptr)
	{

		if (!m_MsgProc((VOID*)this, uMsg, wParam, lParam))
		{
			return TRUE;
		}
	}
	switch (uMsg)
	{
	case WM_CREATE:
		DrawWndBkg();
		break;
	case WM_LBUTTONDOWN:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = OnLButtonDown(wParam, &ptMouse);
		break;
	case WM_LBUTTONUP:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = OnLButtonUp(wParam, &ptMouse);
		break;
	case WM_PAINT:
		Res = OnPaint(wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = OnMouseMove(wParam,&ptMouse);
		break;
	case WM_SIZE:
		Res = OnSize(wParam, lParam);
		break;
	case WM_MOVE:
		Res = OnMove(wParam,lParam);
		break;
	case WM_UPDATE:
		Res = OnUpdate(wParam, lParam);
		break;
	case WM_SETCURSOR:
		Res = OnSetCursor(wParam, lParam);
		break;
	case WM_MOUSELEAVE:
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
		Res = OnMouseLeave(wParam, &ptMouse);
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
	case WM_TIMER:
		Res = OnTimer(wParam, lParam);
		break;
	case WM_SHOWWINDOW:
		Res = OnShowWindow(wParam, lParam);
		break;
	case WM_DROPFILES:
		Res = OnDropFiles(wParam, lParam);
		break;
	case WM_WNDINITED:
		Res = OnWndInited(wParam, lParam);
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

VOID DUI_Window::DrawWndBkg()
{
	m_BkgDC->Clear();
	if (SHADOWWIDTH != 0)
	{
		m_BkgDC->graphics->Clear(Color::Transparent);
		DrawShadow(m_BkgDC->graphics, m_ClientRect, 5);
	}
	SolidBrush Brush(*m_BkgColor);
	if (m_BkgImg != nullptr)
	{
		m_BkgDC->graphics->DrawImage(m_BkgImg, /*m_WndRect->GetLeft()*/0.0 + SHADOWWIDTH,
			/*m_WndRect->GetTop()*/0.0 + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH*2, m_WndRect->Height - SHADOWWIDTH*2);
	}
	else
	{
		m_BkgDC->graphics->FillRectangle(&Brush, 0.0 + SHADOWWIDTH, 0.0 + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH * 2, m_WndRect->Height - SHADOWWIDTH * 2);
	}

	//画边框
	GraphicsPath* pPath;
	pPath = new GraphicsPath;
	RectF BorderRect;
	BorderRect.X = /*m_WndRect->GetLeft()*/0.0 + SHADOWWIDTH;
	BorderRect.Y = /*m_WndRect->GetTop()*/0.0 + SHADOWWIDTH;
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
	m_BkgDC->graphics->DrawPath(&pen, pPath);
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

		m_BkgDC->graphics->DrawPath(&pen, pPath);
		delete pPath;
		//画边框结束
	}
	delete color;

	//m_MemDC->graphics->DrawImage(m_SysBtn, (INT)0, 0);
	//m_MemDC->graphics->DrawImage(m_SysBtnPic, *m_SysBtn_Close.rect, (REAL)((m_SysBtn_Close.status - 1)*SYSBTN_X), (REAL)0, (REAL)(m_SysBtn_Close.rect->Width), (REAL)(m_SysBtn_Close.rect->Height), UnitPixel);


	if (m_Icon != nullptr)
	{
		m_BkgDC->graphics->DrawImage(m_Icon, *m_IconRect);
	}

	Brush.SetColor(*m_Title->color);
	DrawShadowText(m_BkgDC->graphics, 5, m_Title, Color::Black,Color::MakeARGB(100, 50, 50, 50));

	//调试模式，把所有矩形的边框画出来
	if (m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(200, 100, 100, 100), 1);
		//m_BkgDC->graphics->DrawRectangle(&BorderPen, *m_WndRect);
		m_BkgDC->graphics->DrawRectangle(&BorderPen, *m_ClientRect);
		m_BkgDC->graphics->DrawRectangle(&BorderPen, *m_Title->rect);
		m_BkgDC->graphics->DrawRectangle(&BorderPen, *m_IconRect);
	}
	m_BkgDC->AlphaBlend(m_MemDC);
}

BOOL DUI_Window::InitDUIWnd(HWND hWnd, LPTSTR Title, BOOL bSizable)
{
	if (m_hWnd != NULL)
	{
		return FALSE;
	}
	m_hWnd = hWnd;
	DragAcceptFiles(m_hWnd, TRUE);
	m_hDC = GetDC(m_hWnd);
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
	ResItem* lpItem = m_pRdbMgr->GetItemByName(_T("SysBtn_Pic"));
	if (!lpItem)
	{
		MessageBox(m_hWnd, _T("资源加载失败"), _T("错误:"), MB_ICONINFORMATION);
		return FALSE;
	}
	m_SysBtnPic = ImageFromBin(lpItem->lpData, lpItem->uSize);
	//m_SysBtnPic = ImageFromIDResource(IDB_SYSBTN_3, _T("PNG"));
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

VOID DUI_Window::InitRes()
{
	if (m_pRdbMgr != nullptr)
	{
		return;
	}
	m_pRdbMgr = new RDBManager;
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_RDB), _T("RDB"));
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID pBuffer = LockResource(hGlobal);
	m_pRdbMgr->LoadFromBin(pBuffer);
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
	//m_bAnimate = TRUE;
	Create(hWnd, Title, L"", L"", TRUE);
	//OnUpdate(NULL, TRUE);
	ShowWindow(hWnd, SW_SHOW); 
	//SendMessage(hWnd, WM_SHOWWINDOW, NULL, NULL);
	//UpdateWindow(hWnd);
	//return InitDUIWnd(hWnd, Title, NULL);

	return TRUE;
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
		//ResItem* lpItem = m_pRdbMgr->GetItemByName(_T("ICON"));
		//m_Icon = ImageFromBin(lpItem->lpData, lpItem->uSize);
		m_Icon = ImageFromIDResource(IDI_ICON_MAIN, RT_ICON);
		//ASSERT(m_Icon != NULL);
		//m_IconRect = new RectF;
		//IconRect的处理在OnSize中
	}

	if (BackgrdPic != NULL)
	{
		//m_BkgImg = new Image(BackgrdPic);
		ResItem* lpItem = m_pRdbMgr->GetItemByName(_T("BkgPic"));
		if (!lpItem)
		{
			MessageBox(m_hWnd, _T("资源加载失败"), _T("错误:"), MB_ICONINFORMATION);
			return FALSE;
		}
		m_BkgImg = ImageFromBin(lpItem->lpData, lpItem->uSize);
		//m_BkgImg = ImageFromIDResource(IDB_BKGPIC, _T("JPG"));
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
		ReleaseDC(m_hWnd, m_hDC);
		delete m_MemDC;
		delete m_BkgDC;
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
		delete m_pRdbMgr;
		delete m_pAnimArg;
		//FreeCallBackAddr(m_lpfnAnimProc);
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
	if (m_BkgImg != nullptr)
	{
		delete m_BkgImg;
		m_BkgImg = nullptr;
	}

	if (m_BkgColor != nullptr)
	{
		m_BkgColor->SetValue(BackgrdColor);
	}
	else
	{
		m_BkgColor = new Color(BackgrdColor);
	}
	DrawWndBkg();
	Update(TRUE);
	return TRUE;
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
	DrawWndBkg();
	Update();
	return TRUE;
}

BOOL DUI_Window::SetBorderStyle(BorderStyle bs)
{
	m_BorderStyle = bs;
	HRGN hRgn = NULL;
	if (m_BorderStyle.Mode == BM_RoundRect)
	{
		hRgn = CreateRoundRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 5, 5);
	}
	SetWindowRgn(m_hWnd, hRgn, FALSE);
	DeleteObject(hRgn);
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
	for (auto it = m_Controls->begin(); it != m_Controls->end(); ++it)
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
	DrawWndBkg();
	Update();
}

VOID DUI_Window::Update(BOOL bForce)
{
	if (bForce)
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


	if (m_WndRect == nullptr)
	{
		m_WndRect = new Gdiplus::RectF;
		m_WndRect->X = 0;
		m_WndRect->Y = 0;
	}


	
	if (wParam == NULL)
	{
		RECT rect;
		GetWindowRect(m_hWnd, &rect);
		//m_WndRect->X = (REAL)rect.left;
		//m_WndRect->Y = (REAL)rect.top;
		m_WndRect->Width = REAL(rect.right - rect.left); //LOWORD(lParam);
		m_WndRect->Height = REAL(rect.bottom - rect.top); //HIWORD(lParam);
	}
	else
	{
		m_WndRect->Width = REAL(LOWORD(lParam));
		m_WndRect->Height = REAL(HIWORD(lParam));
	}


	if (m_ClientRect == nullptr)
	{
		m_ClientRect = new Gdiplus::RectF;
	}
	m_ClientRect->X = SHADOWWIDTH;
	m_ClientRect->Y = SHADOWWIDTH;
	m_ClientRect->Width = m_WndRect->Width - 2 * SHADOWWIDTH - 2;
	m_ClientRect->Height = m_WndRect->Height - 2 * SHADOWWIDTH - 2;

	if (m_BkgDC == nullptr)
	{
		m_BkgDC = new MemDC;
		m_BkgDC->Create((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	else
	{
		m_BkgDC->ReSize((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	
	if (m_MemDC == nullptr)
	{
		m_MemDC = new MemDC;
		m_MemDC->Create((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	else
	{
		m_MemDC->ReSize((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}

	HRGN hRgn = NULL;
	if (m_BorderStyle.Mode == BM_RoundRect)
	{
		hRgn = CreateRoundRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 5, 5);
	}
	SetWindowRgn(m_hWnd, hRgn, FALSE);
	DeleteObject(hRgn);

	if (m_IconRect == nullptr)
	{
		if (m_Icon != nullptr)
		{
			m_IconRect = new RectF(0, 0, /*(REAL)m_Icon->GetWidth()*/ICONSIZE_X, /*(REAL)m_Icon->GetHeight()*/ICONSIZE_Y);
		}
		else
		{
			m_IconRect = new RectF(0, 0, 0, 0);
		}

	}
	m_IconRect->X = m_ClientRect->GetLeft() + ICONOFFSET_X;
	m_IconRect->Y = m_ClientRect->GetTop() + ICONOFFSET_Y;


	//更新控制按钮的位置
	INT Size_X;
	INT Size_Y;
	INT Start;
	if (m_SysBtn[BT_Close] == nullptr)
	{
		m_SysBtn[BT_Close] = new DUI_ImageButton;
		Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_X"));
		Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_Y"));
		Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_Start"));
		m_SysBtn[BT_Close]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
		m_SysBtn[BT_Close]->m_bAutoReleaseImg = FALSE;
		m_SysBtn[BT_Close]->SetExtra(BT_Close);
		m_SysBtn[BT_Close]->SetPrompt(_T("关闭"));
		m_SysBtnCnt++;
	}

	if (m_SysBtn[BT_Minimize] == nullptr)
	{
		m_SysBtn[BT_Minimize] = new DUI_ImageButton;
		Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_X"));
		Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_Y"));
		Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_Start"));
		m_SysBtn[BT_Minimize]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
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
			Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_X"));
			Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Y"));
			if (IsZoomed(m_hWnd))
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Restore_Start"));
			}
			else
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Start"));
			}
			m_SysBtn[BT_Maxmize]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
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
				m_SysBtn[i]->m_bAutoUpdate = FALSE;
				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith - 1, m_ClientRect->GetTop() + 1);
				m_SysBtn[i]->m_bAutoUpdate = TRUE;
			}
			else
			{
				m_SysBtn[i]->m_bAutoUpdate = FALSE;
				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith + 1, m_ClientRect->GetTop());
				m_SysBtn[i]->m_bAutoUpdate = TRUE;
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
		m_Title->rect->Y = m_ClientRect->GetTop() + 5;
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

	DrawWndBkg();
	Update();
	return TRUE;
}

BOOL DUI_Window::OnMove(WPARAM wParam, LPARAM lParam)
{
	//RECT rect;
	//GetWindowRect(m_hWnd, &rect);
	m_WndRect->X = LOWORD(lParam);//(REAL)rect.left;
	m_WndRect->Y = HIWORD(lParam);//(REAL)rect.top;
	//m_WndRect->Width = REAL(rect.right - rect.left); //LOWORD(lParam);
	//m_WndRect->Height = REAL(rect.bottom - rect.top); //HIWORD(lParam);
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
			m_MemDC->SelectRectClipRgn((INT)pCtrl->m_Rect->X, (INT)pCtrl->m_Rect->Y, (INT)pCtrl->m_Rect->Width, (INT)pCtrl->m_Rect->Height);
			//m_BkgDC->BitBlt(m_MemDC);
			m_BkgDC->AlphaBlend(m_MemDC);
			//INT i = 0;
			for (auto it = m_Controls->begin(); it != m_Controls->end(); it++)
			{
				//i = it - m_Controls->begin();
				if (!(*it)->m_bVisialbe)
				{
					continue;
				}
				if ((*it)->m_Rect->IntersectsWith(*pCtrl->m_Rect))
				{
					//(*it)->OnUpdate(NULL, FALSE);
					(*it)->m_MemDC->AlphaBlend(m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
				}
			}
			m_MemDC->SelectClipRgn();
		}
	}
	if (m_bAnimate)
	{
		return TRUE;
	}
	if (bUpdate == TRUE && m_bInited)
	{
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
			UpdateLayeredWindow(m_hWnd, m_hDC, nullptr, &szWnd, m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
		}
		else
		{
			m_MemDC->AlphaBlend(m_hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
		}
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

BOOL DUI_Window::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if (!WndAnimProc(m_pAnimArg))
	{
		KillTimer(m_hWnd, wParam);
		m_bAnimate = FALSE;
		m_bInited = TRUE;
		m_BorderStyle.Mode = BM_RoundRect;
		SetBorderStyle(m_BorderStyle);
		WndProc(m_hWnd, WM_WNDINITED, NULL, NULL);
	}
	return 0;
}

BOOL DUI_Window::OnShowWindow(WPARAM wParam, LPARAM lParam)
{
	if (m_Alpha == 0)
	{
		WndProc(m_hWnd, WM_WNDINITED, NULL, NULL);
		return TRUE;
	}
	m_bAnimate = TRUE;
	m_BorderStyle.Mode = BM_Normal;
	SetBorderStyle(m_BorderStyle);
	SetTimer(m_hWnd, 0, 16, nullptr);
	return TRUE;
}

BOOL DUI_Window::OnDropFiles(WPARAM wParam, LPARAM lParam)
{
	//HDROP hDrop;
	//hDrop = (HDROP)wParam;

	return TRUE;
}

BOOL DUI_Window::OnWndInited(WPARAM wParam, LPARAM lParam)
{
	m_bInited = TRUE;
	OnControl(INVALID_CONTROLID, CM_SETAUTOUPDATE, NULL, TRUE);
	return TRUE;
}

// BOOL DUI_Window::OnWindowPosChanged(WPARAM wParam, LPARAM lParam)
// {
// 	WINDOWPOS *wp;
// 	if (lParam == NULL)
// 	{
// 		wp = new WINDOWPOS;
// 
// 		RECT rect;
// 		GetWindowRect(m_hWnd, &rect);
// 		wp->cx = rect.right - rect.left;
// 		wp->cy = rect.bottom - rect.top;
// 	}
// 	else
// 	{
// 		wp = (WINDOWPOS*)lParam;
// 		if (wp->flags = SWP_NOSIZE | SWP_NOMOVE)
// 		{
// 			return TRUE;
// 		}
// 	}
// 
// 
// 	if (m_WndRect == nullptr)
// 	{
// 		m_WndRect = new Gdiplus::RectF;
// 	}
// 	m_WndRect->X = 0;
// 	m_WndRect->Y = 0;
// 	m_WndRect->Width = REAL(wp->cx); //LOWORD(lParam);
// 	m_WndRect->Height = REAL(wp->cx); //HIWORD(lParam);
// 	if (lParam == NULL)
// 	{
// 		delete wp;
// 	}
// 	if (m_ClientRect == nullptr)
// 	{
// 		m_ClientRect = new Gdiplus::RectF;
// 	}
// 	m_ClientRect->X = SHADOWWIDTH;
// 	m_ClientRect->Y = SHADOWWIDTH;
// 	m_ClientRect->Width = m_WndRect->Width - 2 * SHADOWWIDTH - 2;
// 	m_ClientRect->Height = m_WndRect->Height - 2 * SHADOWWIDTH - 2;
// 	if (m_MemDC == nullptr)
// 	{
// 		m_MemDC = new MemDC;
// 		m_MemDC->Create((INT)m_WndRect->Width, (INT)m_WndRect->Height);
// 	}
// 	else
// 	{
// 		m_MemDC->ReSize((INT)m_WndRect->Width, (INT)m_WndRect->Height);
// 	}
// 
// 	HRGN hRgn;
// 	if (m_BorderStyle.Mode == BM_RoundRect)
// 	{
// 		hRgn = CreateRoundRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 5, 5);
// 	}
// 	else
// 	{
// 		hRgn = CreateRectRgn(0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height);
// 	}
// 	SetWindowRgn(m_hWnd, hRgn, FALSE);
// 	DeleteObject(hRgn);
// 
// 	if (m_IconRect == nullptr)
// 	{
// 		if (m_Icon != nullptr)
// 		{
// 			m_IconRect = new RectF(0, 0, /*(REAL)m_Icon->GetWidth()*/ICONSIZE_X, /*(REAL)m_Icon->GetHeight()*/ICONSIZE_Y);
// 		}
// 		else
// 		{
// 			m_IconRect = new RectF(0, 0, 0, 0);
// 		}
// 
// 	}
// 	m_IconRect->X = m_ClientRect->GetLeft() + ICONOFFSET_X;
// 	m_IconRect->Y = m_ClientRect->GetTop() + ICONOFFSET_Y;
// 
// 
// 	//更新控制按钮的位置
// 	INT Size_X;
// 	INT Size_Y;
// 	INT Start;
// 	if (m_SysBtn[BT_Close] == nullptr)
// 	{
// 		m_SysBtn[BT_Close] = new DUI_ImageButton;
// 		Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_X"));
// 		Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_Y"));
// 		Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Close_Start"));
// 		m_SysBtn[BT_Close]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
// 		m_SysBtn[BT_Close]->m_bAutoReleaseImg = FALSE;
// 		m_SysBtn[BT_Close]->SetExtra(BT_Close);
// 		m_SysBtn[BT_Close]->SetPrompt(_T("关闭"));
// 		m_SysBtnCnt++;
// 	}
// 
// 	if (m_SysBtn[BT_Minimize] == nullptr)
// 	{
// 		m_SysBtn[BT_Minimize] = new DUI_ImageButton;
// 		Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_X"));
// 		Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_Y"));
// 		Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Min_Start"));
// 		m_SysBtn[BT_Minimize]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
// 		m_SysBtn[BT_Minimize]->m_bAutoReleaseImg = FALSE;
// 		m_SysBtn[BT_Minimize]->SetExtra(BT_Minimize);
// 		m_SysBtn[BT_Minimize]->SetPrompt(_T("最小化"));
// 		SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MINIMIZEBOX);
// 		m_SysBtnCnt++;
// 	}
// 
// 	if (GetSizeable())
// 	{
// 		if (m_SysBtn[BT_Maxmize] == nullptr)
// 		{
// 			m_SysBtn[BT_Maxmize] = new DUI_ImageButton;
// 			Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_X"));
// 			Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Y"));
// 			if (IsZoomed(m_hWnd))
// 			{
// 				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Restore_Start"));
// 			}
// 			else
// 			{
// 				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Start"));
// 			}
// 			m_SysBtn[BT_Maxmize]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
// 			m_SysBtn[BT_Maxmize]->m_bAutoReleaseImg = FALSE;
// 			m_SysBtn[BT_Maxmize]->SetExtra(BT_Maxmize);
// 			SetWindowLong(m_hWnd, GWL_STYLE, GetWindowLong(m_hWnd, GWL_STYLE) | WS_MAXIMIZEBOX);
// 			m_SysBtnCnt++;
// 		}
// 		m_SysBtn[BT_Maxmize]->SetPrompt(IsZoomed(m_hWnd) ? _T("还原") : _T("最大化"));
// 		//if (!m_SysBtn[BT_Maxmize]->m_bVisialbe)
// 		//{
// 		//	m_SysBtn[BT_Maxmize]->m_bVisialbe = TRUE;
// 		//}
// 	}
// 	else
// 	{
// 		if (m_SysBtn[BT_Maxmize] != nullptr)
// 		{
// 			//m_SysBtn[BT_Maxmize]->m_bVisialbe = FALSE;
// 			//m_SysBtn[BT_Maxmize]->Destroy();
// 			//m_SysBtn[BT_Maxmize] = nullptr;
// 			delete m_SysBtn[BT_Maxmize];
// 			m_SysBtn[BT_Maxmize] = nullptr;
// 		}
// 	}
// 
// 
// 
// 	REAL BtnWith = 0;
// 	for (int i = BT_Close; i < BT_None; i++)
// 	{
// 		if (m_SysBtn[i] != nullptr&&m_SysBtn[i]->m_bVisialbe)
// 		{
// 			m_SysBtn[i]->SetClickEventHandler(m_SysBtnClickProcAddr);
// 			BtnWith += m_SysBtn[i]->m_Rect->Width;
// 			if (m_BorderStyle.DoubleBorder)
// 			{
// 				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith - 1, m_ClientRect->GetTop() + 1);
// 			}
// 			else
// 			{
// 				m_SysBtn[i]->Move(m_ClientRect->GetRight() - BtnWith + 1, m_ClientRect->GetTop());
// 			}
// 		}
// 	}
// 
// 	if (m_Title->rect == nullptr)
// 	{
// 		m_Title->rect = new RectF;
// 	}
// 	if (m_Icon != nullptr)
// 	{
// 		m_Title->rect->X = m_ClientRect->GetLeft() + m_IconRect->GetRight() + GAP_ICON_TITLE;
// 		m_Title->rect->Y = m_ClientRect->GetTop() + 5;
// 		m_Title->rect->Width = m_ClientRect->Width - m_IconRect->GetRight() - GAP_ICON_TITLE - BtnWith - GAP_TITLE_SYSBTN;
// 		m_Title->rect->Height = TITLEBARHEIGHT;
// 	}
// 	else
// 	{
// 		m_Title->rect->X = m_ClientRect->GetLeft() + 2;
// 		m_Title->rect->Y = m_ClientRect->GetTop() + 4;
// 		m_Title->rect->Width = m_ClientRect->Width - BtnWith - 2 - GAP_TITLE_SYSBTN;
// 		m_Title->rect->Height = TITLEBARHEIGHT;
// 	}
// 
// 
// 
// 	return FALSE;
// }

RDBManager * DUI_Window::GetRDBMgr()
{
	return m_pRdbMgr;
}

VOID DUI_Window::AddControl(DUI_ControlBase * pControl)
{
	m_Controls->push_back(pControl);
}

BOOL DUI_Window::IsWindowInited()
{
	return m_bInited;
}

MSGPROC DUI_Window::SetMsgProc(MSGPROC Proc)
{
	MSGPROC PrevProc = m_MsgProc;
	m_MsgProc = Proc;
	return PrevProc;
}

BOOL DUI_Window::SysBtnClick(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DUI_ImageButton* pBtn = (DUI_ImageButton*)pThis;
	switch (wParam)
	{
	case BT_Close:
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
		break;
	case BT_Minimize:
		SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		break;
	case BT_Maxmize:
		INT Start;
		if (IsZoomed(m_hWnd))
		{
			Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Start"));
			//117
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
			Point ptMouse;
			GetCursorPos(&ptMouse);
			OnControl(INVALID_CONTROLID, WM_MOUSEMOVE, NULL, MAKELPARAM(ptMouse.X, ptMouse.Y));
			//ShowWindowAsync(m_hWnd, SW_RESTORE);
		}
		else
		{
			Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Restore_Start"));
			//201
			SendMessage(m_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			//ShowWindowAsync(m_hWnd, SW_MAXIMIZE);
		}
		pBtn->SetImages(m_SysBtnPic, Start);
		OnSize(NULL, NULL);
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL DUI_Window::WndAnimProc(AnimArg * pArg)
{
	/*
	//透明渐变
	if (pArg->pDC1 == nullptr)
	{
		pArg->pDC1 = new MemDC((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	pArg->pDC1->Clear();
	m_MemDC->BitBlt(pArg->pDC1);
	pArg->Arg_1 += 10;
	if (pArg->Arg_1 > 255)
	{
		pArg->Arg_1 = 255;
	}

	SIZE szWnd;
	szWnd = { (INT)m_WndRect->Width, (INT)m_WndRect->Height };
	POINT ptSrc = { 0,0 };
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = pArg->Arg_1;
	UpdateLayeredWindow(m_hWnd, m_hDC, NULL, &szWnd, pArg->pDC1->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
	if (pArg->Arg_1 == 255)
	{
		delete pArg->pDC1;
		pArg->pDC1 = nullptr;
		return FALSE;
	}
	else
	{
		return TRUE;
	}*/

	//下滑
	/*if (pArg->pDC1 == nullptr)
	{
		pArg->pDC1 = new MemDC((INT)m_WndRect->Width, (INT)m_WndRect->Height);
	}
	pArg->pDC1->Clear();
	m_MemDC->BitBlt(pArg->pDC1);
	pArg->Arg_1 += (INT)m_WndRect->Height / 14;
	if (pArg->Arg_1 > (INT)m_WndRect->Height)
	{
		pArg->Arg_1 = (INT)m_WndRect->Height;
	}

	SIZE szWnd;
	szWnd = { (INT)m_WndRect->Width, pArg->Arg_1 };
	POINT ptSrc = { 0,0 };
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = 255 * pArg->Arg_1 / (INT)m_WndRect->Height;
	UpdateLayeredWindow(m_hWnd, NULL, NULL, &szWnd, pArg->pDC1->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
	if (pArg->Arg_1 == m_WndRect->Height)
	{
		delete pArg->pDC1;
		pArg->pDC1 = nullptr;

		return FALSE;
	}
	else
	{
		return TRUE;
	}*/

	
	//弹出
	if (pArg->pDC1 == nullptr)
	{
		pArg->pDC1 = new MemDC((INT)m_WndRect->Width + 10 * 2, (INT)m_WndRect->Height + 10 * 2);
		pArg->Arg_2 = 1;
	}
	pArg->pDC1->Clear();
	if (pArg->Arg_2 == 1)
	{
		pArg->Arg_1 += 1;
	}
	else
	{
		pArg->Arg_1 -= 2;
	}
	if (pArg->Arg_1 > 10)
	{
		pArg->Arg_1 = 10;
	}
	SIZE szWnd;
	POINT ptSrc = { 0,0 }, ptDest;
	if (pArg->Arg_2 == 1)
	{
		m_MemDC->AlphaBlend(pArg->pDC1, 0, 0, (INT)m_WndRect->Width + pArg->Arg_1 * 2, (INT)m_WndRect->Height + pArg->Arg_1 * 2, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
		
		ptDest = { (INT)m_WndRect->X - pArg->Arg_1,(INT)m_WndRect->Y - pArg->Arg_1 };
		szWnd = { (INT)m_WndRect->Width + pArg->Arg_1 * 2,(INT)m_WndRect->Height + pArg->Arg_1 * 2 };
	}
	else
	{
		m_MemDC->AlphaBlend(pArg->pDC1, 0, 0, (INT)m_WndRect->Width + pArg->Arg_1 * 2, (INT)m_WndRect->Height + pArg->Arg_1 * 2, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
		ptDest = { (INT)m_WndRect->X - pArg->Arg_1,(INT)m_WndRect->Y - pArg->Arg_1 };
		szWnd = { (INT)m_WndRect->Width + pArg->Arg_1 * 2,(INT)m_WndRect->Height + pArg->Arg_1 * 2 };
	}
	BLENDFUNCTION bf;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.BlendFlags = 0;
	bf.BlendOp = 0;
	bf.SourceConstantAlpha = 255 * (pArg->Arg_2 == 1 ? pArg->Arg_1 / 2 : (20 - pArg->Arg_1) / 2) / 10;
	UpdateLayeredWindow(m_hWnd, m_hDC, &ptDest, &szWnd, pArg->pDC1->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
	if (pArg->Arg_2 == 1 && pArg->Arg_1 == 10)
	{
		pArg->Arg_2 = 2;
		
	}
	else if(pArg->Arg_2 == 2 && pArg->Arg_1 == 0)
	{
		delete pArg->pDC1;
		pArg->pDC1 = nullptr;
		return FALSE;
	}
	return TRUE;
}

BOOL DUI_Window::OnControl(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//return FALSE;
	if (m_Controls->size() == 0)
	{
		return FALSE;
	}
	Point ptMouse;
	if (IsMouseMsg(uMsg))
	{
		ptMouse.X = LOWORD(lParam);
		ptMouse.Y = HIWORD(lParam);
	}
	else
	{
		GetCursorPos(&ptMouse);
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
	else
	{
		if (uMsg == CM_SETAUTOUPDATE)
		{
			uMsg = uMsg;
		}
	}

	if (uMsg == WM_UPDATE)
	{
		for (auto it = m_Controls->begin(); it != m_Controls->end(); ++it)
		{
			if ((*it)->m_bVisialbe == FALSE) continue;
			(*it)->MsgProc(uMsg, wParam, lParam);
		}
	}
	else
	{
		if (m_CaptureCtrlID != INVALID_CONTROLID)
		{
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
		for (auto it = m_Controls->rbegin(); it != m_Controls->rend(); ++it)
		{
			if ((*it)->m_bVisialbe == FALSE) continue;
			if (IsMouseMsg(uMsg))
			{
				bInCtrl = PtInRect((*it)->m_Rect, &ptMouse);
				if (bInCtrl)
				{
					bFound = TRUE;
					lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),ptMouse.Y - (*it)->m_Rect->GetTop());
					Ret = (*it)->MsgProc(uMsg, wParam, lParam);
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
						(*it)->MsgProc(CM_MOUSEGETIN, NULL, NULL);
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
			}
			else
			{
				(*it)->MsgProc(uMsg, wParam, lParam);
			}
			if (IsMouseMsg(uMsg) && bInCtrl)
			{
				//lParam = MAKELPARAM(ptMouse.X - (*it)->m_Rect->GetLeft(),ptMouse.Y - (*it)->m_Rect->GetTop());
				return Ret;//(*it)->MsgProc(uMsg, wParam, lParam);
			}
		}

		if (IsMouseMsg(uMsg))//bFound一定为假
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