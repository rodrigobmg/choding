#include "GdsCameraManagerDX9.h"

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

HRESULT GdsCameraManagerDX9::Create()
{
	return true;
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
	m_camaraContainer.clear();
}

GdsCameraNodePtr GdsCameraManagerDX9::GetCamNode( int iCamIndex )
{
	size_t size = m_camaraContainer.size();

	if ( iCamIndex < 0 || iCamIndex > size )
		return GdsCameraNodePtr( (GdsCameraNode*)NULL );

	return m_camaraContainer[iCamIndex];
}

