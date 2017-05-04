#pragma once
#include "DUI_ControlBase.h"
#include "DUI_Radio.h"
#define RG_ADDRADIO WM_USER + 6//����ѡ�򴴽�ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam�ǵ�ѡ��ID��lParamδ��
#define RG_SELECTCHANGE WM_USER + 7//����ѡ��״̬�ı�ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam����ѡ�еĵ�ѡ��ID��lParamδ��
#define RG_GROUPCHANGE WM_USER + 8//����ѡ��ı�������ѡ��ʱ�����䵥ѡ�鷢�ʹ���Ϣ wParam�ǵ�ѡ��ID��lParamδ��
class DUI_RadioGroup :
	public DUI_ControlBase  //���಻�ڴ�������ʾ�κζ���
{
	friend class DUI_Radio;
public:
	DUI_RadioGroup();
	virtual ~DUI_RadioGroup() override;
	virtual BOOL Create(DUI_Window* Window);
	virtual BOOL Destroy() override;
	INT GetSelect();
	BOOL Select(INT ID);
	BOOL Remove(INT ID);
	//************************************
	// Method:    ClearSelect
	// FullName:  DUI_RadioGroup::ClearSelect
	// Access:    public 
	// Returns:   VOID
	// Qualifier:�����ǰ���ѡ����Ŀ
	//************************************
	VOID ClearSelect();

	//************************************
	// Method:    SelectNext
	// FullName:  DUI_RadioGroup::SelectNext
	// Access:    public 
	// Returns:   VOID
	// Qualifier:ʹ��ѡ��ѡ�����л�����һ��������ĩβʱ�Զ�������ͷ
	//************************************
	VOID SelectNext();
protected:
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	//************************************
	// Method:    IsAvailableID
	// FullName:  DUI_RadioGroup::IsAvailableID
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:�ú������ڼ������ID�Ƿ�Ϊ���ڸõ�ѡ�����ЧID
	//************************************
	BOOL IsAvailableID(INT ID);
	INT m_LastSelectID;
	vector<INT>* m_IDs;
};

