#include "stdafx.h"

#include "Framework.h"
#include "Platform\Win_Application.h"

Framework::Framework()
{
}

Framework::~Framework()
{
}

void Framework::Run()
{
	ColladaStaticMesh loader;
	std::string str("fhi");
	loader.Load( str  , &m_EntitySystem );
// 
// 	for (int i=0 ; i < m_EntitySystem.size(); i++)
// 	{
// 
// 	}

	WinApplication::Run( &m_EntitySystem , &m_RenderSystem , 640 , 480 , false );
}