#pragma once

#include "../Base/CoreBase.h"

class CRendererBase : public CCoreBase{

private:
	
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void render() = 0;

public:
	CRendererBase();
	virtual ~CRendererBase();
	
	virtual void UpdateFrame() = 0;	
};