#ifndef _SUN_OBJECT_NODE_FRUSTUM_H_
#define _SUN_OBJECT_NODE_FRUSTUM_H_


//프러스텀 클래스


class DLL_API sunFrustum : public sunNode
{
	DeclareRTTI
public:
	enum FrustumPlane
	{
		FRUSTUM_PLANE_NEAR = 0,
		FRUSTUM_PLANE_FAR = 1,
		FRUSTUM_PLANE_LEFT = 2,
		FRUSTUM_PLANE_RIGHT = 3,
		FRUSTUM_PLANE_TOP = 4,
		FRUSTUM_PLANE_BOTTOM = 5,

	};

public:
	sunFrustum();
	~sunFrustum();


public:
	const D3DXMATRIX& GetProjectionMatrix();
	const D3DXMATRIX& GetViewMatrix();
	inline float GetFarClip();


public:
	void UpdateFrustumPlane();
	bool VertexIsInFrustum( D3DXVECTOR3* pv );
	bool VertexIsInFrustum( int iCount, D3DXVECTOR3* pv );	
	bool SphereIsInFrustum( D3DXVECTOR3* pv, float radius );

protected:
	float m_fFOV;
	float m_fAspect;
	float m_fNearDist;
	float m_fFarDist;

	D3DXMATRIX m_matProj;
	D3DXMATRIX m_matView;
	D3DXPLANE  m_plFrustum[6];


};





#endif