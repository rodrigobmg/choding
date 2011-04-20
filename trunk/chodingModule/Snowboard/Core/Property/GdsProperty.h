#ifndef _GDS_CORE_PROPERTY_H_
#define _GDS_CORE_PROPERTY_H_

#include "../Base/GdsObject.h"
#include "Animation/GdsAnimation.h"
#include "Material/GdsMaterial.h"
#include "Mesh/GdsMesh.h"

class GdsProperty : public GdsObject
{
	GdsAnimationPtr		m_Animation;
	GdsMaterialPtr		m_Material;
	GdsMeshPtr			m_Mesh;

public:
	GdsProperty(void);
	virtual ~GdsProperty(void);

	void				Clear(){ vClear(); }
	void				Update( float fElapsedTime ){ vUpdate( fElapsedTime ); }
	void				Render( LPDIRECT3DDEVICE9 device ){ vRender( device ); }

	GdsAnimationPtr		GetAnimation();
	GdsMaterialPtr		GetMaterial();
	GdsMeshPtr			GetMesh();

protected:
	
	virtual	void		vClear(){};
	virtual	void		vUpdate( float fElapsedTime );
	virtual void		vRender( LPDIRECT3DDEVICE9 device );

};

typedef boost::shared_ptr< GdsProperty > GdsPropertyPtr;

#endif