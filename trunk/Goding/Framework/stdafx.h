#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <Windows.h>

