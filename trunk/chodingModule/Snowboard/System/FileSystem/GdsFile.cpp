
#include "GdsFile.h"


GdsFile::GdsFile()
{
}

GdsFile::GdsFile( tstring& path , const TCHAR* mode )
{
	//파일 열기
	_wfopen_s( &pfile_ , path.c_str() , mode );
}

GdsFile::~GdsFile()
{
	if ( pfile_ )
		fclose( pfile_ );
}

bool GdsFile::read( size_t readsize ,void* buffer )
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