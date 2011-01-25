#pragma once

#include "../Base/Snow.h"

class CResMrg : public CSnow{

private:
	
	typedef stdext::hash_map<tstring, CSnow*>				HASHMAPRes;
	typedef stdext::hash_map<tstring, CSnow*>::iterator		HASHMAPRes_ITERATOR;

	HASHMAPRes		m_mapRes;


	bool		isExist( const wchar_t* filename );
public:
	CResMrg();
	virtual ~CResMrg();

	bool		Load( const wchar_t* filename , LPDIRECT3DDEVICE9 pDevice );
	CSnow*		Get( const wchar_t* filename );

	void		Create();
	void		Clear();

};