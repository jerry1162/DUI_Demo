#pragma once
#include "stdafx.h"
#include "Functions.h"

class DUI_Window
{
public:
	DUI_Window();
	~DUI_Window();
	BOOL Create(HWND hWnd);
private:
	HWND m_hWnd;
	MemGraphics m_MemGraphics;
	Graphics m_Graphics;
	RectF m_WndRect;
};