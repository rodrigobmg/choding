#ifndef _CHODING_BASEOBJECT_
#define _CHODING_BASEOBJECT_

#include <cassert>

class CObject
{
public:
	CObject(){};
	virtual ~CObject(){};

private:
	int m_iRefCount;
}

#endif