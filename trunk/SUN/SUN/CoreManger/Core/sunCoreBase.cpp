#include "Precompiled.h"
#include "sunCoreBase.h"



sunCoreBase::sunCoreBase():
m_iDestroyOrder(0)
{
	
}

sunCoreBase::~sunCoreBase()
{

}

void sunCoreBase::SetDestroyOrder(int iOrder)
{
	m_iDestroyOrder = iOrder;
}


int sunCoreBase::GetDestoryOrder()
{
	return m_iDestroyOrder;
}


void sunCoreBase::SetCoreName(const tstring& strCoreName)
{
	m_strCoreName = strCoreName;
}

const tstring& sunCoreBase::GetCoreName()
{
	return m_strCoreName;
}