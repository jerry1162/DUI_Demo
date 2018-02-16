#include "stdafx.h"
#include "Functions.h"
#include "CommonControls.h"

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

Image* ImageFromIDResource(UINT resID, LPCTSTR resType)
{
	HINSTANCE hInst = GetModuleHandle(nullptr);//AfxGetResourceHandle();
	if (resType == RT_ICON)
	{
		HICON hIco = (HICON)LoadImage(hInst, MAKEINTRESOURCE(resID), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
		Bitmap* Ico = Bitmap::FromHICON(hIco);
		//TRACE("\nResult:%d\n", DestroyIcon(hIco));
		return Ico;
	}
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(resID), resType);
	if (!hRsrc)
		return NULL;
	// load resource into memory
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (!lpRsrc)
		return NULL;
	// Allocate global memory on which to create stream
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream
	Gdiplus::Image* ima = Gdiplus::Image::FromStream(pstm, TRUE);
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);

	return ima;
}

Image * ImageFromBin(LPVOID lpData, UINT uSize)
{
	if (lpData == nullptr || uSize == 0)
	{
		return nullptr;
	}
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, uSize);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpData, uSize);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	// load from stream
	Gdiplus::Image* ima = Gdiplus::Image::FromStream(pstm, TRUE);
	// free/release stuff
	GlobalUnlock(m_hMem);
	pstm->Release();
	return ima;
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
		graphics1->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(TextOffsetX + ShadowOffsetX + 1,
			TextOffsetY + ShadowOffsetY, Text->rect->Width, Text->rect->Height);
		graphics1->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
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
		graphics->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft() + 1,
			Text->rect->GetTop(), Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft(), Text->rect->GetTop() + 1,
			Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		OffsetRect = new RectF(Text->rect->GetLeft(), Text->rect->GetTop() - 1,
			Text->rect->Width, Text->rect->Height);
		graphics->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
			*OffsetRect, Text->format, brush);
		delete OffsetRect;

		delete brush;
	}
	brush = new SolidBrush(*Text->color);
	//graphics->SetSmoothingMode(SmoothingModeAntiAlias);
	graphics->DrawString(Text->string->GetString(), Text->string->GetLength(), Text->font,
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


//释放代理函数
void FreeCallBackAddr(LPVOID wndProc)
{
	VirtualFree(wndProc, 4096, MEM_RELEASE);
}

ATOM MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = NULL;//CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle(NULL);
	wcex.hIcon = NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DUI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DUI);
	wcex.lpszClassName = _T("DUI_Wnd");//szWindowClass;
	wcex.hIconSm = NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

