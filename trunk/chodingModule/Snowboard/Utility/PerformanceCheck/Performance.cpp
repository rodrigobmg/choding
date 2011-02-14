#include "Performance.h"
#include <Windows.h>
#include <winbase.h>

CPerformance::CPerformance( void )
{
	clear();
}

CPerformance::~CPerformance( void )
{
	clear();
}

void CPerformance::clear()
{
	m_ProfileContainer.clear();
}

void CPerformance::Begin( const TCHAR* name )
{
	PROFILE_CONTAINOER::iterator it = m_ProfileContainer.find( name );
	if ( it != m_ProfileContainer.end() )
	{
		if ( it->second.bValid == true )
		{			
			PROFILE_SAMPLE& sample = it->second;
			unsigned long ulCurTick = GetTickCount();
			unsigned long ulTick	= ulCurTick - sample.ulTick;

			sample.ulCount++;
			if ( sample.ulMax < ulTick )
				sample.ulMax = ulTick;

			if ( sample.ulMin > ulTick )
				sample.ulMin = ulTick;

			sample.ulAvg = ( sample.ulFirstTick - ulCurTick ) / sample.ulCount;
			sample.ulTick = ulTick;
		}
	}
	else
	{
		unsigned long ulTick = GetTickCount();
		PROFILE_SAMPLE sample;
		sample.ulCount++;
		sample.ulMax = ulTick;
		sample.ulMin = ulTick;
		sample.ulAvg = ulTick;
		sample.ulTick = ulTick;
		sample.ulFirstTick = ulTick;
	}
}

void CPerformance::End( const TCHAR* name )
{
	PROFILE_CONTAINOER::iterator it = m_ProfileContainer.find( name );
	if ( it != m_ProfileContainer.end() )
	{
		it->second.bValid = false;
	}
}

void CPerformance::OutputAllData()
{
	//데이타 뽑아내는 방법은 각자 알아서
	PROFILE_CONTAINOER::iterator it = m_ProfileContainer.begin();
	for ( ; it != m_ProfileContainer.end() ; ++it )
	{
	}
}