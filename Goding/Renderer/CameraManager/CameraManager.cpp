#include "stdafx.h"
#include "CameraManager.h"


CameraManager::CameraManager() :
m_pCam(NULL)
{	
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
	D3DXMatrixPerspectiveFovLH(&m_pCam->Projection, (D3DX_PI / 4.0f), (m_pCam->Width / m_pCam->Height), 1.0f, 1000.0f);
}

void CameraManager::Update( float fpsMod )
{
	//Keyboard bools
// 	bool W = (GetAsyncKeyState(0x0057) & 0x8000) ? true : false;
// 	bool S = (GetAsyncKeyState(0x0053) & 0x8000) ? true : false;
// 	bool A = (GetAsyncKeyState(0x0041) & 0x8000) ? true : false;
// 	bool D = (GetAsyncKeyState(0x0044) & 0x8000) ? true : false;
// 	
// 	//Mouse position
// 	POINT Mouse;
// 	GetCursorPos(&Mouse);
// 	
// 	//Set cursor back to middle
// 	SetCursorPos(600, 600);
	
	Vec3 cAxis;
	D3DXVec3Cross(&cAxis, &m_pCam->Up, &m_pCam->Direction);
	
// 	if(W) Position += Direction * (Speed * fpsMod);
// 	if(S) Position -= Direction * (Speed * fpsMod);
// 	if(A) Position -= cAxis * (Speed * fpsMod);
// 	if(D) Position +=  cAxis * (Speed * fpsMod);
	
	Vec4 TransformTemp; // Vec3Transform returns Vector4, use this to convert
	
	//Yaw Quaternion
	Quat RXq;
	D3DXQuaternionRotationAxis(&RXq, &m_pCam->Up, ( (D3DX_PI / 4.0f) / 150) /** (Mouse.x - 600)*/);
	
	//Converted to Matrix
	Matrix44 RX;
	D3DXMatrixRotationQuaternion(&RX, &RXq);
	
	//Transform Direction
	D3DXVec3Transform(&TransformTemp, &m_pCam->Direction, &RX);
	m_pCam->Direction.x = TransformTemp.x; 
	m_pCam->Direction.y = TransformTemp.y; 
	m_pCam->Direction.z = TransformTemp.z;
	
	//Pitch Quaternion
	Quat RYq;
	D3DXVec3Cross(&cAxis, &m_pCam->Up, &m_pCam->Direction);
	D3DXQuaternionRotationAxis(&RYq, &cAxis, ( (D3DX_PI / 4.0f) / 150)/* * (Mouse.y - 600)*/);
	
	//Convert to Matrix
	Matrix44 RY;
	D3DXMatrixRotationQuaternion(&RY, &RYq);
	
	//Transform Direction
	D3DXVec3Transform(&TransformTemp, &m_pCam->Direction, &RY);
	m_pCam->Direction.x = TransformTemp.x; 
	m_pCam->Direction.y = TransformTemp.y; 
	m_pCam->Direction.z = TransformTemp.z;
	
	//Calculate Target
	Vec3 Target = m_pCam->Position + m_pCam->Direction;
	
	//Calculate View Matrix
	D3DXMatrixLookAtLH(&m_pCam->View, &m_pCam->Position, &m_pCam->Target, &m_pCam->Up);
}


