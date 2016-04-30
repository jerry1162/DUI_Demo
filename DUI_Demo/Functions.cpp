#include "stdafx.h"
#include "Functions.h"

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;
 
void GdipStartup()
{
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

void GdipShutdown()
{
	GdiplusShutdown(gdiplusToken);
}

MemGraphics::~MemGraphics()
{
	Destroy();
}

BOOL MemGraphics::Create(int Width, int Height)
{
	m_MemDC = CreateCompatibleDC(0);
	if (0==m_MemDC)
	{
		return TRUE;
	}
	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = 40;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biWidth = Width;
	bmpInfo.bmiHeader.biHeight = Height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biCompression = 0;
	m_hBitmap = CreateDIBSection(m_MemDC, &bmpInfo, 0, 0, 0, 0);
	if (0 == m_hBitmap)
	{
		DeleteDC(m_MemDC);
		return FALSE;
	}
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);
	if (m_Graphics.FromHDC(m_MemDC) != 0)
	{
		SelectObject(m_MemDC, m_hOldBitmap);
		DeleteObject(m_hBitmap);
		DeleteDC(m_MemDC);
		return FALSE;
	}
	m_Width = Width;
	m_Height = Height;
	return TRUE;
}

BOOL MemGraphics::Destroy()
{
	if (m_MemDC==0)
	{
		return FALSE;
	}
	SelectObject(m_MemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	DeleteDC(m_MemDC);
	m_Graphics.~Graphics();
	return 0;
}

BOOL MemGraphics::BitBlt(HDC hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc,
	int nYSrc, DWORD dwRop)
{
	int ret;
	ret=::BitBlt(hDestDC, nXDest, nYDest, wDest, hDest, m_MemDC, nXSrc, nYSrc, dwRop);
	return (ret==0?FALSE:TRUE);
}

BOOL MemGraphics::AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest,
	int nWidthDest, int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc,
	int nHeightSrc, BYTE Alpha = 255)
{
	int ret;
	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = Alpha;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	ret = ::AlphaBlend(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, hHeightDest,
		m_MemDC, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, blendFunction);
	return (ret == 0 ? FALSE : TRUE);
}

HDC MemGraphics::GetMemDC()
{
	return m_MemDC;
}
