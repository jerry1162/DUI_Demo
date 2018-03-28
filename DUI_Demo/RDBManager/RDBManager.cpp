#include "stdafx.h"
#include "RDBManager.h"


RDBManager::RDBManager()
{
	m_pHeader = nullptr;
	m_pItems = nullptr;
	m_LoadType = L_Unknown;
	m_lpBin = nullptr;
}


RDBManager::~RDBManager()
{
	delete m_pHeader;
	if (m_pItems != nullptr)
	{
		for (auto it = m_pItems->begin(); it != m_pItems->end(); it++)
		{
			delete[](*it)->Name;
			if (m_LoadType == L_FromFile )
			{
				//delete[] (*it)->Name;
				operator delete ((*it)->lpData);
			}
			delete *it;
		}
	}
}

VOID RDBManager::Create()
{
	m_pHeader = new RDBHeader;
	m_pItems = new vector<ResItem *>;
	m_LoadType = L_Ceate;
}

BOOL RDBManager::LoadFromFile(LPTSTR Path)
{
	if (m_pHeader != nullptr)
	{
		return FALSE;
	}
	HANDLE pFile;
	DWORD dwBytesRead;

	pFile = CreateFile(Path, GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,        //打开已存在的文件 
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (pFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFile);
		return FALSE;
	}

	m_pHeader = new RDBHeader;
	if (!ReadFile(pFile, m_pHeader, sizeof(RDBHeader), &dwBytesRead, NULL))
	{
		delete m_pHeader;
		m_pHeader = nullptr;
		CloseHandle(pFile);
		return FALSE;
	}
	if (lstrcmp(m_pHeader->Head, _T("RDBFile")) != 0)
	{
		delete m_pHeader;
		m_pHeader = nullptr;
		CloseHandle(pFile);
		return FALSE;
	}
	m_LoadType = L_FromFile;

	ResItem* pItem = nullptr;
	UINT uSize = 0;
	LPVOID lpData = nullptr;
	if (m_pHeader->ItemCount != 0)
	{
		m_pItems = new vector<ResItem *>;
	}
	for (UINT i = 0; i < m_pHeader->ItemCount; i++)
	{
		pItem = new ResItem;//(ResItem*)operator new(sizeof(ResItem) + uSize);
		if (!ReadFile(pFile, pItem, sizeof(ResItem), &dwBytesRead, NULL))
		{
			delete m_pItems;
			m_pItems = nullptr;
			delete pItem;
			CloseHandle(pFile);
			return FALSE;
		}
		LPTSTR Name = new TCHAR[pItem->NameLen];
		if (!ReadFile(pFile, Name, pItem->NameLen * sizeof(TCHAR), &dwBytesRead, NULL))
		{
			delete Name;
			delete m_pItems;
			m_pItems = nullptr;
			delete pItem;
			CloseHandle(pFile);
			return FALSE;
		}
		pItem->Name = Name;
		
		lpData = operator new(pItem->uSize);
		if (!ReadFile(pFile, lpData, pItem->uSize, &dwBytesRead, NULL))
		{
			delete m_pItems;
			m_pItems = nullptr;
			delete pItem;
			delete lpData;
			CloseHandle(pFile);
			return FALSE;
		}
		pItem->lpData = lpData;
		m_pItems->push_back(pItem);
	}

	CloseHandle(pFile);
	return TRUE;
}

BOOL RDBManager::LoadFromBin(LPVOID lpBin)
{
	if (m_pHeader != nullptr)
	{
		return FALSE;
	}
	if (lpBin == nullptr)
	{
		return FALSE;
	}
	m_lpBin = lpBin;
	m_LoadType = L_FromBin;
	char* Addr = (char*)lpBin;
	m_pHeader = /*(RDBHeader*)Addr;//*/new RDBHeader;
	RtlMoveMemory(m_pHeader, Addr, sizeof(RDBHeader));
	if (lstrcmp(m_pHeader->Head, _T("RDBFile")) != 0)
	{
		delete m_pHeader;
		m_pHeader = nullptr;
		return FALSE;
	}
	Addr += sizeof(RDBHeader);
	ResItem* pItem = nullptr;
	UINT uSize = 0;
	LPVOID lpData = nullptr;
	if (m_pHeader->ItemCount != 0)
	{
		m_pItems = new vector<ResItem *>;
	}

	for (UINT i = 0; i < m_pHeader->ItemCount; i++)
	{
		pItem = /*(ResItem*)Addr;//*/new ResItem;
		RtlMoveMemory(pItem, Addr, sizeof(ResItem));
		Addr += sizeof(ResItem);
		LPTSTR Name = new TCHAR[pItem->NameLen];
		RtlMoveMemory(Name, Addr, pItem->NameLen * sizeof(TCHAR));
		pItem->Name = Name;
		Addr += pItem->NameLen * sizeof(TCHAR);
		lpData = Addr;
		pItem->lpData = lpData;
		Addr += pItem->uSize;
		m_pItems->push_back(pItem);
	}

	return TRUE;
}

