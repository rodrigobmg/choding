#include "GdsResMgr.h"

#include "Type/GdsResTexture.h"
#include <algorithm>
#include <omp.h>
#include "../../System/Logger/logger.h"
#include "../../System/PerformanceCheck/Performance.h"
#include "Type/GdsResASE.h"

/*
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include <tbb/partitioner.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
*/

GdsResMgr::GdsResMgr():
m_pDevice( NULL )
{
	SetName( OBJECT_RES_MGR );
	Clear();
}

GdsResMgr::~GdsResMgr()
{
	Release();
}

void GdsResMgr::Clear()
{
	m_listIndexBuffer.clear();
	m_listVertexBuffer.clear();
	m_listRenderToken.clear();
}

HRESULT GdsResMgr::Create( LPDIRECT3DDEVICE9 device )
{
	Clear();
	m_pDevice = device;	
	return true;
}

HRESULT GdsResMgr::Release()
{
	Clear();	
	return true;
}

void GdsResMgr::ReleaseRes( const TCHAR* alias )
{
	
	HASHMAPRes::iterator itAll = m_LoadedResList.find( alias );
	if ( itAll != m_LoadedResList.end() )
		m_LoadedResList.erase( itAll );

}

GdsResBasePtr GdsResMgr::exist( const TCHAR* filename )
{
	HASHMAPRes::iterator itAll = m_LoadedResList.find( filename );
	if ( itAll != m_LoadedResList.end() )
	{
		return itAll->second;
	}

	return GdsResBasePtr( (GdsResBase*)NULL );
}


GdsResBasePtr	GdsResMgr::Get( const TCHAR* filename )
{
	GdsResBasePtr p = exist( filename );
	if ( p == NULL )
	{
		p = load_res(filename);
	}

	LOG_WARNING_F( "Load Resource %s" , filename );
	return p;
}

GdsResBasePtr GdsResMgr::load_res( const TCHAR* filename )
{
	tstring strFilepath( filename );
	for_each( strFilepath.begin() , strFilepath.end() , functor::ToLower() );

	//FILE_LIST::iterator it = std::find( m_ResFileList.begin() , m_ResFileList.end() , strFilepath );
	tstring* strpath = m_ResFileList.find( strFilepath );
	bool bexist = true;
	if ( strpath == NULL )
		bexist = false;

	if ( bexist )
	{
		size_t poscomma = strFilepath.rfind( L"." );
		tstring ext		= strFilepath.substr( poscomma + 1 , strFilepath.length() );
		GdsResBasePtr pRes = resourceFactory( ext.c_str() , strpath->c_str() );
		if ( pRes != NULL )
		{
			if ( stack_data_to_container( filename , pRes ) )
				return pRes;		
		}
	}	

	return GdsResBasePtr( (GdsResBase*)NULL );
}

bool	GdsResMgr::load_res_dir( const TCHAR* dirpath , FILE_LIST& filelist , std::list<tstring>& tokenlist , bool bRecursive )
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
				_stprintf_s( curpath , L"%s\\%s" , dirpath , fd.cFileName );

				load_res_dir( curpath , filelist , tokenlist, bRecursive );
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
				_stprintf_s( curpath , L"%s\\%s" , dirpath , fd.cFileName );
				tstring wfilepath( curpath );
				for_each( wstr.begin() , wstr.end() , functor::ToLower() );
				
				size_t poscomma = wstr.rfind( L"." );
				tstring ext		= wstr.substr( poscomma + 1 , wstr.length() );

				if ( std::binary_search( tokenlist.begin() , tokenlist.end() , ext ) )
					//filelist.push_back( wstr );
					filelist.add( wstr , wfilepath );
			}	
		}
		
	} while (FindNextFile(hFind, &fd));

	DWORD err = GetLastError();
	FindClose(hFind);
	return (err == ERROR_NO_MORE_FILES);
}


void GdsResMgr::make_token( const TCHAR* token , std::list<tstring>& tokenlist , const TCHAR* delimiters )
{
	tstring tstrtoken(token);
	// 맨 첫 글자가 구분자인 경우 무시
	tstring::size_type lastPos = tstrtoken.find_first_not_of(delimiters, 0);
	// 구분자가 아닌 첫 글자를 찾는다
	tstring::size_type pos     = tstrtoken.find_first_of(delimiters, lastPos);

	while ( tstring::npos != pos || tstring::npos != lastPos)
	{
		// token을 찾았으니 vector에 추가한다
		tokenlist.push_back( tstrtoken.substr(lastPos, pos - lastPos) );
		// 구분자를 뛰어넘는다.  "not_of"에 주의하라
		lastPos = tstrtoken.find_first_not_of( delimiters, pos);
		// 다음 구분자가 아닌 글자를 찾는다
		pos = tstrtoken.find_first_of(delimiters, lastPos);
	}

	tokenlist.sort();
}

