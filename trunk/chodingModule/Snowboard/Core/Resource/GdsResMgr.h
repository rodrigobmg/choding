#ifndef _SNOWBOARD_RESMGR_
#define _SNOWBOARD_RESMGR_

#include <map>
#include <list>
#include <HASH_MAP>
#include "Type\GdsResBaseType.h"
#include "Type\GdsResTexture.h"

// ��� ���ҽ� ���ϳ����� �ҹ��ڷ� ����Ѵ�.!!!!!!!!!!!!!!!!

class GdsResMgr : public GdsObject{

private:

	enum RES_TYPE{
		TEX = 0,
	};

 	typedef stdext::hash_map< tstring , GdsResBasePtr >	HASHMAPRes;
 	typedef HASHMAPRes::iterator						HASHMAPRes_ITERATOR;	

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

	GdsResBasePtr		exist( const TCHAR* alias , const TCHAR* filename );
	bool				load_res_dir( const TCHAR* dirpath ,
										std::vector<tstring>& filename, 
										std::list<tstring>& tokenlist , 
										bool bRecursive 
										);
	
	GdsResBasePtr		resourceFactory( const TCHAR* ext ,
										  const TCHAR* filename 
										);

	bool				stack_data_to_container( const TCHAR* alias , 
												const TCHAR* path , 
												GdsResBasePtr pres 
												);

	void				make_token( const TCHAR* token , 
									std::list<tstring>& tokenlist , 
									const TCHAR* delimiters 
									);

public:
	GdsResMgr();
	virtual ~GdsResMgr();

	typedef	struct LOADLIST_WORK_TOKEN
	{
		tstring alias;
		tstring path;
		tstring token;
		bool		 recursive;
		LOADLIST_WORK_TOKEN(){ recursive = false; }
		LOADLIST_WORK_TOKEN( const TCHAR* alias_ , const TCHAR* path_ , const TCHAR* token_ , const bool brecursive_ )
		{
			alias = alias_;
			path  = path_;
			token = token_;
			recursive = brecursive_;
		}
	};


	GdsResBasePtr		Get( const TCHAR* alias , const TCHAR* filename );

	// ������� �۾��ҰŶ� ���������͸� ���� �ʾҽ��ϴ�.~~~ �����ؼ� �Ѱ��ݴϴ�.~~
	HRESULT				CreateList( LOADLIST_WORK_TOKEN work_token );
	void				ReleaseRes( const TCHAR* alias );
	void				ReleaseList( const TCHAR* alias );
	HRESULT				LoadRes( const TCHAR* alias );
	

	virtual void		vClear();
	virtual HRESULT		vCreate( LPDIRECT3DDEVICE9 device );
	virtual	HRESULT		vReCreate( LPDIRECT3DDEVICE9 device , GdsResBasePtr recreated_res );
	virtual HRESULT		vRelease();


};

typedef	boost::shared_ptr< GdsResMgr >	GdsResMgrPtr;

#endif