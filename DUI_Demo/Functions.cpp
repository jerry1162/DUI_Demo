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

BOOL PtInRect(RectF * rect, Point * pt)
{
	if (pt->X >= rect->GetLeft() && pt->Y >= rect->GetTop() && 
		pt->X <= rect->GetLeft() + rect->Width&&pt->Y <= rect->GetTop() + rect->Height)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL IntersectRectF(RectF * rect1, RectF * rect2)
{
	//IntersectRect()
	return 0;
}

VOID DrawShadow(Graphics * graphics, RectF * rect, INT diameter)
{
	graphics->SetSmoothingMode(SmoothingModeAntiAlias);
	GraphicsPath* path;
	path = new GraphicsPath;
	DrawPathRoundRect(path, (REAL)rect->GetLeft(), (REAL)rect->GetTop(), (REAL)rect->Width, (REAL)rect->Height, (REAL)diameter + 4);
	DrawPathRoundRect(path, (REAL)rect->GetLeft() - 4, (REAL)rect->GetTop() - 4, (REAL)rect->Width + 8, (REAL)rect->Height + 8, (REAL)diameter + 4);
	graphics->SetClip(path);
	delete path;
	path = new GraphicsPath;
	DrawPathRoundRect(path, (REAL)rect->GetLeft(), (REAL)rect->GetTop(), (REAL)rect->Width, (REAL)rect->Height, (REAL)diameter + 4);
	Color* color = new Color(Color::MakeARGB(150, 50, 50, 50));

	Gdiplus::Pen pen(*color, 9);
	graphics->DrawPath(&pen, path);

	color->SetValue(Color::MakeARGB(10, 255, 255, 255));
	pen.SetColor(*color);
	pen.SetWidth(7);
	graphics->DrawPath(&pen, path);

	color->SetValue(Color::MakeARGB(15, 255, 255, 255));
	pen.SetColor(*color);
	pen.SetWidth(5);
	graphics->DrawPath(&pen, path);

	color->SetValue(Color::MakeARGB(20, 255, 255, 255));
	pen.SetColor(*color);
	pen.SetWidth(3);
	graphics->DrawPath(&pen, path);

	color->SetValue(Color::MakeARGB(25, 255, 255, 255));
	pen.SetColor(*color);
	pen.SetWidth(1);
	graphics->DrawPath(&pen, path);

	delete color;
	delete path;
	graphics->ResetClip();
	graphics->SetSmoothingMode(SmoothingModeDefault);
}

BOOL DrawPathRoundRect(GraphicsPath* path, REAL left, REAL top, REAL width, REAL height, REAL round)
{
	path->AddArc(left, top, round, round, 180, 90);
	path->AddArc(left + width - round, top, round, round, 270, 90);
	path->AddArc(left + width - round, top + height - round, round, round, 0, 90);
	path->AddArc(left, top + height - round, round, round, 90, 90);
	return path->CloseFigure()==0;
}

BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * &pImg)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), sTR); // type
	if (!hRsrc)
		return FALSE;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return FALSE;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream
	pImg = new Image(pstm);
	// pImg = Gdiplus::Image::FromStream(pstm);
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	return TRUE;
}



VOID DrawShadowText(Graphics * graphics, REAL Rate, GdipString* Text, ARGB ShadowColor,
	ARGB BorderColor, REAL TextOffsetX, REAL TextOffsetY, REAL ShadowOffsetX,
	REAL ShadowOffsetY)
{
	if (Rate <= 0)
	{
		Rate = 1;
	}
	REAL width, height;
	width = Text->rect->Width / Rate;
	height = Text->rect->Height / Rate;
	Gdiplus::Bitmap bitmap1((INT)Text->rect->Width, (INT)Text->rect->Height);
	Graphics* graphics1;
	graphics1 = new Graphics(&bitmap1);
	SolidBrush* brush;
	if (ShadowColor != NULL)
	{
		brush = new SolidBrush(ShadowColor);
		RectF* OffsetRect = new RectF(TextOffsetX + ShadowOffsetX - 1,
			TextOffsetY + ShadowOffsetY, Text->rect->Width, Text->rect->Height);
		graphics1->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(TextOffsetX + ShadowOffsetX + 1,
			TextOffsetY + ShadowOffsetY, Text->rect->Width, Text->rect->Height);
		graphics1->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete brush;
		delete OffsetRect;

		OffsetRect = new RectF(TextOffsetX + ShadowOffsetX,
			TextOffsetY + ShadowOffsetY - 1, Text->rect->Width, Text->rect->Height);
		delete OffsetRect;

		Gdiplus::Bitmap bitmap2((INT)width, (INT)height);
		Graphics* graphics2;
		graphics2 = new Graphics(&bitmap2);
		graphics2->SetSmoothingMode(SmoothingModeAntiAlias);
		graphics2->SetCompositingQuality(CompositingQualityGammaCorrected);
		graphics2->SetInterpolationMode(InterpolationModeHighQualityBilinear);
		graphics2->SetPixelOffsetMode(PixelOffsetModeNone);
		graphics2->DrawImage(&bitmap1, 0.0, 0.0, width, height);
		graphics1->Clear(0);
		graphics1->DrawImage(&bitmap2, 0.0, 0.0, Text->rect->Width, Text->rect->Height);
		delete graphics2;
	}


	GdipString* string = new GdipString;
	string->color = Text->color;
	string->string = Text->string;
	string->font = Text->font;
	string->format = Text->format;
	string->rect = new RectF(TextOffsetX, TextOffsetY, Text->rect->Width,
		Text->rect->Height);
	DrawBorderedText(graphics1, string, BorderColor);
	delete string;

	graphics->DrawImage(&bitmap1, Text->rect->GetLeft(), Text->rect->GetTop());
	delete graphics1;
}

VOID DrawBorderedText(Graphics* graphics, GdipString* Text, ARGB BorderColor)
{
	SolidBrush* brush;
	if (BorderColor != NULL)
	{
		brush = new SolidBrush(BorderColor);
		RectF* OffsetRect = new RectF(Text->rect->GetLeft() - 1,
			Text->rect->GetTop(), Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft() + 1,
			Text->rect->GetTop(), Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft(), Text->rect->GetTop() + 1,
			Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft(), Text->rect->GetTop() - 1,
			Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		delete brush;
	}
	brush = new SolidBrush(*Text->color);
	//graphics->SetSmoothingMode(SmoothingModeAntiAlias);
	graphics->DrawString(Text->string->GetBuffer(), Text->string->GetLength(), Text->font,
		*Text->rect, Text->format, brush);
	delete brush;
}

INT NewID()
{
	static int id = 0;
	id += 1;
	return id;
}

BOOL IsMouseMsg(UINT uMsg)
{
	if (uMsg == WM_MOUSEMOVE || uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP ||
		uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MBUTTONDOWN ||
		uMsg == WM_MBUTTONUP || uMsg == WM_MOUSEWHEEL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}



MemDC::MemDC(int Width, int Height)
{
	Create(Width, Height);
}

MemDC::MemDC()
{

}

MemDC::~MemDC()
{
	Destroy();
}

BOOL MemDC::Create(int Width, int Height)
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
	if (m_MemDC==0)
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
	ret=::BitBlt(hDestDC, nXDest, nYDest, wDest, hDest, m_MemDC, nXSrc, nYSrc, dwRop);
	return (ret==0?FALSE:TRUE);
}

BOOL MemDC::AlphaBlend(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest,
	int hHeightDest, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc,
	BYTE Alpha = 255)
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