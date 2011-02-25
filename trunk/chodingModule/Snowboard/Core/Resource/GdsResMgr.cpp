#include "GdsResMgr.h"

#include "Type/GdsResTexture.h"
#include <algorithm>
#include <omp.h>
#include "Log/logger.h"
#include "PerformanceCheck/Performance.h"

/*
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include <tbb/partitioner.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
*/

GdsResMgr::GdsResMgr()
{
	SetName( OBJECT_RES_MGR );
	Clear();
}

GdsResMgr::~GdsResMgr()
{

}

void GdsResMgr::Clear()
{
	m_mapRes.clear();
	m_mapAllFilelist.clear();
}

HRESULT GdsResMgr::Create( LPDIRECT3DDEVICE9 device )
{
	Clear();
	m_pDevice = device;
	return S_OK;
}

HRESULT GdsResMgr::Release()
{
//	RES_CONTAINER::iterator itAll = m_mapRes.begin();
// 	for ( ; itAll != m_mapRes.end() ;  )
// 	{
// 		for_each( itAll->second.begin() , itAll->second.end() , functor::deleter() );
// 		m_mapRes.erase( itAll++ );
// 	}
	m_mapRes.clear();

	RES_ALL_FILELIST_MAP::iterator itlist = m_mapAllFilelist.begin();
	for ( ; itlist != m_mapAllFilelist.end() ; )
	{
		m_mapAllFilelist.erase( itlist++ );
	}	
	
	Clear();
	return S_OK;
}

void GdsResMgr::ReleaseRes( const TCHAR* alias )
{
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
	//	for_each( itAll->second.begin() , itAll->second.end() , functor::deleter() );		
		m_mapRes.erase( itAll );
	}	

	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
	{
		itList->second.bLoaded = false;
	}
}

void GdsResMgr::ReleaseList( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
	{
		if( itList->second.bLoaded == true )
		{
			assert( 0 && "메모리에 로드중인 리스트는 지울수 없다. 메모리에서 해제한후 지워라");			
			return;
		}
		else
		{
			m_mapAllFilelist.erase( itList );
		}
	}
}

GdsResBasePtr GdsResMgr::isExist( const TCHAR* alias , const TCHAR* filename )
{
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
		HASHMAPRes_ITERATOR it = itAll->second.find( filename );
		if ( it != itAll->second.end() )
			return it->second;
	}

	return GdsResBasePtr( (GdsResBase*)NULL );
}


GdsResBasePtr	GdsResMgr::Get( const TCHAR* alias , const TCHAR* filename )
{
	GdsResBasePtr p = isExist( alias ,filename );
	if ( !p)
		return p;

	return GdsResBasePtr( (GdsResBase*)NULL );
}

bool	GdsResMgr::loadResforDir( const TCHAR* dirpath , FILE_LIST& filelist , std::list<tstring>& tokenlist , bool bRecursive )
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

				loadResforDir( curpath , filelist , tokenlist, bRecursive );
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
				tstring wstr( curpath );
				for_each( wstr.begin() , wstr.end() , functor::ToLower() );
				
				size_t poscomma = wstr.rfind( L"." );
				tstring ext		= wstr.substr( poscomma + 1 , wstr.length() );

				if ( std::binary_search( tokenlist.begin() , tokenlist.end() , ext ) )
					filelist.push_back( wstr );
			}	
		}
		
	} while (FindNextFile(hFind, &fd));

	DWORD err = GetLastError();
	FindClose(hFind);
	return (err == ERROR_NO_MORE_FILES);
}


void GdsResMgr::makeToken( const TCHAR* token , std::list<tstring>& tokenlist , const TCHAR* delimiters )
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

bool GdsResMgr::CreateList( const TCHAR* alias , const TCHAR* path , const TCHAR* token , const bool brecursive )
{	
	RES_ALL_FILELIST_MAP::iterator it = m_mapAllFilelist.find( path );
	if ( it == m_mapAllFilelist.end() )
	{
		std::list<tstring> tokenlist;
		makeToken( token , tokenlist , L";" );
		if ( tokenlist.empty() )
		{
			ASSERT( !"토큰리스트에 값이 하나도 없다." );
			return false;
		}

		RES_STRUCT resStruct;
		loadResforDir( path , resStruct.filelist , tokenlist , brecursive );
		if ( !resStruct.filelist.empty() )
		{
			//resStruct.filelist.sort();
			m_mapAllFilelist.insert( std::pair< const TCHAR* , RES_STRUCT >( alias , resStruct ) );
			return true;
		}
	}
	else
	{
		assert( 0 && " 중복 삽입 " );
		return false;
	}

	return false;
}

