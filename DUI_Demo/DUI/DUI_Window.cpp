#include "stdafx.h"
#include "DUI_Window.h"
#include "DUI_ImageButton.h"
#include "..//Resource.h"

#include "DUI_ImageBox.h"
#include "DUI_CheckBox.h"


DUI_Window::DUI_Window()
{

	//m_hWnd = NULL;
	m_Parent = nullptr;
	m_hDC = nullptr;
	m_MemDC = nullptr;
	m_BkgDC = nullptr;
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
	m_WndProc = (WNDPROC)GetCallBackAddr(this, &DUI_Window::MsgProc);
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
	m_bAllowCtrlUpdate = FALSE;
	//m_lpfnAnimProc= (WNDANIMPROC)GetCallBackAddr(this, &DUI_Window::WndAnimProc);
	//InitRes();
}

DUI_Window::~DUI_Window()
{
	Destroy();
}

LRESULT DUI_Window::MsgProc(INT hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Point ptMouse;
	int Res = 0;
	if (IsWindow((HWND)hwnd))
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
		DrawWnd();
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
	case WM_CLOSE:
		Res = OnClose(wParam, lParam);
		if (Res)
		{
			return 0;
		}
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
		return CallWindowProc(WNDPROC(PrevWndProc), (HWND)hwnd, uMsg, wParam, lParam);
	}
	
}

VOID DUI_Window::DrawWnd(MemDC* hDC)
{
	if (hDC == nullptr)
	{
		hDC = m_BkgDC;
	}
	hDC->Clear();
	DrawWndBkg(hDC);
	DrawWndFrame(hDC);
	DrawTitleBar(hDC);
	hDC->AlphaBlend(m_MemDC);
}

VOID DUI_Window::DrawWndFrame(MemDC* hDC)
{
	if (hDC == nullptr)
	{
		hDC = m_BkgDC;
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
	hDC->graphics->DrawPath(&pen, pPath);
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

		hDC->graphics->DrawPath(&pen, pPath);
		delete pPath;
		//画边框结束
	}
	delete color;

	if (m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(200, 100, 100, 100), 1);
		hDC->graphics->DrawRectangle(&BorderPen, *m_ClientRect);
	}
}

VOID DUI_Window::DrawWndBkg(MemDC* hDC)
{
	if (hDC == nullptr)
	{
		hDC = m_BkgDC;
	}
	if (SHADOWWIDTH != 0)
	{
		hDC->graphics->Clear(Color::Transparent);
		DrawShadow(hDC->graphics, m_ClientRect, 5);
	}
	SolidBrush Brush(*m_BkgColor);
	if (m_BkgImg != nullptr)
	{
		hDC->graphics->DrawImage(m_BkgImg, /*m_WndRect->GetLeft()*/0.0 + SHADOWWIDTH,
			/*m_WndRect->GetTop()*/0.0 + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH*2, m_WndRect->Height - SHADOWWIDTH*2);
	}
	else
	{
		hDC->graphics->FillRectangle(&Brush, 0.0 + SHADOWWIDTH, 0.0 + SHADOWWIDTH, m_WndRect->Width - SHADOWWIDTH * 2, m_WndRect->Height - SHADOWWIDTH * 2);
	}
}

VOID DUI_Window::DrawTitleBar(MemDC* hDC)
{
	if (hDC == nullptr)
	{
		hDC = m_BkgDC;
	}
	if (m_Icon != nullptr)
	{
		hDC->graphics->DrawImage(m_Icon, *m_IconRect);
	}

	SolidBrush Brush(*m_Title->color);
	DrawShadowText(hDC->graphics, m_Title);
	if (m_bDebug)
	{
		Pen BorderPen(Color::MakeARGB(200, 100, 100, 100), 1);
		hDC->graphics->DrawRectangle(&BorderPen, *m_Title->rect);
		hDC->graphics->DrawRectangle(&BorderPen, *m_IconRect);
	}
}

