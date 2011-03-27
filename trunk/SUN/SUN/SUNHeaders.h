#ifndef _SUN_HEADERS_H_
#define _SUN_HEADERS_H_


const D3DXVECTOR3	VECT3_ZERO( 0.0f, 0.0f, 0.0f);

//코어 이름 매크로
#define CORENAME_GRAPHIC		L"GraphicCore"
#define CORENAME_SCENE			L"SceneCore"


//매크로
#define Safe_Release(vPos){if(vPos) { vPos->Release(); vPos = NULL;} }
#define Safe_Delete(vPos){if(vPos)  { delete vPos; vPos = NULL;} }
#define Safe_Delete_Array(vPos){if(vPos) { delete[] vPos; vPos = NULL;} }





//SUN 헤더들
#include "Class/sunSingleton.h"

#include "Class/sunSmartPointer.h"
#include "Class/sunSmartPointerType.h"
#include "Class/sunRef.h"
#include "Class/sunRTTI.h"


#include "Object/Node/sunObject.h"
#include "Object/Node/sunNode.h"
#include "Object/Node/sunRenderNode.h"
#include "Object/sunFrustum.h"
#include "Object/sunCamera.h"


#include "Resource/sunStream.h"
#include "Resource/sunResourceFactory.h"
#include "Resource/sunResource.h"
#include "Resource/sunMesh.h"



#include "CoreManger/Renderer/sunRenderer.h"
#include "CoreManger/Renderer/sunRendererDX9.h"

#include "CoreManger/Core/sunCoreBase.h"
#include "CoreManger/Core/sunGraphicCore.h"
#include "CoreManger/Core/sunSceneCore.h"
#include "CoreManger/sunCoreManager.h"

#include "../SUNBridge/Messenger.h"


extern sunCoreManager* g_pCoreManager;


#endif