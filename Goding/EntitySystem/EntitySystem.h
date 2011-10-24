#ifndef _ENTITYSYSTEM_H_
#define _ENTITYSYSTEM_H_

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <vector>

#include "..\Common\Type\type.h"


#include "Component\IComponent.h"
#include "Entity\IEntity.h"


class DLL EntitySystem
{
public:
	EntitySystem();
	~EntitySystem();

	IEntity*	GetEntity( int32 entityid );

	IEntity*	AllocEntity( int32 entitytype );
	bool		ReleaseEntity( int32 entitytype );

	IComponent*	MakeComponent( int32 comtype );

	void		Update( float fAccumtime );
	
private:

	typedef std::vector< IEntity* > entity_table_t;
	entity_table_t	m_entity_container;

};

#endif