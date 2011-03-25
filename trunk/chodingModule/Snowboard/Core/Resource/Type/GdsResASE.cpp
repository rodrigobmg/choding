#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"
#include "Log/logger.h"

GdsResASE::GdsResASE()
{
	SetName( OBJECT_RES_ASE );
	vClear();
}

GdsResASE::~GdsResASE()
{
}

void GdsResASE::vClear()
{
}

HRESULT GdsResASE::vRelease()
{	
	return true;
}

HRESULT GdsResASE::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	LineContainerA	lines( m_strPath );
	LineContainerA::iterator it = lines.begin();
	LineContainerA::iterator it_end = lines.end();

	for ( ; it != it_end ; ++it )
	{
// 		char* arg1;
// 		char* arg2;
// 		//arg1 = (char*)malloc(80);
// 		arg2 = (char*)malloc( 80 );
// 		size_t size = strlen( *(it) );
// 		strcpy_s( arg1 , size , *(it) );
// 		sscanf_s( *(it) , "*3DSMAX_ASCIIEXPORT%s" , arg2 );
		//LOG_WARNING_F( "%s" , *(it) );
	}
	

	return true;
}
