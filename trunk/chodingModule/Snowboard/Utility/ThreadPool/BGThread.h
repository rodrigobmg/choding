#ifndef __SNOWBOARD_UTILITY_BGTHREAD_H__
#define __SNOWBOARD_UTILITY_BGTHREAD_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "SnowThread.h"
/*
class TFunctor
{
public:
	virtual HRESULT operator()( const TCHAR* name ) = 0;
	virtual HRESULT Call( const TCHAR* name ) = 0;
};

template< class T >
class TSpecificFunctor : public TFunctor
{
private:
	HRESULT (T::*fpt)(const TCHAR*);
	T*	ptObject;

public:
	TSpecificFunctor( T* _ptObject , HRESULT (T::*_fpt)(const TCHAR* ) )
	{
		ptObject = _ptObject;
		fpt = _fpt;
	}

	virtual  HRESULT operator()( const TCHAR* name )
	{
		return (*ptObject->*fpt)(name); 
	}

	virtual HRESULT Call( const TCHAR* name )
	{
		return (*ptObject->*fpt)(name);
	}
};*/


class IWORK_TOKEN
{
	friend class BGThread;

public:
	IWORK_TOKEN(){};
	virtual ~IWORK_TOKEN(){};
	virtual void Execute() = 0;
};

template< class _OWNER , class _PARAMETER >
class WORK_TOKEN : public IWORK_TOKEN
{	
	friend class BGThread;

public:

	_OWNER* pthis;
	typedef HRESULT ( _OWNER::*PFUNC)( _PARAMETER );	
	PFUNC			pf;
	_PARAMETER		parameter;


	WORK_TOKEN( _OWNER* _pthis , _PARAMETER _para  , PFUNC fp )
		:pthis( _pthis ) , pf(fp) , parameter( _para )
	{
	};

	void Execute()
	{
		(pthis->*pf)( parameter );
	}
};


class BGThread : public SnowThread
{
	
public:

	template< class _PARAMETER , class _OWNER , class _FP >
	void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	{
		if ( m_ThreadQueue.size() > MAX_SIZE )
		{
			assert( 0 && "MAX ÃÊ°ú" );
			return;
		}

		//m_ThreadQueue[0] = new WORK_TOKEN< _OWNER , _PARAMETER >( pthis , para , fp );
		IWORK_TOKEN* pWorkToken = new WORK_TOKEN< _OWNER , _PARAMETER >( pthis , para , fp );
		m_ThreadQueue.push( pWorkToken );
		::SetEvent( m_hEvent );
	}

	virtual void Run()
	{
 		//m_ThreadQueue[0]->Execute();
		IWORK_TOKEN* pWorkToken = m_ThreadQueue.front();
		pWorkToken->Execute();
		SAFE_DELETE( pWorkToken );
		m_ThreadQueue.pop();
		if ( !m_ThreadQueue.empty() )
			::SetEvent( m_hEvent );
	}


	enum{
		MAX_SIZE = 10,
	};

	//IWORK_TOKEN**	m_ThreadQueue;
	std::queue< IWORK_TOKEN* > m_ThreadQueue;


public:
	static BGThread *New()
	{
		return new BGThread;
	}

	BGThread()
	{
		SetName( OBJECT_BGTHREAD );
		//m_ThreadQueue = new IWORK_TOKEN*[MAX_SIZE];
	}

	virtual ~BGThread(){}; 
	BGThread(const BGThread&) {} 
	void operator=(const BGThread&) {} 

};


#endif	// __SNOWBOARD_UTILITY_SYNC_H__