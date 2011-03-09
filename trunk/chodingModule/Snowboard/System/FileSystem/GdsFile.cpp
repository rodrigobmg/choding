
#include "GdsFile.h"


GdsFile::GdsFile()
{
}

GdsFile::GdsFile( tstring path )
{
	Setpath( path );
}

GdsFile::~GdsFile()
{
	if ( pfile_ )
		fclose( pfile_ );
}

void GdsFile::Setpath( tstring path )
{
	//파일 열기
	_wfopen_s( &pfile_ , path.c_str() , L"rb" );
}

bool GdsFile::Read( size_t readsize ,void* buffer )
{
	if ( pfile_ == NULL )
	{
		ASSERT( 0 );
		return false;
	}

	fread( buffer , readsize , 1, pfile_ );
	return true;
}

void GdsFile::Write( size_t writesize , BYTE* buffer )
{

}