
// DUI_DemoDlg.h : 头文件
//

#pragma once
#include "Functions.h"
#include "DUI_Window.h"

// CDUI_DemoDlg 对话框
class CDUI_DemoDlg : public CDialogEx
{
// 构造
public:
	CDUI_DemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	DUI_Window m_Window;
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
