#include "d3d9.h"

#include "../Common/Type/type.h"

#include <Windows.h>


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