#ifndef _SUN_COREMANAGER_COREMANAGER_H_
#define _SUN_COREMANAGER_COREMANAGER_H_



class sunCoreBase;

class DLL_API sunCoreManager
{

	DeclareRootRTTI( sunCoreManager )


public:
	sunCoreManager(HWND hWnd);
	~sunCoreManager();

public:
	//virtual HWND GetMainWnd();

public:
	inline HWND GetWinHWND();
	inline sunCoreBase* GetCore( const tstring& strName );
	
public:
	sunCoreBase* RegisterCore( const tstring& strName, sunCoreBase* pCoreBase);
	void		 DestroyCore();


protected:
	typedef stdext::hash_map<tstring, sunCoreBase*>				Core_HASHMAP;
	typedef stdext::hash_map<tstring, sunCoreBase*>::iterator 	Core_HASHMAP_ITERATOR;
	Core_HASHMAP	m_CoreList;


	HWND		m_hWnd;

	int			m_iFPS;						// �ʴ� ���� ������ ��ġ
	int			m_iFPSCnt;					// ������ ���� ��ġ
	float		m_fFPSAccumulTime;			// ������ ��ġ ����� ���� ���� �ð�.
	float		m_fElapsedTime;				// 1������ ��� �ð�
	bool		m_bUsingQPF;				// QPF ���� ���� �÷���
	//LONGLONG	m_i


};


inline HWND sunCoreManager::GetWinHWND()
{
	assert( m_hWnd );
	return m_hWnd;
}

inline sunCoreBase* sunCoreManager::GetCore( const tstring& strName )
{
	Core_HASHMAP_ITERATOR it = m_CoreList.find( strName );
	if( it != m_CoreList.end() )
		return it->second;

	return NULL;
}

#endif