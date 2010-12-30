#ifndef _CHODING_RESMANAGER_
#define _CHODING_RESMANAGER_

#include "../Singleton/CSingleton.h"
#include <string.h>
#include <assert.h>

// 리소스에대한 정의
template< typename T >
class Resource : public CObject
{
public:
	Resource()
	{ 
		m_pRes = NULL;
	}
	virtual ~Resource()
	{
		Release();
	}

	T*	Get() { return m_pRes; }
	T const*	Get() const { return m_pRes; }
	void	Release()
	{
		if ( GetRefCount() == 0 && m_pRes != NULL )
		{
			delete m_pRes;
			m_pRes = NULL;
		}
		else
		{
			assert( "해제는 레퍼런스카운트가 0일때");
		}
	}

private:

	T*		m_pRes;

};

// 리소스를 관리하는 매니져
// 메모리에 적재시키고 포인터를 반환
template< typename Resource >
class CResManager : public CSingleton< CResManager<Resource> >
{
public:
	CResManager()
	{
		m_pRes = NULL;
	};
	virtual ~CResManager()
	{

	};

	void			SetRootDir( std::wstring strRootdir ) { m_wstrRootDir = strRootdir; }
//	Resource*		Get( int32_t iKey );


	//Resource*		Load( wchar_t* filename );
	//template<>		LPDIRECT3DTEXTURE9* Load( wchar_t* filename );

	Resource*		Get()
	{ 
		return m_pRes; 
	}
	
private:
	
	std::wstring	m_wstrRootDir;
	Resource*		m_pRes;		

public:
};

namespace resMgr{

	template< class T >
	T*				Load( wchar_t* filename );
	template<>		LPDIRECT3DTEXTURE9* Load( wchar_t* filename );

}

typedef		Resource< LPDIRECT3DTEXTURE9 >			res_Texture;
typedef		res_Texture*							res_Texture_ptr;
typedef		CResManager< res_Texture >				resMgr_Texture;

//extern		resMgr_Texture*							g_pTextureResMgr;

//g_pTextureResMgr = NULL;

#endif