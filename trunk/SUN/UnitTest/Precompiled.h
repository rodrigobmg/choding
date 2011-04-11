#ifndef _SUN_UINTTEST_PRECOMPILED_H_
#define _SUN_UINTTEST_PRECOMPILED_H_

#include "../SUN/SUNDefine.h"
#include "../SUN/SUNHeaders.h"

#include "gtest/gtest.h"

#ifdef _DEBUG
#pragma comment (lib, "SUN_d.lib")
#pragma comment (lib, "gtestd.lib")
#else
#pragma comment (lib, "SUN.lib")
#pragma comment (lib, "gtest.lib")
#endif




#endif