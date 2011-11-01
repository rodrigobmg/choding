#pragma once 

#include <tchar.h>
#include <string>

#include "..\..\Middleware\dx_sdk\Include\d3dx9math.h"

typedef std::basic_string<TCHAR> tstring;


typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef __int64				int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64	uint64;

typedef D3DXMATRIX		Matrix44;					
typedef D3DXVECTOR4		Vec4;
typedef D3DXVECTOR3		Vec3;				
typedef D3DXVECTOR2		Vec2;
typedef D3DXQUATERNION	Quat;

class Vertex
{
public:
	//Components of a Vertex
	Vec3 Position;
	Vec3 Normal;
	Vec2 UV;
	Vec3 Tangent;
	Vec3 BiTangent;

	//Constructor
	Vertex(Vec3 Position, Vec3 Normal, Vec2 UV, Vec3 Tangent, Vec3 BiTangent)
	{
		this->Position = Position;
		this->Normal = Normal;
		this->UV = UV;
		this->Tangent = Tangent;
		this->BiTangent = BiTangent;
	}
};

#define LPDEVICE						LPDIRECT3DDEVICE9				
#define LIGHT							D3DLIGHT9						
#define MATERIAL						D3DMATERIAL9					
#define PRIMITIVETYPE					D3DPRIMITIVETYPE				
#define LPTEXTURE						LPDIRECT3DTEXTURE9				
#define LPVERTEXBUFFER					LPDIRECT3DVERTEXBUFFER9			
#define LPINDEXBUFFER					LPDIRECT3DINDEXBUFFER9			


#define Direct3D                         IDirect3D9
#define D3DDevice                        IDirect3DDevice9
#define D3DDeviceContext                 IDirect3DDevice9
#define D3DVertexDeclaration             IDirect3DVertexDeclaration9
#define D3DVertexShader                  IDirect3DVertexShader9
#define D3DPixelShader                   IDirect3DPixelShader9
#define D3DBaseTexture                   IDirect3DBaseTexture9
#define D3DTexture                       IDirect3DTexture9
#define D3DVolumeTexture                 IDirect3DVolumeTexture9
#define D3DCubeTexture                   IDirect3DCubeTexture9
#define D3DVertexBuffer                  IDirect3DVertexBuffer9
#define D3DIndexBuffer                   IDirect3DIndexBuffer9
#define D3DSurface                       IDirect3DSurface9
#define D3DDepthSurface                  IDirect3DSurface9
#define D3DQuery                         IDirect3DQuery9
#define D3DXEffect						 ID3DXEffect
#define D3DViewPort                      D3DVIEWPORT9
#define D3DFormat                        D3DFORMAT


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete p; p = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = NULL; } }
#endif 