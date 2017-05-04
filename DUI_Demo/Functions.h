#pragma once
#include "CallBack.hpp"
#include <wingdi.h>
#include <wingdi.h>  
#pragma comment(lib, "MSIMG32.LIB")  

//常量声明  
//#define   WS_EX_LAYERED                       0x00080000   
//#define   LWA_COLORKEY                        0x00000001   
//#define   LWA_ALPHA                           0x00000002   

#include <atlstr.h>
#include <assert.h>

#define ULONG_PTR ULONG 
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

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
Image* ImageFromIDResource(UINT resID, LPCTSTR resType);
VOID DrawShadowText(Graphics* graphics, REAL Rate, GdipString* Text,
	ARGB ShadowColor = NULL, ARGB BorderColor = NULL, REAL TextOffsetX = NULL, REAL TextOffsetY = NULL,
	REAL ShadowOffsetX = NULL, REAL ShadowOffsetY = NULL);
VOID DrawBorderedText(Graphics* graphics, GdipString* Text, ARGB BorderColor);
INT NewID();
BOOL IsMouseMsg(UINT uMsg);

//释放代理函数
void FreeCallBackAddr(LPVOID wndProc);

ATOM                MyRegisterClass();
HWND                InitInstance();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
VOID MessageLoop();
ATOM GetDefaultWndClass();

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
	VOID Clear();//Clear with transparent color
	Graphics* graphics;
private:
	HDC m_MemDC;
	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	int m_Width, m_Height;
};