#pragma once

#include "../Base/CoreBase.h"
#include "Type/ResBaseType.h"

class CResMrg : public CCoreBase{

private:

	enum RES_TYPE{
		TEX = 0,
	};
	
	typedef stdext::hash_map< tstring , CBaseRes* >				HASHMAPRes;
	typedef stdext::hash_map< tstring , CBaseRes* >::iterator	HASHMAPRes_ITERATOR;

	HASHMAPRes		m_mapRes;

	LPDIRECT3DDEVICE9	m_pDevice;

	bool			isExist( const wchar_t* filename );

public:
	CResMrg();
	virtual ~CResMrg();

	CBaseRes*		Get( const wchar_t* filename , bool bforceloading = false );
	int				GetResType( const wchar_t* filename );
	CBaseRes*		CreateRes( RES_TYPE eResType );

	virtual void	Clear();
	virtual HRESULT	Create( LPDIRECT3DDEVICE9 device );
	virtual HRESULT Release();

};