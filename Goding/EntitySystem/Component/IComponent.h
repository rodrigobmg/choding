#pragma once

class IEntity;

typedef int32 com_id;

class IComponent
{
public:
	IComponent(void);
	virtual ~IComponent(void);

	virtual void	SetComponent( com_id id ) = 0;

	com_id			GetComponentID(){ return m_ID; }

	IEntity*		GetEntity(){ return m_pEntity; }
	void			SetEntity( IEntity* entity ){ m_pEntity = entity; }

protected:
	IEntity*		m_pEntity;
	com_id			m_ID;
};

