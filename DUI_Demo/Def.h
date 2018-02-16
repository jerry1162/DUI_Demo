#pragma once
#include "Functions.h"
//���崰�ڻ�������
#define TITLEBARHEIGHT (20)
#define MINWNDWIDTH (200)
#define ICONSIZE_X (16)
#define ICONSIZE_Y (16)
#define SHADOWWIDTH (0) //��ʱ��ʹ����Ӱ
#define INVALID_CONTROLID (-1)
//#define INVALID_CONTROLINDEX -1
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
#define CM_MOUSEGETIN (WM_USER + 2) //��ʾ������ؼ���ע�⣬����Ϣ��WM_MOUSEMOVE��ͬ��ֻ��������ؼ�ʱ���ͣ�����ڿؼ����ƶ�ʱ���ᷢ�ʹ���Ϣ
#define CM_STATECHANGED (WM_USER + 3)//���ؼ�״̬�����仯ʱ���յ�����Ϣ��wParam��ָ�ؼ�ID��lParamΪ��һ��״̬ ����0��ʾ����Ҫ��ChangeState�����Update.
#define CM_CLICKED (WM_USER + 4)//��ť�ؼ��İ����¼� wParam��ָ�ؼ�ID��lParamΪExtra��Ϣ
#define CM_SHOW (WM_USER + 5)//�����ʾ������ wParam��ָ�ؼ�ID��lParam��ʾ��ʾ״̬
	#define CS_SHOW (1)
	#define CS_HIDE (0)
#define CM_SETAUTOUPDATE (WM_USER + 6)//���������Զ�ˢ������ wParam��ָ�ؼ�ID��lParam��ʾ����ֵ

#define WM_WNDINITED (WM_USER + 7) //���ڴ����ڳ�ʼ������һЩ����
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