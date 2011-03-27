#ifndef _TESTAPP_OBJECT_3DGRID_H_
#define _TESTAPP_OBJECT_3DGRID_H_


class Grid3D : public sunRenderNode
{
	DeclareRTTI

public:
	typedef struct _stGridVtx
	{
		D3DXVECTOR3	vPos;
		DWORD		dwColor;

		_stGridVtx() { 
			vPos.x = vPos.y = vPos.z= 0.f;
			dwColor=0xffffff; 
		}

		_stGridVtx( float fX, float fY, float fZ, DWORD dwCol ) {
			vPos.x = fX; 
			vPos.y = fY; 
			vPos.z = fZ; 
			dwColor = dwCol;
		}

		enum {
			FVF = (D3DFVF_XYZ|D3DFVF_DIFFUSE),
		};
	} stGridVtx;

public:
	Grid3D();
	virtual ~Grid3D();

	void Destory ();

public:
	virtual void GetRenderOperation( sunRenderOperation& op );

public:
	void CreateNewGrid( void );
	void SetGridSize( int iSize );

protected:
	sunVertexData*		m_pVertData;
	int					m_iGridSize;


};


#endif