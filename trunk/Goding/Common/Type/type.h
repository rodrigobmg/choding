#include <tchar.h>
#include <String>


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

#define MAT44				D3DXMATRIX						
#define Vec3				D3DXVECTOR3						
#define Vec2				D3DXVECTOR2

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete p; p = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { p->Release(); p = NULL; } }
#endif 