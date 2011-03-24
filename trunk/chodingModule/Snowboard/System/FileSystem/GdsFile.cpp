
#include "GdsFile.h"
#include "..\FrameMemory\GdsFrameMemory.h"


GdsFile::GdsFile():
m_pFile(NULL)
{
}

GdsFile::GdsFile( tstring& path , const TCHAR* mode )
{
	//파일 열기
	_wfopen_s( &m_pFile , path.c_str() , mode );
}

GdsFile::~GdsFile()
{
	if ( m_pFile )
		fclose( m_pFile );
}

bool GdsFile::read( size_t readsize ,void* buffer )
{
	if ( m_pFile == NULL )
	{
		ASSERT( 0 );
		return false;
	}

	fread( buffer , readsize , 1, m_pFile );
	return true;
}

void GdsFile::write( size_t writesize , BYTE* buffer )
{
	fwrite( buffer , writesize , 1 , m_pFile );
}

//////////////////////////////////////////////////////////////////////////
// ReadFileToMemory
//////////////////////////////////////////////////////////////////////////

ReadFileToMemory::ReadFileToMemory( tstring& strPath ):
m_File( strPath , L"rt" ),
m_buffer(NULL)
{
	init();
}

ReadFileToMemory::~ReadFileToMemory()
{
	clear();
}

void ReadFileToMemory::init()
{
	size_t buffersize = sizeof( m_File );
	m_buffer = (wchar_t*)( (char*)FRAMEMEMORY.Alloc( buffersize ) );
	ASSERT( m_buffer );
	m_File.read( buffersize , m_buffer );
}

void ReadFileToMemory::clear()
{
	size_t buffersize = sizeof( m_File );
	FRAMEMEMORY.Free( buffersize , m_buffer );
}

const TCHAR* ReadFileToMemory::Next()
{
	return NULL;
}