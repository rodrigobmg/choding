#include "Precompiled.h"
#include "sunResourceFactory.h"


sunResourceFactory::sunResourceFactory()
{
}

sunResourceFactory::~sunResourceFactory()
{
	DestoryFactory();
}

int sunResourceFactory::InitializeFactory()
{
	return 1;
}

void sunResourceFactory::DestoryFactory()
{
	for(int i =0; i < MAX_GROUP_NUM; ++i)
		m_ResourceGroup_Map[i].clear();
}

sunResourcePtr sunResourceFactory::CreateResource(const tstring& strFileName, eResourceGroup eGroup)
{
	assert( eGroup < MAX_GROUP_NUM );

	sunResourcePtr spResource = GetResource(strFileName, eGroup);
	if(NULL != spResource)
		return spResource;

	//switch(eGroup)
	//{
	//case RES_GROUP_MESH: {spResource = new 
	//}
	//
	

	assert( spResource );
	
	spResource->SetFileName(strFileName);
	spResource->SetName(strFileName);
	spResource->SetResourceGroup(eGroup);
	m_ResourceGroup_Map[eGroup].insert(make_pair(strFileName, spResource));

	if(spResource->Load())
		return spResource;

	return NULL;

}


void sunResourceFactory::RemoveResource( const tstring& strFileName, eResourceGroup eGroup)
{
	assert(eGroup < MAX_GROUP_NUM);

	Resource_HASHMAP_IT it = m_ResourceGroup_Map[eGroup].find(strFileName);
	if(it != m_ResourceGroup_Map[eGroup].end())
		m_ResourceGroup_Map[eGroup].erase(it);

}


sunResourcePtr sunResourceFactory::GetResource( const tstring& strFileName, eResourceGroup eGroup)
{
	assert(eGroup < MAX_GROUP_NUM);

	Resource_HASHMAP_IT it = m_ResourceGroup_Map[eGroup].find(strFileName);
	if(it != m_ResourceGroup_Map[eGroup].end())
		return NULL;


	return (sunResourcePtr)it->second;

}