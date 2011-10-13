#ifndef _SNOWBOARD_COMPONENT_H_
#define _SNOWBOARD_COMPONENT_H_

class GdsGameObject;

class GdsComponent : public GdsMemObject
{
public:
	typedef int goc_id_type;
	GdsComponent();
	virtual ~GdsComponent() = 0 {}

	virtual void	Make() = 0;
	virtual const goc_id_type componentID() const = 0;
	virtual const goc_id_type familyID() const = 0;

	void				SetOwnerGO( GdsGameObject* go ){ m_pGameObject = go; }
	GdsGameObject*		GetOwnerGO() const { return m_pGameObject; }

private:

	GdsGameObject*		m_pGameObject;
};


#endif