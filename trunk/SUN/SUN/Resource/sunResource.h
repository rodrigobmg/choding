#ifndef _SUN_RESOURCE__RESOURCE_H_
#define _SUN_RESOURCE__RESOURCE_H_


class DLL_API sunResource : public sunStream
{
public:
	sunResource();
	virtual ~sunResource();

public:
	typedef sunResourceFactory::eResourceGroup eResourceGroup;

public:
	virtual bool Load(const tstring& strFileName);
	virtual void SetResourceGroup(eResourceGroup eGroup); 
	virtual eResourceGroup GetResourceGroup();
 

public:
	void SetResourceFactory( sunResourceFactory* pResourceFactory);

protected:
	eResourceGroup			m_eGroup;
	sunResourceFactory*		m_pResourceFactory;

};


#endif