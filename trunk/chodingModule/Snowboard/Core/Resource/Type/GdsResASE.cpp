#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"

ImplementBoostPool( GdsResASE )

GdsResASE::GdsResASE()
{
	SetName( OBJECT_RES_ASE );
	vClear();
}

GdsResASE::~GdsResASE()
{
}

void GdsResASE::vClear()
{
}

HRESULT GdsResASE::vCreate()
{
	return true;
}

HRESULT GdsResASE::vRelease()
{	
	return true;
}

HRESULT GdsResASE::vLoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
	return true;
}

HRESULT GdsResASE::vReCreate( LPDIRECT3DDEVICE9 device )
{
	vRelease();
	return vLoadResource( m_strPath.c_str() , device );
}