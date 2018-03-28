#pragma once
#include "..//Common//Def.h"
class DUI_ControlBase;

class DUI_Object
{
public:
	DUI_Object();
	virtual ~DUI_Object();
	virtual BOOL Destroy() = 0;
	virtual ObjType GetObjType() = 0;
	virtual REAL GetX() = 0;
	virtual REAL GetY() = 0;
	virtual REAL GetWidth() = 0;
	virtual REAL GetHeight() = 0;
	virtual LRESULT SendMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	virtual BOOL GetCursorPos(PointF* pt) = 0;
	virtual RectF* GetClientRect() = 0;//获取可以为父组件的组件的客户区矩形，注意，使用完请自己释放
	virtual REAL GetMarginTop() = 0;
	INT GetID();
protected:
	INT m_ID;
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

