#pragma once

#include "../Base/Snow.h"

class CRendererBase : public CSnow{

private:
	
	virtual void begin() = 0;
	virtual void end() = 0;
	virtual void render() = 0;

public:
	CRendererBase();
	virtual ~CRendererBase();
	
	virtual void UpdateFrame() = 0;	
};