HRESULT GdsResMgr::CreateList( LOADLIST_WORK_TOKEN work_token )
{	
	m_strResBasePath = work_token.path;
	if ( !m_ResFileList.empty() )
	{
		ASSERT( 0 );
		return false;
	}

	std::list<tstring> tokenlist;
	make_token( work_token.token.c_str() , tokenlist , L";" );
	if ( tokenlist.empty() )
	{
		ASSERT( !L"토큰리스트에 값이 하나도 없다." );
		return false;
	}


	SAMPLE_PERFORMANCE sample;
	BEGIN_PERFORMANCE( L"CreateList" );

	load_res_dir( work_token.path.c_str() , m_ResFileList , tokenlist , work_token.recursive );
	if ( m_ResFileList.empty() )
	{
		ASSERT( 0 );
		return false;
	}
	m_ResFileList.sort();

	return true;
}

bool GdsResMgr::stack_data_to_container( const TCHAR* filepath , GdsResBasePtr pres )
{
	if ( pres == NULL )
		return false;

	if ( exist( filepath ) )
	{
		ASSERT( 0 && L"키값 중복이 있어서는 안된다." );
		return false;
	}		
	
	HASHMAPRes::iterator it = m_LoadedResList.find( filepath );
	if ( it == m_LoadedResList.end() )
	{
		m_LoadedResList.insert( make_pair( filepath , pres ) );
	}
	
	return true;
}

GdsResBasePtr GdsResMgr::resourceFactory( const TCHAR* ext , const TCHAR* filepath )
{
	if ( !_tcscmp( ext , L"bmp" ) || !_tcscmp( ext , L"tga" ) || !_tcscmp( ext , L"jpg" ) || !_tcscmp( ext , L"dds" ) )
	{
		GdsResTexturePtr ptex = GdsResTexturePtr( new GdsResTexture );
		ptex->Create( filepath , m_pDevice );
		return	ptex;	
	}
	else if ( !_tcscmp( ext , L"ase" ) ) 
	{
		GdsResASEPtr	pAse = GdsResASEPtr( new GdsResASE );
		pAse->Create( filepath , m_pDevice );
		return pAse;
	}

	return GdsResBasePtr( (GdsResBase*)NULL);
}

HRESULT GdsResMgr::ReCreate( LPDIRECT3DDEVICE9 device , GdsResBasePtr recreated_res )
{
	if ( device == NULL || recreated_res == NULL )
		return false;

	recreated_res->ReCreate( device );
	return true;
}

void GdsResMgr::AllocIndexBuffer( LPDIRECT3DINDEXBUFFER9& pIB , uint32_t size )
{
	if ( m_pDevice )
	{
		if ( FAILED( m_pDevice->CreateIndexBuffer( size , 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &pIB, NULL ) ) )
		{
			pIB = NULL;
			return;
		}
		
		m_listIndexBuffer.push_back( pIB );
	//	LOG_CYAN_F( "Alloc IndexBuffer size = %d byte address[0x%08x] Total Count = %d" , size , pIB , m_listIndexBuffer.size() );
	}
}

void GdsResMgr::FreeIndexBuffer( LPDIRECT3DINDEXBUFFER9 pIB )
{
	INDEXBUFFER_LIST::iterator it = m_listIndexBuffer.begin();
	INDEXBUFFER_LIST::iterator it_end = m_listIndexBuffer.end();
	for ( ; it != it_end ; ++it )
	{
		if( pIB == *it )
		{
			(*it)->Release();
			break;
		}
	}
}

void GdsResMgr::AllocVertexBuffer( LPDIRECT3DVERTEXBUFFER9& pVB , uint32_t size )
{
	if ( m_pDevice )
	{
		if ( FAILED( m_pDevice->CreateVertexBuffer( size , 0, GDSVERTEX::FVF, D3DPOOL_DEFAULT, &pVB, NULL ) ))
		{
			pVB = NULL;
			return;
		}
		
		m_listVertexBuffer.push_back( pVB );
	//	LOG_CYAN_F( "Alloc VertexBuffer size = %d byte address[0x%08x] Total Count = %d" , size , pVB , m_listVertexBuffer.size() );
	}
}

void GdsResMgr::FreeVertexBuffer( LPDIRECT3DVERTEXBUFFER9 pVB )
{
	VERTEXBUFFER_LIST::iterator it = m_listVertexBuffer.begin();
	VERTEXBUFFER_LIST::iterator it_end = m_listVertexBuffer.end();
	for ( ; it != it_end ; ++it )
	{
		if ( *it == pVB )
		{
			(*it)->Release();
			break;
		}
	}
}

void GdsResMgr::AllocRenderObject( GdsRenderObjectPtr& p )
{	
	m_listRenderToken.push_back( p );
	p = GdsRenderObjectPtr( new GdsRenderObject );	
//	LOG_CYAN_F( "Alloc RenderObject address[0x%08x] Total Count = %d" , p , m_listRenderToken.size() );
}

void GdsResMgr::FreeRenderObject( GdsRenderObjectPtr p )
{
	RENDERTOKEN_LIST::iterator it = m_listRenderToken.begin();
	RENDERTOKEN_LIST::iterator it_end = m_listRenderToken.end();
	for ( ; it != it_end ; ++it )
	{
		if ( p == *it )
		{
			m_listRenderToken.erase( it );
			break;
		}
	}
}
