#pragma once

#include "Base/Snow.h"

class CCoreMgr : public CSnow{

private:

	typedef stdext::hash_map<tstring, CSnow*>				HASHMAPCore;
	typedef stdext::hash_map<tstring, CSnow*>::iterator		HASHMAPCore_ITERATOR;

	HASHMAPCore		m_mapCore;

public:
	CCoreMgr();
	virtual ~CCoreMgr();

	static CCoreMgr*	New();

	CSnow*			RegisterCore( const tstring& corename, CSnow* pCore );
	void			UnregisterCore( const tstring& corename );
	void			DestroyCore();

	CSnow*			GetCore( const tstring& strCoreName );
	int				GetContainerSize()			{ return (int)m_mapCore.size(); }
	
};