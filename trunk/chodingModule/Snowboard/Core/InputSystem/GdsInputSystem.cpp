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
	if ( msg == WM_KEYUP )
	{
		m_EventDataUp.msg = msg;
		m_EventDataUp.wParam = wParam;
		m_EventDataUp.lParam = lParam;
	}

	if ( msg == WM_KEYDOWN )
	{
		m_EventDataDown.msg = msg;
		m_EventDataDown.wParam = wParam;
		m_EventDataDown.lParam = lParam;
	}
	
}

bool GdsInputSystem::GetKeyIsUp( int ikey )
{
	bool ret = false;
	if ( m_EventDataUp.msg == WM_KEYUP )
	{
		if ( m_EventDataUp.wParam == ikey )
			ret = true;
	}

	clear( m_EventDataUp );
	return ret;
}

bool GdsInputSystem::GetKeyIsDown( int ikey )
{
	bool ret = false;
	if ( m_EventDataDown.msg == WM_KEYDOWN )
	{
		if ( m_EventDataDown.wParam == ikey )
			ret = true;
	}

	clear( m_EventDataDown );
	return ret;
}

void GdsInputSystem::clear( EVENT& token )
{
	token.msg = 0; token.wParam = 0; token.lParam = 0;
}