BOOL DUI_Window::InitDUIWnd(HWND hWnd, LPTSTR Title)
{
	m_ID = (INT)hWnd;
	m_hDC = GetDC((HWND)m_ID);
	m_Sizeable = TRUE;
	//m_WndRect = new RectF;
	//m_Graphics = new Gdiplus::Graphics(hWnd);
	//m_MemDC = new MemDC;

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
	m_Title->format->SetAlignment(StringAlignment::StringAlignmentCenter);
	m_Title->format->SetLineAlignment(StringAlignment::StringAlignmentCenter);
	m_Title->color = new Color(Color::White);
	m_Title->rect = new RectF;
	//TitleRect的处理在OnSize里;
	ResItem* lpItem = m_pRdbMgr->GetItemByName(_T("SysBtn_Pic"));
	if (!lpItem)
	{
		MessageBox((HWND)m_ID, _T("资源加载失败"), _T("错误:"), MB_ICONINFORMATION);
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
		LONG style = GetWindowLong((HWND)m_ID, GWL_EXSTYLE);
		style |= WS_EX_LAYERED;
		SetWindowLong((HWND)m_ID, GWL_EXSTYLE, style);
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
	OnMove(NULL, NULL);
	SetTitle(Title);
	return TRUE;
}

BOOL DUI_Window::InitRes(RDBManager* RdbMgr)
{
	if (m_pRdbMgr != nullptr || RdbMgr == nullptr || RdbMgr->GetLoadType() == L_Unknown)
	{
		return FALSE;
	}
	m_pRdbMgr = RdbMgr;
	ResItem* lpItem = m_pRdbMgr->GetItemByName(_T("Icon"));
	if (lpItem != nullptr)
	{
		m_Icon = ImageFromBin(lpItem->lpData, lpItem->uSize);
	}
	else
	{
		//m_Icon = ImageFromIDResource(IDI_ICON_MAIN, RT_ICON);
		HICON hIcon = (HICON)SendMessage((HWND)m_ID, WM_GETICON, ICON_BIG, NULL);
		m_Icon = Bitmap::FromHICON(hIcon);
	}
	lpItem = m_pRdbMgr->GetItemByName(_T("BkgMode"));
	if (lpItem != nullptr)
	{
		bool* BkgMode = (bool*)lpItem->lpData;
		if (*BkgMode == TRUE)//真表示图片背景
		{
			lpItem = m_pRdbMgr->GetItemByName(_T("BkgImg_1"));
			if (!lpItem)
			{
				MessageBox((HWND)m_ID, _T("背景图加载失败"), _T("错误:"), MB_ICONINFORMATION);
				return FALSE;
			}
			m_BkgImg = ImageFromBin(lpItem->lpData, lpItem->uSize);
		}
		else
		{
			ARGB BkColor = m_pRdbMgr->GetDwordValByName(_T("BkgColor"));
			if (m_BkgColor == nullptr)
			{
				m_BkgColor = new Color(BkColor);
			}
		}
	}
	else
	{
		MessageBox((HWND)m_ID, _T("获取背景模式失败。"), _T("错误:"), MB_ICONINFORMATION);
		return FALSE;
	}
	return TRUE;
}

/*
BOOL DUI_Window::Create(HWND hWnd, LPTSTR Title, LPTSTR Icon, ARGB BkgColor)
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
	return InitDUIWnd(hWnd, Title);
}*/

BOOL DUI_Window::Create(HWND hWnd, RDBManager* RdbMgr)
{
	if (hWnd == NULL)
	{
		return FALSE;
	}
	//m_ID = (INT)hWnd;
	if (!InitRes(RdbMgr))
	{
		//MessageBox((HWND)m_ID, _T("加载资源包出错！"), _T("错误:"), MB_ICONINFORMATION);
		return FALSE;
	}
	return InitDUIWnd(hWnd, m_pRdbMgr->GetTextByName(_T("Title")));
}

BOOL DUI_Window::Create(INT Width, INT Height, DUI_Window* Parent, RDBManager* RdbMgr)
{
	m_BkgColor = new Color(Color::MakeARGB(255, 240, 240, 240));
	m_Parent = Parent;
	if (RdbMgr == nullptr)
	{
		if (m_Parent == nullptr)
		{
			return FALSE;
		}
		InitRes(m_Parent->GetRDBMgr());
	}
	else
	{
		InitRes(RdbMgr);
	}
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	HWND hWnd = CreateWindowW((LPCWSTR)GetDefaultWndClass(), m_Title->string->GetString(), WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_POPUP, (cx - Width) / 2, (cy - Height) / 2, Width, Height, (HWND)m_Parent->m_ID, nullptr, GetModuleHandle(nullptr), nullptr);
	if (hWnd == NULL)
	{
		return FALSE;
	}
	//Create(hWnd, Title, L"", L"");
	ShowWindow(hWnd, SW_SHOW); 
	//SendMessage(hWnd, WM_SHOWWINDOW, NULL, NULL);
	//UpdateWindow(hWnd);
	//return InitDUIWnd(hWnd, Title, NULL);

	return TRUE;
}

/*BOOL DUI_Window::Create(HWND hWnd, LPTSTR Title, LPTSTR Icon, LPTSTR BackgrdPic)
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
			MessageBox((HWND)m_ID, _T("资源加载失败"), _T("错误:"), MB_ICONINFORMATION);
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
	return InitDUIWnd(hWnd, Title);
}*/

VOID DUI_Window::DoModel()
{
	if (m_ID != NULL)
	{
		if (m_Parent != nullptr)
		{
			EnableWindow((HWND)m_Parent->m_ID, FALSE);
		}
		MessageLoop();
		if (m_Parent != nullptr)
		{
			EnableWindow((HWND)m_Parent->m_ID, TRUE);
			SetForegroundWindow((HWND)m_Parent->m_ID);
		}
	}
}

BOOL DUI_Window::Destroy()
{
	if (m_ID != NULL)
	{
		while (m_Controls->size() != 0)
		{
			delete (*m_Controls)[0];
		}
		delete m_Controls;
		SetWindowLong((HWND)m_ID, GWL_WNDPROC, PrevWndProc);
		FreeCallBackAddr(m_WndProc);
		FreeCallBackAddr(m_SysBtnClickProcAddr);
		ReleaseDC((HWND)m_ID, m_hDC);
		m_hDC = NULL;
		SafeDelete(m_MemDC);
		SafeDelete(m_BkgDC);
		SafeDelete(m_WndRect);
		SafeDelete(m_ClientRect);

		SafeDelete(m_Title);
		SafeDelete(m_BkgColor);

		SafeDelete(m_Icon);
		SafeDelete(m_IconRect);
		SafeDelete(m_BkgImg);
		SafeDelete(m_SysBtnPic);
		SafeDelete(m_Prompt);
		SafeDelete(m_pAnimArg);
		m_ID = NULL;
		return TRUE;
	}
	return FALSE;
}

HWND DUI_Window::GetHWND()
{
	return (HWND)m_ID;
}

ObjType DUI_Window::GetObjType()
{
	return ObjType_Wnd;
}

BOOL DUI_Window::SetBkgPic(LPTSTR BackgrdPic)
{
	if (BackgrdPic == NULL || m_ID == NULL)
	{
		return FALSE;
	}
	delete m_BkgImg;
	m_BkgImg = new Image(BackgrdPic);
	Update();
	return TRUE;
}

BOOL DUI_Window::SetBkgPic(ResItem * lpItem)
{
	if (lpItem == nullptr || lpItem->ItemType != RT_Image)
	{
		return FALSE;
	}
	SafeDelete(m_BkgImg);
	m_bAnimate = TRUE;
	SetTimer((HWND)m_ID, TID_WND_CNGBKG, 16, nullptr);
	OnTimer(TID_WND_CNGBKG, NULL);
	m_BkgImg = ImageFromBin(lpItem->lpData, lpItem->uSize);
	DrawWnd();
	return TRUE;
}

BOOL DUI_Window::SetBkgPic(INT PicIndex)
{
	INT Cnt = m_pRdbMgr->GetIntValByName(_T("BkgImgCnt"));
	if (PicIndex <= 0 || PicIndex > Cnt)
	{
		return FALSE;
	}
	CString str;
	str.Format(_T("BkgImg_%d"), PicIndex);
	return SetBkgPic(m_pRdbMgr->GetItemByName((LPTSTR)(LPCTSTR)str));
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
	m_bAnimate = TRUE;
	SetTimer((HWND)m_ID, TID_WND_CNGBKG, 16, nullptr);
	OnTimer(TID_WND_CNGBKG, NULL);
	if (m_BkgColor != nullptr)
	{
		m_BkgColor->SetValue(BackgrdColor);
	}
	else
	{
		m_BkgColor = new Color(BackgrdColor);
	}
	DrawWnd();
	return TRUE;
}

BOOL DUI_Window::SetTitle(LPTSTR Title, BOOL bInner)
{
	//LPWSTR str = new WCHAR[255];
	//GetWindowText(m_hWnd, str, 255);
	m_Title->string->SetString(Title);
	if (bInner)
	{
		SendMessage((HWND)m_ID, WM_SETTEXT, NULL, (LPARAM)Title);
	}
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
	SetWindowRgn((HWND)m_ID, hRgn, FALSE);
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

DUI_ControlBase* DUI_Window::FindControlByID(INT ID)
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
			return *it;
		}
	}
	return nullptr;
}

