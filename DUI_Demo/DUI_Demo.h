
// DUI_Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDUI_DemoApp: 
// �йش����ʵ�֣������ DUI_Demo.cpp
//

class CDUI_DemoApp : public CWinApp
{
public:
	CDUI_DemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CDUI_DemoApp theApp;