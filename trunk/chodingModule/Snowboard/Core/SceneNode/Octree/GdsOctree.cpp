#include "GdsOctree.h"

GdsOctree::GdsOctree( D3DXVECTOR3& minVec , D3DXVECTOR3& maxVec , int iDepth )
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