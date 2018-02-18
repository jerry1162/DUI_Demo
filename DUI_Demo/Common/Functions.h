#pragma once
#include "CallBack.hpp"
//#include <wingdi.h>
//#include <wingdi.h>  
//#pragma comment(lib, "MSIMG32.LIB")  

//常量声明  
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

//释放代理函数
void FreeCallBackAddr(LPVOID wndProc);

ATOM                MyRegisterClass();
HWND                InitInstance();
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
VOID MessageLoop();
ATOM GetDefaultWndClass();
Bitmap* GetBitmapFromHIcon(HICON hIcon, INT Width = -1, INT Height = -1, BOOL bDestroyIcon = TRUE);
Bitmap* SaveHIconToPngFile(HICON hIcon, LPCTSTR lpszPicFileName = nullptr);
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
//获取文件的图标HICON  

#define IS_SMALL   0  //16*16
#define IS_NORMAL  1  //32*32
#define IS_LARGE   2  //48*48
#define IS_EXLARGE 3  //256*256
HICON GetFileIcon(const CString& strFilePath, INT IconSize = IS_NORMAL);

INT GetFileName(CString csFilePath,CString& csFileName);
INT  GetExtName(CString csFileFullName,CString& csExtName);