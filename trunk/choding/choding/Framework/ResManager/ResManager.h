#ifndef _CHODING_RESMANAGER_
#define _CHODING_RESMANAGER_

//#include "../../Main/CommonHeader.h"
#include "../Singleton/CSingleton.h"
#include <string>
#include <d3dx9.h>
#include <assert.h>

template< typename T >
class Resource
{
public:
	Resource()
	{ 
		m_pRes = NULL;
		m_iRefCount = 0; 
		++m_iRefCount;
	}
	virtual ~Resource()
	{
		--m_iRefCount;
		if ( m_iRefCount > 0 )
			ASSERT( 0 );
	}

	T*	Get() { return m_pRes; }
	T const*	Get() const { return m_pRes; }
	void	Release()
	{
		if ( m_iRefCount == 0 && m_pRes != NULL )
		{
			delete m_pRes;
			m_pRes = NULL;
		}
	}

	void	AddRef()
	{
		++m_iRefCount;
	}

	void	DecRef()
	{
		--m_iRefCount;
		if ( m_iRefCount < 0 )
			ASSERT( 0 );
	}

private:

	int	m_iRefCount;
	T*		m_pRes;
};

template< typename Resource >
class CResManager : public CSingleton< CResManager<Resource> >
{
public:
	CResManager(){};
	virtual ~CResManager(){};

	//void		Run();
	//void		AddPath();
	Resource*	Load( std::wstring& wstrResPath )
	{
		printf( "Load Res \n" );
		return NULL;
	}

private:

	std::wstring	m_wstrRootDir;
		
public:
};

//extern Resource< LPDIRECT3DTEXTURE9 >				resTexture;
//extern CResManager< Resource<LPDIRECT3DTEXTURE9> >	g_mgrTexture;

#endif