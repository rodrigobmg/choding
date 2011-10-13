#include "Precompiled.h"
#include "sunRTTI.h"


sunRTTI::sunRTTI( const tstring strClassName, const sunRTTI* pBaseRTTI ):
m_strClassName( strClassName),
m_pBaseRTTI( pBaseRTTI)
{

}

const tstring& sunRTTI::GetClassName() const
{
	return m_strClassName;
}

const sunRTTI* sunRTTI::GetBaseRTTI() const
{
	return m_pBaseRTTI;
}