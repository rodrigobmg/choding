#pragma once

#include "../SceneNode/SceneNode.h"

class CSnow;

class CRendererBase : public CSnow{

public:
	CRendererBase(){ SetName( OBJECT_RENDERBASE ); }
	virtual ~CRendererBase(){};
	
	virtual void Render( SceneNode* pNode ) = 0;	

};