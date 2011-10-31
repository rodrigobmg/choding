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


#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete p; p = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = NULL; } }
#endif 