#pragma once
#include "CallBack.hpp"
//#include <wingdi.h>
//#include <wingdi.h>  
//#pragma comment(lib, "MSIMG32.LIB")  

//��������  
//#define   WS_EX_LAYERED                       0x00080000   
//#define   LWA_COLORKEY                        0x00000001   
//#define   LWA_ALPHA                           0x00000002   

//#include <atlstr.h>
//#include <assert.h>

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
Image* ImageFromBin(LPVOID lpData, UINT uSize);
VOID DrawShadowText(Graphics* graphics, REAL Rate, GdipString* Text,
	ARGB ShadowColor = NULL, ARGB BorderColor = NULL, REAL TextOffsetX = NULL, REAL TextOffsetY = NULL,
	REAL ShadowOffsetX = NULL, REAL ShadowOffsetY = NULL);
VOID DrawBorderedText(Graphics* graphics, GdipString* Text, ARGB BorderColor);
INT NewID();
BOOL IsMouseMsg(UINT uMsg);

//�ͷŴ�����
void FreeCallBackAddr(LPVOID wndProc);

ATOM                MyRegisterClass();
HWND                InitInstance();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
VOID MessageLoop();
ATOM GetDefaultWndClass();
Bitmap* GetBitmapFromHIcon(HICON hIcon, INT Width = -1, INT Height = -1, BOOL bDestroyIcon = TRUE);
Bitmap* SaveHIconToPngFile(HICON hIcon, LPCTSTR lpszPicFileName = nullptr);
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
//��ȡ�ļ���ͼ��HICON  

#define IS_SMALL   0  //16*16
#define IS_NORMAL  1  //32*32
#define IS_LARGE   2  //48*48
#define IS_EXLARGE 3  //256*256
HICON GetFileIcon(const CString& strFilePath, INT IconSize = IS_NORMAL);

INT GetFileName(CString csFilePath,CString& csFileName);
INT  GetExtName(CString csFileFullName,CString& csExtName);

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