
// DUI_DemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DUI_Demo.h"
#include "DUI_DemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDUI_DemoDlg 对话框



CDUI_DemoDlg::CDUI_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DUI_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MAIN);
}

CDUI_DemoDlg::~CDUI_DemoDlg()
{
	FreeCallBackAddr(m_BtnClick);
	FreeCallBackAddr(m_WndProc);
	/*delete m_Lable;
	delete m_Button;
	//delete pImg[0];
	//delete pImg[1];
	//delete pImg[2];
	delete m_ImgBtn;
	delete m_ImageBox;
	delete m_CheckBox;
	delete m_Radio1;
	delete m_Radio2;
	delete m_RGroup;
	delete m_ProgressBar;*/
	delete m_SubWindow;
	delete m_Window;
	SafeDelete(m_RdbMgr);
	GdipShutdown();
}

BOOL CDUI_DemoDlg::MyBtn(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == CM_CLICKED)
	{
		static INT Cnt = m_RdbMgr->GetIntValByName(_T("BkgImgCnt"));
		static INT Cur = 1;
		Cur = Cur++ % 4;
		m_Window->SetBkgPic(Cur);
//		SafeDelete(m_GroupBox);
//		m_GroupBox->SetVisiable(!m_GroupBox->GetVisiable());
//		m_Lable->SetVisiable(!m_Lable->GetVisiable());
// 		m_ImageBox->MoveWithMouse(!m_ImageBox->MoveWithMouse());
// 		m_Window->SetDebugMode(!m_Window->GetDebugMode());
// 		m_Lable->SetText(L"Button");
		static int i = 4;
		i += 1;
		if ((i % 2) == 0)
		{
			m_Window->SetTitle(L"OK");
			m_Window->SetBorderStyle({ BM_RoundRect,NULL,TRUE });
			m_Button->SetText(L"Button");
			m_GroupBox->Size(200, 200);
		}
		else
		{
			m_Window->SetTitle(L"Clicked");
			m_Window->SetBorderStyle({ BM_Normal,NULL,FALSE });
			m_Button->SetText(L"Clicked");
			m_GroupBox->Size(50, 50);
		}
// 		DUI_ControlBase* pCtrl = nullptr;// m_Window->FindControlByID(i);
// 		while (1)
// 		{
// 			pCtrl = m_Window->FindControlByID(i);
// 			if (pCtrl == nullptr)
// 			{
// 				i = 4;
// 				continue;
// 			}
// 			if (pCtrl->GetVisiable())
// 			{
// 				m_SizeBox->Bind(pCtrl);
// 				break;
// 			}
// 			i++;
// 		}
//  		m_Window->SetSizeable(!m_Window->GetSizeable());
// 		if (m_SubWindow == nullptr)
// 		{
// 			m_SubWindow = new DUI_Window;
// 			m_SubWindow->Create(380, 250, m_Window, _T("SubWindow"));
// 			m_SubWindow->DoModel();
// 			SafeDelete(m_SubWindow);
// 		}
// 		DUI_Radio* pSex = (DUI_Radio*)m_Window->FindControlByID(m_RGroup->GetSelect());
// 		m_RGroup->SelectNext();
// 		if (pSex != nullptr)
// 		{
// 			CString str = _T("当前选中的性别为：");
// 			str += pSex->GetText();
// 			MessageBox(str, _T("当前选中项"), MB_ICONINFORMATION);
// 		}
//		m_Window->SetBkgColor(Color::Red);
	}
	return TRUE;
}

BOOL CDUI_DemoDlg::WndProc(VOID * pThis, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		UINT nCount;
		TCHAR szPath[MAX_PATH];
		CString Path;
		HICON hIcon;
		nCount = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		if (nCount)
		{
			DragQueryFile(hDrop, 0, szPath, _countof(szPath));
			Path = szPath;
			CString FileName;
			GetFileName(Path, FileName);
			m_Lable->SetText(FileName.GetBuffer());
			FileName.ReleaseBuffer();
			hIcon = GetFileIcon(Path, IS_EXLARGE);
			if (hIcon)
			{
				m_ImageBox->SetImage(GetBitmapFromHIcon(hIcon), FALSE);
			}

			/*for (UINT nIndex = 0; nIndex < nCount; ++nIndex)
			{
				DragQueryFile(hDrop, nIndex, szPath, _countof(szPath));
				MessageBox(szPath, _T("WM_DROPFILES"));
			}*/
		}

		DragFinish(hDrop);
	}
		break;
	case WM_MOUSEMOVE:
		break;
	}
	return TRUE;
}

