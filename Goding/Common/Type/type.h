#include <tchar.h>
#include <String>
#include <d3dx9math.h>

typedef std::basic_string<TCHAR> tstring;

typedef char				int8;
typedef short				int16;
typedef int					int32;
typedef __int64				int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned __int64	uint64;

typedef LPDIRECT3DDEVICE9		LPDEVICE;
typedef D3DXMATRIX				MAT44;
typedef D3DLIGHT9				LIGHT;
typedef D3DMATERIAL9			MATERIAL;	
typedef D3DPRIMITIVETYPE		PRIMITIVETYPE;
typedef LPDIRECT3DTEXTURE9		LPTEXTURE;
typedef LPDIRECT3DVERTEXBUFFER9	LPVERTEXBUFFER;
typedef LPDIRECT3DINDEXBUFFER9	LPINDEXBUFFER;

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
#define D3DViewPort                      D3DVIEWPORT9
#define D3DFormat                        D3DFORMAT