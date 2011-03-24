#ifndef _GDS_SYSTEM_FILESYSTEM_FILE_H_
#define _GDS_SYSTEM_FILESYSTEM_FILE_H_

#include "../../Framework/Snowboard_stdafx.h"

class GdsFile : noncopyable
{
private:
	
	FILE*	m_pFile;

public:

	GdsFile();
	GdsFile( tstring& path , const TCHAR* mode );
	virtual ~GdsFile();

	bool	read( size_t readsize , void* buffer );
	void	write( size_t writesize , BYTE* buffer );

};

class ReadFileToMemory : noncopyable
{
	ReadFileToMemory( tstring& strPath );
	~ReadFileToMemory();

	GdsFile			m_File;
	const TCHAR*	m_token;
	wchar_t*		m_buffer;

	void			init();
	void			clear();

public:

	void			SetToken( const TCHAR* token ){ m_token = token; }
	const TCHAR*	Next();
};

#endif