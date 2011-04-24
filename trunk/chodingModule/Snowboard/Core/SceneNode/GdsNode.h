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
	
	D3DXMATRIXA16							m_matLocal;
	D3DXMATRIXA16							m_matWorld;

	D3DXQUATERNION							m_qRotate;
	D3DXQUATERNION							m_qWorldRotate;

	D3DXVECTOR3								m_vTranslate;
	D3DXVECTOR3								m_vWorldTranslate;

	D3DXVECTOR3								m_vScale;

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
 	
	void					SetCullType( CULL_TYPE eCull ){ m_eCull = eCull; }
	CULL_TYPE				GetCullType(){ return m_eCull; }

	void					SetProperty( GdsPropertyPtr pProperty ){ m_Property = pProperty; }
	GdsPropertyPtr			GetProperty(){ return m_Property; }



	D3DXVECTOR3&	GetTranslate();
	void					SetTranslate( const D3DXVECTOR3& vPos );
	void					SetTranslate( float fX, float fY, float fZ);

	const	D3DXQUATERNION& GetRotate();
	void					SetRotate( const D3DXVECTOR3& vAxis, float fAngle );
	void					SetRotate( const D3DXQUATERNION& qRot );

	const	D3DXVECTOR3&	GetScale() const;
	void					SetScale( float fScale );
	void					SetScale( float fScaleX, float fScaleY, float fScaleZ);

	const	D3DXVECTOR3&	GetWorldTranslate() const;
	const	D3DXQUATERNION& GetWorldRotate() const;

	const	D3DXMATRIXA16&	GetLocalMatrix() const;
	const	D3DXMATRIXA16	GetWorldMatrix() const;

	void					SetLocalMatrix( const D3DXMATRIXA16& matLocal );
	void					SetLocalFromWorldTransform( const D3DXMATRIXA16& matWorld );

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