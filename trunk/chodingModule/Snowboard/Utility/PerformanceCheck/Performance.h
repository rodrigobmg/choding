#pragma once

#include <map>
#include <tchar.h>
#include "../Utility.h"

namespace util 
{
	namespace Performance
	{
		typedef struct PROFILE_SAMPLE
		{
			unsigned long	ulEndTick;
			unsigned long	ulFirstTick;
			unsigned long	ulMin;
			unsigned long	ulAvg;
			unsigned long	ulMax;
			unsigned long	ulCount;
			unsigned long	ulTotalDeltaTick;
			PROFILE_SAMPLE()
			{
				ulEndTick= 0U;
				ulMin  = 0U;
				ulAvg  = 0U;
				ulMax  = 0U;
				ulCount= 0U;
				ulFirstTick=0U;
				ulTotalDeltaTick = 0U;
			}
		}stPROFILE_SAMPLE;

		typedef std::map< const TCHAR* , stPROFILE_SAMPLE >			PROFILE_CONTAINOER;
		typedef std::pair< const TCHAR* , stPROFILE_SAMPLE >		PAIR;
		
		

		void				clear();

		void				Begin( const TCHAR* name );
		void				End( const TCHAR* name );

		void				OutputAllData();
		void				OutputData( const TCHAR* profilename , // input
										 stPROFILE_SAMPLE& outputsample
										);
		
	}
}	


#define BEGIN_PERFORMANCE( exp )		util::Performance::Begin( exp )
#define END_PERFORMANCE( exp )			util::Performance::End( exp )	
#define OUTPUT_PERFORMANCE( p1 , p2 )	util::Performance::OutputData( p1 , p2 )
#define SAMPLE_PERFORMANCE				util::Performance::stPROFILE_SAMPLE