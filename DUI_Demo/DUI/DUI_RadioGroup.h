#pragma once
#include "DUI_ControlBase.h"
#include "DUI_Radio.h"

class DUI_RadioGroup :
	public DUI_ControlBase  //该类不在窗口中显示任何东西
{
	friend class DUI_Radio;
public:
	DUI_RadioGroup();
	virtual ~DUI_RadioGroup() override;
	virtual BOOL Create(DUI_Object* Parent) override;
	virtual BOOL Destroy() override;
	INT GetSelect();
	BOOL Select(INT ID);
	BOOL Remove(INT ID);
	//************************************
	// Method:    ClearSelect
	// FullName:  DUI_RadioGroup::ClearSelect
	// Access:    public 
	// Returns:   VOID
	// Qualifier:清除当前组的选中项目
	//************************************
	VOID ClearSelect();

	//************************************
	// Method:    SelectNext
	// FullName:  DUI_RadioGroup::SelectNext
	// Access:    public 
	// Returns:   VOID
	// Qualifier:使单选组选中项切换到下一个，处于末尾时自动移至开头
	//************************************
	VOID SelectNext();
protected:
	virtual LRESULT CALLBACK MsgProc(INT ID, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	//************************************
	// Method:    IsAvailableID
	// FullName:  DUI_RadioGroup::IsAvailableID
	// Access:    protected 
	// Returns:   BOOL
	// Qualifier:该函数用于检验给定ID是否为属于该单选组的有效ID
	//************************************
	BOOL IsAvailableID(INT ID);
	INT m_LastSelectID;
	vector<INT>* m_IDs;
};

