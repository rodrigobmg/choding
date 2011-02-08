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

HRESULT CResMrg::Create( LPDIRECT3DDEVICE9 device )
{
	Clear();
	m_pDevice = device;
	return S_OK;
}

HRESULT CResMrg::Release()
{
	HASHMAPRes_ITERATOR it = m_mapRes.begin();
	for ( ; it != m_mapRes.end() ; ++it )
	{
		SAFE_RELEASE( it->second );
	}

	Clear();
	return S_OK;
}

bool CResMrg::isExist( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return true;
	else
		return false;
}

CBaseRes*	CResMrg::CreateRes( RES_TYPE eResType )
{
	if ( eResType == TEX )
	{
		return new CResTexture;
	}

	return NULL;
}

int CResMrg::GetResType( const wchar_t* filename )
{
	size_t lensize = wcslen( filename );
	
	return lensize;
}

CBaseRes* CResMrg::Get( const wchar_t* filename , bool bforceloading )
{
	if ( isExist( filename ) )
	{
		HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
		return it->second;
	}

	//bforceloading이 ture면 다시 로딩해서 읽어 온다. 
	if ( bforceloading == false )
	{
		return NULL;
	}
	else
	{
		HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
		if ( it == m_mapRes.end() )
		{
			//GetResType( filename );
			CResTexture* pTex = dynamic_cast< CResTexture* >( CreateRes( TEX ) );
			pTex->Load( filename, m_pDevice );
			m_mapRes.insert( make_pair( filename , pTex ) );
		}
		else
		{

		}
	}

	return NULL;
}
