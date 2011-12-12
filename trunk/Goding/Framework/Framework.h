#pragma once

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif


#include "..\EntitySystem\EntitySystem.h"
#include "..\Renderer\RenderSystem.h"


class DLL Framework
{
public:
	
	Framework();
	virtual ~Framework();

public:
	
	void			Run();

private:

	EntitySystem*	m_pEntitySystem;
	RenderSystem*	m_pRenderSystem;

	Camera*			m_pCamera;

	typedef std::vector< Entity* > entity_container_table_t;
	entity_container_table_t	m_EntityContainer;

};

