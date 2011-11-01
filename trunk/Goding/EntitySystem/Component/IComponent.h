#pragma once

#include "..\..\Common\Type\CommonType.h"

class Entity;

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif


class DLL IComponent
{
public:
	IComponent(void);
	virtual ~IComponent(void);
	
	enum ENABLE_COMPONENT_TYPE
	{
		STATIC_MESH = 0,
		SKINNED_MESH,
		MORPHING_MESH,
	};

	virtual void	SetComponent( int32 id ) = 0;

	int32			GetComponentID(){ return m_ID; }

	Entity*			GetEntity(){ return m_pEntity; }
	void			SetEntity( Entity* entity ){ m_pEntity = entity; }

protected:
	Entity*			m_pEntity;
	int32			m_ID;
};

