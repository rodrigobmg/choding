#ifndef _GDS_SYSTEM_FILESYSTEM_FILE_H_
#define _GDS_SYSTEM_FILESYSTEM_FILE_H_

#include "../../Core/Base/GdsObject.h"
#include <mbstring.h>

class GdsFile : public GdsObject
{
private:
	
	FILE*	pfile_;

public:

	GdsFile();
	GdsFile( tstring path );
	virtual ~GdsFile();

	void	Setpath( tstring path );
	bool	Read( size_t readsize , void* buffer );
	void	Write( size_t writesize , BYTE* buffer );

};
#endif