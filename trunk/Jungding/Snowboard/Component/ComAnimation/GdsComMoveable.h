#ifndef _SNOWBOARD_COMPONENT_ANIMAION_MOVEABLE_H_
#define _SNOWBOARD_COMPONENT_ANIMAION_MOVEABLE_H_

class GdsComMoveable : public GdsComAnimationFamily
{
public:

	GdsComMoveable();
	virtual ~GdsComMoveable();

	virtual const goc_id_type componentID() const { return COMPONENT_ANIMATION_MOVEABLE; }
	virtual void			Update( float fAccumtime );	
	virtual void			Make();

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

private:


	D3DXMATRIX				m_matLocal;
	D3DXMATRIX				m_matWorld;

	D3DXQUATERNION			m_qRotate;
	D3DXQUATERNION			m_qWorldRotate;

	D3DXVECTOR3				m_vTranslate;
	D3DXVECTOR3				m_vWorldTranslate;

	D3DXVECTOR3				m_vScale;

};

#endif