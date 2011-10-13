#include "stdafx.h"

GdsComponentMaker::GdsComponentMaker()
{
	
}

GdsComponentMaker::~GdsComponentMaker()
{

}

GdsComponent*	GdsComponentMaker::MakeComponent( GdsComponent::goc_id_type id )
{
	switch( id )
	{
	case COMPONENT_VISUAL_RECT:
		{
			GdsComVisualRect* pCom = new GdsComVisualRect;
			pCom->Make();
			return pCom;
		}
		break;

	case COMPONENT_ANIMATION_MOVEABLE:
		{
			GdsComMoveable* pCom = new GdsComMoveable;
			pCom->Make();
			return pCom;
		}
		break;

	default:
		break;
	}

	return NULL;
}