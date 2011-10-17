#ifndef _ENTITYSYSTEM_H_
#define _ENTITYSYSTEM_H_

#include "Entity\IEntity.h"

#include "..\Common\Type\type.h"
#include <vector>

class DLLTYPE EntitySystem
{
public:
	EntitySystem();
	~EntitySystem();

	IEntity*	GetEntity( int32 entityid );

	IEntity*	AllocEntity( int32 entitytype );
	bool		ReleaseEntity( int32 entitytype );

private:

	typedef std::vector< IEntity* > entity_table_t;

	entity_table_t	m_entity_container;

};

#endif