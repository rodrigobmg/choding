#include "GdsOctree.h"

void GdsOctree::clear()
{
	if ( m_pNode )
		SAFE_DELETE( m_pNode );
}