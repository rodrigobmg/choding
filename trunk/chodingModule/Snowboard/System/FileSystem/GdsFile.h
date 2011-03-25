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

	bool	read( long readsize , void* buffer );
	void	write( long writesize , BYTE* buffer );
	long	size();
};

class ReadFileInMemory : noncopyable
{
	GdsFile			m_File;
	const TCHAR*	m_token;
	char*			m_buffer;

	void			init();
	void			clear();


public:

	ReadFileInMemory( tstring& strPath );
	~ReadFileInMemory();

	void			SetToken( const TCHAR* token ){ m_token = token; }
	const TCHAR*	Next();
};

#endif