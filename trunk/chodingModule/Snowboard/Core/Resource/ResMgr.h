#pragma once

#include "../Base/CoreBase.h"
#include "Type/ResBaseType.h"
#include <map>
#include <list>


// ��� ���ҽ� ���ϳ����� �ҹ��ڷ� ����Ѵ�.!!!!!!!!!!!!!!!!

class CResMrg : public CCoreBase{

private:

	enum RES_TYPE{
		TEX = 0,
	};

	typedef stdext::hash_map< tstring , CBaseRes* >				HASHMAPRes;
	typedef stdext::hash_map< tstring , CBaseRes* >::iterator	HASHMAPRes_ITERATOR;

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

	HASHMAPRes			m_mapRes;
	LPDIRECT3DDEVICE9	m_pDevice;

	RES_ALL_FILELIST_MAP	m_mapAllFilelist;

	bool				isExist( const TCHAR* filename );
	bool				loadResforDir( const TCHAR* dirpath , std::list<tstring>& filename, bool bRecursive );
	
	bool				loadFactory( const TCHAR* ext , tstring& filename );

	CBaseRes*			loadTexture( const TCHAR* filename );

public:
	CResMrg();
	virtual ~CResMrg();

	CBaseRes*			Get( const TCHAR* filename , bool bforceloading = false );
	CBaseRes*			CreateRes( RES_TYPE eResType );
	int					GetResType( const TCHAR* filename );	

	void				CreateList( const TCHAR* alias , const TCHAR* path , const bool brecursive );
	void				ReleaseResfromList( const TCHAR* alias );
	HRESULT				LoadResfromList( const TCHAR* alias );
	

	virtual void		Clear();
	virtual HRESULT		Create( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		Release();


};