#include "Precompiled.h"
#include "sunCamera.h"

ImplementRootRTTI( sunCamera );

sunCamera::sunCamera()
{
	m_vPos		= D3DXVECTOR3( 5.0f, 1.0f, -5.0f );
	m_vLookat	= D3DXVECTOR3( 0.0f, 0.0f, -0.0f );
	m_vUp		= D3DXVECTOR3( 0.0f, 1.0f,  0.0f );

	//m_vPos		= D3DXVECTOR3( 0.0f, 3.0f,-5.0f );
	//m_vLookat	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	//m_vUp		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );

	m_fAccel	= 1.0f;

	UpdateView();
}

/// \brief   
/// \return  
/// \author  박기헌 [2010/05/12/]
/// \warning 주의사항 없음
/// \par     요구사항 없음
sunCamera::~sunCamera()
{

}

/// \brief   
/// \param   void
/// \return  void
/// \author  박기헌 [2010/05/12/]
/// \warning 주의사항 없음
/// \par     요구사항 없음
void sunCamera::BegineUpdate( void )
{
	sunFrustum::UpdateFrustumPlane();

	//ev_CInputCore* pInputCore = ev_CInputCore::GetCore();
	//ev_CKeyboardLayer* pKeyboard = pInputCore->GetKeyBoard();
	//ev_CMouseLayer* pMouse = pInputCore->GetMouse();

	//// 왼쪽 쉬프트 키로 카메라 이동 가속
	//if( pKeyboard->GetKeyIsDown( DIK_LSHIFT ) )
	//	m_fAccel = 3.0f;
	//else
	//	m_fAccel = 1.0f;

	//if( pKeyboard->GetKeyIsDown( DIK_D ) )
	//	MoveCamera( 0.2f * m_fAccel, 0.0f, 0.0f );

	//if( pKeyboard->GetKeyIsDown( DIK_A ) )
	//	MoveCamera( -0.2f * m_fAccel, 0.0f, 0.0f );


	//if( pKeyboard->GetKeyIsDown( DIK_Q ) )
	//	MoveCamera( 0.0f, 0.2f * m_fAccel, 0.0f );

	//if( pKeyboard->GetKeyIsDown( DIK_E ) )
	//	MoveCamera( 0.0f, -0.2f * m_fAccel, 0.0f );

	//if( pKeyboard->GetKeyIsDown( DIK_W ) )
	//	MoveCamera( 0.0f, 0.0f, 0.2f * m_fAccel );

	//if( pKeyboard->GetKeyIsDown( DIK_S ) )
	//	MoveCamera( 0.0f, 0.0f, -0.2f * m_fAccel );

	//if( pMouse->GetMouseBtnIsDown( MOUSE_RIGHT_BUTTON ) )
	//{
	//	if( pMouse->GetMouseState().iMoveFactorX )
	//		RotateHorizontal( pMouse->GetMouseState().iMoveFactorX * 0.003f );

	//	if( pMouse->GetMouseState().iMoveFactorY )
	//		RotateVertical( pMouse->GetMouseState().iMoveFactorY * 0.003f );
	//}

	//if( pMouse->GetMouseWheelMovement() )
	//{
	//	Zoom( ( pMouse->GetMouseWheelMovement() ) * 5.0f * g_pKernel->GetElapsedTime() );
	//}


}


void sunCamera::UpdateView( void )
{
	D3DXVec3Normalize( &m_vDir, &( m_vLookat - m_vPos ) );
	D3DXVec3Cross( &m_vCross, &m_vUp, &m_vDir );

	D3DXMatrixLookAtLH( &m_matView, &m_vPos, &m_vLookat, &m_vUp);
}


void sunCamera::MoveCamera( float fDistX, float fDistY, float fDistZ )
{
	D3DXVECTOR3 vMoveX, vMoveY, vMoveZ;

	D3DXVec3Normalize( &vMoveX, &m_vCross );
	D3DXVec3Normalize( &vMoveY, &m_vUp );
	D3DXVec3Normalize( &vMoveZ, &m_vDir );

	vMoveX		*= fDistX;
	vMoveY		*= fDistY;
	vMoveZ		*= fDistZ;

	m_vPos		+= vMoveX + vMoveY + vMoveZ;
	m_vLookat	+= vMoveX + vMoveY + vMoveZ;

	UpdateView();
}

void sunCamera::MoveCamera( D3DXVECTOR3 vDist )
{
	D3DXVECTOR3 vMoveX, vMoveY, vMoveZ;

	D3DXVec3Normalize( &vMoveX, &m_vCross );
	D3DXVec3Normalize( &vMoveY, &m_vUp );
	D3DXVec3Normalize( &vMoveZ, &m_vDir );

	vMoveX		*= vDist.x;
	vMoveY		*= vDist.y;
	vMoveZ		*= vDist.z;

	m_vPos		+= vMoveX + vMoveY + vMoveZ;
	m_vLookat	+= vMoveX + vMoveY + vMoveZ;

	UpdateView();
}


void sunCamera::RotateCamera( float fAngleX, float fAngleY, float fAngleZ /*= 0.0f */ )
{
	RotateVertical( fAngleZ );
	RotateHorizontal( fAngleY );
}


void sunCamera::RotateVertical( float fYaw )
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis( &matRot, &m_vCross, fYaw );

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord( &m_vLookat, &m_vDir, &matRot );
	m_vLookat += m_vPos;

	UpdateView();
}


void sunCamera::RotateHorizontal( float fPitch )
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationAxis( &matRot, &m_vUp, fPitch );

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord( &m_vLookat, &m_vDir, &matRot );
	m_vLookat += m_vPos;

	UpdateView();
}

void sunCamera::Zoom( float fSpeed )
{
	D3DXVECTOR3 vcZ = m_vLookat - m_vPos;
	D3DXVec3Normalize( &vcZ, &vcZ );

	m_vLookat += vcZ * fSpeed;
	m_vPos += vcZ * fSpeed;

	UpdateView();
}

void sunCamera::SetHeight( float fHeight )
{
	m_vPos.y = fHeight;
}