#pragma once

#include <map>

#include "..\Component\IComponent.h"
#include "..\EntityEvent.h"

class IEntity
{
public:
	IEntity(void);
	virtual ~IEntity(void);
	
	void					SetMatrix( D3DXMATRIX& tm ){ m_TM = tm; }
	D3DXMATRIX				GetMatrix(){ return m_TM; }

	IEntity*				GetLinkedEntity(){ return m_pLinkedTo; }
	void					SetLinkedEntity( IEntity* pEntity ){ m_pLinkedTo = pEntity; }

	IComponent*				GetComponent( com_id id );
	void					SetComponent( com_id id , IComponent* pCom );
	
	void					DispatchEvent( EVENT_TYPE event );

	uint32					GetEntityID(){ return m_entityID; }
	void					SetEntityID( uint32 id ){ m_entityID = id; }
private:
	
	typedef std::map< com_id , IComponent* >component_table_t;
	component_table_t	m_Components;
		
	D3DXMATRIX				m_TM;
	IEntity*				m_pLinkedTo;
	uint32					m_entityID;

};