DUI_ControlBase * DUI_Window::FindControlByPoint(PointF * pt)
{
	if (pt == nullptr)
	{
		return nullptr;
	}
	for (auto it = m_Controls->rbegin(); it != m_Controls->rend(); ++it)
	{
		if ((*it)->IsPtInCtrl(pt))
		{
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

BOOL DUI_Window::GetDebugMode()
{
	return m_bDebug;
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

INT DUI_Window::ScreenToClient(PointF * pt)
{
	POINT p;
	p = { (INT)pt->X ,(INT)pt->Y };
	int ret = ::ScreenToClient((HWND)m_ID, &p);
	pt->X = (REAL)p.x;
	pt->Y = (REAL)p.y;
	return ret;
}

BOOL DUI_Window::GetCursorPos(PointF * pt)
{
	BOOL ret = FALSE;
	POINT ptTemp;
	ret = ::GetCursorPos(&ptTemp);
	pt->X = (REAL)ptTemp.x;
	pt->Y = (REAL)ptTemp.y;
	ScreenToClient(pt);
	return ret;
}

RectF * DUI_Window::GetClientRect()
{
	return new RectF(2, TITLEBARHEIGHT, m_WndRect->Width - 4, m_WndRect->Height - TITLEBARHEIGHT - 2);
	//return new RectF(2, 2, m_WndRect->Width - 4, m_WndRect->Height - 4);
}

REAL DUI_Window::GetMarginTop()
{
	return TITLEBARHEIGHT;
}

BOOL DUI_Window::OnMouseMove(WPARAM wParam, Point* ptMouse)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.dwFlags = TME_HOVER | TME_LEAVE;
	tme.dwHoverTime = 10;
	tme.hwndTrack = (HWND)m_ID;
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
		SendMessage((HWND)m_ID, WM_NCLBUTTONDOWN, HTCAPTION, NULL);
		SendMessage((HWND)m_ID, WM_LBUTTONUP, wParam, MAKELPARAM(ptMouse->X, ptMouse->Y));
	}
	else if (m_Sizeable && m_CurCDR != CDR_Normal)
	{
		SendMessage((HWND)m_ID, WM_LBUTTONUP, wParam, MAKELPARAM(ptMouse->X, ptMouse->Y));
		switch (m_CurCDR)
		{
		case CDR_NWSE:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, NULL);
			break;
		case CDR_NS:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, NULL);
			break;
		case CDR_NESW:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, NULL);
			break;
		case CDR_WE:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, NULL);
			break;
		case CDR_NWSE2:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, NULL);
			break;
		case CDR_NS2:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, NULL);
			break;
		case CDR_NESW2:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, NULL);
			break;
		case CDR_WE2:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, NULL);
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
	if (IsIconic((HWND)m_ID))
	{
		return FALSE;
	}

	m_bAllowCtrlUpdate = FALSE;
	if (m_WndRect == nullptr)
	{
		m_WndRect = new Gdiplus::RectF;
	}


	
	if (wParam == NULL)
	{
		RECT rect;
		GetWindowRect((HWND)m_ID, &rect);
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
	SetWindowRgn((HWND)m_ID, hRgn, FALSE);
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
		m_SysBtn[BT_Close]->SetExtraInfo(BT_Close);
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
		m_SysBtn[BT_Minimize]->SetExtraInfo(BT_Minimize);
		m_SysBtn[BT_Minimize]->SetPrompt(_T("最小化"));
		SetWindowLong((HWND)m_ID, GWL_STYLE, GetWindowLong((HWND)m_ID, GWL_STYLE) | WS_MINIMIZEBOX);
		m_SysBtnCnt++;
	}

	if (GetSizeable())
	{
		if (m_SysBtn[BT_Maxmize] == nullptr)
		{
			m_SysBtn[BT_Maxmize] = new DUI_ImageButton;
			Size_X = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_X"));
			Size_Y = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Y"));
			if (IsZoomed((HWND)m_ID))
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Restore_Start"));
			}
			else
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Start"));
			}
			m_SysBtn[BT_Maxmize]->Create(this, 0, 0, (REAL)Size_X, (REAL)Size_Y, m_SysBtnPic, Start);
			m_SysBtn[BT_Maxmize]->m_bAutoReleaseImg = FALSE;
			m_SysBtn[BT_Maxmize]->SetExtraInfo(BT_Maxmize);
			SetWindowLong((HWND)m_ID, GWL_STYLE, GetWindowLong((HWND)m_ID, GWL_STYLE) | WS_MAXIMIZEBOX);
			m_SysBtnCnt++;
		}
		m_SysBtn[BT_Maxmize]->SetPrompt(IsZoomed((HWND)m_ID) ? _T("还原") : _T("最大化"));
	}
	else
	{
		if (m_SysBtn[BT_Maxmize] != nullptr)
		{
			SafeDelete(m_SysBtn[BT_Maxmize]);
		}
	}

	REAL BtnWith = 0;
	for (int i = BT_Close; i < BT_None; i++)
	{
		if (m_SysBtn[i] != nullptr&&m_SysBtn[i]->m_bVisialbe)
		{
			if (!m_SysBtn[i]->GetbShowOnNCRgn())
			{
				m_SysBtn[i]->SetbShowOnNCRgn(TRUE);
			}
			m_SysBtn[i]->SetMsgProc(m_SysBtnClickProcAddr);
			BtnWith += m_SysBtn[i]->m_LogicRect->Width;
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
		m_Title->rect->X = /*m_ClientRect->GetLeft() +*/ m_IconRect->GetRight() + GAP_ICON_TITLE;
		m_Title->rect->Y = /*m_ClientRect->GetTop() +*/ m_Sizeable ? 3.0f : 0.0f;
		m_Title->rect->Width = m_ClientRect->Width - m_IconRect->GetRight() - GAP_ICON_TITLE - BtnWith - GAP_TITLE_SYSBTN;
		m_Title->rect->Height = TITLEBARHEIGHT - 5 - 1;
	}
	else
	{
		m_Title->rect->X = /*m_ClientRect->GetLeft() +*/ 2;
		m_Title->rect->Y = /*m_ClientRect->GetTop() +*/ m_Sizeable ? 2.0f : 0.0f;
		m_Title->rect->Width = m_ClientRect->Width - BtnWith - 2 - GAP_TITLE_SYSBTN;
		m_Title->rect->Height = TITLEBARHEIGHT;
	}

	if (m_bInited)
	{
		m_bAllowCtrlUpdate = TRUE;
	}
	//DrawWnd();
	Update();
	return TRUE;
}

