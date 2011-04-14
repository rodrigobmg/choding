#include "GdsResBaseType.h"

GdsResBase::GdsResBase()
{
	SetName( OBJECT_RES_BASE );	
}

GdsResBase::~GdsResBase()
{

}

HRESULT GdsResBase::Create( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
	m_strPath = path;
	return vLoadResource( device );
}

HRESULT GdsResBase::ReCreate( LPDIRECT3DDEVICE9 device )
{ 
	vRelease();
	return vLoadResource( device ); 
}

HRESULT GdsResBase::LoadResource( LPDIRECT3DDEVICE9 device )
{ 
	return vLoadResource(device); 
}

void GdsResBase::Clear()
{ 
}
