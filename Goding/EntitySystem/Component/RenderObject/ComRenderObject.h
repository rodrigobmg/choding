#pragma once

#include "..\IComponent.h"

class IRenderObject;

class ComRenderObject : public IComponent
{
public:
	ComRenderObject(void);
	virtual ~ComRenderObject(void);
	virtual void			SetComponent( int32 id );

	IRenderObject*			GetRenderObject(){return m_pRenderObject;}
	void					SetRenderObject( IRenderObject* p ){ m_pRenderObject = p; }

private:

	IRenderObject*			m_pRenderObject;
};

