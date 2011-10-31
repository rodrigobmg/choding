#ifndef _ENTITYSYSTEM_H_
#define _ENTITYSYSTEM_H_


#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif
 
#include <vector>

#include "..\Common\Type\CommonType.h"


#include "Component\IComponent.h"
#include "Entity\Entity.h"


class DLL EntitySystem
{
public:
	EntitySystem();
	~EntitySystem();

	void		Init();
	void		Release();

	Entity*		GetEntity( int32 entityid );

	Entity*		AllocEntity();
	bool		ReleaseEntity( int32 entityid );

	IComponent*	MakeComponent( IComponent::ENABLE_COMPONENT_TYPE comtype );

	void		Update( float fAccumtime );
	
private:

	typedef std::vector< Entity* > entity_table_t;
	entity_table_t	m_entity_container;

	typedef std::vector< IComponent* > component_table_t;
	component_table_t m_component_container;

};

#endif