#include "GdsResMgr.h"

#include "Type/GdsResTexture.h"
#include <algorithm>
#include <omp.h>
#include "Log/logger.h"
#include "PerformanceCheck/Performance.h"
#include "Type/GdsResMD2.h"

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
	Release();
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
	Clear();
	return S_OK;
}

void GdsResMgr::ReleaseRes( const TCHAR* alias )
{
	
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
		m_mapRes.erase( itAll );

	
	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
		itList->second.bLoaded = false;

}

void GdsResMgr::ReleaseList( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
	{
		if( itList->second.bLoaded == true )
		{
			assert( 0 && L"�޸𸮿� �ε����� ����Ʈ�� ����� ����. �޸𸮿��� �������� ������");			
			return;
		}
		else
		{
			m_mapAllFilelist.erase( itList );
		}
	}
}

GdsResBasePtr GdsResMgr::vIsExist( const TCHAR* alias , const TCHAR* filename )
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
	GdsResBasePtr p = vIsExist( alias ,filename );
	if ( p )
		return p;

	return GdsResBasePtr( (GdsResBase*)NULL );
}

bool	GdsResMgr::vLoadResforDir( const TCHAR* dirpath , FILE_LIST& filelist , std::list<tstring>& tokenlist , bool bRecursive )
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

				vLoadResforDir( curpath , filelist , tokenlist, bRecursive );
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


void GdsResMgr::vMakeToken( const TCHAR* token , std::list<tstring>& tokenlist , const TCHAR* delimiters )
{
	tstring tstrtoken(token);
	// �� ù ���ڰ� �������� ��� ����
	tstring::size_type lastPos = tstrtoken.find_first_not_of(delimiters, 0);
	// �����ڰ� �ƴ� ù ���ڸ� ã�´�
	tstring::size_type pos     = tstrtoken.find_first_of(delimiters, lastPos);

	while ( tstring::npos != pos || tstring::npos != lastPos)
	{
		// token�� ã������ vector�� �߰��Ѵ�
		tokenlist.push_back( tstrtoken.substr(lastPos, pos - lastPos) );
		// �����ڸ� �پ�Ѵ´�.  "not_of"�� �����϶�
		lastPos = tstrtoken.find_first_not_of( delimiters, pos);
		// ���� �����ڰ� �ƴ� ���ڸ� ã�´�
		pos = tstrtoken.find_first_of(delimiters, lastPos);
	}

	tokenlist.sort();
}

HRESULT GdsResMgr::CreateList( LOADLIST_WORK_TOKEN work_token )
{		
	RES_ALL_FILELIST_MAP::iterator it = m_mapAllFilelist.find( work_token.path );
	if ( it == m_mapAllFilelist.end() )
	{
		std::list<tstring> tokenlist;
		vMakeToken( work_token.token.c_str() , tokenlist , L";" );
		if ( tokenlist.empty() )
		{
			ASSERT( !L"��ū����Ʈ�� ���� �ϳ��� ����." );
			return false;
		}

		SAMPLE_PERFORMANCE sample;
		BEGIN_PERFORMANCE( L"CreateList" );

		RES_STRUCT resStruct;
		vLoadResforDir( work_token.path.c_str() , resStruct.filelist , tokenlist , work_token.recursive );
		if ( !resStruct.filelist.empty() )
		{
			//resStruct.filelist.sort();
			m_mapAllFilelist.insert( std::pair< const TCHAR* , RES_STRUCT >( work_token.alias.c_str() , resStruct ) );
			END_PERFORMANCE( L"CreateList" );
			OUTPUT_PERFORMANCE( L"CreateList" , sample );
			LOG_WARNING_F( " [CreateList] Avg = %d , DeltaTick = %d , Count = %d " , sample.ulAvg , sample.ulTotalDeltaTick , sample.ulCount );
			return true;
		}
	}
	else
	{
		assert( 0 && L" �ߺ� ���� " );
		return false;
	}

	return false;
}

