#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"
#include "../Transform/GdsTransform.h"
#include "../Property/GdsProperty.h"

class GdsNode : public GdsObject , public boost::enable_shared_from_this< GdsNode >
{

public:
	enum CULL_TYPE{
		CULL_ON = 0,
		CULL_OFF,
		CULL_OFF_ALL_CHILD,
	};

private:

	tstring									m_strParentName;

	GdsNode*								m_pParentNode;

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;
	typedef	 std::list< GdsNodePtr >		CHILDNODE_CONTAINER;
	CHILDNODE_CONTAINER						m_ChildNode;	

	GdsPropertyPtr							m_Property;
	D3DXMATRIXA16							m_DXmatWorld;
	GdsTransform							m_matWorld;
	GdsTransform							m_matLocal;


	CULL_TYPE								m_eCull;
	bool									m_bDrawAxis;	

protected:			

	virtual	void							vUpdateGeometry( float fElapsedtime );
	virtual void							vClear();

public:

	GdsNode();
	virtual ~GdsNode();
 	
 	boost::shared_ptr< GdsNode > shared_ptr_this()
 	{
 		return shared_from_this();
 	}

	void					SetParentName( tstring& strParentName ){ m_strParentName = strParentName; }
	tstring&				GetParentName(){ return m_strParentName; }

	GdsNodePtr				GetObjectbyName( tstring& strname );
 	
	void					SetScale( float fvalue ){ m_matLocal.m_fScale = fvalue; }
	float					GetScale(){ return m_matLocal.m_fScale; }

	void					SetCullType( CULL_TYPE eCull ){ m_eCull = eCull; }
	CULL_TYPE				GetCullType(){ return m_eCull; }

	void					SetPropertyState( GdsPropertyPtr pProperty ){ m_Property = pProperty; }
	GdsPropertyPtr			GetPropertyState(){ return m_Property; }

	GdsTransform&			GetTransform(){ return m_matLocal; }
	void					SetTransform( GdsTransform& mat ){ m_matLocal = mat; }

	GdsTransform&			GetWorldTransform(){ return m_matWorld; }
	void					SetWorldTransform( GdsTransform& mat ){ m_matWorld = mat; }

	void					SetTranslate( GdsVector3& pos ){ m_matLocal.m_Translate = pos; }
	void					SetTranslate( float x , float y , float z )
									{
										m_matLocal.m_Translate.x = x ; 
										m_matLocal.m_Translate.y = y ; 
										m_matLocal.m_Translate.z = z; 
									}
	GdsVector3&				GetTranslate() { return m_matLocal.m_Translate; }
	GdsVector3&				GetWorldTanslate(){ return m_matWorld.m_Translate; }

	void					SetRotate( GdsMatrix3& matRot ){ m_matLocal.m_Rotate = matRot; }
	GdsMatrix3&				GetRotate(){ return m_matLocal.m_Rotate; }
	GdsMatrix3&				GetWorldRotate(){ return m_matWorld.m_Rotate; }

	void					MakeXRotation( float fAngle ){ m_matLocal.m_Rotate.MakeXRotation( fAngle ); }
	void					MakeYRotation( float fAngle ){ m_matLocal.m_Rotate.MakeYRotation( fAngle ); }
	void					MakeZRotation( float fAngle ){ m_matLocal.m_Rotate.MakeZRotation( fAngle ); }

	GdsNode*				GetParent();
	void					SetParent( GdsNode* pNode );
	GdsNodePtr				GetAt( unsigned int index );

	// 구현 예정
	void					Deepcopy( GdsNodePtr pResource );
	
	HRESULT					AttachChild( GdsNodePtr pNode );
	HRESULT					DetachChild( GdsNodePtr pNode );
	HRESULT					RemoveAllChild();

	HRESULT					Update( float fElapsedtime );

	void					SetDrawAxis( bool bFlag ) { m_bDrawAxis = bFlag; }
	void					DrawAxis();

	//DeclareBoostPool(GdsNode)
// 	void*	operator new( size_t s );
// 	void	operator delete(void* p);

// private:
// 	static boost::pool<> bpool;

};

// 



typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif