#include "CoreMgr.h"

CCoreMgr::CCoreMgr()
{
	SetName( L"CoreMgr" );
}

CCoreMgr::~CCoreMgr()
{

}

CCoreMgr* CCoreMgr::New()
{
	return new CCoreMgr;
}

CSnow* CCoreMgr::GetCore( const tstring& strCoreName )
{
	HASHMAPCore_ITERATOR iter = m_mapCore.find( strCoreName );
	if ( iter != m_mapCore.end() )
		return iter->second;	

	return NULL;	
}

CSnow* CCoreMgr::RegisterCore( const tstring& corename , CSnow* pCore )
{
	if ( pCore == NULL || corename == L"" )
	{
		assert( "왜 널을 넣으려 했나요?" );
		return NULL;
	}

	m_mapCore.insert( make_pair( corename , pCore ) );
	return pCore;
}

void CCoreMgr::DestroyCore()
{
	HASHMAPCore_ITERATOR itbegin = m_mapCore.begin();
	HASHMAPCore_ITERATOR itend	= m_mapCore.end();

	for ( ; itbegin != itend ; ++itbegin )
	{
		delete itbegin->second;
	}

	m_mapCore.clear();
}

void CCoreMgr::UnregisterCore( const tstring& corename )
{
	HASHMAPCore_ITERATOR it = m_mapCore.find( corename );
	if ( it == m_mapCore.end() )
		return;

	delete it->second;
	m_mapCore.erase( it );
}