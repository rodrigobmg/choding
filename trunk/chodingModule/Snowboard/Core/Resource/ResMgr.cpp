#include "ResMgr.h"

#include "Type/ResTexture.h"
#include <algorithm>

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
	m_mapAllFilelist.clear();
}

HRESULT CResMrg::Create( LPDIRECT3DDEVICE9 device )
{
	Clear();
	m_pDevice = device;
	return S_OK;
}

HRESULT CResMrg::Release()
{
//	std::for_each( m_mapRes.begin() , m_mapRes.end() , functor::deleter() );
//	std::for_each( m_mapAllFilelist.begin() , m_mapAllFilelist.end() , functor::deleter() );
	Clear();
	return S_OK;
}

CBaseRes* CResMrg::isExist( const TCHAR* alias , const TCHAR* filename )
{
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
		HASHMAPRes_ITERATOR it = itAll->second.find( filename );
		if ( it != itAll->second.end() )
			return it->second;
	}

	return NULL;
}


CBaseRes* CResMrg::Get( const TCHAR* alias , const TCHAR* filename )
{
	return isExist( alias ,filename );
}

bool	CResMrg::loadResforDir( const TCHAR* dirpath , FILE_LIST& filelist , bool bRecursive )
{
	if ( dirpath == NULL )
		return false;

	WIN32_FIND_DATA fd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	tstring dirspec( dirpath );
	dirspec += L"\\*";

	hFind = FindFirstFile( dirspec.c_str(), &fd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return false;
	}

	do {
		if ( fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && bRecursive )
		{
			if (   _tcscmp(fd.cFileName, L".")
				&& _tcscmp(fd.cFileName, L"..") 
				&& _tcscmp(fd.cFileName, L".svn")
				)
			{
				TCHAR curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				wsprintf( curpath , L"%s\\%s" , dirpath , fd.cFileName );

				loadResforDir( curpath , filelist , bRecursive );
			}
		}
		else if ( fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			tstring wstr( fd.cFileName );
			size_t pos = wstr.find( L"." );
			if ( pos > 0 )
			{
				TCHAR curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				wsprintf( curpath , L"%s\\%s" , dirpath , fd.cFileName );
				tstring wstr( curpath );
				for_each( wstr.begin() , wstr.end() , functor::ToLower() );
				filelist.push_back( wstr );
			}	
		}
		
	} while (FindNextFile(hFind, &fd));

	DWORD err = GetLastError();
	FindClose(hFind);
	return (err == ERROR_NO_MORE_FILES);
}

void CResMrg::CreateList( const TCHAR* alias , const TCHAR* path , const bool brecursive )
{	
	RES_ALL_FILELIST_MAP::iterator it = m_mapAllFilelist.find( path );
	if ( it == m_mapAllFilelist.end() )
	{
		RES_STRUCT resStruct;
		loadResforDir( path , resStruct.filelist , brecursive );
		if ( !resStruct.filelist.empty() )
		{
			resStruct.filelist.sort();
			m_mapAllFilelist.insert( pair< const TCHAR* , RES_STRUCT >( alias , resStruct ) );
		}		
	}
	else
	{
		assert( 0 && " 중복 삽입 " );
		return;
	}
}


void CResMrg::ReleaseResfromList( const TCHAR* alias )
{
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
		for_each( itAll->second.begin() , itAll->second.end() , functor::deleter() );		
		m_mapRes.erase( itAll );
	}	
}


HRESULT CResMrg::LoadResfromList( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itAlllist = m_mapAllFilelist.find( alias );
	if ( itAlllist == m_mapAllFilelist.end() )
	{
		assert( 0 && " 엉뚱한 리스트의 목록을 로드하려고 했음 " );
		return S_FALSE;
	}

	RES_STRUCT stRes = itAlllist->second;

	if ( stRes.bLoaded == true )
	{
		assert( 0 && " 왜 또 로드해 메모리 모질라면 니가 책임질래? ㅡㅡ " );
		return S_FALSE;
	}
	
	//확장자 구분해서 알아서 맞게끔 로드해서 m_mapRes에 넣어준다.
	FILE_LIST filelist = stRes.filelist;
	FILE_LIST::iterator it = filelist.begin();
	tstring ext;
	for ( ; it != filelist.end() ; ++it )
	{
		size_t poscomma = it->rfind( L"." );
		ext				= it->substr( poscomma + 1 , it->length() );
		if ( !loadFactory( alias , ext.c_str() , (*it) ) )
			return S_FALSE;
	}

	stRes.bLoaded = true;
	return S_OK;
}

bool CResMrg::loadFactory( const TCHAR* alias, const TCHAR* ext , tstring& filepath )
{
	if ( !_tcscmp( ext , L"bmp" ) )
	{
		CResTexture* ptex =  dynamic_cast<CResTexture*>( loadTexture( filepath.c_str() ) );
		if ( ptex == NULL )
			return S_FALSE;
		
		bool bresult = true;
		bresult = stackdata( alias , filepath.c_str() , ptex );		
		return bresult;
	}

	return false;
}

bool CResMrg::stackdata( const TCHAR* alias , const TCHAR* filepath , CBaseRes* pres )
{
	tstring wstrpath(filepath);
	size_t poscomma  = wstrpath.rfind( L"\\" );
	tstring filename = wstrpath.substr( poscomma + 1 , wstrpath.length() );
	if ( isExist( alias , filename.c_str() ) )
	{
		assert( 0 && "키값 중복이 있어서는 안된다." );
		return false;
	}

	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
		itAll->second.insert( pair< const TCHAR* , CBaseRes*>( filename.c_str() , pres ) );
	}
	else
	{
		HASHMAPRes hmap;
		hmap.insert( make_pair( filename.c_str() , pres ) );
		m_mapRes.insert( pair< const TCHAR* , HASHMAPRes >( alias , hmap ) );
	}
	return true;
}

CBaseRes* CResMrg::loadTexture( const TCHAR* filepath )
{
	CResTexture* pRes = new CResTexture;
	if ( pRes == NULL )
		return NULL;

	if ( m_pDevice == NULL )
		return NULL;

	if ( !_tcscmp( filepath , L"" ) )
		return NULL;

	if ( SUCCEEDED( D3DXCreateTextureFromFile( m_pDevice , filepath , pRes->GetPtr() ) ) )
	{
		return pRes;
	}

	return NULL;
}

