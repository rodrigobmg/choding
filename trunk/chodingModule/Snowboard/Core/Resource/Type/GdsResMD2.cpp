#include "GdsResMD2.h"
#include "../../../System/FileSystem/GdsFile.h"

ImplementBoostPool( GdsResMD2 )

GdsResMD2::GdsResMD2()
{
	SetName( OBJECT_RES_MD2 );
	Clear();
}

GdsResMD2::~GdsResMD2()
{
}

void GdsResMD2::Clear()
{
}

HRESULT GdsResMD2::Create()
{
	return S_OK;
}

HRESULT GdsResMD2::Release()
{	
	return S_OK;
}

HRESULT GdsResMD2::LoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
   	GdsFile file( path ); 
   
   	MD2HEADER pMD2Header;
	if ( file.Read( sizeof( MD2HEADER ) , &pMD2Header )  == false )
		return S_FALSE;
   
   	BYTE* pMD2Data = new BYTE[pMD2Header.offsetEnd];
	if( file.Read( sizeof(BYTE)*( pMD2Header.offsetEnd - sizeof(MD2HEADER) ) , &pMD2Data[sizeof(MD2HEADER)] ) == false ) 
		return S_FALSE;

	///////////////////�ﰢ�� �׸��� ���� ����/////////////////////////////////////
	m_uPrimitive = pMD2Header.numTris;

	///////////////////��Ʈ���� �б�(������, �̵���)///////////////////////////////

	MD2MATRIX *pMD2Matrix = new MD2MATRIX;
	ZeroMemory( pMD2Matrix, sizeof(MD2MATRIX) );
	memcpy( pMD2Matrix, &pMD2Data[pMD2Header.offsetFrames], sizeof(MD2MATRIX) );

	///////////////////���ؽ� �б�(x,z,y)//////////////////////////////////////////

	MD2VERTEX *pMD2Vertex = new MD2VERTEX[pMD2Header.numXYZ];
	ZeroMemory( pMD2Vertex, pMD2Header.numXYZ * sizeof(MD2VERTEX) );
	memcpy( pMD2Vertex, &pMD2Data[pMD2Header.offsetFrames + sizeof(MD2MATRIX)], pMD2Header.numXYZ * sizeof(MD2VERTEX) );

	///////////////////�ؽ���(tu,tv)�б�///////////////////////////////////////////

	MD2TEXTURE *pMD2Texture = new MD2TEXTURE[pMD2Header.numST];
	ZeroMemory( pMD2Texture, pMD2Header.numST * sizeof(MD2TEXTURE) );
	memcpy( pMD2Texture, &pMD2Data[pMD2Header.offsetST], pMD2Header.numST * sizeof(MD2TEXTURE) );

	///////////////////�ε���(���ؽ�[3],�ؽ���[3])/////////////////////////////////

	MD2INDICES *pMD2Index = new MD2INDICES[pMD2Header.numTris];
	ZeroMemory( pMD2Index, pMD2Header.numTris * sizeof(MD2INDICES) );
	memcpy( pMD2Index, &pMD2Data[pMD2Header.offsetTris], pMD2Header.numTris * sizeof(MD2INDICES) );

	///////////////////VB ����/////////////////////////////////////////////////////

	UINT Size = 3 * pMD2Header.numTris;
	MD2_VERTEX *Vertices = new MD2_VERTEX[Size];
	ZeroMemory( Vertices, Size * sizeof(MD2_VERTEX) );

	for( int i = 0; i < pMD2Header.numTris; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			Vertices[i*3+j].v = D3DXVECTOR3( //��ǥ = ��ǥ * ������ + �̵���
				pMD2Vertex[pMD2Index[i].vertex[j]].v[0] * pMD2Matrix->scale[0] + pMD2Matrix->translate[0],
				pMD2Vertex[pMD2Index[i].vertex[j]].v[2] * pMD2Matrix->scale[2] + pMD2Matrix->translate[2],
				pMD2Vertex[pMD2Index[i].vertex[j]].v[1] * pMD2Matrix->scale[1] + pMD2Matrix->translate[1] );

			D3DXVec3Scale( &Vertices[i*3+j].v, &Vertices[i*3+j].v, pMD2Matrix->scale[0] );

			Vertices[i*3+j].t = D3DXVECTOR2( 
				pMD2Texture[pMD2Index[i].texture[j]].tu / (float)pMD2Header.skinwidth,
				pMD2Texture[pMD2Index[i].texture[j]].tv / (float)pMD2Header.skinheight );
		}
	}

	///////////////////�޸� ����//////////////////////////////////////////////////////
	delete []pMD2Data;		//�����Ҵ�
	delete pMD2Matrix;		//��Ʈ���� 40����Ʈ
	delete []pMD2Vertex;	//�����Ҵ�
	delete []pMD2Texture;	//�����Ҵ�
	delete []pMD2Index;		//�����Ҵ�
	delete []Vertices;		//�����Ҵ�

	return S_OK;
}