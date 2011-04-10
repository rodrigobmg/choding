#ifndef _SUN_RESOURCE_TEXTURE_H_
#define _SUN_RESOURCE_TEXTURE_H_

class DLL_API sunTexture : public sunResource
{
	DeclareBPool


public:
	sunTexture();
	~sunTexture();

public:
	virtual bool Load(const tstring& strFileName);



public:
	LPDIRECT3DTEXTURE9	GetDX9Texture();
	LPDIRECT3DSURFACE9  GetDX9Surface(int iLevel);


protected:
	LPDIRECT3DTEXTURE9	m_pTexture;
	LPDIRECT3DSURFACE9  m_pSurface;

};




#endif