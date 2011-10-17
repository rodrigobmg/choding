#include "stdafx.h"
#include "EntitySystem.h"

EntitySystem::EntitySystem()
{

}

EntitySystem::~EntitySystem()
{

}

IEntity* EntitySystem::GetEntity( int32 entityid )
{
	return NULL;
}

IEntity* EntitySystem::AllocEntity( int32 entitytype )
{
	return NULL;
}

bool EntitySystem::ReleaseEntity( int32 entitytype )
{
	return true;
}
