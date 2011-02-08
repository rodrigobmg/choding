#pragma once

#include "../Base/CoreBase.h"
#include "Type/ResBaseType.h"

class CResMrg : public CCoreBase{

private:
	
	typedef stdext::hash_map<tstring, CBaseRes*>				HASHMAPRes;
	typedef stdext::hash_map<tstring, CBaseRes*>::iterator		HASHMAPRes_ITERATOR;

	HASHMAPRes		m_mapRes;


	bool		isExist( const wchar_t* filename );
public:
	CResMrg();
	virtual ~CResMrg();

	bool			Load( const wchar_t* filename , LPDIRECT3DDEVICE9 pDevice );
	CBaseRes*		Get( const wchar_t* filename );

	virtual void	Clear();
	virtual HRESULT	Create();
	virtual HRESULT Release();

};