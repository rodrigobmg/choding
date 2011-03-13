#include "GdsCameraManagerDX9.h"

GdsCameraManagerDX9::GdsCameraManagerDX9( void ):
m_pDevice( NULL )
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
	m_iCurCamIndex = -1;
	m_camaraContainer.clear();
}

HRESULT GdsCameraManagerDX9::Create( LPDIRECT3DDEVICE9 device )
{
	m_pDevice = device;
	return true;
}

void GdsCameraManagerDX9::Update( float fElapsedTime )
{
	if ( m_iCurCamIndex <= -1 )
		return;
	
	GdsCameraNodePtr pcam = m_camaraContainer[m_iCurCamIndex];
	if ( pcam )
		pcam->Update( fElapsedTime );

	if ( m_pDevice )
	{
		D3DXVECTOR3 vEyept( 0.f , 3.f , -5.f );
		D3DXVECTOR3 vLookatpt( 0.f , 0.f , 0.f );
		D3DXVECTOR3 vUppt( 0.f , 1.f , 0.f );
		D3DXMATRIXA16 matView;
		D3DXMatrixLookAtLH( &matView , &vEyept , &vLookatpt , &vUppt );
		m_pDevice->SetTransform( D3DTS_VIEW , &matView );

		//m_pDevice->SetTransform( D3DTS_VIEW , &(pcam->GetViewMatrix()) );	
		
		D3DXMATRIXA16 matProj;
		D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI/4, 1.0f, 1.0f, 100.0f );
		m_pDevice->SetTransform( D3DTS_PROJECTION, &matProj );
	}
}

void GdsCameraManagerDX9::SetCam( int index )
{
	int32_t size = (int32_t)m_camaraContainer.size();

	if ( index > size || index <= -1 )
	{
		index = -1;
		assert( 0 );
	}

	m_iCurCamIndex = index;	
}

void GdsCameraManagerDX9::Attach( GdsCameraNodePtr camnode )
{
	m_camaraContainer.push_back( camnode );
}

void GdsCameraManagerDX9::Detach( int index )
{
	if ( m_camaraContainer.size() == 0 )
	{
		assert( 0 && "카메라노드가 없다.");
		return;
	}

	CAM_CONTAINER::iterator it = m_camaraContainer.begin();
	m_camaraContainer.erase( it + index );
}

void GdsCameraManagerDX9::DetachAll()
{
	m_iCurCamIndex = -1;
	m_camaraContainer.clear();
}

