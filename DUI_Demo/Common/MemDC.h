#pragma once
#include "Functions.h"
class MemDC
{
public:
	MemDC(int Width, int Height);
	MemDC();
	~MemDC();
	BOOL Create(int Width, int Height);
	BOOL Destroy();
	BOOL BitBlt(HDC hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc = -1,
		int nYSrc = -1, DWORD dwRop = SRCCOPY);
	BOOL BitBlt(MemDC* hDestDC, int nXDest = 0, int nYDest = 0, int wDest = 0, int hDest = 0, int nXSrc = -1, int nYSrc = -1, DWORD dwRop = SRCCOPY);
	BOOL AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest,
		int hHeightDest, int nXOriginSrc = -1, int nYOriginSrc = -1, int nWidthSrc = 0, int nHeightSrc = 0, BYTE Alpha = 255);
	BOOL AlphaBlend(MemDC* hDestDC, int nXOriginDest = 0, int nYOriginDest = 0, int nWidthDest = 0,int hHeightDest = 0, int nXOriginSrc = -1, int nYOriginSrc = -1, int nWidthSrc = 0, int nHeightSrc = 0, BYTE Alpha = 255);
	INT SelectClipRgn(HRGN hrgn = NULL);
	INT SelectRectClipRgn(INT X, INT Y, INT W, INT H);
	HDC GetMemDC();
	BOOL ReSize(int Width, int Height);
	VOID Clear();//Clear with transparent color
	Graphics* graphics;
private:
	HDC m_MemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_Width, m_Height;
};