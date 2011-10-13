#ifndef _SUN_OBJECT_CAMERA_H_
#define _SUN_OBJECT_CAMERA_H_

// 기본 카메라 클래스
class DLL_API sunCamera : public sunFrustum
{
	DeclareRTTI
public:
	sunCamera();
	virtual ~sunCamera();

public:
	virtual void BegineUpdate( void );

public:
	void UpdateView( void );
	void MoveCamera( float fDistX, float fDistY, float fDistZ );
	void MoveCamera( D3DXVECTOR3 vDist );
	void RotateCamera( float fAngleX, float fAngleY, float fAngleZ = 0.0f );
	void RotateVertical( float fYaw );
	void RotateHorizontal( float fPitch );

	void Zoom( float fSpeed );

	inline void SetHeight( float fHeight );

	inline D3DXVECTOR3	GetCamLook()		{ return m_vLookat;	}
	inline D3DXVECTOR3	GetCamDir()			{ return m_vDir;	}


protected:
	D3DXVECTOR3		m_vLookat;		// 카메라의 시선 위치
	D3DXVECTOR3		m_vDir;			// 카메라의 시선 (단위벡터)
	D3DXVECTOR3		m_vCross;		// 카메라의 시선과 직교하는 (여기서는 좌우를 구할)
	D3DXVECTOR3		m_vUp;			// 카메라의 상방벡터

	float			m_fAccel;		// 카메라 이동 가속
};

#endif