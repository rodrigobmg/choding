#pragma once

#include <map>

#include "..\Component\IComponent.h"
#include "..\EntityEvent.h"
#include "..\..\Common\Camera\Camera.h"


#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL IEntity
{
public:

	IEntity(void);
	virtual ~IEntity(void);
	
	void					SetMatrix( Matrix44& tm ){ m_TM = tm; }
	Matrix44				GetMatrix(){ return m_TM; }

	IEntity*				GetLinkedEntity(){ return m_pLinkedTo; }
	void					SetLinkedEntity( IEntity* pEntity ){ m_pLinkedTo = pEntity; }

	IComponent*				GetComponent( int32 id );
	void					SetComponent( int32 id , IComponent* pCom );
	
	void					DispatchEvent( EntityEvent& event );

	uint32					GetEntityID(){ return m_entityID; }
	void					SetEntityID( uint32 id ){ m_entityID = id; }

	const wchar_t*			GetEntityName(){ return m_EntityName.c_str(); }
	void					SetEntityName( const wchar_t* name ){ m_EntityName = name; }

private:
	
	typedef std::map< int32 , IComponent* >component_table_t;
	component_table_t		m_Components;
		
	Matrix44				m_TM;
	IEntity*				m_pLinkedTo;
	uint32					m_entityID;
	tstring					m_EntityName;

	Camera					m_Camera;

};

