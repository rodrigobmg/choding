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
	m_iCurCamIndex = -1;
	m_camaraContainer.clear();
}

void GdsCameraManagerDX9::Update( float fElapsedTime )
{
	if ( m_iCurCamIndex <= -1 )
		return;
	
	m_camaraContainer[m_iCurCamIndex]->Update( fElapsedTime );
}

void GdsCameraManagerDX9::SetCam( int index )
{
	size_t size = m_camaraContainer.size();

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
	vClear();
}