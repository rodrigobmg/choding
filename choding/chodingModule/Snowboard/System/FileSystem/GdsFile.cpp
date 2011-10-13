
#include "GdsFile.h"
#include "..\FrameMemory\GdsFrameMemory.h"


GdsFile::GdsFile( tstring& path )
{
#ifdef UNICODE
	//파일 열기
	_wfopen_s( &m_pFile , path.c_str() , L"rb" );
#else
	//파일 열기
	fopen_s( &m_pFile , path.c_str() , "rb" );	
#endif

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

void ReadFileInMemory::init( tstring& strPath , void* const addtionToken )
{
	GdsFile File( strPath );
	long buffersize = File.size();
	size_t addsize = sizeof( &addtionToken );
	m_buffer = (BYTE*)FRAMEMEMORY.Alloc( buffersize + addsize );	
	if ( m_buffer == NULL )
	{
		ASSERT( 0 );
		return;
	}

	File.read( buffersize , m_buffer );
	memcpy_s( (BYTE*)m_buffer + buffersize , addsize , addtionToken , addsize );
	m_buffersize = buffersize;
}

void ReadFileInMemory::clear()
{
	FRAMEMEMORY.Free( m_buffersize , m_buffer );
}


LineContainerA::iterator::iterator( char* buff ) : m_curr(buff), m_next(buff)
{
	trim_left();
	next_line();
	trim_right();
}

void LineContainerA::iterator::next_line()
{
	m_next = m_curr;

	while (*m_next) 
	{
		if('\n' == *m_next) 
		{
			*(m_next++) = '\0';
			break;
		}
		++m_next;
	}
}

void LineContainerA::iterator::forward()
{
	m_curr = m_next;

	trim_left();
	next_line();
	trim_right();
}

void LineContainerA::iterator::trim_left()
{
	while(*m_curr) 
	{
		if (!::isspace(*m_curr))
			break;
		++m_curr;
	}
}

void LineContainerA::iterator::trim_right()
{
	char* pos = *(m_next-1) ? m_next-1 : m_next-2;
	for (; pos > m_curr; --pos)
	{
		if (!::isspace(*pos))
			break;

		*(pos) = '\0';
	}
}

LineContainerW::iterator::iterator( wchar_t* buff ) : m_curr(buff), m_next(buff)
{
	trim_left();
	next_line();
	trim_right();
}

void LineContainerW::iterator::next_line()
{
	m_next = m_curr;

	while (*m_next) 
	{
		if(L'\n' == *m_next) 
		{
			*(m_next++) = L'\0';
			break;
		}
		++m_next;
	}
}

void LineContainerW::iterator::forward()
{
	m_curr = m_next;

	trim_left();
	next_line();
	trim_right();
}

void LineContainerW::iterator::trim_left()
{
	while(*m_curr) 
	{
		if (!::iswspace(*m_curr))
			break;
		++m_curr;
	}
}

void LineContainerW::iterator::trim_right()
{
	wchar_t* pos = *(m_next-1) ? m_next-1 : m_next-2;
	for (; pos > m_curr; --pos)
	{
		if (!::iswspace(*pos))
			break;

		*(pos) = L'\0';
	}
}


wchar_t* LineContainerW::vGetBuffer()
{
	char* pBuff = (char*)m_buff.GetBuffer();

	//@{ skip ByteOrderMark
	const BYTE BOM_UCS_4_big_endian		[4] = { 0x00, 0x00, 0xFE, 0xFF, };
	const BYTE BOM_UCS_4_litle_endian	[4] = { 0xFF, 0xFE, 0x00, 0x00, };
	const BYTE BOM_UCS_2_big_endian		[2] = { 0xFE, 0xFF, };
	const BYTE BOM_UCS_2_litle_endian	[2] = { 0xFF, 0xFE, };
	const BYTE BOM_UTF_8				[3] = { 0xEF, 0xBB, 0xBF, };

	if (0 == memcmp(pBuff, BOM_UCS_4_big_endian, sizeof(BOM_UCS_4_big_endian))) {
		pBuff += 4; //UTF-32 : 4byte offset
	} else if (0 == memcmp(pBuff, BOM_UCS_4_litle_endian, sizeof(BOM_UCS_4_litle_endian))) {
		pBuff += 4; //UTF-32 : 4byte offset
	} else if (0 == memcmp(pBuff, BOM_UCS_2_big_endian, sizeof(BOM_UCS_2_big_endian))) {			
		pBuff += 2; //UTF-16 : 2byte offset
	} else if (0 == memcmp(pBuff, BOM_UCS_2_litle_endian, sizeof(BOM_UCS_2_litle_endian))) {
		pBuff += 2; //UTF-16 : 2byte offset
	} else if (0 == memcmp(pBuff, BOM_UTF_8, sizeof(BOM_UTF_8))) {
		pBuff += 3; //UTF-8 : 3byte offset
	} 
	//@}

	return static_cast<wchar_t*>((void*)pBuff);
}