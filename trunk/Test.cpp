// Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

#include "MathLib.h"
#include <iostream>

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
};

template< typename T >
class List
{
public:
	
	Data<T>*	Front;	
	Data<T>*	End;
	
	List(){
		Front = End = NULL;
	}
	~List(){
		delete Front;
		delete End;
	};
	
	
	void		Push( T data );
	T			Pop();
};

template<typename T>
void	List<T>::Push( T data )
{
	Data<T>* p = new Data<T>;
	p->t = data;

	if ( &data == NULL )
		return;

	if ( Front == NULL )
	{
		Front = p;
		Front->pNextData = End;
		End->pPreData = p;
	}
	else
	{

	}
}

template<typename T>
T	List<T>::Pop()
{
	if ( End->pPreData == Front )
		return NULL;

	Data<T>* temp;
	temp = End;
	End->pPreData = End;
	delete temp;

	return End->t;
}


int _tmain(int argc, _TCHAR* argv[])
{
	List<int> list;
// 	for (int i= 0; i< 10 ; ++i)
// 	{
// 		Data* p = new Data(i);
// 		list.Push( p );
// 	}
	list.Push( 0 );
	list.Push( 1 );
	list.Push( 2 );
	cout << list.Pop() << endl;
	list.Push( 3 );
	list.Push( 4 );
	cout << list.Pop() << endl;
	list.Push( 5 );
	cout << list.Pop() << endl;
	cout << list.Pop() << endl;
	cout << list.Pop() << endl;

	return 0;
}

