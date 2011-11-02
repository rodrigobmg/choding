#pragma once

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

	ENABLE_COMPONENT_TYPE	GetComponentID(){ return m_ID; }

	Entity*					GetEntity(){ return m_pEntity; }
	void					SetEntity( Entity* entity ){ m_pEntity = entity; }

	virtual	void			onDeviceReset( D3DDevice* pDevice ){};
	virtual void			onDeviceLost(){};

protected:

	Entity*					m_pEntity;
	ENABLE_COMPONENT_TYPE	m_ID;
};

