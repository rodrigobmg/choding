#include "stdafx.h"

//ImplementBoostpool(GdsGameObject)

GdsGameObject::GdsGameObject()
:m_ID( GAMEOBJECT_DUMMY )
,m_pParentObject(NULL)
{
	D3DXMatrixIdentity( &m_mat );
}

GdsGameObject::GdsGameObject( const go_id_type id )
:m_ID(id)
,m_pParentObject(NULL)
{
	D3DXMatrixIdentity( &m_mat );
}

GdsGameObject::~GdsGameObject()
{
	for_each( m_Components.begin() , m_Components.end() , functor::deleter() );
	m_Components.clear();

	//for_each( m_ChildObject.begin() , m_ChildObject.end() , functor::deleter() );
	m_ChildObject.clear();
}

void GdsGameObject::Init( const go_id_type id )
{
	SetID( id );
}

void GdsGameObject::SetComponent( GdsComponent* newGOC )
{
	newGOC->SetOwnerGO( this );
	component_table_t::iterator it = m_Components.find( newGOC->familyID() );
	if ( it != m_Components.end() )
	{
		//SAFE_DELETE( it->second );
		m_Components.erase( it );
	}

	m_Components.insert( component_pair_t( newGOC->familyID() , newGOC ) );
}

GdsComponent* GdsGameObject::GetComponent( const GdsComponent::goc_id_type familyID )
{
	component_table_t::iterator it = m_Components.find( familyID );
	if ( it != m_Components.end() )
		return it->second;

	return NULL;
}

GdsGameObject* GdsGameObject::GetParent() const
{
	return m_pParentObject;
}

void GdsGameObject::SetParnet( GdsGameObject* const pObject )
{
	m_pParentObject = pObject;
}

void GdsGameObject::AttchChild( GdsGameObjectPtr const pChild )
{
D_BEGIN
	gameobject_table_t::iterator it = m_ChildObject.begin();
	for ( ; it != m_ChildObject.end() ; ++it )
	{
		if ( *it == pChild )
		{
			ASSERT( !"이미 같은 자식을 가지고 있다.");
			break;
		}
	}
/**/
	pChild->SetParnet( this );
	m_ChildObject.push_back( pChild );
}

void GdsGameObject::DetachChild( GdsGameObjectPtr pChild )
{
	gameobject_table_t::iterator it = m_ChildObject.begin();
	for ( ; it != m_ChildObject.end() ; ++it )
	{
		if ( *it == pChild )
		{
			m_ChildObject.erase( it );	
			break;
		}
	}
}

GdsGameObjectPtr GdsGameObject::GetChild( const go_id_type id )
{
	gameobject_table_t::iterator it = m_ChildObject.begin();
	for ( ; it != m_ChildObject.end() ; ++it )
	{
		if ( (*it)->GetID() == id )
		{
			return *it;
		}
	}
	return GdsGameObjectPtr( (GdsGameObject*)NULL );
}

void GdsGameObject::Update( const float fAccumtime )
{
// 	component_table_t::iterator it = m_Components.begin();
// 	for( ; it != m_Components.end() ; ++it )
// 	{
// 		it->second->Update( fAccumtime );
// 	}

	component_table_t::iterator it = m_Components.begin();
	for( ; it != m_Components.end() ; ++it )
	{
		if ( it->second->familyID() == COMPONENT_VISUAL_FAMILY )
		{
			GdsComVisual* pVisual = static_cast< GdsComVisual* >( it->second );
			pVisual->Render( fAccumtime );
		}
		//it->second->Update( fAccumtime );
	}	

	gameobject_table_t::iterator object_it = m_ChildObject.begin();
	for ( ; object_it != m_ChildObject.end() ; ++object_it )
	{
		(*object_it)->Update( fAccumtime );
	}
}