#include "Precompiled.h"
#include "sunStream.h"


sunStream::sunStream()
{

}

sunStream::~sunStream()
{

}


void sunStream::SetState( eLodingState eState )
{
	m_eState = eState;
}


sunStream::eLodingState sunStream::GetState()
{
	return m_eState;
}


void sunStream::SetName( const tstring& strFileName )
{
	m_strName = strFileName;
}

const tstring& sunStream::GetName()
{
	return m_strName;
}

void sunStream::SetFileName( const tstring& strFileName )
{
	m_strFileName = strFileName;
}

const tstring& sunStream::GetFileName()
{
	return m_strFileName;
}


void sunStream::SetFullFileName( const tstring& strFileName )
{
	m_strFileName = strFileName;
}

const tstring& sunStream::GetFullFileName()
{
	return m_strFullPathFileName;
}


