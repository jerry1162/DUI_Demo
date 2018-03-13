#pragma once
#include <vector>
using namespace std;
enum RESTYPE
{
	RT_BOOL = 1,
	RT_INT,
	RT_DWORD,
	RT_Text,
	RT_Icon,
	RT_Image,
	RT_Binary,
	RT_Unknown = -1
};

enum RDBLOAD
{
	L_Ceate = 1,
	L_FromFile,
	L_FromBin,
	L_Unknown = -1
};

struct ResItem
{
	UINT NameLen;
	UINT uSize;//��Դ��С
	INT Index;
	RESTYPE ItemType;
	LPVOID lpData;
	LPTSTR Name;
	ResItem()
	{
		Name = nullptr;
		NameLen = 0;
		uSize = 0;
		Index = 0;
		ItemType = RT_Unknown;
		lpData = nullptr;
	}
};

struct RDBHeader
{
	TCHAR Head[8];//�̶�ͷ������RDBFile�� 8
	UINT uSize;//��Դ����С 4
	UINT ItemCount;//��Դ���� 4
	RDBHeader():Head(_T("RDBFile"))
	{
		uSize = sizeof(RDBHeader);
		ItemCount = 0;
	}
};

class RDBManager
{
public:
	RDBManager();
	~RDBManager();
	VOID Create();
	BOOL LoadFromFile(LPTSTR Path);
	BOOL LoadFromBin(LPVOID lpBin);
	BOOL AddRes(LPTSTR Name, RESTYPE Type, LPVOID lpData, UINT uSize = 0);
	BOOL AddFromLocalRes(LPTSTR Name, RESTYPE Type, UINT ID, LPCWSTR lpType);
	BOOL AddIcon(LPTSTR Name, INT ID);
	BOOL Save(LPTSTR FileName);
	LPVOID GetResByIndex(UINT Index);
	LPVOID GetResByName(LPTSTR Name);
	INT GetIntValByName(LPTSTR Name);
	bool GetBoolValByName(LPTSTR Name);
	DWORD GetDwordValByName(LPTSTR Name);
	LPTSTR GetTextByName(LPTSTR Name);
	ResItem* GetItemByName(LPTSTR Name);
	LPVOID GetCurBin();//����Դ���Ӷ��������ݼ��أ��򷵻��ڴ��ַ�����򷵻�null
private:
	RDBHeader* m_pHeader;
	vector<ResItem*>* m_pItems;
	RDBLOAD m_LoadType;
	LPVOID m_lpBin;
};

