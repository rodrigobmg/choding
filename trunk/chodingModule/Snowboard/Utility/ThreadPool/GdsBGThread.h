#ifndef __SNOWBOARD_UTILITY_BGTHREAD_H__
#define __SNOWBOARD_UTILITY_BGTHREAD_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "GdsThread.h"
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

class GdsBGThread : public GdsThread
{
	class IWORK_TOKEN
	{

	public:
		IWORK_TOKEN(){};
		virtual ~IWORK_TOKEN(){};
		virtual void Execute() = 0;
	};

	template< class _OWNER , class _PARAMETER >
	class WORK_TOKEN : public IWORK_TOKEN
	{	

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
	
public:

	template< class _PARAMETER , class _OWNER , class _FP >
	void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	{
		if ( m_ThreadQueue.size() > m_byMaxcapacity )
		{
			assert( 0 && "MAX ÃÊ°ú" );
			return;
		}

		IWORK_TOKEN* pWorkToken = new WORK_TOKEN< _OWNER , _PARAMETER >( pthis , para , fp );
		m_ThreadQueue.push( pWorkToken );
		::SetEvent( m_hEvent );
	}

	virtual void Run()
	{
		IWORK_TOKEN* pWorkToken = m_ThreadQueue.front();
		pWorkToken->Execute();
		SAFE_DELETE( pWorkToken );
		m_ThreadQueue.pop();
		if ( !m_ThreadQueue.empty() )
			::SetEvent( m_hEvent );
	}

	void		SetMaxcapacity( BYTE byValue )	{ m_byMaxcapacity = byValue; }
	BYTE		GetMaxcapacity()				{ return m_byMaxcapacity;	}

	std::queue< IWORK_TOKEN* > m_ThreadQueue;
	BYTE		m_byMaxcapacity;


public:
	static GdsBGThread *New()
	{
		return new GdsBGThread;
	}

	GdsBGThread()
	{
		SetName( OBJECT_BGTHREAD );
		SetMaxcapacity( 1 );
	}

	virtual ~GdsBGThread(){}; 

};

typedef boost::shared_ptr< GdsBGThread >	GdsBGThreadPtr;

#endif	// __SNOWBOARD_UTILITY_SYNC_H__