#ifndef _IMESH_H_
#define _IMESH_H_

#include "..\IRenderObject.h"

class IMesh : public IRenderObject
{
public:
	IMesh();
	virtual ~IMesh();

	virtual void Draw( D3DDevice* pDevice , D3DXEffect* pEffect );
	virtual void onDeviceReset();
	virtual void onDeviceLost();

		//TempVertex Buffer
	IDirect3DVertexBuffer9* vertexBuffer;

	//Index Buffer
	IDirect3DIndexBuffer9* indexBuffer;	

};

#endif