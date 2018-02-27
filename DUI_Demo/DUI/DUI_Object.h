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
	virtual BOOL CanBeParent();
	virtual REAL GetX() = 0;
	virtual REAL GetY() = 0;
	virtual REAL GetWidth() = 0;
	virtual REAL GetHeight() = 0;
	virtual LRESULT SendMsg(UINT uMsg, WPARAM wParam = NULL, LPARAM lParam = NULL);
	virtual BOOL GetCursorPos(PointF* pt) = 0;
	virtual REAL GetMarginTop(); //��ȡ�ӿؼ�����ʱ��Topƫ��
	virtual RectF* GetClientRect();//��ȡ����Ϊ�����������Ŀͻ������Σ�ע�⣬ʹ�������Լ��ͷ�
	INT GetID();
protected:
	INT m_ID;
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};

