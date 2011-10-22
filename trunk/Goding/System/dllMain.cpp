#include "stdafx.h"

HMODULE gDLLHandle = NULL;

BOOL APIENTRY DllMain( HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	gDLLHandle = (HMODULE)hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:    


		break;
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: 
		break;
	}
	//	int sbh = _set_sbh_threshold(1016);

	return TRUE;
}