HRESULT GdsResMgr::LoadRes( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itAlllist = m_mapAllFilelist.find( alias );
	if ( itAlllist == m_mapAllFilelist.end() )
	{
		assert( 0 && " 엉뚱한 리스트의 목록을 로드하려고 했음 " );
		return S_FALSE;
	}

	RES_STRUCT& stRes = itAlllist->second;

	if ( stRes.bLoaded == true )
	{
		assert( 0 && " 왜 또 로드해 메모리 모질라면 니가 책임질래? ㅡㅡ " );
		return S_FALSE;
	}
	
	//확장자 구분해서 알아서 맞게끔 로드해서 m_mapRes에 넣어준다.
	FILE_LIST filelist = stRes.filelist;
	FILE_LIST::iterator it = filelist.begin();
	tstring ext;
	int size = static_cast< int>( filelist.size() );
	bool	bSuccess  = S_OK;
	

// 	struct resFunctor{
// 
// 		bool bSuccess;
// 		FILE_LIST ffilelist;
// 		const TCHAR* alias;
// 		GdsResMgr* pthis;
// 
// 		void operator()( const tbb::blocked_range<size_t>& r ) const
// 		{
// 			for ( size_t i = r.begin() ; i != r.end() ; ++i )
// 			{
// 				if ( bSuccess == S_FALSE )
// 					continue;
// 
// 				size_t poscomma = ffilelist[i].rfind( L"." );
// 				tstring ext		= ffilelist[i].substr( poscomma + 1 , ffilelist[i].length() );
// 				if ( !pthis->loadFactory( alias , ext.c_str() , ffilelist[i].c_str() ) )
// 				{
// 				//	bSuccess = S_FALSE;
// 					continue;
// 				}
// 			}			
// 		}
// 
// 		resFunctor( GdsResMgr* p , FILE_LIST& rfilelist , const TCHAR* palias , bool bflag ){
// 			ffilelist = rfilelist;
// 			bSuccess = bflag;
// 			alias = palias;
// 			GdsResMgr* pthis = p;
// 		};
// 
// 	};
// 	tbb::task_scheduler_init();
// 	tbb::parallel_for( tbb::blocked_range< size_t >( 0, filelist.size() ), resFunctor( this , filelist , alias , bSuccess ) , tbb::auto_partitioner() );


//#pragma omp parallel for

	SAMPLE_PERFORMANCE sample;

	LOG_WARNING_F(" res load start " );
  	for ( int index = 0 ; index < size ; ++index )
  	{ 
		BEGIN_PERFORMANCE( L"load test" );

  		if ( bSuccess == S_FALSE )
  			break;
  		
  		size_t poscomma = filelist[index].rfind( L"." );
  		ext				= filelist[index].substr( poscomma + 1 , filelist[index].length() );
  		if ( !loadFactory( alias , ext.c_str() , filelist[index].c_str() ) )
  		{
  			bSuccess = S_FALSE;
  			break;
  		}
  	}
	END_PERFORMANCE( L"load test" );
	OUTPUT_PERFORMANCE( L"load test" , sample );
	LOG_WARNING_F(" res load finish avg tick = %d , max tick = %d , min tick = %d , total tick = %d count = %d",
				sample.ulAvg , sample.ulMax , sample.ulMin , sample.ulTotalDeltaTick , sample.ulCount );

	stRes.bLoaded = bSuccess;

	return bSuccess;
}

bool GdsResMgr::loadFactory( const TCHAR* alias, const TCHAR* ext , const TCHAR* filepath )
{
	//::EnterCriticalSection(&this->m_oCriticalSection);

	if ( !_tcscmp( ext , L"bmp" ) )
	{
		GdsResTexture* ptex =  dynamic_cast<GdsResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;
	
		
		bool bresult = stackdata( alias , filepath , ptex );		
		
		return bresult;
	}
	else if( !_tcscmp( ext , L"tga" ) )
	{
		GdsResTexture* ptex =  dynamic_cast<GdsResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;


		bool bresult = stackdata( alias , filepath , ptex );		

		return bresult;
	}
	else if( !_tcscmp( ext , L"jpg" ) )
	{
		GdsResTexture* ptex =  dynamic_cast<GdsResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;


		bool bresult = stackdata( alias , filepath , ptex );		

		return bresult;
	}
	else if( !_tcscmp( ext , L"dds" ) )
	{
		GdsResTexture* ptex =  dynamic_cast<GdsResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;


		bool bresult = stackdata( alias , filepath , ptex );		

		return bresult;
	}

	return false;
}

bool GdsResMgr::stackdata( const TCHAR* alias , const TCHAR* filepath , GdsResBase* pres )
{
	tstring wstrpath(filepath);
	size_t poscomma  = wstrpath.rfind( L"\\" );
	tstring filename = wstrpath.substr( poscomma + 1 , wstrpath.length() );
	if ( isExist( alias , filename.c_str() ) )
	{
		assert( 0 && "키값 중복이 있어서는 안된다." );
		return false;
	}		
	RES_CONTAINER::iterator itResAll = m_mapRes.find( alias );
	if ( itResAll != m_mapRes.end() )
	{
		HASHMAPRes_ITERATOR itRes = itResAll->second.find( filename );
		if ( itRes == itResAll->second.end() )
		{
			itResAll->second.insert( make_pair( filename , pres ) );
		}
		else
		{
			ASSERT( !"리소스 중복 저장" );
		}
	}
	else
	{
		HASHMAPRes hmapRes;
		hmapRes.insert( make_pair( filename , pres ) );
		m_mapRes.insert( std::pair< const TCHAR* , HASHMAPRes >( alias , hmapRes ) );
	}	
	return true;
}

GdsResBase* GdsResMgr::loadTexture( const TCHAR* filepath )
{
	GdsResTexture* pRes = new GdsResTexture;
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

