
// DUI_DemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DUI_Demo.h"
#include "DUI_DemoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDUI_DemoDlg �Ի���



CDUI_DemoDlg::CDUI_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DUI_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CDUI_DemoDlg::~CDUI_DemoDlg()
{
	FreeCallBackAddr(m_BtnClick);
	delete m_Lable;
	delete m_Button;
	//delete pImg[0];
	//delete pImg[1];
	//delete pImg[2];
	delete m_ImgBtn;
	delete m_CheckBox;
	delete m_Radio1;
	delete m_Radio2;
	delete m_RGroup;
	delete m_SubWindow;
	delete m_Window;
	GdipShutdown();
}

LRESULT CDUI_DemoDlg::MyBtn(HWND hWnd, UINT uMsg, WPARAM ID, LONG Extra)
{
	m_Lable->SetVisiable(!m_Lable->GetVisiable());
	//static int i = 0;
	//i += 1;
	//if ((i % 2) == 0)
	//{
	//	m_Window->SetTitle(L"OK");
	//	m_Window->SetBorderStyle({ BM_RoundRect,NULL,TRUE });
	//	m_Window->SetDebugMode(FALSE);
	//	m_Button->SetText(L"Button");
	//}
	//else
	//{
	//	m_Window->SetTitle(L"Clicked");
	//	m_Window->SetBorderStyle({ BM_Normal,Color::MakeARGB(125,0,0,0),FALSE });
	//	m_Window->SetDebugMode(TRUE);
	//	m_Button->SetText(L"Clicked");
	//}
	//m_Window->SetSizeable(!m_Window->GetSizeable());
// 	if (m_SubWindow == nullptr)
// 	{
// 		m_SubWindow = new DUI_Window;
// 		m_SubWindow->Create(380, 250, m_Window, _T("SubWindow"));
// 		m_SubWindow->DoModel();
// 		delete m_SubWindow;
// 		m_SubWindow = nullptr;
// 	}
	DUI_Radio* pSex = (DUI_Radio*)m_Window->FindControlByID(m_RGroup->GetSelect());
	m_RGroup->SelectNext();
	//if (pSex != nullptr)
	//{
	//	CString str = _T("��ǰѡ�е��Ա�Ϊ��");
	//	str += pSex->GetText();
	//	MessageBox(str, _T("��ǰѡ����"), MB_ICONINFORMATION);
	//}
	return LRESULT(TRUE);
}

void CDUI_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDUI_DemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CDUI_DemoDlg ��Ϣ�������

BOOL CDUI_DemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//SetWindowText(L"Direct UI");
	GdipStartup();
	m_SubWindow = nullptr;
	m_Window = new DUI_Window;
 	if (!m_Window->Create(m_hWnd, L"Direct UI", L"..\\Image\\Icon.ico",
 		L"..\\Image\\BkGrd\\bkg2.jpg",1))
 	{
 		MessageBox(L"����ʧ��");
 	}
	//if (!m_Window->Create(m_hWnd, L"Direct UI", L"..\\Image\\Icon.ico",
	//	Color::MakeARGB(255, 240, 240, 240), FALSE))//80, 140, 200
	//{
	//	MessageBox(L"����ʧ��");
	//	return TRUE;
	//}
	//_CrtSetBreakAlloc(3670);
	//m_Window.SetBorderStyle({ BM_Normal,Color::MakeARGB(125,0,0,0),FALSE });
	//m_Window->SetSizeable(TRUE);
	//m_Window->SetDebugMode(TRUE);
	m_Lable = new DUI_Lable;
	m_Lable->Create(m_Window, 30, 20, 50, 25, _T("Lable1"));
	m_Button = new DUI_Button;
	m_Button->Create(m_Window, 65, 10, 76, 26, _T("������ť"));
	m_BtnClick = (CLICKPROC)GetCallBackAddr(this, &CDUI_DemoDlg::MyBtn);
	m_Button->SetClickEventHandler(m_BtnClick);

	pImg[0] = ImageFromIDResource(IDB_BTN_1, _T("PNG"));
	pImg[1] = ImageFromIDResource(IDB_BTN_2, _T("PNG"));
	pImg[2] = ImageFromIDResource(IDB_BTN_3, _T("PNG"));
	m_ImgBtn = new DUI_ImageButton;
	m_ImgBtn->Create(m_Window, 60, 50, 76, 26, pImg[0], pImg[1], pImg[2], _T("ImageBtn"));

	m_CheckBox = new DUI_CheckBox;
	m_CheckBox->Create(m_Window, 150, 20, 75, 15, _T("Box"));

	m_RGroup = new DUI_RadioGroup;
	m_RGroup->Create(m_Window);
	m_Radio1 = new DUI_Radio;
	m_Radio1->Create(m_Window, m_RGroup, 150, 40, 75, 15, _T("��"));
	m_Radio2 = new DUI_Radio;
	m_Radio2->Create(m_Window, m_RGroup, 230, 40, 75, 15, _T("Ů"));
	//m_Window->SetDebugMode(TRUE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CDUI_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDUI_DemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDUI_DemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

