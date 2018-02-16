
// DUI_DemoDlg.h : ͷ�ļ�
//

#pragma once
#include "Functions.h"
#include "DUI_Window.h"
//#include "ControlBase.h"
#include "DUI_Lable.h"
#include "DUI_Button.h"
#include "DUI_ImageButton.h"
#include "DUI_CheckBox.h"
#include "DUI_RadioGroup.h"
#include "DUI_Radio.h"
#include "DUI_ProgressBar.h"
#include "DUI_ImageBox.h"

// CDUI_DemoDlg �Ի���
class CDUI_DemoDlg : public CDialogEx
{
// ����
public:
	CDUI_DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

	~CDUI_DemoDlg();
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
