#pragma once

//#include "../../Framework/Snowboard_stdafx.h"

#include <hash_map>
#include <tchar.h>

class CPerformance
{
private:
	
	typedef struct PROFILE_SAMPLE
	{
		bool			bValid;
		unsigned long	ulTick;
		unsigned long	ulFirstTick;
		unsigned long	ulMin;
		unsigned long	ulAvg;
		unsigned long	ulMax;
		unsigned long	ulCount;
		PROFILE_SAMPLE()
		{
			bValid = true;
			ulTick = 0U;
			ulMin  = 0U;
			ulAvg  = 0U;
			ulMax  = 0U;
			ulCount= 0U;
			ulFirstTick=0U;
		}
	};

	typedef stdext::hash_map< const TCHAR* , PROFILE_SAMPLE >	PROFILE_CONTAINOER;
	PROFILE_CONTAINOER		m_ProfileContainer;

	void					clear();

public:
	CPerformance(void);
	~CPerformance(void);
	
	void				Begin( const TCHAR* name );
	void				End( const TCHAR* name );

	void				OutputAllData();
	
};
