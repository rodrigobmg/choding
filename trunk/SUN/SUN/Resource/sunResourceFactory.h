#ifndef _SUN_RESOURCE_RESOURCEFACTORY_H_
#define _SUN_RESOURCE_RESOURCEFACTORY_H_

class sunResource;


class DLL_API sunResourceFactory
{
public:
	typedef enum _eResourceGroup
	{
		RES_GROUP_MESH = 0,
		RES_GROUP_TEXTURE,
		MAX_GROUP_NUM,
	} eResourceGroup;

public:
	sunResourceFactory();
	virtual ~sunResourceFactory();

public:
	int		InitializeFactory();
	void	DestoryFactory();

	sunResourcePtr CreateResource (const tstring& strFileName, eResourceGroup eGroup);
	void		   RemoveResource( const tstring& strFileName, eResourceGroup eGroup);
	sunResourcePtr GetResource( const tstring& strFileName, eResourceGroup eGroup);

protected:
	typedef stdext::hash_map<tstring, sunResource*>					Resource_HASHMAP;
	typedef stdext::hash_map<tstring, sunResource*>::iterator		Resource_HASHMAP_IT;


	Resource_HASHMAP	m_ResourceGroup_Map[MAX_GROUP_NUM];

};



#endif