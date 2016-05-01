#pragma once
#include "stdafx.h"

void GdipStartup();
void GdipShutdown();

class MemGraphics : public Gdiplus::Graphics
{
public:
	MemGraphics(HWND hwnd, BOOL icm = FALSE);
	~MemGraphics();
	BOOL Create(int Width, int Height);
	BOOL Destroy();
	BOOL BitBlt(HDC hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc,
		int nYSrc, DWORD dwRop);
	BOOL AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest,
		int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc,
		BYTE Alpha);
	HDC GetMemDC();
private:
	HDC m_MemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_Width, m_Height;
};