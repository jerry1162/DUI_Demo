#pragma once
#include <vector>
#include <typeinfo>
using namespace std;
using namespace Gdiplus;
#include "Functions.h"
#include "MemDC.h"

enum ObjType
{
	ObjType_Unknown = 0,
	ObjType_Wnd,
	ObjType_Ctrl
};

//���崰�ڻ�������
#define TITLEBARHEIGHT (20)
#define MINWNDWIDTH (200)
#define ICONSIZE_X (16)
#define ICONSIZE_Y (16)
#define SHADOWWIDTH (0) //��ʱ��ʹ����Ӱ
#define INVALID_CONTROLID (-1)
#define DEF_BORDERWHIDTH (5)
#define GAP_ICON_TITLE (5)
#define GAP_TITLE_SYSBTN (2)
#define ICONOFFSET_X (3)
#define ICONOFFSET_Y (3)
#define SYSBTN_X (39)
#define SYSBTN_Y (19)

//�Զ�����Ϣ
//˵�������ڽ��յ�hWndΪ����Ч���ھ��ʱ����������ϢΪ���͸��ؼ�����Ϣ��hWndֵΪ�ؼ�ID
#define WM_UPDATE (WM_USER + 1) //�Դ�����˵��wParam��ָ����Ҫ���µ�RectF��ָ��,Ϊ����ˢ����������,lParam��ʾ�Ƿ���µ�������   �Կؼ���˵��wParam��ָ�ؼ�ID��lParam��ͬ
#define WM_WNDINITED (WM_USER + 2) //���ڴ����ڳ�ʼ������һЩ����

#define CM_MOUSELEAVE (WM_USER + 99)  //��ʾ����뿪�ؼ�,wParamΪ�ؼ�ID��lParamδ��
#define CM_MOUSEGETIN (WM_USER + 100) //��ʾ������ؼ���ע�⣬����Ϣ��WM_MOUSEMOVE��ͬ��ֻ��������ؼ�ʱ���ͣ�����ڿؼ����ƶ�ʱ���ᷢ�ʹ���Ϣ
#define CM_STATECHANGED (WM_USER + 101)//���ؼ�״̬�����仯ʱ���յ�����Ϣ��wParam��ָ�ؼ�ID��lParamΪ��һ��״̬ ����0��ʾ����Ҫ��ChangeState�����Update.
#define CM_CLICKED (WM_USER + 102)//�ؼ��İ����¼� wParam��ָ�ؼ�ID��lParamδ��
#define CM_SHOW (WM_USER + 103)//�����ʾ������ wParam��ָ�ؼ�ID��lParam��ʾ��ʾ״̬
	#define CS_SHOW (1)
	#define CS_HIDE (0)
#define CM_SETAUTOUPDATE (WM_USER + 104)//���������Զ�ˢ������ wParam��ָ�ؼ�ID��lParam��ʾ����ֵ
#define CM_CALCRECT (WM_USER + 105)//����ˢ�¿ؼ����� wParam�ǿؼ�ID��lParamΪNULL
#define CM_ADDCTRL (WM_USER + 106) //����֪ͨ����������һ�������wParamΪ�ؼ�ָ�룬lParamδ�á�


#define RG_ADDRADIO (WM_USER + 107)//����ѡ�򴴽�ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam�ǵ�ѡ��ID��lParamδ��
#define RG_SELECTCHANGE (WM_USER + 108)//����ѡ��״̬�ı�ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam����ѡ�еĵ�ѡ��ID��lParamδ��
#define RG_GROUPCHANGE (WM_USER + 109)//����ѡ��ı�������ѡ��ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam�ǵ�ѡ��ID��lParamδ��

#define CM_CREATE (WM_USER + 109) //��������������ControlBase�ַ��������wParam�ǿؼ�ID��lParamδ��
#define CM_SETCURSOR (WM_USER + 110) //��Ҫ�������ָ��ʱ���ͣ�wParam�ǿؼ�ID��lParamδ��
#define CM_MOVE (WM_USER + 111) //�����λ�ñ��ƶ����͸��ؼ���Ҳ������Move���ͣ�wParam�ǿؼ�ID��lParamδ��
#define CM_SIZE (WM_USER + 112) //�������С���ı���͸��ؼ���Ҳ������Size���ͣ�wParam�ǿؼ�ID��lParamδ��
#define CM_PARENTDESTROY (WM_USER + 113) //�����������ǰ���͸��ؼ���wParam�ǿؼ�ID��lParamδ��





typedef BOOL(CALLBACK *MSGPROC) (VOID* pThis, UINT uMsg, WPARAM wParam, LPARAM lParam);//����ֵ��ʾ����Ϣ�Ƿ�Ҫ�������ݣ������ʾ�������´��ݡ�ע���û���Ϣ��������ڶ��������Ϣ����
//typedef LRESULT(CALLBACK *CLICKPROC) (VOID* pThis,UINT uMsg, WPARAM ID, LONG Extra);
enum BorderMode
{
	BM_Normal = 1,//��ͨģʽ�������α߿�
	BM_RoundRect
};
struct BorderStyle
{
	BorderMode Mode;
	ARGB Color;
	BOOL DoubleBorder;
	BOOL operator==(BorderStyle bs)
	{
		return (Mode == bs.Mode&&Color == bs.Color&&DoubleBorder == bs.DoubleBorder);
	}
};
enum CursorDirection
{
	CDR_Normal = 0,
	CDR_NWSE,
	CDR_NS,
	CDR_NESW,
	CDR_WE,
	CDR_NWSE2,
	CDR_NS2,
	CDR_NESW2,
	CDR_WE2
};
enum SYSBTNTYPE
{
	BT_Close = 1,
	BT_Maxmize,
	BT_Minimize,
	BT_None
};

struct AnimArg
{
	INT Arg_1;
	INT Arg_2;
	MemDC* pDC1;
	MemDC* pDC2;
	AnimArg()
	{
		Arg_1 = 0;
		Arg_2 = 0;
		pDC1 = nullptr;
		pDC2 = nullptr;
	}
};

//typedef BOOL(CALLBACK *WNDANIMPROC)(AnimArg* pArg);


enum DUI_Status
{
	S_Normal = 1,
	S_HighLight,
	S_Pushed,
	S_Focus,
	S_Disabled,
	//S_Hide,
	S_Invalid = -1
};