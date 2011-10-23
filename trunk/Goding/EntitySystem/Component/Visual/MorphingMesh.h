#pragma once

#include "..\IComponent.h"

class MorphingMesh : public IComponent
{
public:
	MorphingMesh(void);
	virtual ~MorphingMesh(void);
	virtual void			SetComponent( int32 id );

};

