#include "ResMgr.h"

#include "Type/ResTexture.h"
#include "../CoreMgr.h"
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
	//std::for_each( m_mapRes.begin() , m_mapRes.end() , functor::deleter() );
	//std::for_each( m_mapAllFilelist.begin() , m_mapAllFilelist.end() , functor::deleter() );
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
		}
		else
		{
		}
	}

	return NULL;
}

bool	CResMrg::loadResforDir( const wchar_t* dirpath , FILE_LIST& filelist , bool bRecursive )
{
	if ( dirpath == NULL )
		return false;

	WIN32_FIND_DATA fd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	wstring dirspec( dirpath );
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
				wchar_t curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				wsprintf( curpath , L"%s\\%s" , dirpath , fd.cFileName );

				loadResforDir( curpath , filelist , bRecursive );
			}
		}
		else if ( fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			wstring wstr( fd.cFileName );
			size_t pos = wstr.find( L"." );
			if ( pos > 0 )
			{
				wchar_t curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				wsprintf( curpath , L"%s\\%s" , dirpath , fd.cFileName );
				wstring wstr( curpath );
				for_each( wstr.begin() , wstr.end() , functor::ToLowerW() );
				filelist.push_back( wstr );
			}	
		}
		
	} while (FindNextFile(hFind, &fd));

	DWORD err = GetLastError();
	FindClose(hFind);
	return (err == ERROR_NO_MORE_FILES);
}

void CResMrg::CreateList( const wchar_t* alias , const wchar_t* path , const bool brecursive )
{	
	RES_ALL_FILELIST_MAP::iterator it = m_mapAllFilelist.find( path );
	if ( it == m_mapAllFilelist.end() )
	{
		RES_STRUCT resStruct;
		loadResforDir( path , resStruct.filelist , brecursive );
		if ( !resStruct.filelist.empty() )
		{
			resStruct.filelist.sort();
			m_mapAllFilelist.insert( pair< const wchar_t* , RES_STRUCT >( alias , resStruct ) );
		}		
	}
	else
	{
		assert( 0 && " 중복 삽입 " );
		return;
	}
}

HRESULT CResMrg::LoadResfromList( const wchar_t* alias )
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
	wstring ext;
	for ( ; it != filelist.end() ; ++it )
	{
		size_t poscomma = it->rfind( L"." );
		ext				= it->substr( poscomma + 1 , it->length() );
		loadFactory( ext.c_str() , (*it) );
	}

	stRes.bLoaded = true;
}

bool CResMrg::loadFactory( const wchar_t* ext , wstring& filepath )
{
	if ( !wcscmp( ext , L"bmp" ) )
	{
		CResTexture* ptex =  dynamic_cast<CResTexture*>( loadTexture( filepath.c_str() ) );
		if ( ptex == NULL )
			return S_FALSE;

		wstring wstrpath(filepath);
		size_t poscomma  = wstrpath.rfind( L"\\" );
		wstring filename = wstrpath.substr( poscomma + 1 , wstrpath.length() );
		m_mapRes.insert( make_pair( filename.c_str() , ptex ) );
	}
}

CBaseRes* CResMrg::loadTexture( const wchar_t* filepath )
{
	CResTexture* pRes = new CResTexture;
	if ( pRes == NULL )
		return NULL;

	if ( m_pDevice == NULL )
		return NULL;

	if ( !wcscmp( filepath , L"" ) )
		return NULL;

	if ( SUCCEEDED( D3DXCreateTextureFromFile( m_pDevice , filepath , pRes->GetPtr() ) ) )
	{
		return pRes;
	}
}

