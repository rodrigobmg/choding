#ifndef _SNOWBOARD_GAMEOBJECT_H_
#define _SNOWBOARD_GAMEOBJECT_H_

typedef	int go_id_type;

class GdsGameObject : public GdsMemObject
{
public:
	typedef	boost::shared_ptr< GdsGameObject >	GdsGameObjectPtr;

	GdsGameObject();
	explicit GdsGameObject( const go_id_type id );
	virtual ~GdsGameObject();

	void				Init( const go_id_type id );
	const go_id_type	GetID() const { return m_ID; }
	void				SetID( const go_id_type id ){m_ID = id;}

	GdsComponent*		GetComponent( const GdsComponent::goc_id_type familyID );
	void				SetComponent( GdsComponent* newGOC );

	GdsGameObject*		GetParent() const;
	void				SetParnet( GdsGameObject* const pObject );

	void				AttchChild( GdsGameObjectPtr const pChild );
	void				DetachChild( GdsGameObjectPtr pChild );
	GdsGameObjectPtr	GetChild( const go_id_type id );

	void				Update( const float fAccumtime );

	D3DXMATRIX&			GetMatrix(){ return m_mat; }
	void				SetMatrix( const D3DXMATRIX& mat ){ m_mat = mat; }

private:

	go_id_type		m_ID;

	typedef pair< const GdsComponent::goc_id_type , GdsComponent* > component_pair_t;
	typedef std::map< const GdsComponent::goc_id_type , GdsComponent* >
		component_table_t;
	component_table_t	m_Components;

	typedef std::vector< GdsGameObjectPtr > gameobject_table_t;
	gameobject_table_t	m_ChildObject;

	GdsGameObject*		m_pParentObject;
	D3DXMATRIX			m_mat;

	//DeclearBoostpool
};

typedef	boost::shared_ptr< GdsGameObject >	GdsGameObjectPtr;

#endif