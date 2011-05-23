#ifndef _SNOWBOARD_SCENENODE_BASE_
#define _SNOWBOARD_SCENENODE_BASE_

#include "../Base\GdsObject.h"
#include "../Renderer/GdsRenderObject.h"

class GdsNode : public GdsObject
{
	// oc-tree
	struct Node
	{

		Node*		m_pChild[8];
		int			m_iNumOfChild;
		GDSINDEX*	m_pFace;
		int			m_iCountOfFace;
		D3DXVECTOR3	m_minPos;
		D3DXVECTOR3	m_maxPos;
		D3DXVECTOR3	m_cenPos;
		float		m_fRadius;

		Node(int iNumFaces, D3DXVECTOR3& vMin, D3DXVECTOR3& vMax)
			: m_iCountOfFace( iNumFaces ) , m_minPos(vMin) , m_maxPos(vMax) , m_cenPos((vMin+vMax)*0.5 )
			, m_iNumOfChild( 0 )
			, m_fRadius( 0.f )
		{
			for (int i=0 ; i<8 ; i++)
				m_pChild[i] = NULL;

			m_pFace = new GDSINDEX[m_iCountOfFace];
		}

		~Node()
		{			
			Clear();
		}

		void Clear()
		{
			for (int i=0 ; i<8 ; i++)
				SAFE_DELETE( m_pChild[i] );

			SAFE_DELETE( m_pFace );
		}

	};

	void			build( Node* node );
	int				genTriIndex( Node* node , LPVOID pIB , int iCurIndexCount );
public:	

	void			CreateOctree();
	void			ReleaseOctree(){ m_bUseOctree = false; SAFE_DELETE( m_pOctreeRootNode ); }
	void			GenOctreeFaceIndex();

	void			SetLimitedFacePerNode( int iCount ){ m_iLimitedCountOfFacePerNode = iCount; }
	int				GetTotalOctreenode(){ return m_iCountOfOctreeNode; }

private:

	int				m_iCountOfOctreeNode;
	int				m_iLimitedCountOfFacePerNode;      // 노드당 최소 평면 수
	GDSVERTEX*		m_pVert;							// 정점 배열
	Node*			m_pOctreeRootNode;	

private:
	
	bool									m_bShowBox;
	bool									m_bShowAxis;
	bool									m_bUseOctree;

	D3DXMATRIX								m_matLocal;
	D3DXMATRIX								m_matWorld;

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

	typedef std::pair< GdsRenderObjectPtr , int > RENDERTOKEN;
	typedef std::vector< RENDERTOKEN >	RENDER_OBJECT_CONTAINER;
	RENDER_OBJECT_CONTAINER					m_list_RenderObject;
		
	bool									m_bBillboard;
	bool									m_bCull;

protected:			

	virtual	void							vUpdate( float fElapsedtime );
	virtual void							vClear();

public:

	GdsNode();
	virtual ~GdsNode();
 	
	void					AddRenderObject( GdsRenderObjectPtr pRenderObject , int iRenderStateIndex );
	GdsRenderObjectPtr		GetRenderObject( int index ){ return m_list_RenderObject.at(index).first; }
//		//강제로 카운터 올리고 자기 참조
//  	boost::shared_ptr< GdsNode > shared_ptr_this()
//  	{
//  		return shared_from_this();
//  	}

	
	void					SetDrawAxis( bool bShow ){ m_bShowAxis = bShow; }
	void					SetDrawBox( bool bShow ){ m_bShowBox = bShow; }
	
	void					SetBillboard( bool bflag ){ m_bBillboard = bflag; }

	void					SetParentName( tstring& strParentName ){ m_strParentName = strParentName; }
	tstring&				GetParentName(){ return m_strParentName; }

	GdsNodePtr				GetObjectbyName( tstring& strname );
 	
	void					SetCull( bool bCull ){ m_bCull = bCull; }
	bool					GetCull(){ return m_bCull; }


	D3DXVECTOR3&			GetTranslate(){ return m_vTranslate; }
	void					SetTranslate( const D3DXVECTOR3& vPos ){ m_vTranslate = vPos; }
	void					SetTranslate( float fX, float fY, float fZ){ m_vTranslate = D3DXVECTOR3( fX , fY , fZ ); }

	const	D3DXQUATERNION& GetRotate(){ return m_qRotate; }
	void					SetRotate( const D3DXVECTOR3& vAxis, float fAngle ){ D3DXQuaternionRotationAxis( &m_qRotate, &vAxis, fAngle); }
	void					SetRotate( const D3DXQUATERNION& qRot ){ m_qRotate = qRot; }

	const	D3DXVECTOR3&	GetScale() const { return m_vScale; }
	void					SetScale( float fScale )
							{
								ASSERT( fScale >= 0.0f );
								float f = fabs(fScale);
								m_vScale = D3DXVECTOR3( f, f, f);
							}
	void					SetScale( float fScaleX, float fScaleY, float fScaleZ)
							{
								ASSERT( fScaleX >= 0.0f); ASSERT( fScaleY >= 0.0f); ASSERT( fScaleZ >= 0.0f);
								float fX = fabs(fScaleX); float fY = fabs(fScaleY); float fZ = fabs(fScaleZ);
								m_vScale = D3DXVECTOR3( fX, fY, fZ);
							}

	const	D3DXVECTOR3&	GetWorldTranslate() const{ return m_vWorldTranslate; }
	const	D3DXQUATERNION& GetWorldRotate() const{	return m_qWorldRotate; }
	const	D3DXMATRIX&		GetLocalMatrix() const{	return m_matLocal; }
	const	D3DXMATRIX&		GetWorldMatrix() const{	return m_matWorld; }

	void					SetLocalMatrix( const D3DXMATRIX& matLocal ){m_matLocal = matLocal;}
	void					SetLocalFromWorldTransform( const D3DXMATRIX& matWorld );

 	GdsNode*				GetParent();
 	void					SetParent( GdsNode* pNode );

	GdsNodePtr				GetAt( unsigned int index );
	
	HRESULT					AttachChild( GdsNodePtr pNode );
	HRESULT					DetachChild( GdsNodePtr pNode );
	HRESULT					RemoveAllChild();
	HRESULT					Update( float fElapsedtime );
};

typedef boost::shared_ptr< GdsNode >	GdsNodePtr;

#endif