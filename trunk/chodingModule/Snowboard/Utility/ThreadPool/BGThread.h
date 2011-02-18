#ifndef __SNOWBOARD_UTILITY_BGTHREAD_H__
#define __SNOWBOARD_UTILITY_BGTHREAD_H__

#include "../../Framework/Snowboard_stdafx.h"
#include "SnowThread.h"
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
	TSpecificFunctor( T* _ptObject , void(T::*_fpt)(const TCHAR* ) )
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
};

class BGThread : public SnowThread
{

	enum{
		MAX_SIZE = 10,
	};
	typedef struct WORK_TOKEN
	{
		CSnow*				pIntance;
		void (CSnow::*pFunc)();
	};

	std::queue< WORK_TOKEN > m_ThreadQueue;
	std::queue< TFunctor* >	m_ThreadFunctorQueue;
//
// Initialize & Destroy Methods
public:
	static BGThread *New();

	BGThread();
	virtual ~BGThread(); 
	BGThread(const BGThread&) {} 
	void operator=(const BGThread&) {} 


//
// Member Functions
public:

	void	Push( CSnow* ins , void (CSnow::*pf)() );
	void	Push( TFunctor* tFunctor );
	virtual void Run();

};


#endif	// __SNOWBOARD_UTILITY_SYNC_H__