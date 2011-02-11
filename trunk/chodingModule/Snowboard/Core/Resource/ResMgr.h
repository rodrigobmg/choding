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

	typedef stdext::hash_map< tstring , CBaseRes* >				HASHMAPRes;
	typedef stdext::hash_map< tstring , CBaseRes* >::iterator	HASHMAPRes_ITERATOR;

	typedef std::list<wstring>	FILE_LIST;
	typedef struct RES_STRUCT{
		bool bLoaded;
		FILE_LIST filelist;
		RES_STRUCT()
		{
			bLoaded = false;
			filelist.clear();
		}
	};	

	
	typedef std::map< const wchar_t* , RES_STRUCT >	RES_ALL_FILELIST_MAP;	

	HASHMAPRes			m_mapRes;
	LPDIRECT3DDEVICE9	m_pDevice;

	RES_ALL_FILELIST_MAP	m_mapAllFilelist;

	bool				isExist( const wchar_t* filename );
	bool				loadResforDir( const wchar_t* dirpath , std::list<wstring>& filename, bool bRecursive );
	
	bool				loadFactory( const wchar_t* ext , wstring& filename );

	CBaseRes*			loadTexture( const wchar_t* filename );

public:
	CResMrg();
	virtual ~CResMrg();

	CBaseRes*			Get( const wchar_t* filename , bool bforceloading = false );
	CBaseRes*			CreateRes( RES_TYPE eResType );
	int					GetResType( const wchar_t* filename );	

	void				CreateList( const wchar_t* alias , const wchar_t* path , const bool brecursive );
	HRESULT				LoadResfromList( const wchar_t* alias );
	

	virtual void		Clear();
	virtual HRESULT		Create( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		Release();


};