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
		m_pDevice->SetTransform( D3DTS_VIEW , &(pcam->GetViewMatrix()) );
		
		m_pDevice->SetTransform( D3DTS_PROJECTION, &(pcam->GetProjMatrix()) );
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

