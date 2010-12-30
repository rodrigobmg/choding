#ifndef _CHODING_PROFILE_
#define _CHODING_PROFILE_

namespace PROFILE{


	typedef struct {
		bool bValid;                 //Whether this data is valid
		unsigned int iProfileInstances;      //# of times ProfileBegin called
		int iOpenProfiles;           //# of times ProfileBegin w/o ProfileEnd
		char szName[256];            //Name of sample
		float fStartTime;            //The current open profile start time
		float fAccumulator;          //All samples this frame added together
		float fChildrenSampleTime;   //Time taken by all children
		unsigned int iNumParents;            //Number of profile parents
	} ProfileSample;

	typedef struct {
		bool bValid;        //Whether the data is valid
		char szName[256];   //Name of the sample
		float fAve;         //Average time per frame (percentage)
		float fMin;         //Minimum time per frame (percentage)
		float fMax;         //Maximum time per frame (percentage)
	} ProfileSampleHistory;

	void ProfileInit( void );
	void ProfileBegin( char* name );
	void ProfileEnd( char* name );
	void ProfileDumpOutputToBuffer( void );
	void StoreProfileInHistory( char* name, float percent );
	void GetProfileFromHistory( char* name, float* ave, float* min, float* max );
	void ProfileDraw( void );
}


#endif
