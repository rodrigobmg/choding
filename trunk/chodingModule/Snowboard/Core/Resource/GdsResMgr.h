#ifndef _SNOWBOARD_RESMGR_
#define _SNOWBOARD_RESMGR_

#include "Type/GdsResBaseType.h"
#include "Type/GdsResTexture.h"
#include <map>
#include <list>
#include <HASH_MAP>


class GdsObject;

// 모든 리소스 파일네임은 소문자로 취급한다.!!!!!!!!!!!!!!!!

class GdsResMrg : public GdsObject{

private:

	enum RES_TYPE{
		TEX = 0,
	};

 	typedef stdext::hash_map< tstring , GdsBaseRes* >				HASHMAPRes;
 	typedef stdext::hash_map< tstring , GdsBaseRes* >::iterator	HASHMAPRes_ITERATOR;	

	typedef std::vector<tstring>	FILE_LIST;

	typedef struct RES_STRUCT{
		bool bLoaded;
		FILE_LIST filelist;
		RES_STRUCT()
		{
			bLoaded = false;
			filelist.clear();
		}
		~RES_STRUCT()
		{
			filelist.clear();
		}
	};	


	typedef std::map< tstring , RES_STRUCT >	RES_ALL_FILELIST_MAP;		
	typedef std::map< tstring , HASHMAPRes >	RES_CONTAINER;			


	RES_CONTAINER			m_mapRes;
	RES_ALL_FILELIST_MAP	m_mapAllFilelist;

	LPDIRECT3DDEVICE9	m_pDevice;

	GdsBaseRes*			isExist( const TCHAR* alias , const TCHAR* filename );
	bool				loadResforDir( const TCHAR* dirpath ,
										std::vector<tstring>& filename, 
										std::list<tstring>& tokenlist , 
										bool bRecursive 
										);
	
	bool				loadFactory( const TCHAR* alias, 
										const TCHAR* ext ,
										const TCHAR* filename 
										);

	GdsBaseRes*			loadTexture( const TCHAR* filename );
	bool				stackdata( const TCHAR* alias , 
									const TCHAR* path , 
									GdsBaseRes* pres 
									);
	
	void				makeToken( const TCHAR* token , 
									std::list<tstring>& tokenlist , 
									const TCHAR* delimiters 
									);


public:
	GdsResMrg();
	virtual ~GdsResMrg();


	GdsBaseRes*			Get( const TCHAR* alias , const TCHAR* filename );

	bool				CreateList( const TCHAR* alias , 
									const TCHAR* path , 
									const TCHAR* token , 
									const bool brecursive 
									);
	void				ReleaseRes( const TCHAR* alias );
	void				ReleaseList( const TCHAR* alias );
	HRESULT				LoadRes( const TCHAR* alias );
	

	virtual void		Clear();
	virtual HRESULT		Create( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		Release();


};

#endif