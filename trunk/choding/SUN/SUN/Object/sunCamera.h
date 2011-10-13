#ifndef _SUN_OBJECT_CAMERA_H_
#define _SUN_OBJECT_CAMERA_H_

// �⺻ ī�޶� Ŭ����
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
	D3DXVECTOR3		m_vLookat;		// ī�޶��� �ü� ��ġ
	D3DXVECTOR3		m_vDir;			// ī�޶��� �ü� (��������)
	D3DXVECTOR3		m_vCross;		// ī�޶��� �ü��� �����ϴ� (���⼭�� �¿츦 ����)
	D3DXVECTOR3		m_vUp;			// ī�޶��� ��溤��

	float			m_fAccel;		// ī�޶� �̵� ����
};

#endif