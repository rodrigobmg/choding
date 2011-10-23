#pragma once

#include "..\IComponent.h"

class SkinnedMesh : public IComponent
{
public:
	SkinnedMesh(void);
	virtual ~SkinnedMesh(void);
	virtual void			SetComponent( int32 id );

};

