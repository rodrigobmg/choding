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

bool CResMrg::Load( const wchar_t* filename , LPDIRECT3DDEVICE9 pDevice )
{
	if ( pDevice == NULL )
		return false;

	if ( !isExist( filename ) )
	{
		//없네?
		CResTexture* pResTex = new CResTexture;		
		D3DXCreateTextureFromFile( pDevice, filename , (LPDIRECT3DTEXTURE9*)pResTex->Get() );
		m_mapRes.insert( make_pair( filename , pResTex ) );
	}
	else
	{
		CResTexture* pResTex = new CResTexture;
		//있네?
		D3DXCreateTextureFromFile( pDevice , filename , (LPDIRECT3DTEXTURE9*)pResTex->Get() );
		m_mapRes.insert( make_pair( filename , pResTex ) );
	}
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

CSnow* CResMrg::Get( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return it->second;
	else
		return NULL;
}

void CResMrg::Create()
{
	Clear();
}