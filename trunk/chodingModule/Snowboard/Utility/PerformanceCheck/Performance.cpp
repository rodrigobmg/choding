#include "Performance.h"
#include <Windows.h>
#include <winbase.h>

namespace util
{
	namespace Performance
	{
		PROFILE_CONTAINOER		m_ProfileContainer;

		void clear()		
		{
			m_ProfileContainer.clear();		
		}

		void Begin( const TCHAR* name )
		{
			PROFILE_CONTAINOER::iterator it = m_ProfileContainer.find( name );
			if ( it != m_ProfileContainer.end() )
			{	
				stPROFILE_SAMPLE& sample = it->second;
				unsigned long ulCurTick = GetTickCount();
				unsigned long ulDeltaTick = ulCurTick - sample.ulEndTick;

				sample.ulCount++;
				if ( sample.ulMax < ulDeltaTick )
					sample.ulMax = ulDeltaTick;

				if ( sample.ulMin > ulDeltaTick )
					sample.ulMin = ulDeltaTick;

				sample.ulEndTick = ulCurTick;
			}
			else
			{
				unsigned long ulTick = GetTickCount();
				PROFILE_SAMPLE sample;
				sample.ulCount++;
				sample.ulMax = ulTick;
				sample.ulMin = ulTick;
				sample.ulAvg = 0;
				sample.ulFirstTick = ulTick;
				m_ProfileContainer.insert( PAIR( name , sample ) );
			}
		}

		void End( const TCHAR* name )
		{
			PROFILE_CONTAINOER::iterator it = m_ProfileContainer.find( name );
			if ( it != m_ProfileContainer.end() )
			{
				stPROFILE_SAMPLE& sample = it->second;
				sample.ulEndTick = GetTickCount();
				
				sample.ulAvg = ( sample.ulEndTick - sample.ulFirstTick ) / sample.ulCount;
				sample.ulTotalDeltaTick = sample.ulEndTick - sample.ulFirstTick;
			}
		}

		void OutputAllData()
		{
			//데이타 뽑아내는 방법은 각자 알아서
			PROFILE_CONTAINOER::iterator it = m_ProfileContainer.begin();
			for ( ; it != m_ProfileContainer.end() ; ++it )
			{
			}
		}

		void OutputData( const TCHAR* profilename , stPROFILE_SAMPLE& outputsample )
		{
			PROFILE_CONTAINOER::iterator it = m_ProfileContainer.find( profilename );
			if ( it != m_ProfileContainer.end() )
			{
				outputsample = it->second;
// 				outputsample.ulAvg = outputsample.ulAvg / 1000;
// 				outputsample.ulEndTick = outputsample.ulEndTick / 1000;
// 				outputsample.ulFirstTick = outputsample.ulFirstTick / 1000;
// 				outputsample.ulMax = outputsample.ulMax / 1000;
// 				outputsample.ulMin = outputsample.ulMin / 1000;
// 				outputsample.ulTotalDeltaTick = outputsample.ulTotalDeltaTick / 1000;
			}
		}
	}	
}