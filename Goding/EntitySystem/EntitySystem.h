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

typedef std::vector< Entity* > entity_table_t;
typedef std::vector< IComponent* > component_table_t;

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
	
	entity_table_t*	GetActivatedEntityList(){ return &m_activated_entity_container; }
	entity_table_t*	GetAallEntityList(){ return &m_entity_container; }

	void		Update( float fAccumtime );
	
	

private:
	
	entity_table_t	m_entity_container;	
	component_table_t m_component_container;
	entity_table_t	m_activated_entity_container;

};

#endif