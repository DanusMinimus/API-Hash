// api_hook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*##############################Includes##########################################*/
#include "hasher.h"

int main()
{
    PAPI_NODE papiHead = NULL;
    LPVOID pLoadLibraryA = NULL;
    LPVOID pGetProcAddr = NULL;
    HMODULE hUser32Lib = NULL;
    LPVOID pMessageBoxA = NULL;

    /*
    Fille linked list with IAT data
    */
    papiHead = HASHER_fillIAT();
    LISTAPI_printList(papiHead);

    /*
    Bruteforce the entire IAT looking for each hash within the linked list
    */
    HASHER_bruteForceIAT(papiHead);

    /*
    Locate GetProcAddress and LoadLibraryA within the IAT using hashes
    */
    pLoadLibraryA = HASHER_locateHashInIAT(LOADLIBHASH);
    pGetProcAddr = HASHER_locateHashInIAT(GETPROCHASH);

    CreateWINAPI(MyLoadLibrary, HMODULE, pLoadLibraryA, LPCSTR);
    hUser32Lib = CallWINAPI(MyLoadLibrary, "User32.dll");

    pMessageBoxA = HASHER_locateHashInEAT((PIMAGE_DOS_HEADER)hUser32Lib, MSGBOXHASH);

    CreateWINAPI(MyMessageBoxA, LRESULT, pMessageBoxA, HWND, LPCSTR, LPCSTR, UINT);
    CallWINAPI(MyMessageBoxA, NULL, "Hello API HASH", "Rekt", 1);

    return 0;
}
