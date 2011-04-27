#ifndef _GDS_CORE_NODE_OCTREE_H_
#define _GDS_CORE_NODE_OCTREE_H_

#include "../../Base/GdsObject.h"

class GdsOctree : public GdsObject
{	

	bool				buildNode( int iCurDepth );		

public:

	GdsOctree( D3DXVECTOR3* vBuffer , size_t buffer_size , int iMaxDepth );
	virtual ~GdsOctree();

	void				Build();

};

typedef boost::shared_ptr< GdsOctree >	GdsOctreePtr;

#endif