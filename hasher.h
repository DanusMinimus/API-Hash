#pragma once
/*##############################Includes##########################################*/
#include <Windows.h>
#include "listapi.h"

/*##############################Macros#########################################*/
#define TypeDefAPI(ProcName, TypeDef, ...) \
	typedef TypeDef (WINAPI* ProcName)(__VA_ARGS__)

#define CreateWINAPI(ProcName, TypeDef, ProcAddr, ...) \
	TypeDef(WINAPI* ProcName)(__VA_ARGS__) = (TypeDef(WINAPI* )(__VA_ARGS__))ProcAddr; \

#define CallWINAPI(ProcName, ...) \
	ProcName(__VA_ARGS__)
/*##############################Constants#########################################*/

/*##############################Typedef#########################################*/

/*##############################Structure Defentions##############################*/

/*##############################Function Defeneitions##############################*/
/*
This function returns a linked list containing a PAPI_NODE list with the entire IAT data stored in it
OUT - PAPI_NODE List
*/
PAPI_NODE HASHER_fillIAT();

/*
This function takes a PAPI_NODE head and fills it up with all the API data loaded for the import descriptor entry
IN pBaseAddr - the base address of the PE file
IN pImportDescriptor - the address of the import descriptor
IN headNode - the head of the list
OUT BOOL - Success code
The function assumes valid values have been passed into it*/
BOOL HASHER_fillAPIName(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN PAPI_NODE headNode);


/*
This function returns the DLL name within the import descriptor entry
IN pBaseAddr - the base address of the PE file
IN pImportDescriptor - the address of the import descriptor
IN currNode - the current node to set
OUT BOOL - Success code
The function assumes valid values have been passed into it*/
BOOL HASHER_setDLLName(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN PAPI_NODE currNode);

/*
This function accepts a string and returns its CRC32 Hash
IN pcstrString - the string to hash
OUT - DWORD Value containing the hash
*/
DWORD HASHER_crc32b(IN PCSTR pcstrString);

/*
This function iterates through the entire Thunk within the Import Descriptor, attempting to resolve hashes for the current node
IN currNode - A list containing hashes for APIs
*/
VOID HASHER_bruteForceIAT(IN PAPI_NODE currNode);

/*
This function returns a specific API that matches that hash within the IAT
IN dwHash - The CRC32 Hash

OUT LPVOID - The procedure address matching the supllied hash
*/
LPVOID HASHER_locateHashInIAT(IN DWORD dwHash);

/*
This function accepts a CRC32 hash identfier and returns its matched API address
IN pBaseAddr - the base address of the PE file
IN pImportDescriptor - The address of the Import Descriptor entry
IN hDLLHandle - A handle to the DLL to get the proc address from
IN dwHash - The CRC32 Hash

OUT - A pointer to the function API
*/
LPVOID HASHER_getProcByHash(IN PIMAGE_DOS_HEADER pBaseAddr, IN PIMAGE_IMPORT_DESCRIPTOR pImportDescriptor, IN HMODULE hDLLHandle, IN DWORD dwHash);

/*
This function returns the Import Descriptor address
OUT - The address of the Import Descriptor
*/
PIMAGE_IMPORT_DESCRIPTOR HASHER_getImportDescriptor();
