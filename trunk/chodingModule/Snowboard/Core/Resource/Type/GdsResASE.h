#ifndef _GDS_CORE_RESOURCE_TYPE_ASE_H_
#define _GDS_CORE_RESOURCE_TYPE_ASE_H_

#include "GdsResBaseType.h"

class GdsResASE : public GdsResBase{

public:

	DeclareBoostPool	

private:

public:
	GdsResASE();
	virtual ~GdsResASE();
	
	virtual void		vClear();
	virtual HRESULT		vCreate();
	virtual HRESULT		vRelease();
	virtual HRESULT		vReCreate( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		vLoadResource( const TCHAR* path  , LPDIRECT3DDEVICE9 device );
		
};

typedef boost::shared_ptr< GdsResASE >	GdsResASEPtr;

#endif
