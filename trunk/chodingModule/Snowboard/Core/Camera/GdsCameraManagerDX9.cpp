#include "GdsCameraManagerDX9.h"

GdsCameraManagerDX9::GdsCameraManagerDX9( void ):
m_pDevice(NULL)
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

HRESULT GdsCameraManagerDX9::Create( LPDIRECT3DDEVICE9 device )
{
	m_pDevice = device;
	return true;
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

void GdsCameraManagerDX9::Update( float fElapsedTime )
{
	GdsCameraPtr cam = m_camaraContainer[m_iCurCamIndex];
	cam->Update(fElapsedTime);
}

