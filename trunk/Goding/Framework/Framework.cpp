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

	WinApplication::Run( &m_EntitySystem , &m_RenderSystem , 640 , 480 , false );
}