void CDUI_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDUI_DemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDUI_DemoDlg 消息处理程序

BOOL CDUI_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(4496);
	GdipStartup();
	m_SubWindow = nullptr;
	
	LPVOID pResDB = GetResAddr(MAKEINTRESOURCE(IDR_RDB), _T("RDB"));
	m_RdbMgr = new RDBManager;
	if (!m_RdbMgr->LoadFromBin(pResDB))
	{
		MessageBox(L"资源包加载失败！");
		return TRUE;
	}
	 

	m_Window = new DUI_Window;
	if (!m_Window->Create(m_hWnd, m_RdbMgr))//m_Window->Create(m_hWnd, L"Direct UI", L"..\\Image\\Icon.ico",L"..\\Image\\BkGrd\\bkg2.jpg")
	{
		MessageBox(L"创建失败");
		return TRUE;
	}
// 	if (!m_Window->Create(m_hWnd, L"Direct UI", L"..\\Image\\Icon.ico",
// 		Color::MakeARGB(255, 80, 140, 200), FALSE))//80, 140, 200
// 	{
// 		MessageBox(L"创建失败");
// 		return TRUE;
// 	}

	//m_Window.SetBorderStyle({ BM_Normal,Color::MakeARGB(125,0,0,0),FALSE });
	//m_Window->SetSizeable(TRUE);
	//m_Window->SetDebugMode(TRUE);


	m_Lable = new DUI_Lable;
	m_Lable->Create(m_Window, 120, 120, 200, 25, _T("Lable1"));
	m_Lable->SetCursor(IDC_IBEAM);
	m_Lable->SetPrompt(_T("Lable"));

	m_Button = new DUI_Button;
	m_Button->Create(m_Window, 60, 20, 76, 26, _T("正常按钮"));
	m_BtnClick = (MSGPROC)GetCallBackAddr(this, &CDUI_DemoDlg::MyBtn);
	m_Button->SetMsgProc(m_BtnClick);
	m_Button->SetCursor(IDC_HAND);

	pImg[0] = ImageFromIDResource(IDB_BTN_1, _T("PNG"));
	pImg[1] = ImageFromIDResource(IDB_BTN_2, _T("PNG"));
	pImg[2] = ImageFromIDResource(IDB_BTN_3, _T("PNG"));
	m_ImgBtn = new DUI_ImageButton;
	m_ImgBtn->Create(m_Window, 60, 50, 80, 35, pImg[0], pImg[1], pImg[2], _T("ImageBtn"), new Rect(5, 5, 5, 5));//76, 26
	m_ImgBtn->SetCursor(IDC_HAND);

	m_GroupBox = new DUI_GroupBox;
	m_GroupBox->Create(m_Window, 120, 155, 100, 70, _T("分  组  框"));
	m_GroupBox->MoveWithMouse(TRUE);
	m_GroupBox->SetCursor(IDC_SIZEALL);

	m_CheckBox = new DUI_CheckBox; //ID 8
	m_CheckBox->Create(m_GroupBox, 0, 0, 75, 15, _T("Box"));//150, 20, 75, 15
	m_CheckBox->MoveWithMouse(TRUE);

	m_RGroup = new DUI_RadioGroup;
	m_RGroup->Create(m_Window);
	m_Radio1 = new DUI_Radio;
	m_Radio1->Create(m_Window, m_RGroup, 150, 40, 75, 18, _T("男"));
	m_Radio2 = new DUI_Radio;
	m_Radio2->Create(m_Window, m_RGroup, 150, 60, 75, 18, _T("女"));

	m_ProgressBar = new DUI_ProgressBar;
	m_ProgressBar->Create(m_Window, 60, 85, 210, 25, _T("进度条"));
	m_ProgressBar->SetCurPos(100);

	m_ImageBox = new DUI_ImageBox;//ID 13
	m_ImageBox->Create(m_Window, 60, 120, 100, 100);

	m_SizeBox = new DUI_SizeBox;
	m_SizeBox->Create(m_Window, 60, 180, 70, 70);

	//m_ScrollBar = new DUI_ScrollBar;
	//m_ScrollBar->Create(m_Window, 60, 180, 15, 150);

	m_WndProc = (MSGPROC)GetCallBackAddr(this, &CDUI_DemoDlg::WndProc);
	m_Window->AcceptDropFile();
	m_Window->SetMsgProc(m_WndProc);
	//m_Window->SetDebugMode(TRUE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDUI_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDUI_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

