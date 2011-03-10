#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"

class GdsNode : public GdsObject , public boost::enable_shared_from_this< GdsNode >
{
		
protected:	

	typedef boost::shared_ptr< GdsNode >	GdsNodePtr;
	GdsNodePtr								m_pParentNode;

	typedef	 std::list< GdsNodePtr >		CHILDLIST;
	CHILDLIST								m_listChildNode;

	virtual	void							vInitGeometry();
	virtual void							vRender();
	virtual void							vClear();

	D3DXMATRIXA16							m_matWorld;
	D3DXMATRIXA16							m_matLocal;

	LPDIRECT3DDEVICE9						m_Device;

public:
	GdsNode();
	virtual ~GdsNode();

	boost::shared_ptr< GdsNode > shared_ptr_this()
	{
		return shared_from_this();
	}

	void							SetDevice( LPDIRECT3DDEVICE9 device ){ m_Device = device; }
	LPDIRECT3DDEVICE9				GetDevice(){ return m_Device; }

	D3DXMATRIXA16&					GetTransform();
	void							SetTransform( D3DXMATRIXA16& mat );

	D3DXMATRIXA16&					GetWorldTransform();
	void							SetWorldTransform( D3DXMATRIXA16& mat );

	HRESULT							Update( float fElapsedtime );
	GdsNodePtr						GetParent();
	void							SetParent( GdsNodePtr pNode );
	GdsNodePtr						GetAt( unsigned int index );
	
	virtual HRESULT					AttachChild( GdsNodePtr pNode );
	virtual HRESULT					DetachChild( GdsNodePtr pNode );
	virtual HRESULT					RemoveAllChild();

};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif