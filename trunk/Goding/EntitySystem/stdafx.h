#include <vector>

#ifdef _WINDLL
#define DLLTYPE __declspec(dllexport)
#else
#define DLLTYPE __declspec(dllimport)
#endif

