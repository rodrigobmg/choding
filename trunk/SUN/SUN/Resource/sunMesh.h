#ifndef _SUN_RESOURCE_MESH_H_
#define _SUN_RESOURCE_MESH_H_


//정점 정보 구조체
typedef struct _stVertex
{
	D3DXVECTOR3 vPosition;			// 정점위치
	D3DXVECTOR3 vNormal;			// 법선벡터
	DWORD		dwColor;			// 색깔.

//	D3DXVECTOR3 vTangent;			// 접선벡터
//	D3DXVECTOR3 vBinormal;			// 종선벡터
//	D3DXVECTOR2 vUV;				// UVMap

//	vector<float>	Weight;			// 가중치
//	vector<float>	BoneID;			// 정점에 영향을 주는 BoneID

	_stVertex()
	{
		vPosition	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vNormal		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		dwColor		= D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
//		vUV			= D3DXVECTOR2(-1.0f, -1.0f);
	}


	~_stVertex()
	{
//		Weight.clear();
//		BoneID.clear();
	}

} stVertex;

//인덱스 정보 구조체

typedef struct _stIndex
{
	WORD	wIndex[3];
} stIndex;


// 메시 자원 클래스
class DLL_API sunMesh : public sunResource
{	
	DeclareBPool

public:
	sunMesh();
	virtual ~sunMesh();

public:
	virtual bool Load(const tstring& strFileName);

public:
	inline LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclaration();


	sunVertexData* GetVertexData();
	sunIndexData*  GetindexData();

public:
	int	 CreateVIB();

	void SetVertexInfo(vector<stVector3f>* vecVertex, vector<stVector3f>* vecVertexNormal);
	void SetIndexInfo(vector<stVector3i>* vecIndex);


protected:
	sunVertexData*		m_pVertexData;
	sunIndexData*		m_pIndexData;

	vector<stVertex>	m_vecVertex;
	vector<stIndex>		m_vecIndex;


};


#endif
