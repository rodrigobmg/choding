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
		LOG_WARNING_F( "%s" , *(it) );
	}
	

	return true;
}
