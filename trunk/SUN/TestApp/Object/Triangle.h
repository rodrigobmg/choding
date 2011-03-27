#ifndef _TESTAPP_OBJECT_TRIANGLE_H_
#define _TESTAPP_OBJECT_TRIANGLE_H_


class Triangle : public sunRenderNode
{
	DeclareRTTI

public:
	typedef struct _stTriangleVtx
	{
		D3DXVECTOR3	vPos;
		DWORD		dwColor;

		_stTriangleVtx() { 
			vPos.x = vPos.y = vPos.z = 0.0f;
			dwColor=0xffffff; 
		}

		_stTriangleVtx( float fX, float fY, float fZ, DWORD dwCol ) {
			vPos.x = fX; 
			vPos.y = fY; 
			vPos.z = fZ; 
			dwColor = dwCol;
		}

		enum {
			FVF = (D3DFVF_XYZ | D3DFVF_DIFFUSE),
		};
	} stTriangleVtx;

public:
	Triangle();
	virtual ~Triangle();

	void Destory();

	
public:
	virtual void GetRenderOperation(sunRenderOperation& op);

public:
	HRESULT CreateTriangle();


protected:
	sunVertexData*	m_pVertData;
	

};



#endif