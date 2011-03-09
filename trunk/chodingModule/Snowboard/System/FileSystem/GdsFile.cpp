
#include "GdsFile.h"


GdsFile::GdsFile()
{
}

GdsFile::GdsFile( tstring path )
{
	setpath( path );
}

GdsFile::~GdsFile()
{
	if ( pfile_ )
		fclose( pfile_ );
}

void GdsFile::setpath( tstring path )
{
	//파일 열기
	_wfopen_s( &pfile_ , path.c_str() , L"rb" );
}

bool GdsFile::read( size_t readsize ,BYTE* buffer )
{
	if ( pfile_ == NULL )
	{
		ASSERT( 0 );
		return false;
	}

	fread( buffer , readsize , 1, pfile_ );
	return true;
}

void GdsFile::write( size_t writesize , BYTE* buffer )
{

}