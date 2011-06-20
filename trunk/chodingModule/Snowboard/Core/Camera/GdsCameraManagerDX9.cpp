#include "GdsCameraManagerDX9.h"
#include "InputSystem\GdsInputSystem.h"

GdsCameraManagerDX9::GdsCameraManagerDX9( void )
:m_bMouseDrag(false)
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

void GdsCameraManagerDX9::Init()
{
	GdsCameraPtr	camnode = GdsCameraPtr( new GdsCamera );

	D3DXVECTOR3 vEyePt( 1.0f, 100.0f,-100.0f );
	D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );

	camnode->SetLootAtLH( vEyePt , vLookatPt , vUpVec );

	GdsFrustum frustum( -0.5 , 0.5 , -0.5 , 0.5 , 1.f , 500.f , false );
	camnode->SetFrustum( frustum );

	CAMMGR.Attach( camnode );
	CAMMGR.SetCurCam( 0 );	
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
	if ( INPUTSYSTEM.GetKeyIsDown( VK_W ) )
	{
		CAMMGR.MoveForwardEye( 2.0f );
		CAMMGR.MoveForwardLookat( 2.0f );
	}

	if ( INPUTSYSTEM.GetKeyIsDown( VK_A ) )
		CAMMGR.MoveLeftEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_D ) )
	{
		CAMMGR.MoveRightEye( 2.0f );
		CAMMGR.MoveBackLookat( 2.0f );
	}

	if ( INPUTSYSTEM.GetKeyIsDown( VK_S ) )
		CAMMGR.MoveBackEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_Q ) )
		CAMMGR.MoveUpEye( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_E ) )
		CAMMGR.MoveDownEye( 2.0f );


 	if ( m_bMouseDrag )
 	{
 		int x ,y ,z;
 		INPUTSYSTEM.GetMousePosDelta( x, y, z );
 		//LOG_INFO_F( "Mouse DeltaPos %d %d %d\n" , x ,y,z );
 		//CAMMGR.MoveLeftLookat( 2.0f );
 		if ( x > 0 )
 			CAMMGR.MoveRightLookat( 0.1 );
 		else
 			CAMMGR.MoveLeftLookat( 0.1 );
 
 		if ( y > 0 )
 			CAMMGR.MoveDownLookat( 0.1 );
 		else
 			CAMMGR.MoveUpLookat( 0.1 );
 
 		if ( z > 0 )
		{
			CAMMGR.MoveForwardEye( 0.1 );
			CAMMGR.MoveForwardLookat( 0.1 );
		}
 		else
		{
 			CAMMGR.MoveRightEye( 0.1 );
			CAMMGR.MoveBackLookat( 0.1 );
		}
 	}
 
 	if ( INPUTSYSTEM.GetMouseIsDown( VM_LBTN ) )
 	{
 		m_bMouseDrag = true;
 	}	
 
 	if ( INPUTSYSTEM.GetMouseIsUp( VM_LBTN ) )
 	{
 		m_bMouseDrag = false;
 	}

	if ( INPUTSYSTEM.GetKeyIsDown( VK_I ) )
	{
		CAMMGR.MoveForwardEye( 2.0f );
		CAMMGR.MoveForwardLookat( 2.0f );
	}

	if ( INPUTSYSTEM.GetKeyIsDown( VK_J ) )
		CAMMGR.MoveLeftLookat( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_L ) )
		CAMMGR.MoveRightLookat( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_K ) )
	{
		CAMMGR.MoveRightEye( 2.0f );
		CAMMGR.MoveBackLookat( 2.0f );
	}

	if ( INPUTSYSTEM.GetKeyIsDown( VK_U ) )
		CAMMGR.MoveUpLookat( 2.0f );

	if ( INPUTSYSTEM.GetKeyIsDown( VK_O ) )
		CAMMGR.MoveDownLookat( 2.0f );


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