HWND InitInstance()
{
	HWND hWnd = CreateWindowW(_T("DUI_Wnd"), _T("DUI_Wnd"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, GetModuleHandle(NULL), nullptr);

	if (!hWnd)
	{
		return NULL;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return hWnd;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

VOID MessageLoop()
{
	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

ATOM GetDefaultWndClass()
{
	static ATOM Class = MyRegisterClass();
	return Class;
}

Bitmap* GetBitmapFromHIcon(HICON hIcon, INT Width, INT Height, BOOL bDestroyIcon)
{
	if (hIcon == NULL)
	{
		return nullptr;
	}

	ICONINFO icInfo = { 0 };
	if (!::GetIconInfo(hIcon, &icInfo))
	{
		return nullptr;
	}

	BITMAP bitmap = { 0 };
	GetObject(icInfo.hbmColor, sizeof(BITMAP), &bitmap);

	Gdiplus::Bitmap* pBitmap = nullptr;
	Gdiplus::Bitmap* pWrapBitmap = nullptr;
	Bitmap* bmp = nullptr;
	if (bitmap.bmBitsPixel != 32)
	{
		bmp = Gdiplus::Bitmap::FromHICON(hIcon);
	}
	else
	{
		pWrapBitmap = Gdiplus::Bitmap::FromHBITMAP(icInfo.hbmColor, NULL);
		if (!pWrapBitmap)
			return nullptr;

		BitmapData bitmapData;
		Rect rcImage(0, 0, pWrapBitmap->GetWidth(), pWrapBitmap->GetHeight());
		pWrapBitmap->LockBits(&rcImage, ImageLockModeRead, pWrapBitmap->GetPixelFormat(), &bitmapData);
		pBitmap = new Bitmap(bitmapData.Width, bitmapData.Height, bitmapData.Stride, PixelFormat32bppARGB, (BYTE*)bitmapData.Scan0);
		pWrapBitmap->UnlockBits(&bitmapData);

		if (Width == -1)
		{
			Width = bitmapData.Width;
		}
		if (Height == -1)
		{
			Height = bitmapData.Height;
		}
		bmp= new Bitmap(Width, Height, NULL, PixelFormat32bppARGB, nullptr);
		Graphics* pGraphics = new Graphics(bmp);
		pGraphics->DrawImage(pBitmap, 0, 0, Width, Height);
		delete pGraphics;

	}

	if (pWrapBitmap)
		delete pWrapBitmap;
	if (pBitmap)
	{
		delete pBitmap;
	}
	DeleteObject(icInfo.hbmColor);
	DeleteObject(icInfo.hbmMask);
	if (bDestroyIcon)
	{
		DestroyIcon(hIcon);
	}
	return bmp;
}


//获取文件的图标HICON  
HICON GetFileIcon(const CString& strFilePath, INT IconSize)
{
	SHFILEINFO SHFI;
	ZeroMemory(&SHFI, sizeof(SHFI));
	if (IconSize < 0)
	{
		return NULL;
	}
	else if (IconSize <= IS_NORMAL)
	{
		DWORD_PTR ret = ::SHGetFileInfo(strFilePath, FILE_ATTRIBUTE_NORMAL, &SHFI, sizeof(SHFI),
			SHGFI_ICON | (IconSize == IS_NORMAL ? SHGFI_LARGEICON : SHGFI_SMALLICON));
		if (ret != 0)
		{
			return SHFI.hIcon;
		}
	}
	else if (IconSize <= IS_EXLARGE)
	{
		SHFILEINFO sfi;
		ZeroMemory(&sfi, sizeof(SHFILEINFO));
		::SHGetFileInfo(strFilePath, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);
		HIMAGELIST* imageList = nullptr;
		//SHIL_EXTRALARGE获取48 * 48的图标， SHIL_JUMBO 获取256 * 256的图标。
		HRESULT hResult = ::SHGetImageList(IconSize == IS_LARGE ? SHIL_EXTRALARGE : SHIL_JUMBO, IID_IImageList, (void**)&imageList);
		HICON icon_handle = NULL;
		if (hResult == S_OK)
		{
			hResult = ((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_NORMAL, &icon_handle);
			return icon_handle;
		}
	}
	return NULL;
}

//获取特定图片类型的编码  
INT GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT num = 0, size = 0;

	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure  

	Gdiplus::ImageCodecInfo* pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));

	Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);
	bool found = false;
	for (UINT ix = 0; !found && ix < num; ++ix)
	{
		if (_wcsicmp(pImageCodecInfo[ix].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[ix].Clsid;
			found = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return found;
}

//根据图标HICON保存图片为Png图片  
Bitmap* SaveHIconToPngFile(HICON hIcon, LPCTSTR lpszPicFileName)
{
	if (hIcon == NULL)
	{
		return nullptr;
	}

	ICONINFO icInfo = { 0 };
	if (!::GetIconInfo(hIcon, &icInfo))
	{
		return nullptr;
	}

	BITMAP bitmap;
	GetObject(icInfo.hbmColor, sizeof(BITMAP), &bitmap);

	Gdiplus::Bitmap* pBitmap = NULL;
	Gdiplus::Bitmap* pWrapBitmap = NULL;

	//do
	//{
		if (bitmap.bmBitsPixel != 32)
		{
			pBitmap = Gdiplus::Bitmap::FromHICON(hIcon);
		}
		else
		{
			pWrapBitmap = Gdiplus::Bitmap::FromHBITMAP(icInfo.hbmColor, NULL);
			//if (!pWrapBitmap)
				//break;

			Gdiplus::BitmapData bitmapData;
			Gdiplus::Rect rcImage(0, 0, pWrapBitmap->GetWidth(), pWrapBitmap->GetHeight());

			pWrapBitmap->LockBits(&rcImage, Gdiplus::ImageLockModeRead, pWrapBitmap->GetPixelFormat(), &bitmapData);
			pBitmap = new (Gdiplus::Bitmap)(bitmapData.Width, bitmapData.Height, bitmapData.Stride, PixelFormat32bppARGB, (BYTE*)bitmapData.Scan0);
			pWrapBitmap->UnlockBits(&bitmapData);
		}

		if (lpszPicFileName != nullptr)
		{
			CLSID encoderCLSID;
			GetEncoderClsid(_T("image/png"), &encoderCLSID);
			Gdiplus::Status st = pBitmap->Save(lpszPicFileName, &encoderCLSID, NULL);
		}

		//if (st != Gdiplus::Ok)
			//break;

	//} while (false);

	//delete pBitmap;
	if (pWrapBitmap)
		delete pWrapBitmap;
	DeleteObject(icInfo.hbmColor);
	DeleteObject(icInfo.hbmMask);

	return pBitmap;
}

INT GetFileName(CString csFilePath, CString& csFileName)
{
	int nPos = csFilePath.ReverseFind('\\'); // 文件路径，以'\'斜杠分隔的路径  
	csFileName = csFilePath.Right(csFilePath.GetLength() - nPos - 1); // 获取文件全名，包括文件名和扩展名  
	return csFileName.GetLength();
}

INT  GetExtName(CString csFileFullName, CString& csExtName)
{
	int nPos = csFileFullName.ReverseFind('.');
	csExtName = csFileFullName.Right(csFileFullName.GetLength() - nPos - 1); // 获取扩展名  
	return csExtName.GetLength();
}

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