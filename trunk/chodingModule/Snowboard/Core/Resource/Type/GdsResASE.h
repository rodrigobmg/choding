#ifndef _GDS_CORE_RESOURCE_TYPE_ASE_H_
#define _GDS_CORE_RESOURCE_TYPE_ASE_H_

#include "GdsResBaseType.h"
#include "..\..\..\System\FileSystem\GdsFile.h"

class GdsResASE : public GdsResBase
{

	bool				DecodeSCENE( LineContainerA& line );
	bool				DecodeMATERIAL_LIST( LineContainerA& line );
	bool				DecodeGEOMOBJECT( LineContainerA& line );
public:
	GdsResASE();
	virtual ~GdsResASE();
	
	virtual void		vClear();
	virtual HRESULT		vRelease();
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device );
		
};

typedef boost::shared_ptr< GdsResASE >	GdsResASEPtr;

#endif
