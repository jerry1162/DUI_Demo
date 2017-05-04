
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
	CLICKPROC m_BtnClick;
	Image* pImg[3];
	LRESULT CALLBACK MyBtn(HWND hwnd, UINT uMsg, WPARAM ID, LONG Extra);
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
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
