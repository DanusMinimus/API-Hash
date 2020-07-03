#pragma once

/*##############################Includes##########################################*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

/*##############################Constants#########################################*/
#define HASH_MESSAGEBOXA 0xDEADBEEF
#define HASH_GETPROCADDR 0xDEADBEEF
#define HASH_VIRTUALALLOC 0xDEADBEEF
#define STR_SIZE 20

/*##############################Structure Defentions##############################*/
typedef struct API_NODE
{
	CHAR cstrDLL_Name[STR_SIZE];
	PCHAR pcstrAPI_Name[50];
	DWORD dwHashArray[50];
	UINT nStringSize;
	API_NODE * papiNext;

}*PAPI_NODE;


/*##############################Function Defeneitions##############################*/
/*
Function accepts an empty API_NODE and initializes it

OUT - PAPI_NODE:
Initialized PAPI_NODE
*/
PAPI_NODE LISTAPI_initNode();

/*
Function accepts the current API_NODE and sets the next API_NODE
IN - pnodeCurr:
The current node in the list

IN - pnodeCurr:
The next node to be appended to the list

OUT - BOOL:
Success Code
*/
BOOL LISTAPI_setNext(IN PAPI_NODE pnodeCurr, IN PAPI_NODE pnodeNext);

/*
Function accepts the current API_NODE and returns the next API_NODE
IN - pnodeCurr The current node in the list

OUT - PAPI_NODE:
Next API_NODE in the list
*/
PAPI_NODE LISTAPI_getNext(IN PAPI_NODE pnodeCurr);

/*
Function accepts the current API_NODE and sets the DLL Name of the API node
IN - pnodeCurr The current node in the list
IN - pcstrDLLName:DLL string name

OUT - BOOL:
Success Code
*/
BOOL LISTAPI_setDataDLL(IN PAPI_NODE pnodeCurr, IN PCSTR pcstrDLLName);

/*
Function accepts the current API_NODE and sets the API Name of the API node
IN - pnodeCurr The current node in the list
IN - pcstrDLLName DLL string name
IN - dwHash CRC32 hash identfier

OUT - BOOL Success Code
*/
BOOL LISTAPI_setDataAPI(IN PAPI_NODE pnodeCurr, IN PCSTR pcstrAPIName, IN DWORD dwHash);

/*
Function accepts the current API_NODE head and prints the entire list and its contents
IN - pnodeHead The head of the pnode list

OUT - BOOL Success Code
*/
BOOL LISTAPI_printList(IN PAPI_NODE pnodeHead);

/*
This functions frees a node within the list
IN pnodeCurr - The current node to free
OUT - Success Code
*/
BOOL LISTAPI_freeNode(IN PAPI_NODE pnodeCurr);

/*
Free list function
IN pnodeHead - The head of the list
OUT - Success Code
*/
BOOL LISTAPI_freeList(IN PAPI_NODE pnodeHead);

