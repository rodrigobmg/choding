#include "GdsCameraManagerDX9.h"
#include "InputSystem\GdsInputSystem.h"

GdsCameraManagerDX9::GdsCameraManagerDX9( void )
{
	SetName( OBJECT_CAMMGR_DX9 );
	vClear();
}

GdsCameraManagerDX9::~GdsCameraManagerDX9( void )
{
	vClear();
}

void GdsCameraManagerDX9::vClear()
{
	m_camaraContainer.clear();
}

void GdsCameraManagerDX9::Attach( GdsCameraPtr camnode )
{
	m_camaraContainer.push_back( camnode );
}

void GdsCameraManagerDX9::Detach( int index )
{
	if ( m_camaraContainer.size() == 0 )
	{
		ASSERT( 0 && "카메라노드가 없다.");
		return;
	}

	CAM_CONTAINER::iterator it = m_camaraContainer.begin();
	m_camaraContainer.erase( it + index );
}

void GdsCameraManagerDX9::DetachAll()
{
	m_camaraContainer.clear();
}

GdsCameraPtr GdsCameraManagerDX9::GetCamNode( int iCamIndex )
{
	int size = static_cast<int>( m_camaraContainer.size() );

	if ( iCamIndex < 0 || iCamIndex > size )
		return GdsCameraPtr( (GdsCamera*)NULL );

	return m_camaraContainer[iCamIndex];
}

void GdsCameraManagerDX9::Update( float fElapsedtime )
{
	if ( GetCurCam() )
		GetCurCam()->Update(fElapsedtime);
}

void GdsCameraManagerDX9::MoveForwardLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matLook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 zAxis( matLook._13 , matLook._23 , matLook._33 );
		D3DXVec3Normalize( &zAxis , &zAxis );
		zAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat += zAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveBackLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matLook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 zAxis( matLook._13 , matLook._23 , matLook._33 );
		D3DXVec3Normalize( &zAxis , &zAxis );
		zAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat -= zAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveLeftLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 xAxis( matlook._11 , matlook._21 , matlook._31 );
		D3DXVec3Normalize( &xAxis , &xAxis );
		xAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat -= xAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveRightLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 xAxis( matlook._11 , matlook._21 , matlook._31 );
		D3DXVec3Normalize( &xAxis , &xAxis );
		xAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat += xAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveUpLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 yAxis( matlook._12 , matlook._22 , matlook._32 );
		D3DXVec3Normalize( &yAxis , &yAxis );
		yAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat += yAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveDownLookat( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 yAxis( matlook._12 , matlook._22 , matlook._32 );
		D3DXVec3Normalize( &yAxis , &yAxis );
		yAxis *= delta;
		D3DXVECTOR3 posLookat = GetCurCam()->GetLookAt();
		posLookat -= yAxis;
		GetCurCam()->SetLookAt( posLookat );
	}
}

void GdsCameraManagerDX9::MoveForwardEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matLook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 zAxis( matLook._13 , matLook._23 , matLook._33 );
		D3DXVec3Normalize( &zAxis , &zAxis );
		zAxis *= delta;
		D3DXVECTOR3 poseye = GetCurCam()->GetEye();
		poseye += zAxis;
		GetCurCam()->SetEye( poseye );
	}
}

void GdsCameraManagerDX9::MoveBackEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matLook = GetCurCam()->GetViewMat();
		
		D3DXVECTOR3 zAxis( matLook._13 , matLook._23 , matLook._33 );
		D3DXVec3Normalize( &zAxis , &zAxis );
		zAxis *= delta;
		D3DXVECTOR3 poseye = GetCurCam()->GetEye();
		poseye -= zAxis;
		GetCurCam()->SetEye( poseye );
	}
}

void GdsCameraManagerDX9::MoveLeftEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 xAxis( matlook._11 , matlook._21 , matlook._31 );
		D3DXVec3Normalize( &xAxis , &xAxis );
		xAxis *= delta;
		D3DXVECTOR3 posEye = GetCurCam()->GetEye();
		posEye -= xAxis;
		GetCurCam()->SetEye( posEye );
	}
}

void GdsCameraManagerDX9::MoveRightEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 xAxis( matlook._11 , matlook._21 , matlook._31 );
		D3DXVec3Normalize( &xAxis , &xAxis );
		xAxis *= delta;
		D3DXVECTOR3 posEye = GetCurCam()->GetEye();
		posEye += xAxis;
		GetCurCam()->SetEye( posEye );
	}
}

void GdsCameraManagerDX9::MoveUpEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 yAxis( matlook._12 , matlook._22 , matlook._32 );
		D3DXVec3Normalize( &yAxis , &yAxis );
		yAxis *= delta;
		D3DXVECTOR3 posEye = GetCurCam()->GetEye();
		posEye += yAxis;
		GetCurCam()->SetEye( posEye );
	}
}

void GdsCameraManagerDX9::MoveDownEye( float delta )
{
	if ( GetCurCam() )
	{
		D3DXMATRIXA16 matlook = GetCurCam()->GetViewMat();

		D3DXVECTOR3 yAxis( matlook._12 , matlook._22 , matlook._32 );
		D3DXVec3Normalize( &yAxis , &yAxis );
		yAxis *= delta;
		D3DXVECTOR3 posEye = GetCurCam()->GetEye();
		posEye -= yAxis;
		GetCurCam()->SetEye( posEye );
	}
}