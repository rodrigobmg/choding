#pragma once
#include "..\Component\IComponent.h"

#include <map>

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
	
private:
	
	typedef std::map< com_id , IComponent* >component_table_t;
	component_table_t	m_Components;
		
	D3DXMATRIX				m_TM;
	IEntity*				m_pLinkedTo;

};

