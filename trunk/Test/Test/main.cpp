// Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include <iostream>
#include <tchar.h>

using namespace std;

template< typename T >
class Data
{
public:

	Data*	pPreData;
	Data*	pNextData;
	T		t;

	Data()
	{
		pNextData = NULL;
		pPreData = NULL;
		t = 0;
	}

	Data( T v )
	{
		pNextData = NULL;
		pPreData = NULL;
		t = v;
	}

	~Data()
	{
		pPreData = NULL;
		pNextData = NULL;
	}
};

template< typename T >
class List
{
public:

	Data<T>*	Front;	
	Data<T>*	End;

	List(){
		Front = new Data<T>(-1);
		End = new Data<T>(-1);

		Front->pPreData	= NULL;
		Front->pNextData = End;

		End->pPreData	= Front;
		End->pNextData	= NULL;
	}
	~List(){
		delete Front;
		delete End;
	};


	void		push_front( T data );
	void		push_back( T data );
	T			pop_front();
	T			pop_back();
};

template<typename T>
void	List<T>::push_front( T data )
{
	Data<T>* p = new Data<T>(data);

	if ( &data == NULL )
		return;

	p->pNextData = Front->pNextData;
	p->pPreData	 = Front;

	Front->pNextData->pPreData = p;
	Front->pNextData = p;

}

template<typename T>
void	List<T>::push_back( T data )
{
	Data<T>* p = new Data<T>(data);

	if ( &data == NULL )
		return;

	p->pNextData = End;
	p->pPreData	 = End->pPreData;

	End->pPreData->pNextData = p;
	End->pPreData = p;

}

template<typename T>
T	List<T>::pop_front()
{
	if ( Front->pNextData == End )
		return static_cast<T>(NULL);

	Data<T>* temp = Front->pNextData;
	T t = temp->t;

	Front->pNextData = temp->pNextData;
	temp->pNextData->pPreData = Front;
	delete temp;

	return t;
}

template<typename T>
T	List<T>::pop_back()
{
	if ( End->pPreData == Front )
		return static_cast<T>(NULL);

	Data<T>* temp = End->pPreData;
	T t = temp->t;

	temp->pPreData->pNextData = End;
	End->pPreData = temp->pPreData;

	delete temp;

	return t;
}

int _tmain(int argc, _TCHAR* argv[])
{
	List<int> list;
	
	list.push_front( 0 );
	list.push_front( 1 );
	list.push_back( 2 );	
	list.push_back( 3 );
	list.push_front( 4 );
	cout << list.pop_front() << endl;
	list.push_front( 5 );
	cout << list.pop_front() << endl;
	cout << list.pop_back() << endl;
	cout << list.pop_back() << endl;


	int ivalue = 0xFFFFFFFF;
	int offset = 1;
	int size = sizeof(int);
	int count = 0;
	for (int i = 0 ; i < 31 ; ++i )
	{
		if ( ivalue & offset )
		{
			count++;
			
		}
		offset = offset << 1;
	}



	return 0;
}

