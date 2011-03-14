#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"
#include "../Transform/GdsTransform.h"
#include "../Resource/Type/GdsResBaseType.h"

class GdsNode : public GdsObject , public boost::enable_shared_from_this< GdsNode >
{
protected:		

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;
	GdsNodePtr								m_pParentNode;

	typedef	 std::list< GdsNodePtr >		CHILDLIST;
	CHILDLIST								m_listChildNode;

	virtual	void							vInitGeometry( float fElapsedtime );
	virtual void							vRender( float fElapsedtime );
	virtual void							vClear();

	
	GdsTransform							m_matWorld;
	GdsTransform							m_matLocal;

	LPDIRECT3DDEVICE9						m_Device;

	GdsResBasePtr							m_pResource;

public:
	GdsNode();
	virtual ~GdsNode();

	boost::shared_ptr< GdsNode > shared_ptr_this()
	{
		return shared_from_this();
	}

	void							SetDevice( LPDIRECT3DDEVICE9 device ){ m_Device = device; }
	LPDIRECT3DDEVICE9				GetDevice(){ return m_Device; }

	void							SetResource( GdsResBasePtr pResource ){ m_pResource = pResource; }
	GdsResBasePtr					GetResource() { return m_pResource; }

	GdsTransform&					GetTransform(){ return m_matLocal; }
	void							SetTransform( GdsTransform& mat ){ m_matLocal = mat; }

	GdsTransform&					GetWorldTransform(){ return m_matWorld; }
	void							SetWorldTransform( GdsTransform& mat ){ m_matWorld = mat; }

	HRESULT							Update( float fElapsedtime );
	void							InitGeometry( float fElapsedtime );
	void							Render( float fElapsedtime );


	GdsNodePtr						GetParent();
	void							SetParent( GdsNodePtr pNode );
	GdsNodePtr						GetAt( unsigned int index );

	void							Deepcopy( GdsNodePtr pResource );
	
	HRESULT					AttachChild( GdsNodePtr pNode );
	HRESULT					DetachChild( GdsNodePtr pNode );
	HRESULT					RemoveAllChild();

};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif