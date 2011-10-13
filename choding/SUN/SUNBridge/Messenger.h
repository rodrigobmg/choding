#ifndef _SUNBRIDGE_MESSENGER_H_
#define _SUNBRIDGE_MESSENGER_H_


class MessengerToCSharp
{
public:
	MessengerToCSharp() {}
	virtual ~MessengerToCSharp() {}


public:
	virtual void WriteLog( LPCTSTR strLog ) = 0;

};


#endif