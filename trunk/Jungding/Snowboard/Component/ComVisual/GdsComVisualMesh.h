#ifndef _SNOWBOARD_COMPONENT_VISUAL_MESH_H_
#define _SNOWBOARD_COMPONENT_VISUAL_MESH_H_

class GdsComVisualMesh : public GdsComVisual
{
public:

	GdsComVisualMesh();
	virtual ~GdsComVisualMesh();

	virtual const goc_id_type componentID() const	{ return COMPONENT_VISUAL_MESH; }

	virtual void	Make();
	virtual	void	Render( float fAccumtime );

};

#endif