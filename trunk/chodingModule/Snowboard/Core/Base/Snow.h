#pragma once

#include "../../Framework/Snowboard_stdafx.h"

class CSnow
{
public:
	CSnow();
	virtual ~CSnow();

	void			DecRefCount();
	void			IncRefCount()						{ ++m_iRefCount; }
	int32_t			GetRefCount()						{ return m_iRefCount; }	

	void			SetName(const tstring& strName )	{ m_strName = strName; }
	const tstring&	GetName()							{ return m_strName; }

	virtual	void	Release();

private:

	int32_t		m_iRefCount;
	tstring		m_strName;
};