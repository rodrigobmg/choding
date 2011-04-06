#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"
#include "../../../System/Logger/logger.h"

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
 		char* arg2;
		size_t size = strlen( *(it) );
 		arg2 = (char*)malloc( size );
 		
 		//strcpy_s( arg1 , size , *(it) );
 		sscanf_s( *(it) , "*3DSMAX_ASCIIEXPORT\t%s" , arg2 , sizeof( char )*size );
		int check = atoi( arg2 );
		size = strlen( *(it) );
	}
	

	return true;
}
