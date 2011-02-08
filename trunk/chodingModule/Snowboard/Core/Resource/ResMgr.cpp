#include "ResMgr.h"

#include "Type/ResTexture.h"
#include "../CoreMgr.h"


CResMrg::CResMrg()
{
	SetName( OBJECT_RES_MGR );
	Clear();
}

CResMrg::~CResMrg()
{

}

void CResMrg::Clear()
{
	m_mapRes.clear();
}

HRESULT CResMrg::Create()
{
	Clear();
	return S_OK;
}

HRESULT CResMrg::Release()
{
	return S_OK;
}

bool CResMrg::Load( const wchar_t* filename , LPDIRECT3DDEVICE9 pDevice )
{
	return true;
}

bool CResMrg::isExist( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return true;
	else
		return false;
}

CBaseRes* CResMrg::Get( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return it->second;
	else
		return NULL;
}
