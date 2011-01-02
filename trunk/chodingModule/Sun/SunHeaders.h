#pragma once


//코어 이름
#define CORENAME_GRAPHIC			L"GraphicCore"



#define Safe_Release(vPos){if(vPos){vPos->Release(); vPos = NULL;}}
#define Safe_Delete(vPos) {if(vPos){delete vPos; vPos = NULL;}}
#define Safe_Delete_Array(vPos) { if(vPos) { delete[] vPos; vPos = NULL; } }



using namespace std;
typedef basic_string<TCHAR> tstring;




#include "CoreManager/Core/SunCoreBase.h"
#include "CoreManager/Core/SunGraphicCore.h"

#include "CoreManager/SunCoreManager.h"

extern Sun_CCoreManager*		g_pCoreManager;
