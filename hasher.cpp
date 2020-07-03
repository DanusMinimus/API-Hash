#pragma once
/*##############################Includes##########################################*/
#include "hasher.h"

/*##############################Function Defentions##############################*/

PAPI_NODE HASHER_fillIAT()
{
	BOOL bReturnVal = FALSE;
	PAPI_NODE papiHead = NULL;
	PAPI_NODE papiCurrNode = NULL;

	PIMAGE_DOS_HEADER pBaseAddr = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);

	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = HASHER_getImportDescriptor();

	papiHead = LISTAPI_initNode();
	papiCurrNode = papiHead;

	for (; NULL != pImportDescriptor->Name; pImportDescriptor++)
	{

		if (NULL == papiCurrNode)
		{
			goto lbl_cleanup;
		}

		bReturnVal = HASHER_setDLLName(pBaseAddr, pImportDescriptor, papiCurrNode);

		if (FALSE == bReturnVal)
		{
			goto lbl_cleanup;
		}

		bReturnVal = HASHER_fillAPIName(pBaseAddr, pImportDescriptor, papiCurrNode);

		if (FALSE == bReturnVal)
		{
			goto lbl_cleanup;
		}

		if (0 == (pImportDescriptor + 1)->Name)
		{
			goto lbl_cleanup;
		}

		papiCurrNode->papiNext = LISTAPI_initNode();
		papiCurrNode = papiCurrNode->papiNext;
	}

lbl_cleanup:
	return papiHead;
}

BOOL HASHER_setDLLName(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN PAPI_NODE currNode)
{
	BOOL bReturnVal = FALSE;
	DWORD dwNameAddr = ((DWORD)(pBaseAddr)+pImportDescriptor->Name);
	bReturnVal = LISTAPI_setDataDLL(currNode, (LPCSTR)(dwNameAddr));
	return bReturnVal;
}

BOOL HASHER_fillAPIName(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN PAPI_NODE headNode)
{
	BOOL bReturnVal = FALSE;
	DWORD dwHash;

	PIMAGE_IMPORT_BY_NAME pName;
	PIMAGE_THUNK_DATA pFirstThunk;

	pFirstThunk = (PIMAGE_THUNK_DATA)((DWORD)(pBaseAddr)+(DWORD)(pImportDescriptor->OriginalFirstThunk));

	for (;0 != (DWORD)(pFirstThunk->u1.AddressOfData); pFirstThunk++)
	{
		pName = (PIMAGE_IMPORT_BY_NAME)((DWORD)(pBaseAddr)+(DWORD)(pFirstThunk->u1.AddressOfData));

		dwHash = HASHER_crc32b((LPCSTR)(pName->Name));
		bReturnVal = LISTAPI_setDataAPI(headNode, (LPCSTR)(pName->Name), dwHash);

		if (FALSE == bReturnVal)
		{
			goto lbl_cleanup;
		}

	}

lbl_cleanup:
	return bReturnVal;
}

DWORD HASHER_crc32b(IN PCSTR pcstrString) 
{
	INT nIndex;
	INT nTimes;
	INT nByte;
	INT nCrc;
	INT nMask;

	nIndex = 0;
	nCrc = 0xFFFFFFFF;
	while (pcstrString[nIndex] != 0) {
		nByte = pcstrString[nIndex];            // Get next byte.
		nCrc = nCrc ^ nByte;
		for (nTimes = 7; nTimes >= 0; nTimes--) {    // Do eight times.
			nMask = -(nCrc & 1);
			nCrc = (nCrc >> 1) ^ (0xEDB88320 & nMask);
		}
		nIndex = nIndex + 1;
	}
	return ~nCrc;
}

LPVOID HASHER_getProcByHash(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN HMODULE hDLLHandle, IN DWORD dwHash)
{
	PIMAGE_IMPORT_BY_NAME pName;
	PIMAGE_THUNK_DATA pFirstThunk;
	DWORD dwhashCompare;
	PVOID pvoidWINAPIFunc = NULL;

	pFirstThunk = (PIMAGE_THUNK_DATA)((DWORD)(pBaseAddr)+(DWORD)(pImportDescriptor->OriginalFirstThunk));

	for (; 0 != (DWORD)(pFirstThunk->u1.AddressOfData); pFirstThunk++)
	{
		pName = (PIMAGE_IMPORT_BY_NAME)((DWORD)(pBaseAddr)+(DWORD)(pFirstThunk->u1.AddressOfData));

		dwhashCompare = HASHER_crc32b((LPCSTR)(pName->Name));

		if (dwHash == dwhashCompare)
		{	
			pvoidWINAPIFunc = GetProcAddress(hDLLHandle, (pName->Name));
		}

	}

	return pvoidWINAPIFunc;
}

VOID HASHER_bruteForceIAT(IN PAPI_NODE currNode)
{
	DWORD dwHash;
	HMODULE hDLLHandle;
	LPVOID pProcAddr;

	PIMAGE_DOS_HEADER pBaseAddr = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = HASHER_getImportDescriptor();


	for (; NULL != pImportDescriptor->Name && NULL != currNode; pImportDescriptor++)
	{
		hDLLHandle = LoadLibraryA(currNode->cstrDLL_Name);
		printf("Loading DLL: %s\n", currNode->cstrDLL_Name);

		for (INT nIndex = 0; nIndex < (INT)currNode->nStringSize; nIndex++)
		{
			dwHash = currNode->dwHashArray[nIndex];
			pProcAddr = HASHER_getProcByHash(pBaseAddr, pImportDescriptor, hDLLHandle, dwHash);
			printf("\t|-Address of API Hash(%s) 0x%08x: 0x%08x\n", currNode->pcstrAPI_Name[nIndex], dwHash, (UINT)pProcAddr);
		}

		printf("\n");
		currNode = currNode->papiNext;
	}

}

LPVOID HASHER_locateHashInIAT(IN DWORD dwHash)
{
	HMODULE hDLLHandle;
	DWORD dwNameAddr;
	LPVOID pProcAddr = NULL;

	PIMAGE_DOS_HEADER pBaseAddr = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = HASHER_getImportDescriptor();

	for (; NULL != pImportDescriptor->Name; pImportDescriptor++)
	{
		dwNameAddr = ((DWORD)(pBaseAddr)+pImportDescriptor->Name);
		hDLLHandle = LoadLibraryA((LPCSTR)dwNameAddr);
		pProcAddr = HASHER_getProcByHash(pBaseAddr, pImportDescriptor, hDLLHandle, dwHash);

		if (NULL != pProcAddr)
		{
			printf("Successfully located CRC32 Hash within the IAT at 0x%08x\n", (UINT)pProcAddr);
			break;
		}
	}

	return pProcAddr;
}


PIMAGE_IMPORT_DESCRIPTOR HASHER_getImportDescriptor()
{
	PIMAGE_DOS_HEADER pBaseAddr = (PIMAGE_DOS_HEADER)GetModuleHandle(NULL);
	PIMAGE_NT_HEADERS pIMAGE_NT_HEADER = (PIMAGE_NT_HEADERS)((DWORD)pBaseAddr + pBaseAddr->e_lfanew);
	IMAGE_OPTIONAL_HEADER sOptionalHeader = (pIMAGE_NT_HEADER->OptionalHeader);
	IMAGE_DATA_DIRECTORY sEntryImport = sOptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];

	PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)(pBaseAddr)+sEntryImport.VirtualAddress);

	return pImportDescriptor;
}