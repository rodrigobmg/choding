#include "Precompiled.h"
#include "SunCoreBase.h"


Sun_CCoreBase::Sun_CCoreBase():m_strCoreName(NULL)
{
}

Sun_CCoreBase::~Sun_CCoreBase()
{
}

void Sun_CCoreBase::SetCoreName(const tstring& strCoreName)
{
	m_strCoreName = strCoreName;
}