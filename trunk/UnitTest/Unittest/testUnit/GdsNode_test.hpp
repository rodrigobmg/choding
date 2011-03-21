
#pragma once
#include <boost\pool\singleton_pool.hpp>
#include "..\..\..\chodingModule\Snowboard\Core\SceneNode\GdsNode.h"

class Testa
{
public:
	Testa();
	virtual ~Testa();
};

Testa::Testa()
{
	int a = 0;
}

Testa::~Testa()
{

}

class Testb : public Testa
{
public:
	Testb();
	virtual ~Testb();

 	void* operator new( size_t size );
 	void operator delete(void* p);

};

struct MyPoolTag1 { };
typedef boost::singleton_pool<MyPoolTag1, sizeof(Testb)> my_pool1;

Testb::Testb()
{

}

Testb::~Testb()
{

}

 void* Testb::operator new( size_t s )
 {
 	return my_pool1::malloc();
 }
 
 void Testb::operator delete( void* p )
 {
 	my_pool1::free(p);
 }

TEST( test ,  Module )
{
	for (size_t t = 0 ; t < 100 ; t++)
	{
		GdsNodePtr node = GdsNodePtr( new GdsNode );
	}
	
	GdsNodePtr node2 = GdsNodePtr( new GdsNode );
}
