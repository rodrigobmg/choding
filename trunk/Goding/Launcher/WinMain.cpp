
#ifndef _GODING_MAIN_H_
#define _GODING_MAIN_H_

#include <Windows.h>

#include "../Framework/Framework.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR cmdline, int iWinMode)
{
	
 	Framework mainFrame;
 	mainFrame.Run();
	
	return 0;
}

#endif