BOOL RDBManager::AddRes(LPTSTR Name, RESTYPE Type, LPVOID lpData, UINT uSize)
{
	if (m_pHeader == nullptr)
	{
		return FALSE;
	}
	if (lpData == nullptr)
	{
		return FALSE;
	}
	if (uSize == 0)
	{
		switch (Type)
		{
		case RT_BOOL:
			uSize = sizeof(bool);
			break;
		case RT_INT:
			uSize = sizeof(int);
			break;
		case RT_DWORD:
			uSize = sizeof(DWORD);
			break;
		case RT_Text:
			uSize = (lstrlen((LPTSTR)lpData) + 1) * sizeof(TCHAR);
			break;
		default:
			return FALSE;
			break;
		}
	}
	else if(Type == RT_Text)
	{
		uSize += 1;
		uSize *= sizeof(TCHAR);
	}
	ResItem* pItem = new ResItem;//(ResItem*)operator new(sizeof(ResItem) + uSize);
	//ZeroMemory(pItem, sizeof(ResItem) + uSize);
	pItem->Name = Name;
	pItem->NameLen = lstrlen(Name) + 1;
	pItem->Index = ++m_pHeader->ItemCount;
	pItem->ItemType = Type;
	pItem->uSize = uSize;
	uSize += sizeof(ResItem);
	m_pHeader->uSize += uSize;
	pItem->lpData = lpData;
	m_pItems->push_back(pItem);
	return TRUE;
}

BOOL RDBManager::AddFromLocalRes(LPTSTR Name, RESTYPE Type, UINT ID, LPCWSTR lpType)
{
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(ID), lpType);
	if (!hRsrc)
	{
		return FALSE;
	}
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	LPVOID pBuffer = LockResource(hGlobal);
	return this->AddRes(Name, Type, pBuffer, dwSize);
}

BOOL RDBManager::AddIcon(LPTSTR Name, INT ID)
{
	return 0;
}

BOOL RDBManager::Save(LPTSTR FileName)
{
	HANDLE  hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD   dwSize = sizeof(RDBHeader),dwWrite = 0;
	if (!WriteFile(hFile, m_pHeader, dwSize, &dwWrite, NULL))
	{
		CloseHandle(hFile);
		return FALSE;
	}
	LPVOID lpData = nullptr, lpName = nullptr;
	for (auto it = m_pItems->begin(); it != m_pItems->end(); it++)
	{
		dwSize = sizeof(ResItem);
		lpData = (*it)->lpData;
		(*it)->lpData = nullptr;
		lpName = (*it)->Name;
		(*it)->Name = nullptr;
		if (!WriteFile(hFile, *it, dwSize, &dwWrite, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}

		if (!WriteFile(hFile, lpName, (*it)->NameLen * sizeof(TCHAR),
			&dwWrite, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}

		if (!WriteFile(hFile, lpData, (*it)->uSize, &dwWrite, NULL))
		{
			CloseHandle(hFile);
			return FALSE;
		}
	}
	CloseHandle(hFile);
	return TRUE;
}

LPVOID RDBManager::GetResByIndex(UINT Index)
{
	if (Index > m_pHeader->ItemCount - 1 || Index < 0)
	{
		return nullptr;
	}
	return m_pItems->at(Index)->lpData;
}

LPVOID RDBManager::GetResByName(LPTSTR Name)
{
	for (auto it : *m_pItems)
	{
		if (lstrcmp(Name, it->Name) == 0)
		{
			return it->lpData;
		}
	}
	return nullptr;
}

INT RDBManager::GetIntValByName(LPTSTR Name)
{
	return *(INT*)GetResByName(Name);
}

bool RDBManager::GetBoolValByName(LPTSTR Name)
{
	return *(bool*)GetResByName(Name);
}

DWORD RDBManager::GetDwordValByName(LPTSTR Name)
{
	return *(DWORD*)GetResByName(Name);
}

LPTSTR RDBManager::GetTextByName(LPTSTR Name)
{
	return (LPTSTR)GetResByName(Name);
}

ResItem * RDBManager::GetItemByName(LPTSTR Name)
{
	for (auto it : *m_pItems)
	{
		if (lstrcmp(Name, it->Name) == 0)
		{
			return it;
		}
	}
	return nullptr;
}

LPVOID RDBManager::GetCurBin()
{
	if (m_LoadType == L_FromFile)
	{
		return nullptr;
	}
	else
	{
		return m_lpBin;
	}
}

RDBLOAD RDBManager::GetLoadType()
{
	return m_LoadType;
}
