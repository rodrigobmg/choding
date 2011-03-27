#include "Precompiled.h"
#include "sunCoreManager.h"

ImplementRootRTTI(sunCoreManager);

sunCoreManager::sunCoreManager(HWND hWnd) :
m_hWnd(hWnd)
{
	m_iFPS				= 0;				
	m_iFPSCnt			= 0;			
	m_fFPSAccumulTime	= 0.0f;	
	m_fElapsedTime		= 0.0f;		
	m_bUsingQPF			= false;	
}

sunCoreManager::~sunCoreManager()
{

}



void sunCoreManager::DestroyCore()
{
	int iDestoryOrder = static_cast<int>(m_CoreList.size()) - 1;
	Core_HASHMAP_ITERATOR iterCore = m_CoreList.begin();

	while( iterCore != m_CoreList.end())
	{
		//커널 소멸 순서는 커널 등록 역순으로 한다.
		sunCoreBase* pCore = dynamic_cast<sunCoreBase*>(iterCore->second);
		assert(pCore);

		if( pCore->GetDestoryOrder() == iDestoryOrder )
		{
			pCore->DestoryCore();
			Safe_Delete( pCore );
			m_CoreList.erase( iterCore );
			iterCore = m_CoreList.begin();
			-- iDestoryOrder;
		}
		else
			++iterCore;
	}
		
	m_CoreList.clear();
}

sunCoreBase* sunCoreManager::RegisterCore( const tstring& strName, sunCoreBase* pCore)
{
	assert( pCore );

	pCore->InitializeCore();
	pCore->SetDestroyOrder( (int)m_CoreList.size());
	pCore->SetCoreName( strName );
	m_CoreList.insert( make_pair(strName, pCore));

	return pCore;
}

//inline sunCoreBase* sunCoreManager::GetCore( const tstring& strName )
//{
//	Core_HASHMAP_ITERATOR it = m_CoreList.find( strName );
//	if( it != m_CoreList.end() )
//		return it->second;
//
//	return NULL;
//}
//
//
//inline HWND sunCoreManager::GetWinHWND( void )
//{
//	assert( m_hWnd );
//	return m_hWnd;
//}