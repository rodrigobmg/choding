#include "stdafx.h"
#include "CameraManager.h"


CameraManager::CameraManager()
{
	m_pCam = new Camera;
	m_pCam->Position = Vec3( 0, 0, -5);
	m_pCam->Target	 = Vec3( 0, 0, 0 );
	m_pCam->Up		 = Vec3( 0, 1, 0 );
	m_pCam->Width	 = 640;
	m_pCam->Height	 = 480;

	SetCurCam( m_pCam );
}

CameraManager::~CameraManager()
{

}

void CameraManager::SetCurCam( Camera* cam )
{
	m_pCam->Position = cam->Position;
	m_pCam->Up		 = cam->Up;
	m_pCam->Direction= cam->Direction;
	m_pCam->Target   = cam->Target;
	m_pCam->Height	 = cam->Height;
	m_pCam->Width	 = cam->Width;
	
	m_pCam->Direction = Vec3(m_pCam->Target - m_pCam->Position); 
	D3DXVec3Normalize(&m_pCam->Direction, &m_pCam->Direction);
	
	D3DXMatrixLookAtLH(&m_pCam->View, &m_pCam->Position, &m_pCam->Target, &m_pCam->Up);
	D3DXMatrixPerspectiveFovLH(&m_pCam->Projection, (D3DX_PI / 4.0f), (m_pCam->Width / m_pCam->Height), 0.10f, 1000.0f);
}

void CameraManager::onDeviceReset()
{
	if ( m_pCam == NULL )
		return;

	D3DXMatrixPerspectiveFovLH(&m_pCam->Projection, (D3DX_PI / 4.0f), (m_pCam->Width / m_pCam->Height), 1.0f, 1000.0f);
}

void CameraManager::Update( float fpsMod )
{
	if ( m_pCam == NULL )
		return;

	bool W = (GetAsyncKeyState(0x0057) & 0x8000) ? true : false;
	bool S = (GetAsyncKeyState(0x0053) & 0x8000) ? true : false;
	bool A = (GetAsyncKeyState(0x0041) & 0x8000) ? true : false;
	bool D = (GetAsyncKeyState(0x0044) & 0x8000) ? true : false;

	POINT Mouse;
	GetCursorPos(&Mouse);

	SetCursorPos(600, 600);

	Vec3 cAxis;
	D3DXVec3Cross(&cAxis, &m_pCam->Up, &m_pCam->Direction);

	float Speed = 1.0f;
	fpsMod = 0.01f;
	if(W) m_pCam->Position += m_pCam->Direction * (Speed * fpsMod);
	if(S) m_pCam->Position -= m_pCam->Direction * (Speed * fpsMod);
	if(A) m_pCam->Position -= cAxis * (Speed * fpsMod);
	if(D) m_pCam->Position +=  cAxis * (Speed * fpsMod);

	Vec4 TransformTemp;

	Quat RXq;
	D3DXQuaternionRotationAxis(&RXq, &m_pCam->Up, ( (D3DX_PI / 4.0f) / 150) * (Mouse.x - 600));

	Matrix44 RX;
	D3DXMatrixRotationQuaternion(&RX, &RXq);

	D3DXVec3Transform(&TransformTemp, &m_pCam->Direction, &RX);
	m_pCam->Direction.x = TransformTemp.x; m_pCam->Direction.y = TransformTemp.y; m_pCam->Direction.z = TransformTemp.z;

	Quat RYq;
	D3DXVec3Cross(&cAxis, &m_pCam->Up, &m_pCam->Direction);
	D3DXQuaternionRotationAxis(&RYq, &cAxis, ( (D3DX_PI / 4.0f) / 150) * (Mouse.y - 600));

	Matrix44 RY;
	D3DXMatrixRotationQuaternion(&RY, &RYq);

	D3DXVec3Transform(&TransformTemp, &m_pCam->Direction, &RY);
	m_pCam->Direction.x = TransformTemp.x; m_pCam->Direction.y = TransformTemp.y; m_pCam->Direction.z = TransformTemp.z;

	Vec3 Target = m_pCam->Position + m_pCam->Direction;
	D3DXMatrixLookAtLH(&m_pCam->View, &m_pCam->Position, &Target, &m_pCam->Up);
}


