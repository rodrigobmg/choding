#ifndef _SNOWBOARD_COMPONENT_VISUAL_RECT_H_
#define _SNOWBOARD_COMPONENT_VISUAL_RECT_H_

class GdsComVisualRect : public GdsComVisual
{
public:

	GdsComVisualRect();
	virtual ~GdsComVisualRect();

	virtual const goc_id_type componentID() const	{ return COMPONENT_VISUAL_RECT; }

	virtual void	Make();
	virtual	void	Render( float fAccumtime );

};

#endif