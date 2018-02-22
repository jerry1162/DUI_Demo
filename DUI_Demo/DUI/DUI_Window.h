#pragma once
#include "..//RDBManager//RDBManager.h"
#include "DUI_ControlBase.h"
#include "DUI_Prompt.h"


class DUI_ImageButton;

class DUI_Window :public DUI_Object
{
public:
	friend class DUI_ControlBase;
	DUI_Window();
	~DUI_Window();
	BOOL Create(HWND hWnd, LPTSTR Title = L"", LPTSTR Icon = NULL,
		LPTSTR BackgrdPic = NULL, BOOL bSizeable = FALSE);
	BOOL Create(HWND hWnd, LPTSTR Title = L"", LPTSTR Icon = NULL,
		ARGB BkgColor = Color::MakeARGB(255, 240, 240, 240), BOOL bSizeable = FALSE);
	BOOL Create(INT Width = 380, INT Height = 250, DUI_Window* Parent = nullptr, LPTSTR Title = L"", LPTSTR Icon = NULL);//�����ô˷����������ں��ֶ�����DoModel()������Ϣѭ����
	VOID DoModel();

	//�麯��ʵ��
	virtual BOOL Destroy() override;
	virtual ObjType GetObjType() override;
	virtual BOOL CanBeParent();
	virtual REAL GetX() override;
	virtual REAL GetY() override;
	virtual REAL GetWidth() override;
	virtual REAL GetHeight() override;
	virtual BOOL GetCursorPos(Point* pt) override;
	virtual REAL GetMarginTop() override;
	virtual RectF* GetClientRect() override;


	BOOL SetBkgPic(LPTSTR BackgrdPic = NULL);
	BOOL SetBkgColor(ARGB BackgrdColor = NULL);
	BOOL SetTitle(LPTSTR Title, BOOL bInner = TRUE);//�ڶ����������������Ƿ����ڲ����ã�Ĭ��Ϊ�棬���ڴ����ⲿ��������Ϣ���ô��ڱ�������
	BOOL SetBorderStyle(BorderStyle bs);
	BOOL SetSizeable(BOOL bSizeable);
	BOOL GetSizeable();
	DUI_ControlBase* FindControlByID(INT ID);
	DUI_ControlBase* FindControlByPoint(Point* pt);
	VOID SetDebugMode(BOOL bDebug);
	BOOL GetDebugMode();
	VOID Update(BOOL bForce = TRUE);
	DUI_Prompt* GetWndPrompt();
	INT ScreenToClient(Point* pt);
	RDBManager* GetRDBMgr();
	BOOL IsWindowInited();
	MSGPROC SetMsgProc(MSGPROC Proc);
	VOID AcceptDropFile(BOOL bAccept = TRUE);
	HWND GetHWND();
	VOID AddControl(DUI_ControlBase * pCtrl);
	VOID Flush();
	HCURSOR SetCursor(LPTSTR CursorName);
	HCURSOR SetCursor(HCURSOR hCursor);
	//HWND m_hWnd;
protected:
	virtual LRESULT CALLBACK MsgProc(INT hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	VOID DrawWndBkg();
	DUI_Window* m_Parent;
	HDC m_hDC;
	MemDC* m_MemDC;
	MemDC* m_BkgDC;
	//Gdiplus::Graphics* m_Graphics;
	Gdiplus::RectF* m_WndRect;
	Gdiplus::RectF* m_ClientRect;

	LONG PrevWndProc;
	WNDPROC m_WndProc;

	BOOL InitDUIWnd(HWND hWnd, LPTSTR Title = L"", BOOL bSizable = FALSE);
	VOID InitRes();
	BOOL OnControl(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam);//����Ture��ʾ��Ϣ����Ҫ�������� IDΪINVALID_CONTROLID(-1)���ʾ����Ϣ�����͸����пؼ�

	//MsgProcFuncs
	BOOL OnMouseMove(WPARAM wParam,Point* ptMouse);
	BOOL OnLButtonDown(WPARAM wParam, Point* ptMouse);
	BOOL OnLButtonUp(WPARAM wParam, Point* ptMouse);
	BOOL OnMouseLeave(WPARAM wParam, Point* ptMouse);
	BOOL OnSize(WPARAM wParam, LPARAM lParam);
	BOOL OnMove(WPARAM wParam, LPARAM lParam);
	BOOL OnPaint(WPARAM wParam, LPARAM lParam);
	BOOL OnUpdate(WPARAM wParam, BOOL bUpdate);//wParam��ָ����Ҫ���µ�RectF��ָ��,Ϊ����ˢ����������,lParam��ʾ�Ƿ���µ�������  //����ȡ���ֲ�����
	BOOL OnSetCursor(WPARAM wParam, LPARAM lParam);
	BOOL OnGetMinMaxInfo(WPARAM wParam, LPARAM lParam);
	BOOL OnTimer(WPARAM wParam, LPARAM lParam);
	BOOL OnShowWindow(WPARAM wParam, LPARAM lParam);
	BOOL OnDropFiles(WPARAM wParam, LPARAM lParam);
	BOOL OnWndInited(WPARAM wParam, LPARAM lParam);

	BYTE m_Alpha;
	BOOL m_bInited;//��������WM_SHOWWINDOW�󣬴˱�ʶΪ��
	GdipString* m_Title;
	Color* m_BkgColor;
	Image* m_Icon;
	RectF* m_IconRect;
	Image* m_BkgImg;
	Image* m_SysBtnPic;
	BOOL m_Sizeable;
	BorderStyle m_BorderStyle;
	DUI_ImageButton* m_SysBtn[4];
	MSGPROC m_SysBtnClickProcAddr;
	BOOL CALLBACK SysBtnClick(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);
	INT m_SysBtnCnt;
	
	BOOL m_bMouseDown;
	BOOL m_bDebug;

	DUI_Prompt* m_Prompt;
	vector<DUI_ControlBase*>* m_Controls;
	INT  m_CaptureCtrlID;
	INT m_FocusCtrlID;
	INT m_LastHoverCtrlID;
	CursorDirection m_CurCDR;

	RDBManager* m_pRdbMgr;
	AnimArg* m_pAnimArg;
	MSGPROC m_MsgProc;
	//WNDANIMPROC m_lpfnAnimProc;
	BOOL WndAnimProc(AnimArg* pArg);
	BOOL m_bAnimate;
};