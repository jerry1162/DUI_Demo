
template <typename T>//产生一个代理函数
LPVOID  GetCallBackAddr(LPVOID pThis, T MethodAddr)
{
	const unsigned char BlockCode[] = {
		0x8B, 0x44, 0x24, 0x10,			//	mov         eax,dword ptr [esp+10h]
		0x8B, 0x4C, 0x24, 0x0C,			//	mov         ecx,dword ptr [esp+0Ch]
		0x8B, 0x54, 0x24, 0x08,			//	mov         edx,dword ptr [esp+8]
		0x50,							//	push        eax
		0x8B, 0x44, 0x24, 0x08,			//	mov         eax,dword ptr [esp+8]
		0x51,							//	push        ecx
		0xB9, 0x00, 0x00, 0x00, 0x00,	//	mov         ecx,0 （类的this指针）
		0x52,							//	push        edx
		0x50,							//	push        eax
		0x51,							//	push		ecx
		0xE8, 0x00,0x00,0x00,0x00,	//	call        CWndProc::WndProc
		0xC2, 0x10, 0x00				//	ret         10h
	};

	size_t CodeBytes = sizeof(BlockCode);
	LPVOID  Block = VirtualAlloc(nullptr, 4096, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	memcpy(Block, BlockCode, CodeBytes);
	unsigned char * bBlock = (unsigned char *)Block;
	*PLONG32(&bBlock[19]) = LONG32(pThis);
	unsigned char* p = bBlock + 27;
	__asm
	{
		mov eax, MethodAddr
		sub eax, 4
		mov edi, p
		sub eax, edi
		mov[edi], eax
	}
	return (LPVOID)Block;
}