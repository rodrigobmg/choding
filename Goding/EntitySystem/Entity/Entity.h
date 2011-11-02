#pragma once

#include <map>

#include "..\Component\IComponent.h"
#include "..\EntityEvent.h"

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL Entity
{
public:

	enum COMPONENT_TYPE
	{
		VISUAL = 0,
	};

	Entity(void);
	virtual ~Entity(void);
	
	void					SetMatrix( Matrix44& tm ){ m_TM = tm; }
	Matrix44				GetMatrix(){ return m_TM; }

	Entity*					GetLinkedEntity(){ return m_pLinkedTo; }
	void					SetLinkedEntity( Entity* pEntity ){ m_pLinkedTo = pEntity; }

	IComponent*				GetComponent( COMPONENT_TYPE id );
	void					SetComponent( COMPONENT_TYPE id , IComponent* pCom );
	
	void					DispatchEvent( EntityEvent& event );

	uint32					GetEntityID(){ return m_entityID; }
	void					SetEntityID( uint32 id ){ m_entityID = id; }

	const TCHAR*			GetEntityName(){ return m_EntityName.c_str(); }
	void					SetEntityName( const wchar_t* name ){ m_EntityName = name; }

	void					Activate( bool flag ){ m_bActivated = flag; }
	bool					IsActivated(){ return m_bActivated; }

private:
	
	typedef std::map< COMPONENT_TYPE , IComponent* >component_table_t;
	component_table_t		m_Components;
		
	Matrix44				m_TM;
	Entity*					m_pLinkedTo;
	uint32					m_entityID;
	tstring					m_EntityName;
	bool					m_bActivated;
};

