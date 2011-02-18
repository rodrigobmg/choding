#include "ResMgr.h"

#include "Type/ResTexture.h"
#include <algorithm>
#include <omp.h>
#include "Log/logger.h"

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include <tbb/partitioner.h>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

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
	::InitializeCriticalSection(&this->m_oCriticalSection);

	Clear();
	m_pDevice = device;
	return S_OK;
}

HRESULT CResMrg::Release()
{
	RES_CONTAINER::iterator itAll = m_mapRes.begin();
	for ( ; itAll != m_mapRes.end() ;  )
	{
		for_each( itAll->second.begin() , itAll->second.end() , functor::deleter() );
		m_mapRes.erase( itAll++ );
	}

	RES_ALL_FILELIST_MAP::iterator itlist = m_mapAllFilelist.begin();
	for ( ; itlist != m_mapAllFilelist.end() ; )
	{
		m_mapAllFilelist.erase( itlist++ );
	}	
	
	Clear();
	return S_OK;
}

void CResMrg::ReleaseRes( const TCHAR* alias )
{
	RES_CONTAINER::iterator itAll = m_mapRes.find( alias );
	if ( itAll != m_mapRes.end() )
	{
		for_each( itAll->second.begin() , itAll->second.end() , functor::release() );		
		m_mapRes.erase( itAll );
	}	

	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
	{
		itList->second.bLoaded = false;
	}
}

void CResMrg::ReleaseList( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itList = m_mapAllFilelist.find( alias );
	if ( itList != m_mapAllFilelist.end() )
	{
		if( itList->second.bLoaded == true )
		{
			assert( 0 && "�޸𸮿� �ε����� ����Ʈ�� ����� ����. �޸𸮿��� �������� ������");			
			return;
		}
		else
		{
			m_mapAllFilelist.erase( itList );
		}
	}
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
	CBaseRes* p = isExist( alias ,filename );
	if ( p != NULL )
		p->IncRefCount();
	return p;
}

bool	CResMrg::loadResforDir( const TCHAR* dirpath , FILE_LIST& filelist , std::list<tstring>& tokenlist , bool bRecursive )
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


void CResMrg::makeToken( const TCHAR* token , std::list<tstring>& tokenlist , const TCHAR* delimiters )
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

bool CResMrg::CreateList( const TCHAR* alias , const TCHAR* path , const TCHAR* token , const bool brecursive )
{	
	RES_ALL_FILELIST_MAP::iterator it = m_mapAllFilelist.find( path );
	if ( it == m_mapAllFilelist.end() )
	{
		std::list<tstring> tokenlist;
		makeToken( token , tokenlist , L";" );
		if ( tokenlist.empty() )
		{
			ASSERT( !"��ū����Ʈ�� ���� �ϳ��� ����." );
			return false;
		}

		RES_STRUCT resStruct;
		loadResforDir( path , resStruct.filelist , tokenlist , brecursive );
		if ( !resStruct.filelist.empty() )
		{
			//resStruct.filelist.sort();
			m_mapAllFilelist.insert( pair< const TCHAR* , RES_STRUCT >( alias , resStruct ) );
			return true;
		}
	}
	else
	{
		assert( 0 && " �ߺ� ���� " );
		return false;
	}

	return false;
}

HRESULT CResMrg::LoadRes( const TCHAR* alias )
{
	RES_ALL_FILELIST_MAP::iterator itAlllist = m_mapAllFilelist.find( alias );
	if ( itAlllist == m_mapAllFilelist.end() )
	{
		assert( 0 && " ������ ����Ʈ�� ����� �ε��Ϸ��� ���� " );
		return S_FALSE;
	}

	RES_STRUCT& stRes = itAlllist->second;

	if ( stRes.bLoaded == true )
	{
		assert( 0 && " �� �� �ε��� �޸� ������� �ϰ� å������? �Ѥ� " );
		return S_FALSE;
	}
	
	//Ȯ���� �����ؼ� �˾Ƽ� �°Բ� �ε��ؼ� m_mapRes�� �־��ش�.
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
// 		CResMrg* pthis;
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
// 		resFunctor( CResMrg* p , FILE_LIST& rfilelist , const TCHAR* palias , bool bflag ){
// 			ffilelist = rfilelist;
// 			bSuccess = bflag;
// 			alias = palias;
// 			CResMrg* pthis = p;
// 		};
// 
// 	};
// 	tbb::task_scheduler_init();
// 	tbb::parallel_for( tbb::blocked_range< size_t >( 0, filelist.size() ), resFunctor( this , filelist , alias , bSuccess ) , tbb::auto_partitioner() );


//#pragma omp parallel for
  	for ( int index = 0 ; index < size ; ++index )
  	{ 
  		if ( bSuccess == S_FALSE )
  			continue;
  		
  		size_t poscomma = filelist[index].rfind( L"." );
  		ext				= filelist[index].substr( poscomma + 1 , filelist[index].length() );
  		if ( !loadFactory( alias , ext.c_str() , filelist[index].c_str() ) )
  		{
  			bSuccess = S_FALSE;
  			continue;
  		}
  	}

	stRes.bLoaded = bSuccess;

	return bSuccess;
}

bool CResMrg::loadFactory( const TCHAR* alias, const TCHAR* ext , const TCHAR* filepath )
{
	//::EnterCriticalSection(&this->m_oCriticalSection);

	if ( !_tcscmp( ext , L"bmp" ) )
	{
		CResTexture* ptex =  dynamic_cast<CResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;
	
		
		bool bresult = stackdata( alias , filepath , ptex );		
		
		return bresult;
	}
	else if( !_tcscmp( ext , L"tga" ) )
	{
		CResTexture* ptex =  dynamic_cast<CResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;


		bool bresult = stackdata( alias , filepath , ptex );		

		return bresult;
	}
	else if( !_tcscmp( ext , L"jpg" ) )
	{
		CResTexture* ptex =  dynamic_cast<CResTexture*>( loadTexture( filepath ) );
		if ( ptex == NULL )
			return S_FALSE;


		bool bresult = stackdata( alias , filepath , ptex );		

		return bresult;
	}

	//::LeaveCriticalSection(&this->m_oCriticalSection);

	return false;
}

bool CResMrg::stackdata( const TCHAR* alias , const TCHAR* filepath , CBaseRes* pres )
{
	tstring wstrpath(filepath);
	size_t poscomma  = wstrpath.rfind( L"\\" );
	tstring filename = wstrpath.substr( poscomma + 1 , wstrpath.length() );
	if ( isExist( alias , filename.c_str() ) )
	{
		assert( 0 && "Ű�� �ߺ��� �־�� �ȵȴ�." );
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
			ASSERT( !"���ҽ� �ߺ� ����" );
		}
	}
	else
	{
		HASHMAPRes hmapRes;
		hmapRes.insert( make_pair( filename , pres ) );
		m_mapRes.insert( pair< const TCHAR* , HASHMAPRes >( alias , hmapRes ) );
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

