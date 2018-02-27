#include "stdafx.h"
#include "DUI_Radio.h"
#include "..//resource.h"

DUI_Radio::DUI_Radio()
{
	m_bChecked = FALSE;
	pImg = nullptr;
	m_Group = nullptr;
}


DUI_Radio::~DUI_Radio()
{
	Destroy();
}

BOOL DUI_Radio::Create(DUI_Window * Window, DUI_RadioGroup * Group, REAL Left, REAL Top, REAL Width, REAL Height, LPCWSTR Text, BOOL bVisiable)
{
	BOOL Ret = DUI_ControlBase::Create(Window, Left, Top, Width, Height, Text, FALSE);
	if (Ret)
	{
		ChangeGroup(Group);
		INT size = m_ParentWnd->GetRDBMgr()->GetIntValByName(_T("Radio_Size"));
		m_Text->rect->X = (REAL)size;
		m_Text->rect->Width -= size;
		m_Text->color->SetValue(Color::White);
		ResItem* lpItem = m_ParentWnd->GetRDBMgr()->GetItemByName(_T("Radio_Pic"));
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

BOOL DUI_Radio::Destroy()
{
	if (Existence())
	{
		delete pImg;
		pImg = nullptr;
	}
	return DUI_ControlBase::Destroy();
}


BOOL DUI_Radio::ChangeGroup(DUI_RadioGroup* Group)
{
	if (Group == m_Group)
	{
		return FALSE;
	}
	if (m_Group != nullptr)
	{
		m_Group->MsgProc(m_Group->m_ID, RG_SELECTCHANGE, m_ID, NULL);
	}
	m_Group = Group;
	m_Group->MsgProc(m_Group->m_ID, RG_ADDRADIO, m_ID, NULL);
	return TRUE;
}

LRESULT DUI_Radio::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		m_bChecked = TRUE;
		m_Group->MsgProc(m_Group->m_ID, RG_SELECTCHANGE, m_ID, NULL);
	}
	return Ret;
}

VOID DUI_Radio::Draw(DUI_Status s)
{
	m_MemDC->Clear();
	if (m_bVisialbe)
	{
		if (s == S_Invalid)
		{
			s = m_CurState;
		}
		if (s == S_Focus || s == S_Disabled)
		{
			s = S_Normal;
		}
		m_MemDC->graphics->DrawImage(pImg, RectF(0, 0, 15, 15), (REAL)(s - 1) * 15 + (m_bChecked ? (15 * 3) : 0), 0, (REAL)15, (REAL)15, UnitPixel);
		DrawShadowText(m_MemDC->graphics, m_Text);
		DUI_ControlBase::Draw();
	}
}


VOID DUI_Radio::Select(BOOL Select)
{
	if (Select)
	{
		MsgProc(m_ID, CM_CLICKED, m_ID, NULL);
	}
	else
	{
		m_bChecked = FALSE;
	}
	if (m_bAnimating)
	{
		EndAnimate();
	}
	StartAnimate();
}
