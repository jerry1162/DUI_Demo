
// DUI_DemoDlg.h : 头文件
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
// CDUI_DemoDlg 对话框
class CDUI_DemoDlg : public CDialogEx
{
// 构造
public:
	CDUI_DemoDlg(CWnd* pParent = NULL);	// 标准构造函数
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
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUI_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};
