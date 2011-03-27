#ifndef _SUN_OBJECT_NODE_NODE_H_
#define _SUN_OBJECT_NODE_NODE_H_

//이녀석들을 어디에 넣어야할지 모르겠다.....
typedef list<sunNodePtr>			NodeList;
typedef list<sunNodePtr>::iterator  NodeListIT;


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

	inline const D3DXMATRIX& GetLocalMatrix() const;
	inline const D3DXMATRIX& GetWorldMatrix() const;
	inline void SetLocalMatrix( const D3DXMATRIX& matLocal );
	inline void SetLocalFromWorldTransform( const D3DXMATRIX& matWorld );

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