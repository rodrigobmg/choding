#include "ResTexture.h"

CResTexture::CResTexture()
{
	SetName( OBJECT_RES_TEXTURE );
	Clear();
}

CResTexture::~CResTexture()
{

}

void CResTexture::Clear()
{
	m_pTexture = NULL;
}

void CResTexture::Destroy()
{
	SAFE_DELETE(m_pTexture);
}

void CResTexture::Create()
{

}