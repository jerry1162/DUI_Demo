#pragma once
#include "CallBack.hpp"

#define StartPauseDebug() BOOL __bPaused__=FALSE;
#define PAUSE() _asm int 3;__bPaused__=TRUE;
#define PauseWhen(X) if(X) __bPaused__=TRUE; if(X)_asm int 3
#define ExcuteAfterPaused(X) if(__bPaused__) X
#define SafeDelete(pObj) if (pObj != nullptr)\
{\
	delete pObj;\
	pObj=nullptr;\
}


//常量声明  
//#define   WS_EX_LAYERED                       0x00080000   
//#define   LWA_COLORKEY                        0x00000001   
//#define   LWA_ALPHA                           0x00000002   

struct GdipString
{
	CString* string;
	Gdiplus::Font* font;
	StringFormat* format;
	Color* color;
	RectF* rect;
	/*GdipString()
	{
		string = nullptr;
		font = nullptr;
		format = nullptr;
		color = nullptr;
		rect = nullptr;
	}
	~GdipString()
	{
		delete string;
		delete font;
		delete format;
		delete color;
		delete rect;
	}*/
};

void GdipStartup();
void GdipShutdown();
BOOL PtInRect(RectF* rect, Point* pt);
VOID DrawShadow(Graphics* graphics, RectF* rect, INT diameter);
BOOL DrawPathRoundRect(GraphicsPath* path, REAL left, REAL top, REAL width, REAL height, REAL round);
Image* ImageFromIDResource(UINT resID, LPCTSTR resType);
Image* ImageFromBin(LPVOID lpData, UINT uSize);
VOID DrawShadowText(Graphics* graphics, GdipString* Text, REAL Rate = 5,
	ARGB ShadowColor = Color::Black, ARGB BorderColor = Color::MakeARGB(100, 50, 50, 50), REAL TextOffsetX = 0, REAL TextOffsetY = 0,
	REAL ShadowOffsetX = NULL, REAL ShadowOffsetY = NULL);
VOID DrawBorderedText(Graphics* graphics, GdipString* Text, ARGB BorderColor = Color::MakeARGB(180, 50, 50, 50));
INT NewID();
BOOL IsMouseMsg(UINT uMsg);
BOOL IsControlMsg(UINT uMsg);

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

