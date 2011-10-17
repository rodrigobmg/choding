#include <vector>

#ifdef _WINDLL
#define DLLTYPE __declspec(dllexport)
#else
#define DLLTYPE __declspec(dllimport)
#endif

#include <map>
#include "d3dx9.h"