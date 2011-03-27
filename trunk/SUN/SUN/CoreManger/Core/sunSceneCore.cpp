#include "Precompiled.h"
#include "sunSceneCore.h"



sunSceneCore::sunSceneCore()
{
	m_pGraphicCore	= NULL;

	m_pCurrentScene	= NULL;
	m_pNextScene	= NULL;

	m_spProgressCamera	= NULL;
}


sunSceneCore::~sunSceneCore()
{
	m_spProgressCamera = NULL;
}

sunSceneCore* sunSceneCore::GetCore()
{
	assert( g_pCoreManager);
	return static_cast<sunSceneCore*>(g_pCoreManager->GetCore( CORENAME_SCENE ));
}

int sunSceneCore::InitializeCore()
{
	m_pGraphicCore = sunGraphicCore::GetCore();
	assert(m_pGraphicCore);

	return true;
}

void sunSceneCore::DestoryCore()
{
	//소멸순서 씬 -> 그래픽

//	assert( m_pGraphicCore );
//		m_pGraphicCore->ClearRenderList();

	m_spProgressCamera = NULL;

	if(NULL != m_pCurrentScene)
		m_pCurrentScene->DestroyScene();
	if(NULL != m_pNextScene)
		m_pNextScene->DestroyScene();

	Safe_Delete(m_pCurrentScene);
	Safe_Delete(m_pNextScene);

}

void sunSceneCore::UpdateCore()
{
	BeginUpdate();
	Update();
	EndUpdate();
}



void sunSceneCore::BeginUpdate()
{
	if(m_pNextScene)
	{
		if(m_pCurrentScene)
		{
			m_pCurrentScene->DestroyScene();
			Safe_Delete( m_pCurrentScene );
		}


		//m_pNextScene->SetSceneCore(this);
		if( m_pNextScene->InitializeScene() )
		{
			m_pCurrentScene = m_pNextScene;
			m_pNextScene = NULL;
		}
		else
		{
			Safe_Delete( m_pNextScene );
		}

		if(m_pCurrentScene)
		{
			m_pCurrentScene->BeginUpdateScene();
		}
	}
}


void sunSceneCore::Update()
{
	if(m_pCurrentScene)
	{
		m_spProgressCamera = m_pCurrentScene->GetDefaultCamera();

		m_pCurrentScene->UpdateScene();
		m_pCurrentScene->UpdateSceneList();
	}
}

void sunSceneCore::EndUpdate()
{
	if(m_pCurrentScene)
	{
		m_pCurrentScene->EndUpdateScene();

		RegistRenderList();
	}
}

void sunSceneCore::RegistRenderList()
{
	assert(m_pGraphicCore);

	if(NULL == m_pCurrentScene)
		return;

	sunNodePtr spRoot = m_pCurrentScene->GetRootNode();
	if(NULL == spRoot)
		return;

	// 그래픽 코어에 렌더리스트 등록을 요청
	m_pGraphicCore->ClearRenderList();
	m_pGraphicCore->RegistRenderList( spRoot->GetChildList() );
	
	
	assert(m_pGraphicCore->GetRenderer());

	m_pGraphicCore->GetRenderer()->SetCameraData( m_spProgressCamera );


}

void sunSceneCore::SetNextScene( sunSceneBase *pScene )
{
	assert(pScene);
	m_pNextScene = pScene;
}

inline sunCameraPtr sunSceneCore::GetProgressCamera()
{
	return m_spProgressCamera;
}


inline sunSceneBase* sunSceneCore::GetCurrentScene()
{
	assert(m_pCurrentScene);
	return m_pCurrentScene;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


ImplementRootRTTI( sunSceneBase );

sunSceneBase::sunSceneBase()
{
	m_spRootNode = new sunNode;
	m_spRootNode->SetName(L"Root");

	m_spDefaultCamera = NULL;

}


sunSceneBase::~sunSceneBase()
{
	assert(m_spRootNode);
	//m_spRootNode->Destory();
	m_spRootNode = NULL;

	m_spDefaultCamera = NULL;
}
void sunSceneBase::AppendScene( sunNodePtr spScene )
{
	assert( spScene );
	m_spRootNode->AddChild( spScene );
}


void sunSceneBase::PrependScene( sunNodePtr spScene )
{
	assert(spScene);
	m_spRootNode->AddChildToFront( spScene );
}

void sunSceneBase::RemoveScene( sunNodePtr spScene )
{
	assert(spScene);
	m_spRootNode->RemoveChild( spScene );
}

void sunSceneBase::UpdateSceneList()
{
	if( m_spRootNode )
		m_spRootNode->UpdateListner();
}


inline sunNodePtr sunSceneBase::GetRootNode()
{
	return m_spRootNode;
}



void sunSceneBase::SetDefaultCamera( sunCameraPtr spCamera )
{
	assert( spCamera );
	m_spDefaultCamera = spCamera;

}

sunCameraPtr sunSceneBase::GetDefaultCamera( void )
{
	assert( m_spDefaultCamera );
	return m_spDefaultCamera;
}