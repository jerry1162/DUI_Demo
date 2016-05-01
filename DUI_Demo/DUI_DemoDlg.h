
// DUI_DemoDlg.h : ͷ�ļ�
//

#pragma once
#include "Functions.h"
#include "DUI_Window.h"

// CDUI_DemoDlg �Ի���
class CDUI_DemoDlg : public CDialogEx
{
// ����
public:
	CDUI_DemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	DUI_Window m_Window;
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