HRESULT GdsResMgr::LoadRes( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itAlllist = m_mapAllFilelist.find( alias );
	if ( itAlllist == m_mapAllFilelist.end() )
	{
		assert( 0 && L"������ ����Ʈ�� ����� �ε��Ϸ��� ���� " );
		return S_FALSE;
	}

	RES_STRUCT& stRes = itAlllist->second;

	if ( stRes.bLoaded == true )
	{
		assert( 0 && L" �� �� �ε��� �޸� ������� �ϰ� å������? �Ѥ� " );
		return S_FALSE;
	}
	
	//Ȯ���� �����ؼ� �˾Ƽ� �°Բ� �ε��ؼ� m_mapRes�� �־��ش�.
	FILE_LIST filelist = stRes.filelist;
	FILE_LIST::iterator it = filelist.begin();
	tstring ext;
	int size = static_cast< int>( filelist.size() );
	bool	bSuccess  = S_OK;
	
//#pragma omp parallel for

	SAMPLE_PERFORMANCE sample;

  	for ( int index = 0 ; index < size ; ++index )
  	{ 
		BEGIN_PERFORMANCE( L"LoadRes" );

  		if ( bSuccess == S_FALSE )
  			break;
  		
  		size_t poscomma = filelist[index].rfind( L"." );
  		ext				= filelist[index].substr( poscomma + 1 , filelist[index].length() );
		GdsResBasePtr pRes = vResourceFactory( ext.c_str() , filelist[index].c_str() );
  		if ( pRes == NULL )
  		{
  			bSuccess = S_FALSE;
  			break;
  		}
		
		bool bret = vStackdata_to_Container( alias , filelist[index].c_str() , pRes );
		if ( bret == false )
		{
			bSuccess = S_FALSE;
			break;
		}
  	}

	// ����Ÿ �ε� ���Ἲ ���� ����Ÿ �ѹ�
	if ( bSuccess == S_FALSE )
	{
		RES_CONTAINER::iterator it = m_mapRes.find( alias );
		if ( it != m_mapRes.end() ) 
		{
			m_mapRes.erase( it );
		}
	}

	END_PERFORMANCE( L"LoadRes" );
	OUTPUT_PERFORMANCE( L"LoadRes" , sample );
	LOG_WARNING_F(" [LoadRes] Avg = %d , DeltaTick = %d , Count = %d", sample.ulAvg , sample.ulTotalDeltaTick , sample.ulCount );
	stRes.bLoaded = bSuccess;
	return bSuccess;
}

bool GdsResMgr::vStackdata_to_Container( const TCHAR* alias , const TCHAR* filepath , GdsResBasePtr pres )
{
	if ( pres == NULL )
		return false;


	tstring wstrpath(filepath);
	size_t poscomma  = wstrpath.rfind( L"\\" );
	tstring filename = wstrpath.substr( poscomma + 1 , wstrpath.length() );
	if ( vIsExist( alias , filename.c_str() ) )
	{
		assert( 0 && L"Ű�� �ߺ��� �־�� �ȵȴ�." );
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
			ASSERT( !L"���ҽ� �ߺ� ����" );
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

GdsResBasePtr GdsResMgr::vResourceFactory( const TCHAR* ext , const TCHAR* filepath )
{
	if ( !_tcscmp( ext , L"bmp" ) || !_tcscmp( ext , L"tga" ) || !_tcscmp( ext , L"jpg" ) || !_tcscmp( ext , L"dds" ) )
	{
		GdsResTexturePtr ptex = GdsResTexturePtr( new GdsResTexture );
		ptex->LoadResource( filepath , m_pDevice );
		return	ptex;	
	}
	else if ( !_tcscmp( ext , L"md2" ) )
	{
		GdsResMD2Ptr	pMd2 = GdsResMD2Ptr( new GdsResMD2 );
		pMd2->LoadResource( filepath , m_pDevice );
	}

	return GdsResBasePtr( (GdsResBase*)NULL);
}