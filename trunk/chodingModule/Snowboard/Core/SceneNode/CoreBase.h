#pragma once

#include "Snow.h"

class CCoreBase : public CSnow{


public:
	CCoreBase();
	virtual ~CCoreBase();

	virtual void	Clear();
	virtual HRESULT	Create();
	virtual HRESULT	Release();
};