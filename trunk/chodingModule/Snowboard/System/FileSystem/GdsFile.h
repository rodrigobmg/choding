#ifndef _GDS_SYSTEM_FILESYSTEM_FILE_H_
#define _GDS_SYSTEM_FILESYSTEM_FILE_H_

#include "../../Framework/Snowboard_stdafx.h"

class GdsFile : noncopyable
{
private:
	
	FILE*	pfile_;

public:

	GdsFile();
	GdsFile( tstring& path , const TCHAR* mode );
	virtual ~GdsFile();

	bool	read( size_t readsize , void* buffer );
	void	write( size_t writesize , BYTE* buffer );

};

class Linecontainer : noncopyable
{

};

#endif