#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"
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

	D3DXMATRIXA16							m_matWorld;
	D3DXMATRIXA16							m_matLocal;

	float									m_ScaleX;
	float									m_ScaleY;
	float									m_ScaleZ;

	tstring									m_strParentName;

	GdsNode*								m_pParentNode;

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;
	typedef	 std::list< GdsNodePtr >		CHILDNODE_CONTAINER;
	CHILDNODE_CONTAINER						m_ChildNode;	

	GdsPropertyPtr							m_Property;
		
	bool									m_bBillboard;
	CULL_TYPE								m_eCull;

	

protected:			

	virtual	void							vUpdate( float fElapsedtime );
	virtual void							vClear();

public:

	GdsNode();
	virtual ~GdsNode();
 	
 	boost::shared_ptr< GdsNode > shared_ptr_this()
 	{
 		return shared_from_this();
 	}

	void					SetBillboard( bool bflag ){ m_bBillboard = bflag; }
	void					SetDrawAxis( bool bFlag ) { m_Property->GetMesh()->SetDrawAxis( bFlag ); }

	void					SetParentName( tstring& strParentName ){ m_strParentName = strParentName; }
	tstring&				GetParentName(){ return m_strParentName; }

	GdsNodePtr				GetObjectbyName( tstring& strname );
 	
	void					SetScale( D3DXVECTOR3& scale );
	float					GetScale();

	void					SetCullType( CULL_TYPE eCull ){ m_eCull = eCull; }
	CULL_TYPE				GetCullType(){ return m_eCull; }

	void					SetProperty( GdsPropertyPtr pProperty ){ m_Property = pProperty; }
	GdsPropertyPtr			GetProperty(){ return m_Property; }

	D3DXMATRIXA16&			GetTransform(){ return m_matLocal; }
	void					SetTransform( D3DXMATRIXA16& mat ){ m_matLocal = mat; }

	D3DXMATRIXA16&			GetWorldTransform(){ return m_matWorld; }
	void					SetWorldTransform( D3DXMATRIXA16& mat ){ m_matWorld = mat; }

	void					SetTranslate( D3DXVECTOR3& pos );
	D3DXVECTOR3&			GetTranslate();

	D3DXVECTOR3&			GetWorldTanslate();

	void					SetRotate( D3DXMATRIXA16& matRot );
	D3DXMATRIXA16&			GetRotate();

 	GdsNode*				GetParent();
 	void					SetParent( GdsNode* pNode );

	GdsNodePtr				GetAt( unsigned int index );

	// 구현 예정
	void					Deepcopy( GdsNodePtr pResource );
	
	HRESULT					AttachChild( GdsNodePtr pNode );
	HRESULT					DetachChild( GdsNodePtr pNode );
	HRESULT					RemoveAllChild();

	HRESULT					Update( float fElapsedtime );


};

// 



typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif