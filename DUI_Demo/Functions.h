#pragma once
#include "stdafx.h"

void GdipStartup();
void GdipShutdown();

class MemDC
{
public:
	MemDC(int Width, int Height);
	MemDC();
	~MemDC();
	BOOL Create(int Width, int Height);
	BOOL Destroy();
	BOOL BitBlt(HDC hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc,
		int nYSrc, DWORD dwRop);
	BOOL AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest,
		int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc,
		BYTE Alpha);
	HDC GetMemDC();

	Graphics* graphics;
private:
	HDC m_MemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_Width, m_Height;
};