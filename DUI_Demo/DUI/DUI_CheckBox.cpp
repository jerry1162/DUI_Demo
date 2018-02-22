#include "stdafx.h"
#include "DUI_CheckBox.h"
#include "..//resource.h"

DUI_CheckBox::DUI_CheckBox()
{
	m_bChecked = FALSE;
	pImg = nullptr;
}


DUI_CheckBox::~DUI_CheckBox()
{
	Destroy();
}

BOOL DUI_CheckBox::Create(DUI_Object * Parent, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bChecked, BOOL bVisiable)
{
	BOOL Ret = DUI_Button::Create(Parent, Left, Top, Width, Height, Text, FALSE);
	if (Ret)
	{
		INT size = m_ParentWnd->GetRDBMgr()->GetIntValByName(_T("CheckBox_Size"));
		m_Text->rect->X = (REAL)size;
		m_Text->rect->Width -= size;
		m_Text->color->SetValue(Color::White);

		ResItem* lpItem = m_ParentWnd->GetRDBMgr()->GetItemByName(_T("CheckBox_Pic"));
		if (!lpItem)
		{
			MessageBox(m_ParentWnd->GetHWND(), _T("×ÊÔ´¼ÓÔØÊ§°Ü"), _T("´íÎó:"), MB_ICONINFORMATION);
			return FALSE;
		}
		pImg = ImageFromBin(lpItem->lpData, lpItem->uSize);

		SetVisiable(bVisiable);
		Update();
	}
	return Ret;
}

BOOL DUI_CheckBox::GetCheckStatus()
{
	return m_bChecked;
}

VOID DUI_CheckBox::SetCheckStatus(BOOL bCheckd)
{
	m_bChecked = bCheckd;
	Update();
}

BOOL DUI_CheckBox::Destroy()
{
	if (Existence())
	{
		delete pImg;
	}
	return DUI_Button::Destroy();;
}

LRESULT DUI_CheckBox::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Ret;
	Ret = DUI_Button::MsgProc(m_ID, uMsg, wParam, lParam);
	//////////////////////////////////////////////////////////////////////////

	if (Ret == -1)
	{
		return TRUE;
	}

	if (uMsg == CM_CLICKED)
	{
		m_bChecked = !m_bChecked;
	}

	return Ret;
}

VOID DUI_CheckBox::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		//switch (s)
		//{
		//case S_Normal:
		//	//m_Text->color = &m_TextColor;
		//	m_Text->color->SetValue(Color::White);
		//	break;
		//case S_HighLight:
		//	m_Text->color->SetValue(Color::Blue);
		//	break;
		//case S_Pushed:
		//	m_Text->color->SetValue(Color::Red);
		//	break;
		//case S_Focus:
		//	m_Text->color->SetValue(Color::Gray);
		//	break;
		//default:
		//	break;
		//}
		if (s == S_Focus || s == S_Disabled)
		{
			s = S_Normal;
		}
		m_MemDC->graphics->DrawImage(pImg, RectF(0, 0, CHECK_SIZE, CHECK_SIZE), (REAL)(s - 1)*CHECK_SIZE + (m_bChecked ? (CHECK_SIZE * 3) : 0), 0, (REAL)CHECK_SIZE, (REAL)CHECK_SIZE, UnitPixel);
		DrawShadowText(m_MemDC->graphics, m_Text);
		DUI_ControlBase::Draw();
	}
}
