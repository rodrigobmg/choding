#ifndef _GDS_SYSTEM_FILESYSTEM_FILE_H_
#define _GDS_SYSTEM_FILESYSTEM_FILE_H_

#include "../../Framework/Snowboard_stdafx.h"

class GdsFile : noncopyable
{
private:
	
	FILE*	m_pFile;

public:

	explicit GdsFile( tstring& path );
	~GdsFile();

	bool	read( long readsize , void* buffer );
	void	write( long writesize , BYTE* buffer );
	long	size();
};


class ReadFileInMemory : noncopyable
{
	BYTE*			m_buffer;
	long			m_buffersize;

	void			init( tstring& strPath , void* const addtionToken );
	void			clear();


public:

	explicit ReadFileInMemory( tstring& strPath );
	
	template< class T >
	ReadFileInMemory( tstring& strPath , T const& additionToken )
	{
		init( strPath , (void*)&additionToken );
	}

	~ReadFileInMemory()
	{
		clear();
	};

	void*			GetBuffer() const { return m_buffer; }
	long			GetBufferSize(){ return m_buffersize; }

};

class LineContainerA : noncopyable
{
public:
	class iterator : public std::iterator<std::input_iterator_tag, const char*, void>
	{
	public:
		explicit iterator(char* buff);

		char* operator*() {ASSERT(!empty()); return m_curr;}
		const char* operator*() const {ASSERT(!empty()); return m_curr;}

		iterator& operator++() { forward(); return *this; }
		iterator operator++(int i) { i=i; iterator temp = *this; ++*this; return temp; }
		bool operator==(const iterator& rhs) const {return m_curr == rhs.m_curr;}
		bool operator!=(const iterator& rhs) const {return m_curr != rhs.m_curr;}

		int length()const { return (m_next > m_curr) ? static_cast<int>( (m_next-m_curr) ) : 0; }
		bool empty()const { return m_curr == m_next; }

	private:

		void next_line();
		void forward();
		void trim_left();
		void trim_right();

	private:

		char* m_curr;
		char* m_next;
	};

	explicit LineContainerA( tstring& full_path) : m_buff(full_path, '\0') {}
	bool empty()const { return (NULL == m_buff.GetBuffer()); }

	iterator begin() { return iterator((char*)m_buff.GetBuffer()); }
	iterator end() { return iterator((char*)m_buff.GetBuffer() + m_buff.GetBufferSize()); }

private:

	ReadFileInMemory	m_buff;

};


class LineContainerW : noncopyable
{
public:
	class iterator : public std::iterator<std::input_iterator_tag, const wchar_t*, void>
	{
	public:
		explicit iterator(wchar_t* buff);

		wchar_t* operator*() {ASSERT(!empty()); return m_curr;}
		const wchar_t* operator*() const {ASSERT(!empty()); return m_curr;}

		iterator& operator++() { forward(); return *this; }
		iterator operator++(int i) { i=i; iterator temp = *this; ++*this; return temp; }
		bool operator==(const iterator& rhs) const {return m_curr == rhs.m_curr;}
		bool operator!=(const iterator& rhs) const {return m_curr != rhs.m_curr;}

		int length()const { return (m_next > m_curr) ? static_cast<int>( (m_next-m_curr) ) : 0; }
		bool empty()const { return m_curr == m_next; }

	private:

		void next_line();
		void forward();
		void trim_left();
		void trim_right();

	private:

		wchar_t* m_curr;
		wchar_t* m_next;
	};

	explicit LineContainerW( tstring& full_path) : m_buff(full_path, L'\0') {}
	bool empty()const { return (NULL == m_buff.GetBuffer()); }

	iterator begin() { return iterator(vGetBuffer()); }
	iterator end() { return iterator((wchar_t*)((char*)m_buff.GetBuffer() + m_buff.GetBufferSize())); }

private:

	wchar_t*	vGetBuffer();

	ReadFileInMemory	m_buff;

};

#endif