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

LRESULT DUI_Object::SendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return MsgProc(m_ID, uMsg, wParam, lParam);
}

INT DUI_Object::GetID()
{
	return m_ID;
}