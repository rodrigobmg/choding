#ifndef __SNOWBOARD_UTILITY_BGTHREAD_H__
#define __SNOWBOARD_UTILITY_BGTHREAD_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "GdsThread.h"

class GdsBGThread : public GdsThread
{
	class IWORK_TOKEN
	{

	public:
		IWORK_TOKEN(){};
		virtual ~IWORK_TOKEN(){};
		virtual void Execute() = 0;
	};

	template< class _PARAMETER , class _OWNER >
	class WORK_TOKEN : public IWORK_TOKEN
	{	

	public:

		_OWNER* pthis;
		typedef void ( _OWNER::*PFUNC)( _PARAMETER );	
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

	GdsBGThread()
	{
		SetName( OBJECT_BGTHREAD );
		SetMaxcapacity( 1 );
	}

	virtual ~GdsBGThread()
	{
		while( !m_ThreadQueue.empty() )
		{
			IWORK_TOKEN* ptoken = m_ThreadQueue.front();
			SAFE_DELETE( ptoken );			
			m_ThreadQueue.pop();
		}
	}; 

	static GdsBGThread *New()
	{
		return new GdsBGThread;
	}

	template< class _PARAMETER , class _OWNER , class _FP >
	void	Push( _OWNER* pthis , _PARAMETER para , _FP fp )
	{
		if ( m_ThreadQueue.size() > m_byMaxcapacity )
		{
			ASSERT( 0 && "MAX ÃÊ°ú" );
			return;
		}

		IWORK_TOKEN* pWorkToken = new WORK_TOKEN< _PARAMETER , _OWNER >( pthis , para , fp );
		m_ThreadQueue.push( pWorkToken );
		::SetEvent( m_hEvent );
	}

	virtual void Run()
	{
		IWORK_TOKEN* pWorkToken = m_ThreadQueue.front();
		m_ThreadQueue.pop();
		unsigned long CurTick = GetTickCount();
		pWorkToken->Execute();
		m_ProcessTick = GetTickCount() - CurTick;
		SAFE_DELETE( pWorkToken );		

		if ( !m_ThreadQueue.empty() )
			::SetEvent( m_hEvent );
	}

	void		SetMaxcapacity( BYTE byValue )	{ m_byMaxcapacity = byValue; }
	BYTE		GetMaxcapacity()				{ return m_byMaxcapacity;	}
	int			GetCountWaitingJob() const		{ return (int)m_ThreadQueue.size(); }
	unsigned long	GetProcessTick(){ return m_ProcessTick; }

private:

	std::queue< IWORK_TOKEN* > m_ThreadQueue;
	BYTE		m_byMaxcapacity;
	unsigned long	m_ProcessTick;

};

typedef boost::shared_ptr< GdsBGThread >	GdsBGThreadPtr;

#endif	// __SNOWBOARD_UTILITY_SYNC_H__