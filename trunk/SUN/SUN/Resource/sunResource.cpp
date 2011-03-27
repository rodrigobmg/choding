#include "Precompiled.h"
#include "sunResource.h"

#include "Precompiled.h"
#include "sunResource.h"


sunResource::sunResource()
{
	m_eGroup = sunResourceFactory::MAX_GROUP_NUM;
	m_pResourceFactory = NULL;
}

sunResource::~sunResource()
{
	
	assert(m_pResourceFactory);
	
	m_pResourceFactory->RemoveResource(m_strFileName, m_eGroup);
}

bool sunResource::Load()
{
	return true;
}

void sunResource::SetResourceGroup( eResourceGroup eGroup )
{
	m_eGroup = eGroup;
}

sunResource::eResourceGroup sunResource::GetResourceGroup()
{
	return m_eGroup;
}

void sunResource::SetResourceFactory( sunResourceFactory* pResourceFactory )
{
	assert(pResourceFactory);
	m_pResourceFactory = pResourceFactory;
}


