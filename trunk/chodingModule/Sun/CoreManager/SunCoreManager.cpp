#include "Precompiled.h"
#include "SunCoreManager.h"

Sun_CCoreManager::Sun_CCoreManager(HWND hWnd):m_hWnd(NULL)
{
	m_hWnd = hWnd;
}


Sun_CCoreManager::~Sun_CCoreManager()
{
	DestroyCore();
}



void Sun_CCoreManager::DestroyCore()
{


	Core_HASHMAP_ITERATOR iterCore = m_CoreList.begin();

	while( iterCore != m_CoreList.end())
	{
		Sun_CCoreBase *pCore = iterCore->second;
		if(pCore)
		{
			pCore->DestoryCore();
			Safe_Delete(pCore);
			m_CoreList.erase( iterCore++ );
		}
		else
		{
			assert(0);
		}
	}

	m_CoreList.clear();
	
}

Sun_CCoreBase* Sun_CCoreManager::RegisterCore( const tstring& strName, Sun_CCoreBase* pCore)
{

	assert(pCore);
	pCore->InitializeCore();
	pCore->SetCoreName( strName );

	//insert가 실패하는 경우도 있으니 예외 처리해야함.
	//m_CoreList.insert( make_pair( L"", pCore));

	m_CoreList.insert( make_pair( strName, pCore ) );

	return pCore;
}

