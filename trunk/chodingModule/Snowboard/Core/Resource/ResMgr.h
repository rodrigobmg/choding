#pragma once

#include "../Base/CoreBase.h"
#include "Type/ResBaseType.h"
#include <map>
#include <list>


// 모든 리소스 파일네임은 소문자로 취급한다.!!!!!!!!!!!!!!!!

class CResMrg : public CCoreBase{

private:

	enum RES_TYPE{
		TEX = 0,
	};

	typedef stdext::hash_map< const TCHAR* , CBaseRes* >			HASHMAPRes;
	typedef stdext::hash_map< const TCHAR* , CBaseRes* >::iterator	HASHMAPRes_ITERATOR;	

	typedef std::list<tstring>	FILE_LIST;
	typedef struct RES_STRUCT{
		bool bLoaded;
		FILE_LIST filelist;
		RES_STRUCT()
		{
			bLoaded = false;
			filelist.clear();
		}
	};	
	typedef std::map< const TCHAR* , RES_STRUCT >	RES_ALL_FILELIST_MAP;	
	
	typedef std::map< const TCHAR* , HASHMAPRes >	RES_CONTAINER;			


	RES_CONTAINER			m_mapRes;
	RES_ALL_FILELIST_MAP	m_mapAllFilelist;

	LPDIRECT3DDEVICE9	m_pDevice;

	CBaseRes*			isExist( const TCHAR* alias , const TCHAR* filename );
	bool				loadResforDir( const TCHAR* dirpath , std::list<tstring>& filename, bool bRecursive );
	
	bool				loadFactory( const TCHAR* alias, const TCHAR* ext , tstring& filename );

	CBaseRes*			loadTexture( const TCHAR* filename );

	bool				stackdata( const TCHAR* alias , const TCHAR* path , CBaseRes* pres );

public:
	CResMrg();
	virtual ~CResMrg();

	CBaseRes*			Get( const TCHAR* alias , const TCHAR* filename );

	void				CreateList( const TCHAR* alias , const TCHAR* path , const bool brecursive );
	void				ReleaseResfromList( const TCHAR* alias );
	HRESULT				LoadResfromList( const TCHAR* alias );
	

	virtual void		Clear();
	virtual HRESULT		Create( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		Release();


};