#ifndef _SNOWBOARD_RESMGR_
#define _SNOWBOARD_RESMGR_


// 모든 리소스 파일네임은 소문자로 취급한다.!!!!!!!!!!!!!!!!

class GdsResMgr
{

private:

	typedef std::vector< LPDIRECT3DINDEXBUFFER9 >	INDEXBUFFER_LIST;
	INDEXBUFFER_LIST	m_listIndexBuffer;

	typedef std::vector< LPDIRECT3DVERTEXBUFFER9 >	VERTEXBUFFER_LIST;	
	VERTEXBUFFER_LIST	m_listVertexBuffer;
	
 	//typedef stdext::hash_map< tstring , GdsResBasePtr >	res_map_table_t;
	typedef std::map< tstring , GdsResBasePtr >	res_map_table_t;
	typedef GdsLookupTable< tstring , tstring > FILE_LIST;

	typedef std::list< GdsGameObject* >	res_gameobject_table_t;
	res_gameobject_table_t	m_res_objects;

	typedef std::list< GdsComponent* > res_component_table_t;
	res_component_table_t	m_res_components;

	FILE_LIST			m_ResFileList;
	res_map_table_t		m_LoadedResList;
	tstring				m_strResBasePath;
	LPDIRECT3DDEVICE9	m_pDevice;

	GdsResBasePtr		exist( const TCHAR* filename );
	bool				load_res_dir( const TCHAR* dirpath ,
										FILE_LIST& filename, 
										std::list<tstring>& tokenlist , 
										bool bRecursive 
										);
	
	GdsResBasePtr		resourceFactory( const TCHAR* ext ,
										  const TCHAR* filename 
										);

	bool				stack_data_to_container( const TCHAR* path , 
												GdsResBasePtr pres 
												);

	void				make_token( const TCHAR* token , 
									std::list<tstring>& tokenlist , 
									const TCHAR* delimiters 
									);

	GdsResBasePtr		load_res( const TCHAR* alias );

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
		LOADLIST_WORK_TOKEN( const TCHAR* path_ , const TCHAR* token_ , const bool brecursive_ )
		{
			path  = path_;
			token = token_;
			recursive = brecursive_;
		}
	};


	GdsResBasePtr		Get( const TCHAR* filename );

	// 쓰레드로 작업할거라서 참조포인터를 쓰지 않았습니다.~~~ 복사해서 넘겨줍니다.~~
	HRESULT				CreateList( LOADLIST_WORK_TOKEN work_token );
	void				ReleaseRes( const TCHAR* resfilename );

	void				Clear();
	HRESULT				Create( LPDIRECT3DDEVICE9 device );
	HRESULT				ReCreate( LPDIRECT3DDEVICE9 device , GdsResBasePtr recreated_res );
	HRESULT				Release();

	void				AllocIndexBuffer( LPDIRECT3DINDEXBUFFER9& pIB , uint32 size );
	void				FreeIndexBuffer( LPDIRECT3DINDEXBUFFER9 pIB );
	int					GetCountIndexBuffer(){ m_listIndexBuffer.size(); }

	void				AllocVertexBuffer( LPDIRECT3DVERTEXBUFFER9& pVB , uint32 size );
	void				FreeVertexBuffer( LPDIRECT3DVERTEXBUFFER9 pVB );
	int					GetCountVertexBuffer(){ m_listVertexBuffer.size(); }

	GdsGameObject*		AllocGameObject();
	void				FreeGameObject( GdsGameObject* pObject );
	int					GetCountGameObject();

	GdsComponent*		AllocGameComponent();
	void				FreeGameComponent( GdsComponent* pComponent );
	int					GetCountComponent();

};

#define RESMGR	singleton_holder< GdsResMgr >::instance()

#endif