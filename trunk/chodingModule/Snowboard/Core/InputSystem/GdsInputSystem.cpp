#include "GdsInputSystem.h"

GdsInputSystem::GdsInputSystem()
{
}

GdsInputSystem::~GdsInputSystem()
{

}

void GdsInputSystem::SetDefaultEvent()
{
	//MappingEvent();
}

void GdsInputSystem::MappingEvent( int ikey , int ievent )
{
	m_mapEvent.add( ikey , ievent );
	m_mapEvent.sort();
}

void GdsInputSystem::Record( UINT msg , WPARAM wParam , LPARAM lParam )
{
	if ( msg == WM_KEYDOWN )
	{
		m_EventData.msg = msg;
		m_EventData.wParam = wParam;
		m_EventData.lParam = lParam;
	}

	if ( msg == WM_KEYUP )
	{
		m_EventData.msg = 0;
		m_EventData.wParam = 0;
		m_EventData.lParam = 0;
	}
	
}

bool GdsInputSystem::GetKeyIsUp( int ikey )
{
	return false;
}

bool GdsInputSystem::GetKeyIsDown( int ikey )
{
	if ( m_EventData.msg == WM_KEYDOWN )
	{
		if ( m_EventData.wParam == ikey )
			return true;
	}


	return false;
}