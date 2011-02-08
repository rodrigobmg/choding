#include "CoreBase.h"

CCoreBase::CCoreBase()
{
	SetName( OBJECT_BASE_CORE );
}

CCoreBase::~CCoreBase()
{

}

HRESULT CCoreBase::Create()
{
	return S_OK;
}

HRESULT CCoreBase::Release()
{
	return S_OK;
}

void CCoreBase::Clear()
{
	
}