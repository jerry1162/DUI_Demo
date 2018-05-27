#include "stdafx.h"
#include "DUI_RadioGroup.h"


DUI_RadioGroup::DUI_RadioGroup()
{
	m_LastSelectID = INVALID_CONTROLID;
	//m_CurSelect = INVALID_CONTROLID;
	m_IDs = nullptr;
}


DUI_RadioGroup::~DUI_RadioGroup()
{
	Destroy();
}

BOOL DUI_RadioGroup::Create(DUI_Object * Parent)
{
	/*if (!Parent->CanBeParent())
	{
		return FALSE;
	}
	m_ID = NewID();
	m_Parent = Parent;

	if (Parent->GetObjType() == ObjType_Wnd)
	{
		m_ParentWnd = (DUI_Window*)m_Parent;
	}
	else
	{
		m_ParentWnd = ((DUI_ControlBase*)m_Parent)->GetParentWnd();
	}

	m_bVisialbe = FALSE;*/

	BOOL ret = DUI_ControlBase::Create(Parent);
	if (!ret)
	{
		return FALSE;
	}
	m_IDs = new vector<INT>;
	return TRUE;
}

INT DUI_RadioGroup::GetSelect()
{
	return m_LastSelectID;
}

BOOL DUI_RadioGroup::Select(INT ID)
{
	if (ID == INVALID_CONTROLID || !IsAvailableID(ID))
	{
		return FALSE;
	}
	DUI_Radio* pRadio = nullptr;
	pRadio = (DUI_Radio*)m_ParentWnd->FindControlByID(ID);
	if (pRadio == nullptr)
	{
		return FALSE;
	}
	pRadio->Select(TRUE);
	return TRUE;
}

BOOL DUI_RadioGroup::Remove(INT ID)
{
	if (ID == INVALID_CONTROLID)
	{
		return FALSE;
	}
	for (auto it = m_IDs->begin(); it != m_IDs->end(); it++)
	{
		if ((*it) == ID)
		{
			m_IDs->erase(it);
			return TRUE;
		}
	}
	return FALSE;
}

VOID DUI_RadioGroup::ClearSelect()
{
	DUI_Radio* pRadio = nullptr;
	pRadio = (DUI_Radio*)m_ParentWnd->FindControlByID(m_LastSelectID);
	if (pRadio != nullptr)
	{
		pRadio->Select(FALSE);
	}
}

VOID DUI_RadioGroup::SelectNext()
{
	if (m_IDs->size() <= 1)
	{
		return;
	}
	BOOL bFound = FALSE;
	for (auto ID : *m_IDs)
	{
		if (ID == m_LastSelectID)
		{
			bFound = TRUE;
			continue;
		}
		if (bFound)
		{
			Select(ID);
			return;
		}
	}
	Select(m_IDs->at(0));
}

BOOL DUI_RadioGroup::Destroy()
{
	if (Existence())
	{
		delete m_IDs;
		return TRUE;
	}
	return FALSE;
}

LRESULT DUI_RadioGroup::MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DUI_ControlBase::MsgProc(m_ID, uMsg, wParam, lParam);
	DUI_Radio* pRadio = nullptr;
	switch (uMsg)
	{
	case RG_ADDRADIO:
		m_IDs->push_back(wParam);
		break;
	case RG_SELECTCHANGE:
		if (m_LastSelectID != INVALID_CONTROLID && m_LastSelectID != wParam)
		{
			pRadio = (DUI_Radio*)m_ParentWnd->FindControlByID(m_LastSelectID);
			pRadio->m_bChecked = FALSE;
			/*if (pRadio->m_bAnimating)
			{
				pRadio->EndAnimate();
			}*/
			pRadio->StartAnimate();
		}
		m_LastSelectID = wParam;
		break;
	case RG_GROUPCHANGE:
		if (m_LastSelectID == wParam)
		{
			SelectNext();
		}
		Remove(wParam);
		break;
	default:
		break;
	}
	return LRESULT();
}

BOOL DUI_RadioGroup::IsAvailableID(INT ID)
{
	if (ID == INVALID_CONTROLID)
	{
		return FALSE;
	}

	for (auto it:*m_IDs)
	{
		if (it == ID)
		{
			return TRUE;
		}
	}
	return FALSE;
}

