
// DUI_DemoDlg.h : ͷ�ļ�
//

#pragma once
#include ".//RDBManager//RDBManager.h"
#include ".//Common//Functions.h"
#include ".//DUI//DUI_Window.h"
#include ".//DUI//DUI_Lable.h"
#include ".//DUI//DUI_Button.h"
#include ".//DUI//DUI_ImageButton.h"
#include ".//DUI//DUI_CheckBox.h"
#include ".//DUI//DUI_RadioGroup.h"
#include ".//DUI//DUI_Radio.h"
#include ".//DUI//DUI_ProgressBar.h"
#include ".//DUI//DUI_ImageBox.h"
#include ".//DUI//DUI_GroupBox.h"
#include ".//DUI//DUI_SizeBox.h"
#include ".//DUI//DUI_ScrollBar.h"

// CDUI_DemoDlg �Ի���
class CDUI_DemoDlg : public CDialogEx
{
// ����
public:
	CDUI_DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

	~CDUI_DemoDlg();
	RDBManager* m_RdbMgr;
	DUI_Window* m_Window;
	DUI_Window* m_SubWindow;
	DUI_ImageButton* m_ImgBtn;
	DUI_CheckBox* m_CheckBox;
	DUI_RadioGroup* m_RGroup;
	DUI_Radio* m_Radio1;
	DUI_Radio* m_Radio2;
	DUI_Lable* m_Lable;
	DUI_Button* m_Button;
	MSGPROC m_BtnClick;
	MSGPROC m_WndProc;
	Image* pImg[3];
	BOOL CALLBACK MyBtn(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK WndProc(VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);
	DUI_ProgressBar* m_ProgressBar;
	DUI_ImageBox* m_ImageBox;
	DUI_GroupBox* m_GroupBox;
	DUI_SizeBox*  m_SizeBox;
	DUI_ScrollBar* m_ScrollBar;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUI_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
