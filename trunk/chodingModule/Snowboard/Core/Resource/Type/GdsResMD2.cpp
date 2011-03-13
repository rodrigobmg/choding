#include "GdsResMD2.h"
#include "../../../System/FileSystem/GdsFile.h"

ImplementBoostPool( GdsResMD2 )

GdsResMD2::GdsResMD2()
{
	SetName( OBJECT_RES_MD2 );
	m_dFVF = D3DFVF_XYZ|/*D3DFVF_DIFFUSE|*/D3DFVF_TEX1;
	vClear();
}

GdsResMD2::~GdsResMD2()
{
}

void GdsResMD2::vClear()
{
	m_pVB = NULL;
	m_pTexture = NULL;
}

HRESULT GdsResMD2::vCreate()
{
	if ( m_pTexture != NULL || m_pVB != NULL )
	{
		assert( 0 );
		return false;
	}
	return true;
}

HRESULT GdsResMD2::vRelease()
{	
	if ( m_pTexture )
		m_pTexture->Release();
	if ( m_pVB )
		m_pVB->Release();

	return true;
}

HRESULT GdsResMD2::vLoadResource( const TCHAR* path , LPDIRECT3DDEVICE9 device )
{
	if ( device == NULL )
	{
		assert( 0 );
		return false;
	}

	m_strPath = path;

	size_t poscomma = m_strPath.rfind( L"\\" );
	tstring texturefilepath	= m_strPath.substr( 0 , poscomma );
	texturefilepath += L"\\skin.jpg";

	D3DXCreateTextureFromFile( device , texturefilepath.c_str() , &m_pTexture );


   	GdsFile file( path ); 
   
   	MD2HEADER pMD2Header;
	if ( file.Read( sizeof( MD2HEADER ) , &pMD2Header )  == false )
		return false;
   
   	BYTE* pMD2Data = new BYTE[pMD2Header.offsetEnd];
	if( file.Read( sizeof(BYTE)*( pMD2Header.offsetEnd - sizeof(MD2HEADER) ) , &pMD2Data[sizeof(MD2HEADER)] ) == false ) 
		return false;

	///////////////////삼각형 그리기 개수 설정/////////////////////////////////////
	m_uPrimitive = pMD2Header.numTris;

	///////////////////매트릭스 읽기(스케일, 이동값)///////////////////////////////

	MD2MATRIX *pMD2Matrix = new MD2MATRIX;
	ZeroMemory( pMD2Matrix, sizeof(MD2MATRIX) );
	memcpy( pMD2Matrix, &pMD2Data[pMD2Header.offsetFrames], sizeof(MD2MATRIX) );

	///////////////////버텍스 읽기(x,z,y)//////////////////////////////////////////

	MD2VERTEX *pMD2Vertex = new MD2VERTEX[pMD2Header.numXYZ];
	ZeroMemory( pMD2Vertex, pMD2Header.numXYZ * sizeof(MD2VERTEX) );
	memcpy( pMD2Vertex, &pMD2Data[pMD2Header.offsetFrames + sizeof(MD2MATRIX)], pMD2Header.numXYZ * sizeof(MD2VERTEX) );

	///////////////////텍스쳐(tu,tv)읽기///////////////////////////////////////////

	MD2TEXTURE *pMD2Texture = new MD2TEXTURE[pMD2Header.numST];
	ZeroMemory( pMD2Texture, pMD2Header.numST * sizeof(MD2TEXTURE) );
	memcpy( pMD2Texture, &pMD2Data[pMD2Header.offsetST], pMD2Header.numST * sizeof(MD2TEXTURE) );

	///////////////////인덱스(버텍스[3],텍스쳐[3])/////////////////////////////////

	MD2INDICES *pMD2Index = new MD2INDICES[pMD2Header.numTris];
	ZeroMemory( pMD2Index, pMD2Header.numTris * sizeof(MD2INDICES) );
	memcpy( pMD2Index, &pMD2Data[pMD2Header.offsetTris], pMD2Header.numTris * sizeof(MD2INDICES) );

	///////////////////VB 셋팅/////////////////////////////////////////////////////

	UINT Size = 3 * pMD2Header.numTris;
	MD2_VERTEX *Vertices = new MD2_VERTEX[Size];
	ZeroMemory( Vertices, Size * sizeof(MD2_VERTEX) );

	for( int i = 0; i < pMD2Header.numTris; i++ )
	{
		for( int j = 0; j < 3; j++ )
		{
			Vertices[i*3+j].v = D3DXVECTOR3( //좌표 = 좌표 * 스케일 + 이동값
				pMD2Vertex[pMD2Index[i].vertex[j]].v[0] * pMD2Matrix->scale[0] + pMD2Matrix->translate[0],
				pMD2Vertex[pMD2Index[i].vertex[j]].v[2] * pMD2Matrix->scale[2] + pMD2Matrix->translate[2],
				pMD2Vertex[pMD2Index[i].vertex[j]].v[1] * pMD2Matrix->scale[1] + pMD2Matrix->translate[1] );

			D3DXVec3Scale( &Vertices[i*3+j].v, &Vertices[i*3+j].v, pMD2Matrix->scale[0] );

			Vertices[i*3+j].t = D3DXVECTOR2( 
				pMD2Texture[pMD2Index[i].texture[j]].tu / (float)pMD2Header.skinwidth,
				pMD2Texture[pMD2Index[i].texture[j]].tv / (float)pMD2Header.skinheight );
		}
	}

 	if( FAILED( device->CreateVertexBuffer( Size * sizeof(MD2_VERTEX), 
											0, 
											m_dFVF , 
											D3DPOOL_DEFAULT, 
											&m_pVB, 
											NULL ) ) 
											)
 	{
		return E_FAIL; 
	}
 
 	void *pVertices;
 	if( FAILED( m_pVB->Lock( 0, Size * sizeof(MD2_VERTEX), (void**)&pVertices, 0 ) ) )
 	{
		return E_FAIL; 
	}

 	memcpy( pVertices, Vertices, Size * sizeof(MD2_VERTEX) );
 	m_pVB->Unlock();

	///////////////////메모리 해제//////////////////////////////////////////////////////
	delete []pMD2Data;		//동적할당
	delete pMD2Matrix;		//매트릭스 40바이트
	delete []pMD2Vertex;	//동적할당
	delete []pMD2Texture;	//동적할당
	delete []pMD2Index;		//동적할당
	delete []Vertices;		//동적할당

	return true;
}

HRESULT GdsResMD2::vReCreate( LPDIRECT3DDEVICE9 device )
{
	vRelease();
	return vLoadResource( m_strPath.c_str() , device );
}

void GdsResMD2::SetVB( LPDIRECT3DVERTEXBUFFER9 vb )
{
	if ( m_pVB )
		m_pVB->Release();

	m_pVB = vb;
}

LPDIRECT3DVERTEXBUFFER9 GdsResMD2::GetVB()
{
	return m_pVB;
}

void GdsResMD2::SetFVF( DWORD flag )
{
	m_dFVF = flag;
}

DWORD GdsResMD2::GetFVF()
{
	return m_dFVF;
}

void GdsResMD2::SetTexture( LPDIRECT3DTEXTURE9 texture )
{
	if ( m_pTexture )
		m_pTexture->Release();

	m_pTexture = texture;
}

LPDIRECT3DTEXTURE9 GdsResMD2::GetTexture()
{
	return m_pTexture;
}

void GdsResMD2::SetPrimitive( UINT uPrimitive )
{
	m_uPrimitive = uPrimitive;
}

UINT GdsResMD2::GetPrimitive()
{
	return m_uPrimitive;
}	
