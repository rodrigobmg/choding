#include "GdsInputSystem.h"

GdsInputSystem::GdsInputSystem()
:m_iPreMousePosX(0)
,m_iPreMousePosY(0)
,m_iPreMousePosZ(0)
,m_iMouseDeltaX(0)
,m_iMouseDeltaY(0)
,m_iMouseDeltaZ(0)
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

void GdsInputSystem::Record( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam )
{
	if ( msg == WM_KEYUP )
	{
		m_EventDataUp.hwnd = hwnd;
		m_EventDataUp.msg = msg;
		m_EventDataUp.wParam = wParam;
		m_EventDataUp.lParam = lParam;
	}

	if ( msg == WM_KEYDOWN )
	{
		m_EventDataDown.hwnd = hwnd;
		m_EventDataDown.msg = msg;
		m_EventDataDown.wParam = wParam;
		m_EventDataDown.lParam = lParam;
	}

	if ( msg == WM_LBUTTONDOWN || msg == WM_RBUTTONDOWN || msg == WM_MBUTTONDOWN )
	{
		m_EventMouseDown.hwnd = hwnd;
		m_EventMouseDown.msg = msg;
		m_EventMouseDown.wParam = wParam;
		m_EventMouseDown.lParam = lParam;
	}

	if ( msg == WM_LBUTTONUP || msg == WM_RBUTTONUP || msg == WM_MBUTTONUP )
	{
		m_EventMouseUp.hwnd = hwnd;
		m_EventMouseUp.msg = msg;
		m_EventMouseUp.wParam = wParam;
		m_EventMouseUp.lParam = lParam;
	}	

	if ( msg == WM_MOUSEMOVE )
	{
// 		MouseX = LOWORD(lParam); 
// 		MouseY = HIWORD(lParam); 
		int iCurMousePosX = LOWORD(lParam);
		int iCurMousePosY = HIWORD(lParam);
		if ( m_iPreMousePosX == 0 )
		{
			m_iPreMousePosX = iCurMousePosX;
			m_iPreMousePosY = iCurMousePosY;
		}
		else
		{
			m_iMouseDeltaX = iCurMousePosX - m_iPreMousePosX;
			m_iMouseDeltaY = iCurMousePosY - m_iPreMousePosY;
			m_iPreMousePosX = iCurMousePosX;
			m_iPreMousePosY = iCurMousePosY;
		}
	}

	if ( msg == WM_MOUSEWHEEL )
	{

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

	if ( ret )
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

	if ( ret )
		clear( m_EventDataDown );

	return ret;
}

void GdsInputSystem::clear( EVENT& token )
{
	token.msg = 0; token.wParam = 0; token.lParam = 0;
}

bool GdsInputSystem::GetMouseIsUp( int ikey )
{
	bool ret = false;
	if ( ikey == VM_LBTN && m_EventMouseUp.msg == WM_LBUTTONUP )
	{
		ret = true;
	}
	else if ( ikey == VM_RBTN && m_EventMouseUp.msg == WM_RBUTTONUP )
	{
		ret = true;
	}
	else if ( ikey == VM_MBTN && m_EventMouseUp.msg == WM_MBUTTONUP )
	{
		ret = true;
	}

	if ( ret ) clear( m_EventMouseUp );

	return ret;
}

bool GdsInputSystem::GetMouseIsDown( int ikey )
{
	bool ret = false;
	if ( ikey == VM_LBTN && m_EventMouseDown.msg == WM_LBUTTONDOWN )
	{
		ret = true;
	}
	else if ( ikey == VM_RBTN && m_EventMouseDown.msg == WM_RBUTTONDOWN )
	{
		ret = true;
	}
	else if ( ikey == VM_MBTN && m_EventMouseDown.msg == WM_MBUTTONDOWN )
	{
		ret = true;
	}

	if ( ret ) clear( m_EventMouseDown );

	return ret;
}

void GdsInputSystem::GetMousePosDelta( int& x , int& y , int& z )
{
	x = m_iMouseDeltaX;
	y = m_iMouseDeltaY;
	z = m_iMouseDeltaZ;
}