#pragma once

#include <map>

#include "..\Component\IComponent.h"
#include "..\EntityEvent.h"

class IEntity
{
public:

	IEntity(void);
	virtual ~IEntity(void);
	
	void					SetMatrix( MAT44& tm ){ m_TM = tm; }
	MAT44					GetMatrix(){ return m_TM; }

	IEntity*				GetLinkedEntity(){ return m_pLinkedTo; }
	void					SetLinkedEntity( IEntity* pEntity ){ m_pLinkedTo = pEntity; }

	IComponent*				GetComponent( int32 id );
	void					SetComponent( int32 id , IComponent* pCom );
	
	void					DispatchEvent( EntityEvent event );

	uint32					GetEntityID(){ return m_entityID; }
	void					SetEntityID( uint32 id ){ m_entityID = id; }
private:
	
	typedef std::map< int32 , IComponent* >component_table_t;
	component_table_t	m_Components;
		
	MAT44				m_TM;
	IEntity*				m_pLinkedTo;
	uint32					m_entityID;

};

