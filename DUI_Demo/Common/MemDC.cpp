#include "stdafx.h"
#include "MemDC.h"
MemDC::MemDC(int Width, int Height)
{
	m_MemDC = NULL;
	m_hBitmap = NULL;
	m_hOldBitmap = NULL;
	m_Width = m_Height = 0;
	graphics = nullptr;
	Create(Width, Height); 
}

MemDC::MemDC()
{
	m_MemDC = NULL;
	m_hBitmap = NULL;
	m_hOldBitmap = NULL;
	m_Width = m_Height = 0;
	graphics = nullptr;
}

MemDC::~MemDC()
{
	Destroy();
}

BOOL MemDC::Create(int Width, int Height)
{
	m_MemDC = CreateCompatibleDC(0);
	if (NULL == m_MemDC)
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
	if (NULL == m_hBitmap)
	{
		DeleteDC(m_MemDC);
		return FALSE;
	}
	m_hOldBitmap = (HBITMAP)SelectObject(m_MemDC, m_hBitmap);

	graphics = Graphics::FromHDC(m_MemDC);
	if (graphics == NULL)
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

BOOL MemDC::Destroy()
{
	if (m_MemDC == NULL)
	{
		return FALSE;
	}
	SelectObject(m_MemDC, m_hOldBitmap);
	DeleteObject(m_hBitmap);
	delete graphics;
	DeleteDC(m_MemDC);
	m_MemDC = NULL;
	return TRUE;
}

BOOL MemDC::BitBlt(HDC hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc,
	int nYSrc, DWORD dwRop)
{
	int ret;
	if (nXSrc == -1)
	{
		nXSrc = nXDest;
	}
	if (nYSrc == -1)
	{
		nYSrc = nYDest;
	}
	ret=::BitBlt(hDestDC, nXDest, nYDest, wDest, hDest, m_MemDC, nXSrc, nYSrc, dwRop);
	return (ret==0?FALSE:TRUE);
}

BOOL MemDC::BitBlt(MemDC * hDestDC, int nXDest, int nYDest, int wDest, int hDest, int nXSrc, int nYSrc, DWORD dwRop)
{
	int ret;
	if (hDestDC == nullptr)
	{
		return FALSE;
	}
	if (nXSrc == -1)
	{
		nXSrc = nXDest;
	}
	if (nYSrc == -1)
	{
		nYSrc = nYDest;
	}
	if (wDest == 0)
	{
		wDest = hDestDC->m_Width;
	}
	if (hDest == 0)
	{
		hDest = hDestDC->m_Height;
	}
	ret = ::BitBlt(hDestDC->GetMemDC(), nXDest, nYDest, wDest, hDest, m_MemDC, nXSrc, nYSrc, dwRop);
	return (ret == 0 ? FALSE : TRUE);
}

BOOL MemDC::AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest,
	int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc,
	BYTE Alpha)
{
	int ret;
	if (nXOriginSrc == -1)
	{
		nXOriginSrc = nXOriginDest;
	}
	if (nYOriginSrc == -1)
	{
		nYOriginSrc = nYOriginDest;
	}
	if (nWidthSrc == 0)
	{
		nWidthSrc = nWidthDest;
	}
	if (nHeightSrc == 0)
	{
		nHeightSrc = nWidthDest;
	}
	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = Alpha;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	ret = ::AlphaBlend(hdcDest, nXOriginDest, nYOriginDest, nWidthDest, hHeightDest,
		m_MemDC, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, blendFunction);
	return (ret == 0 ? FALSE : TRUE);
}

BOOL MemDC::AlphaBlend(MemDC * hDestDC, int nXOriginDest, int nYOriginDest, int nWidthDest, int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, BYTE Alpha)
{
	int ret;
	if (hDestDC == nullptr)
	{
		return FALSE;
	}
	if (nWidthDest == 0)
	{
		nWidthDest = hDestDC->m_Width;
	}
	if (hHeightDest == 0)
	{
		hHeightDest = hDestDC->m_Height;
	}
	if (nXOriginSrc == -1)
	{
		nXOriginSrc = nXOriginDest;
	}
	if (nYOriginSrc == -1)
	{
		nYOriginSrc = nYOriginDest;
	}
	if (nWidthSrc == 0)
	{
		nWidthSrc = nWidthDest;
	}
	if (nHeightSrc == 0)
	{
		nHeightSrc = hHeightDest;
	}
	BLENDFUNCTION blendFunction;
	blendFunction.BlendOp = AC_SRC_OVER;
	blendFunction.BlendFlags = 0;
	blendFunction.SourceConstantAlpha = Alpha;
	blendFunction.AlphaFormat = AC_SRC_ALPHA;
	ret = ::AlphaBlend(hDestDC->GetMemDC(), nXOriginDest, nYOriginDest, nWidthDest, hHeightDest,m_MemDC, nXOriginSrc, nYOriginSrc, nWidthSrc, nHeightSrc, blendFunction);
	return (ret == 0 ? FALSE : TRUE);
}

INT MemDC::SelectClipRgn(HRGN hrgn)
{
	return ::SelectClipRgn(m_MemDC,hrgn);
}

INT MemDC::SelectRectClipRgn(INT X, INT Y, INT W, INT H)
{
	HRGN hRgn = CreateRectRgn(X, Y, X + W, Y + H);
	INT Ret = SelectClipRgn(hRgn);
	DeleteObject(hRgn);
	return Ret;
}

INT MemDC::SelectRectClipRgn(RectF* pRect)
{
	if (pRect == nullptr)
	{
		return SelectClipRgn();
	}
	else
	{
		return SelectRectClipRgn((INT)pRect->X, (INT)pRect->Y, (INT)pRect->Width, (INT)pRect->Height);
	}

}

HDC MemDC::GetMemDC()
{
	return m_MemDC;
}

BOOL MemDC::ReSize(int Width, int Height)
{
	Destroy();
	return Create(Width, Height);
}

VOID MemDC::Clear()
{
	graphics->Clear(TRANSPARENT);
}