#include "Precompiled.h"
#include "sunObject.h"

ImplementRootRTTI( sunObject );

sunObject::sunObject()
{
	m_strName = L"None";
}

sunObject::~sunObject()
{
}

void sunObject::Destory()
{
	
}

//void sunObject::SetName( const tstring& strName)
//{
//	m_strName = strName;
//}
//
//const tstring& sunObject::GetName()
//{
//	return m_strName;
//}
//
//bool sunObject::IsName( const tstring& strName)
//{
//	return ( m_strName.compare( strName ) == 0 );
//}
//
sunObject* sunObject::CloneMe()
{
	return NULL;
}