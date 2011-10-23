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
	WinApplication::Run( &m_EntitySystem , &m_RenderSystem , 640 , 480 , false );
}