BOOL DUI_Window::OnMove(WPARAM wParam, LPARAM lParam)
{
	if (wParam == NULL)
	{
		RECT rect;
		GetWindowRect((HWND)m_ID, &rect);
		m_WndRect->X = (REAL)rect.left;
		m_WndRect->Y = (REAL)rect.top;
	}
	else
	{
		m_WndRect->X = LOWORD(lParam);//(REAL)rect.left;
		m_WndRect->Y = HIWORD(lParam);//(REAL)rect.top;
	}
	return TRUE;
}

BOOL DUI_Window::OnPaint(WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint((HWND)m_ID, &ps);
	//HDC hDC = m_Graphics->GetHDC();
	//m_MemDC->BitBlt(hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height,0, 0, SRCCOPY);
	Update();
	//m_Graphics->ReleaseHDC(hDC);
	EndPaint((HWND)m_ID, &ps);
	return TRUE;
}

BOOL DUI_Window::OnUpdate(WPARAM wParam, BOOL bUpdate)
{
	if (wParam == NULL)
	{
		DrawWnd();
		OnControl(INVALID_CONTROLID, WM_UPDATE, wParam, FALSE);
	}
	else
	{
		OnControl(wParam, WM_UPDATE, wParam, bUpdate);
		return TRUE;
	}
// 	else
// 	{
// 		DUI_ControlBase* pCtrl = FindControlByID((INT)wParam);
// 		if (pCtrl != nullptr)
// 		{
// // 			RectF* rect = pCtrl->m_Rect->Clone();
// // 			if (pCtrl->m_Parent->GetObjType() == ObjType_Ctrl)
// // 			{
// // 				rect->Intersect(*((DUI_ControlBase*)pCtrl->m_Parent)->m_Rect);
// // 			}
// // 			m_MemDC->SelectRectClipRgn(rect);
// 
// 
// 			m_BkgDC->BitBlt(m_MemDC, (INT)pCtrl->m_VRect->X, (INT)pCtrl->m_VRect->Y, (INT)pCtrl->m_VRect->Width, (INT)pCtrl->m_VRect->Height);
// 			RectF* rfRect = nullptr;
// 			for (auto it = m_Controls->begin(); it != m_Controls->end(); it++)
// 			{
// 				if (!(*it)->m_bVisialbe)
// 				{
// 					continue;
// 				}
// 				if (*it == pCtrl)
// 				{
// 					pCtrl->m_MemDC->AlphaBlend(m_MemDC, (int)pCtrl->m_VRect->GetLeft(), (int)pCtrl->m_VRect->GetTop(), (int)pCtrl->m_VRect->Width, (int)pCtrl->m_VRect->Height, INT(pCtrl->m_VRect->X - pCtrl->m_Rect->X), INT(pCtrl->m_VRect->Y - pCtrl->m_Rect->Y), (int)pCtrl->m_VRect->Width, (int)pCtrl->m_VRect->Height, (*it)->m_Alpha);
// 					continue;
// 				}
// 				rfRect = pCtrl->m_VRect->Clone();
// 				if (rfRect->Intersect(*(*it)->m_VRect))
// 				{
// 					(*it)->m_MemDC->AlphaBlend(m_MemDC, (int)rfRect->GetLeft(), (int)rfRect->GetTop(), (int)rfRect->Width, (int)rfRect->Height, INT(rfRect->X - (*it)->m_VRect->X), INT(rfRect->Y - (*it)->m_VRect->Y), (int)rfRect->Width, (int)rfRect->Height, (*it)->m_Alpha);
// 				}
// 				delete rfRect;
// 
// 
// 
// 
// // 				if ((*it)->m_Rect->IntersectsWith(*pCtrl->m_Rect))
// // 				{
// // 					(*it)->m_MemDC->AlphaBlend(m_MemDC->GetMemDC(), (int)(*it)->m_Rect->GetLeft(), (int)(*it)->m_Rect->GetTop(), (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, 0, 0, (int)(*it)->m_Rect->Width, (int)(*it)->m_Rect->Height, (*it)->m_Alpha);
// // 				}
// 			}
// // 			delete rect;
// // 			m_MemDC->SelectClipRgn();
// 		}
// 	}


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
			UpdateLayeredWindow((HWND)m_ID, m_hDC, nullptr, &szWnd, m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
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
		PointF ptMouse;
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
	switch (wParam)
	{
	case TID_WND_SHOW:
		if (WndAnimProc(m_pAnimArg, wParam))
		{
			KillTimer((HWND)m_ID, wParam);
			m_bAnimate = FALSE;
			m_bInited = TRUE;
			m_BorderStyle.Mode = BM_RoundRect;
			SetBorderStyle(m_BorderStyle);
			MsgProc(m_ID, WM_WNDINITED, NULL, NULL);
		}
		break;
	case TID_WND_CLOSE:
		if (WndAnimProc(m_pAnimArg, wParam))
		{
			KillTimer((HWND)m_ID, wParam);
			CallWindowProc(WNDPROC(PrevWndProc), (HWND)m_ID, WM_CLOSE, NULL, NULL);
			m_Alpha = 1;
			m_bAllowCtrlUpdate = TRUE;
			m_bAnimate = FALSE;
			m_BorderStyle.Mode = BM_RoundRect;
			SetBorderStyle(m_BorderStyle);
		}
		break;
	case TID_WND_CNGBKG:
		if (WndAnimProc(m_pAnimArg, wParam))
		{
			KillTimer((HWND)m_ID, wParam);
			m_bAnimate = FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL DUI_Window::OnShowWindow(WPARAM wParam, LPARAM lParam)
{
	if (m_Alpha == 0)
	{
		MsgProc(m_ID, WM_WNDINITED, NULL, NULL);
		return TRUE;
	}
	m_bAnimate = TRUE;
	m_BorderStyle.Mode = BM_Normal;
	SetBorderStyle(m_BorderStyle);
	SetTimer((HWND)m_ID, TID_WND_SHOW, 16, nullptr);
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
	m_bAllowCtrlUpdate = TRUE;
	OnControl(INVALID_CONTROLID, CM_SETAUTOUPDATE, NULL, TRUE);
	return TRUE;
}

BOOL DUI_Window::OnClose(WPARAM wParam, LPARAM lParam)
{

	if (m_Alpha == 0)
	{
		return FALSE;
	}
	m_bAllowCtrlUpdate = FALSE;
	m_bAnimate = TRUE;
	m_BorderStyle.Mode = BM_Normal;
	SetBorderStyle(m_BorderStyle);
	SetTimer((HWND)m_ID, TID_WND_CLOSE, 16, nullptr);
	return TRUE;
}

RDBManager* DUI_Window::GetRDBMgr()
{
	return m_pRdbMgr;
}

VOID DUI_Window::AddControl(DUI_ControlBase * pCtrl)
{
	m_Controls->push_back(pCtrl);
}

VOID DUI_Window::Flush()
{
	if (m_Alpha != 0)
	{
		SIZE szWnd;
		szWnd = { (INT)m_WndRect->Width, (INT)m_WndRect->Height };
		POINT ptSrc = { 0,0 }, ptDest = { (INT)m_WndRect->X,(INT)m_WndRect->Y };;
		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.BlendFlags = 0;
		bf.BlendOp = 0;
		bf.SourceConstantAlpha = m_Alpha;
		UpdateLayeredWindow((HWND)m_ID, m_hDC, &ptDest, &szWnd, m_MemDC->GetMemDC(), &ptSrc, NULL, &bf, ULW_ALPHA);
	}
	else
	{
		m_MemDC->AlphaBlend(m_hDC, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 0, 0, (INT)m_WndRect->Width, (INT)m_WndRect->Height, 255);
	}
}

HCURSOR DUI_Window::SetCursor(LPTSTR CursorName)
{
	return ::SetCursor(LoadCursor(NULL, CursorName));
}

HCURSOR DUI_Window::SetCursor(HCURSOR hCursor)
{
	return ::SetCursor(hCursor);
}

VOID DUI_Window::SetAllowCtrlUpdate(BOOL bAllow)
{
	m_bAllowCtrlUpdate = bAllow;
}

REAL DUI_Window::GetX()
{
	return m_WndRect->X;
}

REAL DUI_Window::GetY()
{
	return m_WndRect->Y;
}

REAL DUI_Window::GetWidth()
{
	return m_WndRect->Width;
}

REAL DUI_Window::GetHeight()
{
	return m_WndRect->Height;
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

VOID DUI_Window::AcceptDropFile(BOOL bAccept)
{
	DragAcceptFiles((HWND)m_ID, bAccept);
}

BOOL DUI_Window::SysBtnClick(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == CM_CLICKED)
	{
		DUI_ImageButton* pBtn = (DUI_ImageButton*)pThis;
		switch (pBtn->GetExtraInfo())
		{
		case BT_Close:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_CLOSE, 0);
			break;
		case BT_Minimize:
			SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			break;
		case BT_Maxmize:
			INT Start;
			if (IsZoomed((HWND)m_ID))
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Max_Start"));
				//117
				SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_RESTORE, 0);
				//ShowWindowAsync(m_hWnd, SW_RESTORE);
			}
			else
			{
				Start = m_pRdbMgr->GetIntValByName(_T("SysBtn_Restore_Start"));
				//201
				SendMessage((HWND)m_ID, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				//ShowWindowAsync(m_hWnd, SW_MAXIMIZE);
			}
			pBtn->SetImages(m_SysBtnPic, Start);
			break;
		default:
			break;
		}
		PointF ptMouse;
		GetCursorPos(&ptMouse);
		OnControl(INVALID_CONTROLID, WM_MOUSEMOVE, NULL, MAKELPARAM(ptMouse.X, ptMouse.Y));
	}
	return TRUE;
}

BOOL DUI_Window::WndAnimProc(AnimArg * pArg, INT AnimType)
{
	BOOL bDone = TRUE;
	pArg->Alpha = m_Alpha;
	switch (AnimType)
	{
	case TID_WND_SHOW:
 		bDone = WndAnim_Pop_Show(m_pAnimArg, m_WndRect, m_MemDC);
// 		bDone = WndAnim_Shade_Show(m_pAnimArg, m_WndRect, m_MemDC);
//		bDone = WndAnim_QQ_Show(m_pAnimArg, m_WndRect, m_MemDC);
		break;
	case TID_WND_CLOSE:
 		bDone = WndAnim_Pop_Hide(m_pAnimArg, m_WndRect, m_MemDC);
// 		bDone = WndAnim_Shade_Hide(m_pAnimArg, m_WndRect, m_MemDC);
//		bDone = WndAnim_QQ_Hide(m_pAnimArg, m_WndRect, m_MemDC);
		break;
	case TID_WND_CNGBKG:
	{
		StartPauseDebug();
		if (pArg->pDC1 == nullptr)
		{
			pArg->pDC1 = new MemDC((INT)m_WndRect->Width, (INT)m_WndRect->Height);
			pArg->pDC2 = new MemDC((INT)m_WndRect->Width, (INT)m_WndRect->Height);
			m_BkgDC->BitBlt(pArg->pDC2);
			pArg->Arg_1 = m_Alpha;
			return FALSE;
		}
		pArg->szWnd = { (INT)m_WndRect->Width, (INT)m_WndRect->Height };
		pArg->ptSrc = { 0,0 };
		pArg->ptDest = { (INT)m_WndRect->X,(INT)m_WndRect->Y };
		pArg->pDC1->Clear();
		pArg->Arg_1 -= 10;
		if (pArg->Arg_1 <= 0)
		{
			pArg->Arg_1 = 0;
		}
		m_BkgDC->AlphaBlend(pArg->pDC1, 0, 0, 0, 0, -1, -1, 0, 0, 255 /*- pArg->Arg_1*/);
		pArg->pDC2->AlphaBlend(pArg->pDC1, 0, 0, 0, 0, -1, -1, 0, 0, pArg->Arg_1);
		
 		

		pArg->pDC1->BitBlt(m_MemDC);
		//未知原因，无法直接贴到BkgDC上，所以采取替换BkgDC
		//pArg->pDC1->AlphaBlend(m_BkgDC);
		pArg->Arg_2 = (INT)m_BkgDC;
		m_BkgDC = pArg->pDC1;
 		OnControl(INVALID_CONTROLID, WM_UPDATE, NULL, FALSE);
		m_BkgDC = (MemDC*)pArg->Arg_2;
 		m_MemDC->BitBlt(pArg->pDC1);
		if (pArg->Arg_1 == 0)
		{
			bDone = TRUE;
		}
		else
		{
			bDone = FALSE;
		}
	
	}
		break;
	}
	if (pArg->pDC1 != nullptr)
	{
		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.BlendFlags = 0;
		bf.BlendOp = 0;
		bf.SourceConstantAlpha = m_pAnimArg->Alpha;
		UpdateLayeredWindow((HWND)m_ID, m_hDC, &m_pAnimArg->ptDest, &m_pAnimArg->szWnd, pArg->pDC1->GetMemDC(), &m_pAnimArg->ptSrc, NULL, &bf, ULW_ALPHA);
	}
	if (bDone)
	{
		SafeDelete(pArg->pDC1);
		pArg->Arg_1 = pArg->Arg_2 = 0;
	}
	return bDone;
}

BOOL DUI_Window::OnControl(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_Controls->size() == 0)
	{
		return FALSE;
	}
	PointF ptMouse;
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
			PointF* ptClient = new PointF(ptMouse);
			pCtrl->WndToClient(ptClient);
			lParam = MAKELPARAM(ptClient->X, ptClient->Y);
			delete ptClient;
		}
		return pCtrl->MsgProc(ID, uMsg, wParam, lParam);
	}

	//发送给所有控件的消息

	if (uMsg == WM_UPDATE)
	{
		for (auto it = m_Controls->begin(); it != m_Controls->end(); ++it)
		{
			if (!(*it)->m_bVisialbe) continue;
			(*it)->MsgProc((*it)->m_ID, uMsg, wParam, lParam);
		}
		return FALSE;
	}

	if (m_CaptureCtrlID != INVALID_CONTROLID)
	{
		BOOL bInCtrl = FALSE;
		pCtrl = FindControlByID(m_CaptureCtrlID);
		bInCtrl = pCtrl->IsPtInCtrl(&ptMouse);
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
				break;
			case WM_LBUTTONUP:
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
					pCtrl->MsgProc(pCtrl->m_ID, CM_MOUSELEAVE, pCtrl->m_ID, NULL);
					MsgProc(m_ID, WM_MOUSEMOVE, wParam, lParam);
				}
				break;
			default:
				break;
			}
			PointF* ptClient = new PointF(ptMouse);
			pCtrl->WndToClient(ptClient);
			lParam = MAKELPARAM(ptClient->X, ptClient->Y);
			delete ptClient;
		}
		else if (uMsg == WM_SETCURSOR)
		{
			if (bInCtrl)
			{
				pCtrl->MsgProc(pCtrl->m_ID, CM_SETCURSOR, pCtrl->m_ID, NULL);
				return TRUE;
			}
		}
		return pCtrl->MsgProc(pCtrl->m_ID, uMsg, wParam, lParam);
	}


	BOOL bInCtrl = FALSE, bFound = FALSE, Ret;
	for (auto it = m_Controls->rbegin(); it != m_Controls->rend(); ++it)
	{
		StartPauseDebug();
		if ((*it)->m_bVisialbe == FALSE) continue;
		//PauseWhen((*it)->m_ID == 14);
		bInCtrl = (*it)->IsPtInCtrl(&ptMouse);
		if (IsMouseMsg(uMsg))
		{
			if (bInCtrl)
			{
				bFound = TRUE;
				PointF* ptClient = new PointF(ptMouse);
				(*it)->WndToClient(ptClient);
				lParam = MAKELPARAM(ptClient->X, ptClient->Y);
				delete ptClient;
				Ret = (*it)->MsgProc((*it)->m_ID, uMsg, wParam, lParam);
			}
			switch (uMsg)
			{
			case WM_MOUSEMOVE:
				if (m_LastHoverCtrlID != (*it)->m_ID && bInCtrl)
				{
					if (m_LastHoverCtrlID != INVALID_CONTROLID)
					{
						DUI_ControlBase* p = FindControlByID(m_LastHoverCtrlID);
						p->MsgProc(m_LastHoverCtrlID, CM_MOUSELEAVE, wParam, NULL);
						p->ChangeState(S_Normal);
					}
					(*it)->MsgProc((*it)->m_ID, CM_MOUSEGETIN, NULL, NULL);
					(*it)->ChangeState(S_HighLight);
					m_LastHoverCtrlID = (*it)->m_ID;
				}
				break;
			case WM_LBUTTONDOWN:
				if (bInCtrl)
				{
					m_bMouseDown = TRUE;
					SetCapture((HWND)m_ID);
					m_CaptureCtrlID = (*it)->m_ID;
					//m_LastHoverCtrlID = m_CaptureCtrlID;
					(*it)->ChangeState(S_Pushed);
				}
				break;
			default:
				break;
			}
			if (bInCtrl)
			{
				return Ret;
			}
		}
		else if (uMsg == WM_SETCURSOR)
		{
			if (bInCtrl)
			{
				(*it)->MsgProc((*it)->m_ID, CM_SETCURSOR, (*it)->m_ID, NULL);
				return TRUE;
			}
		}
		else
		{
			(*it)->MsgProc((*it)->m_ID, uMsg, wParam, lParam);
		}
	}

	if (IsMouseMsg(uMsg))//bFound一定为假
	{
		//标识在窗口上，而非控件上。
		if (uMsg == WM_LBUTTONDOWN)
		{
			m_CaptureCtrlID = INVALID_CONTROLID;
		}
		else if (uMsg == WM_MOUSEMOVE && m_LastHoverCtrlID != INVALID_CONTROLID)
		{
			pCtrl = FindControlByID(m_LastHoverCtrlID);
			if (pCtrl == nullptr)
			{
				m_LastHoverCtrlID = INVALID_CONTROLID;
				return FALSE;
			}
			pCtrl->MsgProc(pCtrl->m_ID, CM_MOUSELEAVE, pCtrl->m_ID, NULL);
			pCtrl->ChangeState(S_Normal);
			m_LastHoverCtrlID = INVALID_CONTROLID;
		}

	}
	return FALSE;
}