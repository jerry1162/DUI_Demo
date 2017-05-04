#pragma once
class DUI_Prompt
{
public:
	DUI_Prompt();
	~DUI_Prompt();
	BOOL Create(HWND hParent, LPTSTR Text, RectF* Rect);
	BOOL SetText(LPTSTR Text);
	BOOL IsCreated();
	VOID Destroy();
private:
	HWND m_hWnd;
	HWND m_hParent;
	RectF* m_Rect;
	LPCWSTR m_Text;
};

