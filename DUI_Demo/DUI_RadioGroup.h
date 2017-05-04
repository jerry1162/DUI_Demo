#pragma once
#include "DUI_ControlBase.h"
#include "DUI_Radio.h"
#define RG_ADDRADIO WM_USER + 6//当单选框创建时，向其单选组发送此消息 wParam是单选框ID，lParam未用
#define RG_SELECTCHANGE WM_USER + 7//当单选框状态改变时，向其单选组发送此消息 wParam是新选中的单选框ID，lParam未用
#define RG_GROUPCHANGE WM_USER + 8//当单选框改变所属单选组时，向其单选组发送此消息 wParam是单选框ID，lParam未用
class DUI_RadioGroup :
	public DUI_ControlBase  //该类不在窗口中显示任何东西
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
	virtual LRESULT MsgProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
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

