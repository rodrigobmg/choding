#ifndef _SUN_OBJECT_NODE_RENDERNODE_H_
#define _SUN_OBJECT_NODE_RENDERNODE_H_



class sunRenderOperation;

class DLL_API sunRenderNode : public sunNode
{	

	DeclareRTTI

public:
	sunRenderNode();
	virtual ~sunRenderNode();


public:
	virtual void BegineUpdate();
	virtual void Update();
	virtual void EndUpdate();

	virtual void GetRenderOperation(sunRenderOperation& op);

public:
	void SetMesh( sunMeshPtr spMesh );
	sunMeshPtr GetMesh( void );


public:

	void SetSelect( bool bFlag );
	bool IsSelected();


	void SetVisible( bool bFlag );
	void SetVisibleToChild( sunNodePtr spNode, bool bFlag);

public:
	bool GetVisible();
	

protected:
	bool		m_bVisible;
	bool		m_bSelected;

	sunMeshPtr  m_spMesh;

};


// ���ؽ� ���� Ŭ����
class DLL_API sunVertexData
{
	DeclareBPool

public:
	sunVertexData(): m_pVertexDeclaration(NULL), m_pVB(NULL), m_iVertexStart(0), m_iVertexCount(0),
		m_iVertexSize(0){}

	~sunVertexData()
	{
		Safe_Release(m_pVertexDeclaration);
		Safe_Release(m_pVB);
	}

public:
	LPDIRECT3DVERTEXDECLARATION9 m_pVertexDeclaration;
	LPDIRECT3DVERTEXBUFFER9		 m_pVB;

	UINT	m_iVertexStart;
	UINT	m_iVertexCount;
	UINT	m_iVertexSize;

};


// �ε��� ���� Ŭ����
class DLL_API sunIndexData
{
	DeclareBPool

public:
	sunIndexData() : m_pIB(NULL), m_iIndexStart(0), m_iIndexCount(0){}
	~sunIndexData()
	{
		Safe_Release( m_pIB );
	}

	void Destory()
	{
		
	}

public:
	LPDIRECT3DINDEXBUFFER9 m_pIB;

	UINT	m_iIndexStart;
	UINT	m_iIndexCount;

};


class DLL_API sunRenderOperation
{
public:
	sunRenderOperation();
	~sunRenderOperation();

public:
	/* Index�� ����� ��� PrimCount�� �ε��� ������ �����Ѵ�.
	�� ���� ���
	D3DPT_POINTLIST		- ���� ������ ����
	D3DPT_LINELIST		- ���� ���� / 2
	D3DPT_LINESTRIP		- ���� ���� - 1
	D3DPT_TRIANGLELIST	- ���� ���� / 3
	D3DPT_TRIANGLESTRIP	- ���� ���� - 2
	D3DPT_TRIANGLEFAN	- ���� ���� - 2
	*/

	D3DPRIMITIVETYPE	m_PrimType;
	DWORD				m_dwPrimCount;

	
	sunRenderNodePtr	m_spRenderNode;

	bool				m_bUseIndexes;
	sunIndexData*		m_pIndexData;
	sunVertexData*		m_pVertexData;

	bool				m_bUseTexture;
	LPDIRECT3DTEXTURE9	m_pTexData[5];


	list< sunRenderOperation > m_RenderOpList;
	//vector< D3DXMATRIX >	   m_BoneList_Vector;
};




#endif