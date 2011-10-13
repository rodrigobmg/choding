#ifndef _GDS_SYSTEM_FRAMEMEMORY_H_
#define _GDS_SYSTEM_FRAMEMEMORY_H_

#include "../../Framework/Snowboard_stdafx.h"

class GdsFrameMemory : noncopyable
{
public:
	explicit GdsFrameMemory():frame_size(0) , buffer(NULL) , cur_pos(0) {};
	~GdsFrameMemory(){ clear(); }
	
	void Init( size_t framesize_ )
	{
		ASSERT( buffer == NULL );
		buffer = (char*)::malloc( framesize_ );		
		ASSERT( buffer != NULL );
		frame_size = framesize_;
	}

	//not thread safe
	void* Alloc( size_t allocsize )
	{
		ASSERT( cur_pos + allocsize < frame_size );
		if ( cur_pos + allocsize > frame_size )
			return NULL;

		void* p = buffer + cur_pos;
		cur_pos += allocsize;
		stack.push_back( p );
		return p;
	}

	void Free( size_t freesize , void* p )
	{
		ASSERT( stack.back() == p );
		stack.pop_back();
		cur_pos -= freesize;
	}

	size_t GetSize(){ return frame_size; }

private:

	void clear()
	{
		ASSERT( stack.empty() );

		free( buffer );
		cur_pos = 0;
		frame_size = 0;
	}

	size_t cur_pos;
	size_t frame_size;
	char*	buffer;
	std::vector< void* > stack;

};

#define FRAMEMEMORY	singleton_holder< GdsFrameMemory >::instance()

#endif
