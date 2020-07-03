#pragma once

/*##############################Includes##########################################*/
#include "listapi.h"

/*##############################Function Defentions##############################*/

PAPI_NODE LISTAPI_initNode()
{	
	BOOL bRetualVal = FALSE;
	PAPI_NODE pnodeCurr = NULL;

	pnodeCurr = (PAPI_NODE)malloc(sizeof(API_NODE));
	
	if (NULL == pnodeCurr)
	{
		printf("Failed to allocate memory for node initialziation, Error code: %d\n", GetLastError());
		goto lbl_cleanup;
	}

	pnodeCurr->papiNext = NULL;
	pnodeCurr->nStringSize = 0;

	bRetualVal = TRUE;

lbl_cleanup:
	return pnodeCurr;
}

BOOL LISTAPI_setNext(IN PAPI_NODE pnodeCurr, IN PAPI_NODE pnodeNext)
{
	BOOL bRetualVal = FALSE;
	if (NULL == pnodeCurr || NULL == pnodeNext)
	{
		printf("Set next failed, node passed are nulled!\n");
		goto lbl_cleanup;
	}
	
	pnodeCurr->papiNext;

	bRetualVal = TRUE;

lbl_cleanup:
	return bRetualVal;
}

PAPI_NODE LISTAPI_getNext(IN PAPI_NODE pnodeCurr)
{
	if (NULL == pnodeCurr)
	{
		printf("Failed to get next node - parameters are null!\n");
		goto lbl_cleanup;
	}

	return pnodeCurr->papiNext;

lbl_cleanup:
	return NULL;
}

BOOL LISTAPI_setDataDLL(IN PAPI_NODE pnodeCurr, IN PCSTR pcstrDLLName)
{
	BOOL bRetualVal = FALSE;
	errno_t errTestCpy = 0;
	
	if (NULL == pnodeCurr || NULL == pcstrDLLName)
	{
		printf("Failed to get setDataDLL - parameters are null!\n");
		goto lbl_cleanup;
	}

	errTestCpy = strcpy_s(pnodeCurr->cstrDLL_Name, STR_SIZE, pcstrDLLName);

	if (0 != errTestCpy)
	{
		printf("Failed to strcpy_s for node setDataDLL, Error code: %d\n", GetLastError());
		goto lbl_cleanup;
	}

	bRetualVal = TRUE;

lbl_cleanup:
	return bRetualVal;
}

BOOL LISTAPI_setDataAPI(IN PAPI_NODE pnodeCurr, IN PCSTR pcstrAPIName, IN DWORD dwHash)
{
	BOOL bRetualVal = FALSE;
	errno_t errTestCpy = 0;
	PCHAR * cstrCurrString;

	INT nSizeOfAPIToCopy = strlen(pcstrAPIName)+1;

	if (NULL == pnodeCurr || NULL == pcstrAPIName)
	{
		printf("Failed to get setDataAPI - parameters are null!\n");
		goto lbl_cleanup;
	}

	pnodeCurr->pcstrAPI_Name[pnodeCurr->nStringSize] = (PCHAR)(malloc(sizeof(PCHAR) * nSizeOfAPIToCopy));

	errTestCpy = strcpy_s((pnodeCurr->pcstrAPI_Name)[pnodeCurr->nStringSize], nSizeOfAPIToCopy, pcstrAPIName);

	if (0 != errTestCpy)
	{
		printf("Failed to strcpy_s for node setDataAPI, Error code: %d\n", GetLastError());
		goto lbl_cleanup;
	}

	pnodeCurr->dwHashArray[pnodeCurr->nStringSize] = dwHash; 

	pnodeCurr->nStringSize++;

	bRetualVal = TRUE;

lbl_cleanup:
	return bRetualVal;
}

BOOL LISTAPI_printList(IN PAPI_NODE pnodeHead)
{
	BOOL bRetualVal = FALSE;
	PAPI_NODE pnodeCurr = pnodeHead;

	if (NULL == pnodeCurr)
	{
		printf("Failed to get printList - parameters are null!\n");
		goto lbl_cleanup;
	}

	while (NULL != pnodeCurr)
	{
		printf("DLL Name: %s \n", pnodeCurr->cstrDLL_Name);

		for(INT nIndex = 0; nIndex < (INT)pnodeCurr->nStringSize; nIndex++)
		{	
			printf("\t|-");
			printf("API Name: %s \n", (pnodeCurr->pcstrAPI_Name[nIndex]));

			printf("\t|-");
			printf("Hash: 0x%08x \n", (pnodeCurr->dwHashArray[nIndex]));
			printf("\t|\n");
		}

		printf("\n");
		pnodeCurr = pnodeCurr->papiNext;
	} 

lbl_cleanup:
	return bRetualVal;
}

BOOL LISTAPI_freeNode(IN PAPI_NODE pnodeCurr)
{	
	BOOL bReturnVal = FALSE;
	if (NULL == pnodeCurr)
	{
		printf("Failed to get freeNode - parameters are null!\n");
		goto lbl_cleanup;
	}

	free(pnodeCurr);
	bReturnVal = TRUE;

lbl_cleanup:
	return bReturnVal;
}

