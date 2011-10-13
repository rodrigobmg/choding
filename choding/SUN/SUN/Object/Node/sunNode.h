#ifndef _SUN_OBJECT_NODE_NODE_H_
#define _SUN_OBJECT_NODE_NODE_H_

//이녀석들을 어디에 넣어야할지 모르겠다.....
typedef list<sunNodePtr>			NodeList;
typedef list<sunNodePtr>::iterator  NodeListIT;



typedef struct _stVector3i
{
	int x;
	int y;
	int z;

	_stVector3i():
	x(0),
	y(0),
	z(0)
	{

	}
} stVector3i;


typedef struct _stVector3f
{
	float x;
	float y;
	float z;
	
	_stVector3f():
	x(0.0f),
	y(0.0f),
	z(0.0f)
	{

	}

} stVector3f;

typedef struct _stVector2f
{
	float x;
	float y;

	_stVector2f():
	x(0.0f),
	y(0.0f)
	{

	}

} stVector2f;

typedef struct _stVector4f
{
	float x;
	float y;
	float z;
	float w;

	_stVector4f():
	x(0.0f),
	y(0.0f),
	z(0.0f),
	w(0.0f)
	{

	}

} stVector4f;


typedef struct _stMat16
{
	stVector4f kRow[4];
} stMat16;




class DLL_API sunNode : public sunObject
{

	DeclareRTTI


public:
	sunNode();
	virtual ~sunNode();
	void Destroy();

public:
	inline void SetParent( sunNode* pParent);
	inline void AddChild( sunNodePtr spChild);
	inline void AddChildToFront( sunNodePtr spChild );
	inline sunNodePtr GetNodeByName( const tstring& strName, bool bSearchAllChildren = true);
	inline const list< sunNodePtr >& GetChildList();

	inline const D3DXVECTOR3& GetTranslate();
	inline void  SetTranslate( const D3DXVECTOR3& vPos );
	inline void  SetTranslate( float fX, float fY, float fZ);

	inline const D3DXQUATERNION& GetRotate();
	inline void  SetRotate( const D3DXVECTOR3& vAxis, float fAngle );
	inline void  SetRotate( const D3DXQUATERNION& qRot );

	inline const D3DXVECTOR3& GetScale() const;
	inline void  SetScale( float fScale );
	inline void	 SetScale( float fScaleX, float fScaleY, float fScaleZ);

	inline const D3DXVECTOR3& GetWorldTranslate() const;
	inline const D3DXQUATERNION& GetWorldRotate() const;

	const D3DXMATRIX& GetLocalMatrix() const;
	const D3DXMATRIX& GetWorldMatrix() const;

	void SetLocalMatrix( const D3DXMATRIX& matLocal );
	inline void SetLocalFromWorldTransform( const D3DXMATRIX& matWorld );


	void		SetLocalMatrix( const stMat16& Mat4X4 );
	void		SetWorldMatrix( const stMat16& Mat4X4 );

public:
	virtual void BegineUpdate();
	virtual void Update();
	virtual void EndUpdate();
	virtual void UpdateWorldData();


public:
	void RemoveChild( sunNodePtr spNode );
	void RemoveAllChild();
	void UpdateListner();

protected:
	sunNode*			m_pParent;
	list< sunNodePtr>	m_ChildList;
	typedef list<sunNodePtr>::iterator ChildIT;

	D3DXMATRIX			m_matWorld;
	D3DXMATRIX			m_matLocal;
	D3DXMATRIX			m_matAni;

	D3DXVECTOR3			m_vPos;
	D3DXQUATERNION		m_qRot;

	D3DXVECTOR3			m_vWorldPos;
	D3DXQUATERNION		m_qWorldRot;

	D3DXVECTOR3			m_vScale;


};

inline void sunNode::SetParent( sunNode* pParent)
{
	//assert(pParent);

	m_pParent = pParent;
}


inline void sunNode::AddChild( sunNodePtr spChild )
{
	assert(spChild);
	spChild->SetParent(this);
	m_ChildList.push_back( spChild );
}

inline void sunNode::AddChildToFront( sunNodePtr spChild )
{
	assert( spChild );
	spChild->SetParent( this );
	m_ChildList.push_back( spChild );

}

inline const list<sunNodePtr>& sunNode::GetChildList()
{
	return m_ChildList;
}


#endif