#include "stdafx.h"
#include "DUI_Prompt.h"


DUI_Prompt::DUI_Prompt()
{
	m_hWnd = NULL;
	m_hParent = NULL;
}


DUI_Prompt::~DUI_Prompt()
{
	if (IsCreated())
	{
		Destroy();
	}
}

BOOL DUI_Prompt::Create(HWND hParent, LPTSTR Text, RectF* Rect)
{
	if (m_hWnd != NULL)
	{
		DestroyWindow(m_hWnd);
	}
	m_hParent = hParent;
	m_Rect = Rect;
	if (Text == nullptr)
	{
		return FALSE;
	}
	m_Text = Text;
	m_Rect = Rect;
	m_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_LAYERED, _T("tooltips_class32"), _T(""), TTS_ALWAYSTIP | TTS_NOPREFIX, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, m_hParent, nullptr, GetModuleHandle(nullptr), nullptr);
	if (!m_hWnd)
	{
		return FALSE;
	}
	SetWindowPos(m_hWnd, HWND_TOPMOST, NULL, NULL, NULL, NULL, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	TOOLINFO* Info = new TOOLINFO;
	Info->cbSize = sizeof(TOOLINFO);
	Info->uFlags = TTF_SUBCLASS;
	Info->hwnd = m_hParent;
	Info->hinst = GetModuleHandle(nullptr);
	Info->uId = NULL;
	Info->lpszText = Text;
	Info->rect.left = m_Rect->X;
	Info->rect.top = m_Rect->Y;
	Info->rect.right = m_Rect->X + m_Rect->Width;
	Info->rect.bottom = m_Rect->Y + m_Rect->Height;
	SendMessage(m_hWnd, TTM_ADDTOOL, NULL, (LPARAM)Info);
	SendMessage(m_hWnd, TTM_SETTIPBKCOLOR, RGB(255, 255, 191), (LPARAM)Info);
	SendMessage(m_hWnd, TTM_SETTIPTEXTCOLOR, 0, (LPARAM)Info);
	return TRUE;
}

BOOL DUI_Prompt::SetText(LPTSTR Text)
{
	if (!IsCreated())
	{
		return FALSE;
	}
	return Create(m_hParent, Text, m_Rect);;
}

BOOL DUI_Prompt::IsCreated()
{
	return m_hWnd != NULL;
}

VOID DUI_Prompt::Destroy()
{
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;
}
