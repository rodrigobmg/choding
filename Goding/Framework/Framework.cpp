#include "stdafx.h"

#include "Framework.h"
#include "Platform\Win_Application.h"

Framework::Framework():
m_pEntitySystem(NULL),
m_pRenderSystem(NULL)
{
	m_pRenderSystem = new RenderSystem;
	m_pEntitySystem = new EntitySystem;
}

Framework::~Framework()
{
}

void Framework::Run()
{
	WinApplication::Init( m_pEntitySystem , m_pRenderSystem );

	ColladaStaticMesh loader;
	std::string str("..//asset//StaticExample.dae");
	loader.Load( str  , WinApplication::GetEntitySystem() );
 
	WinApplication::Run( 640 , 480 , false );
}