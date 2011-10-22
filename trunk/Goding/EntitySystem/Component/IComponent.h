#pragma once

#include "..\..\Common\Type\type.h"

class IEntity;

class IComponent
{
public:
	IComponent(void);
	virtual ~IComponent(void);

	virtual void	SetComponent( int32 id ) = 0;

	int32			GetComponentID(){ return m_ID; }

	IEntity*		GetEntity(){ return m_pEntity; }
	void			SetEntity( IEntity* entity ){ m_pEntity = entity; }

protected:
	IEntity*		m_pEntity;
	int32			m_ID;
};

