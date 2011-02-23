#pragma once

#include "../Base/CoreBase.h"

class CRendererBase : public CCoreBase{

public:
	CRendererBase(){ SetName( OBJECT_RENDERBASE ); }
	virtual ~CRendererBase();
	
	virtual void Render() = 0;	

};