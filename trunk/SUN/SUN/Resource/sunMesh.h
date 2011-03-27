#ifndef _SUN_RESOURCE_MESH_H_
#define _SUN_RESOURCE_MESH_H_


//���� ���� ����ü
typedef struct _stVertex
{
	D3DXVECTOR3 vPosition;			// ������ġ
	D3DXVECTOR3 vNormal;			// ��������
	D3DXVECTOR3 vTangent;			// ��������
	D3DXVECTOR3 vBinormal;			// ��������
	D3DXVECTOR2 vUV;				// UVMap

	vector<float>	Weight;			// ����ġ
	vector<float>	BoneID;			// ������ ������ �ִ� BoneID

	_stVertex()
	{
		vPosition	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vNormal		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vUV			= D3DXVECTOR2(-1.0f, -1.0f);
	}


	~_stVertex()
	{
		Weight.clear();
		BoneID.clear();
	}

} stVertex;

//�ε��� ���� ����ü

typedef struct _stIndex
{
	WORD	wIndex[3];
} stIndex;


// �޽� �ڿ� Ŭ����
class DLL_API sunMesh : public sunResource
{	
	DeclareBPool

public:
	sunMesh();
	virtual ~sunMesh();

public:
	virtual bool Load();

public:
	inline LPDIRECT3DVERTEXDECLARATION9 GetVertexDeclaration();


	inline sunVertexData* GetVertexData();
	inline sunIndexData*  GetindexData();

public:
	int	 CreateVIB();

protected:
	sunVertexData*		m_pVertexData;
	sunIndexData*		m_pIndexData;

	vector<stVertex>	m_vecVertex;
	vector<stIndex>		m_vecIndex;


};


#endif
