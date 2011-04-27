#include "GdsOctree.h"

GdsOctree::GdsOctree( D3DXVECTOR3* vBuffer , size_t buffer_size , int iMaxDepth )
{

}

GdsOctree::~GdsOctree()
{

}

void GdsOctree::Build()
{
	buildNode( 0 );	
}

bool GdsOctree::buildNode( int iCurDepth )
{
	iCurDepth = iCurDepth;
	return iCurDepth;
}