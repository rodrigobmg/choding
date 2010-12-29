#pragma once 

#include <cassert>
#include "CObject.h"

CObject::CObject()
{
	this->Initialize();
	this->IncRefCount();
}

CObject::~CObject()
{
	this->DecRefCount();
	if ( m_iRefCount != 0 )
		assert( "������� 0�̾���� " );
}

void CObject::Initialize()
{
	m_iRefCount = 0;
}

void CObject::IncRefCount()
{
	++m_iRefCount;
}

void CObject::DecRefCount()
{
	--m_iRefCount;
	if ( m_iRefCount < 0 )
		assert( "�� 0���� ����?" );
}