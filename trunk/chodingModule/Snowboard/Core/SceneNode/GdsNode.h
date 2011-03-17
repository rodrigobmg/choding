#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"
#include "../Transform/GdsTransform.h"
#include "../Property/GdsPropertyState.h"

class GdsNode : public GdsObject , public boost::enable_shared_from_this< GdsNode >
{
protected:		

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;
	GdsNodePtr								m_pParentNode;

	typedef	 std::list< GdsNodePtr >		CHILDNODE_CONTAINER;
	CHILDNODE_CONTAINER						m_ChildNode;

	virtual	void							vInitGeometry( float fElapsedtime );
	virtual void							vRender( float fElapsedtime );
	virtual void							vClear();

	
	GdsTransform							m_matWorld;
	GdsTransform							m_matLocal;

	LPDIRECT3DDEVICE9						m_Device;

	GdsPropertyStatePtr						m_PropertyState;

public:
	GdsNode();
	virtual ~GdsNode();

	boost::shared_ptr< GdsNode > shared_ptr_this()
	{
		return shared_from_this();
	}

	void					SetDevice( LPDIRECT3DDEVICE9 device ){ m_Device = device; }
	LPDIRECT3DDEVICE9		GetDevice(){ return m_Device; }

	void					SetPropertyState( GdsPropertyStatePtr pProperty ){ m_PropertyState = pProperty; }
	GdsPropertyStatePtr		GetPropertyState(){ return m_PropertyState; }

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

	GdsNodePtr				GetParent();
	void					SetParent( GdsNodePtr pNode );
	GdsNodePtr				GetAt( unsigned int index );

	// 구현 예정
	void					Deepcopy( GdsNodePtr pResource );
	
	HRESULT					AttachChild( GdsNodePtr pNode );
	HRESULT					DetachChild( GdsNodePtr pNode );
	HRESULT					RemoveAllChild();

	HRESULT					Update( float fElapsedtime );
	void					InitGeometry( float fElapsedtime );
	void					Render( float fElapsedtime );

};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif