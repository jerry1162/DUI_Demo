#include "stdafx.h"
#include "DUI_Object.h"


DUI_Object::DUI_Object()
{
	m_ID = NULL;
}


DUI_Object::~DUI_Object()
{
	m_ID = NULL;
}

BOOL DUI_Object::CanBeParent()
{
	return FALSE;
}

LRESULT DUI_Object::SendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(m_ID, uMsg, wParam, lParam);
}

REAL DUI_Object::GetMarginTop()
{
	return 0.0;
}

RectF * DUI_Object::GetClientRect()
{
	return nullptr;
}

INT DUI_Object::GetID()
{
	return m_ID;
}