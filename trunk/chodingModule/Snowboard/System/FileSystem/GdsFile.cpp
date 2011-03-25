
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
	ASSERT( m_pFile );
}

GdsFile::~GdsFile()
{
	if ( m_pFile )
		fclose( m_pFile );
}

bool GdsFile::read( long readsize ,void* buffer )
{
	if ( m_pFile == NULL )
	{
		ASSERT( 0 );
		return false;
	}

	fread( buffer , readsize , 1, m_pFile );

	return true;
}

void GdsFile::write( long writesize , BYTE* buffer )
{
	fwrite( buffer , writesize , 1 , m_pFile );
}

long GdsFile::size()
{
	if (fseek(m_pFile, 0, SEEK_END))
		return 0xffffffff;

	long size = ftell(m_pFile);

	fseek(m_pFile, 0, SEEK_SET);
	return size;
}

//////////////////////////////////////////////////////////////////////////
// ReadFileInMemory
//////////////////////////////////////////////////////////////////////////

ReadFileInMemory::ReadFileInMemory( tstring& strPath ):
m_File( strPath , L"rb" ),
m_buffer(NULL)
{
	init();
}

ReadFileInMemory::~ReadFileInMemory()
{
	clear();
}

void ReadFileInMemory::init()
{
	long buffersize = m_File.size();
	m_buffer = (char*)FRAMEMEMORY.Alloc( buffersize );
	ASSERT( m_buffer );
	m_File.read( buffersize , m_buffer );
}

void ReadFileInMemory::clear()
{
	size_t buffersize = sizeof( m_File );
	FRAMEMEMORY.Free( buffersize , m_buffer );
}

const TCHAR* ReadFileInMemory::Next()
{
	return NULL;
}