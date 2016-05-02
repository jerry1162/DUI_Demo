#pragma once
#include "stdafx.h"

struct GdipString
{
	CString* string;
	Gdiplus::Font* font;
	StringFormat* format;
	Color* color;
	RectF* rect;
};

void GdipStartup();
void GdipShutdown();
BOOL PtInRect(RectF* rect, Point* pt);
VOID DrawShadow(Graphics* graphics, RectF* rect, INT diameter);
BOOL DrawPathRoundRect(GraphicsPath* path, REAL left, REAL top, REAL width, REAL height, REAL round);
BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *  pImg);
VOID DrawShadowText(Graphics* graphics, REAL Rate, GdipString* Text,
	ARGB ShadowColor = NULL, ARGB BorderColor = NULL, REAL TextOffsetX = NULL, REAL TextOffsetY = NULL,
	REAL ShadowOffsetX = NULL, REAL ShadowOffsetY = NULL);
VOID DrawBorderedText(Graphics* graphics, GdipString* Text, ARGB BorderColor);

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
	BOOL ReSize(int Width, int Height);
	Graphics* graphics;
private:
	HDC m_MemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_Width, m_Height;
};