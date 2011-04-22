#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"
#include "../../../System/Logger/logger.h"
#include "../../../System/FrameMemory/GdsFrameMemory.h"
#include "Renderer/GdsRendererDX9.h"
#include "../GdsResMgr.h"

GdsResASE::GdsResASE():
m_VertexList(NULL),
m_FaceList(NULL),
m_TVertex(NULL)
{
	SetName( OBJECT_RES_ASE );
	vClear();
	m_RootNode = GdsNodePtr( (GdsNode*)NULL );
}

GdsResASE::~GdsResASE()
{
	m_vecNodeList.clear();
	m_vecMaterialList.clear();
	SAFE_DELETE_ARRAY( m_VertexList );
	SAFE_DELETE_ARRAY( m_FaceList );
	SAFE_DELETE_ARRAY( m_TVertex);
}

void GdsResASE::vClear()
{
}

HRESULT GdsResASE::vRelease()
{	
	return true;
}

HRESULT GdsResASE::vLoadResource( LPDIRECT3DDEVICE9 device )
{
	LineContainerA	lines( m_strPath );
	LineContainerA::iterator it = lines.begin();
	LineContainerA::iterator it_end = lines.end();

	if ( CheckKeyword( "*3DSMAX_ASCIIEXPORT" , it ) == false )
		return false;

	m_iCountBone = 0;
	
	for( ; it != it_end ; )
	{
		
		if ( CheckKeyword( "*SCENE" , it ) )
		{
			DecodeSCENE( it );
		}
		else if ( CheckKeyword( "*MATERIAL_LIST" , it ) )
		{
			DecodeMATERIAL_LIST( it );
		}
		else if ( CheckKeyword( "*GEOMOBJECT" , it ) )
		{
			GdsNodePtr pNode = GdsNodePtr( new GdsNode );
			pNode->SetDrawAxis( true );
			m_vecNodeList.push_back( pNode );
			DecodeGEOMOBJECT( it , pNode );
			++m_iCountBone;
		}
		else
		{
			++it;
		}
	}

	m_vecNodeList.swap( m_vecNodeList );
	parseNode( m_vecNodeList );	
	parseMaterial( m_vecNodeList , device );
	m_vecNodeList.clear();
	m_vecMaterialList.clear();

	return true;
}


bool GdsResASE::parseMaterial( NODE_LIST& nodelist , LPDIRECT3DDEVICE9 device )
{
	return true;
}

bool GdsResASE::parseNode( NODE_LIST& nodelist )
{
	if ( m_vecNodeList.empty() )
		return false;

	NODE_LIST::iterator it = m_vecNodeList.begin();
	NODE_LIST::iterator it_end = m_vecNodeList.end();
	
	for( ; it != it_end ; ++it )
	{
		if ( (*it)->GetName() == L"Bip01" )
			m_RootNode = *it;
		else if ( (*it)->GetParentName() != L"" )
		{
			attachNode( *it , (*it)->GetParentName() );
		}
	}

	return true;
}

bool GdsResASE::attachNode( GdsNodePtr childNode , tstring& parentname )
{

	if ( m_vecNodeList.empty() )
		return false;

	NODE_LIST::iterator it = m_vecNodeList.begin();
	NODE_LIST::iterator it_end = m_vecNodeList.end();
	for ( ; it != it_end ; ++it )
	{
		if ( (*it)->GetName() == parentname )
		{
			(*it)->AttachChild( childNode );
			break;
		}
	}
	return true;
}

bool GdsResASE::DecodeGEOMOBJECT( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		std::string node_name;
		GetValue( "*NODE_NAME" , line , "\t " , node_name );
		if ( node_name != "" )
		{			
			tstring str = util::string::mb2wc( node_name.c_str() );
			pNode->SetName( str );
		}		

		std::string parent_name;
		GetValue( "*NODE_PARENT" , line , "\t " , parent_name );
		if ( parent_name != "" )
		{
			tstring str = util::string::mb2wc( parent_name.c_str() );
			pNode->SetParentName( str );
		}
	
		if ( CheckKeyword( "*NODE_TM" , line ) )
		{
			DecodeTM( line , pNode );
		}

		if( CheckKeyword( "*MESH" , line ) )
		{
			DecodeMESH( line , pNode );
		}

		GetValue( "*MATERIAL_REF" , line , "\t " , m_iMeshRef );

		if( CheckKeyword( "*TM_ANIMATION" , line ) )
		{
			DecodeANIMATION( line , pNode );
		}

		if ( CheckKeyword( "}" , line ) )
		{
			return true;
		}

		++line;

	} while (true);
	
	return false;
}


bool GdsResASE::DecodeANIMATION( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	while ( true )
	{
		std::string name;
		GetValue( "*NODE_NAME" , line , "\t " , name );
		if ( name != "" )
		{
			tstring tstrname = util::string::mb2wc( name.c_str() );
			if ( pNode->GetName() != tstrname )
				return false;
		}		
	
		if ( CheckKeyword( "*CONTROL_ROT_TRACK" , line ) 
			|| CheckKeyword( "*CONTROL_ROT_TCB" , line ) 
			|| CheckKeyword( "*CONTROL_ROT_BEZIER" , line ) 
			)
		{
			DecodeROT_TRACK( line , pNode );
		}

		if( CheckKeyword( "*CONTROL_SCALE_TRACK" , line ) 
			|| CheckKeyword( "*CONTROL_SCALE_TCB" , line )
			|| CheckKeyword( "*CONTROL_SCALE_BEZIER" , line ) 
			)
		{
			DecodeSCALE_TRACK( line , pNode );
		}

		if( CheckKeyword( "*CONTROL_POS_TRACK" , line )
			|| CheckKeyword( "*CONTROL_POS_TCB" , line )
			|| CheckKeyword( "*CONTROL_POS_BEZIER" , line )
			)
		{
			DecodePOS_TRACK( line , pNode );
		}

		if ( CheckKeyword( "}" , line ) )
			return true;
	}

	return false;
}


bool GdsResASE::DecodeROT_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodeSCALE_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodePOS_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}


bool GdsResASE::DecodeMESH( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	// skip *TIMEVALUE
	++line;

	int iCountVertex;
	if ( GetValue( "*MESH_NUMVERTEX" , line , "\t " , iCountVertex ) )
	{	
		if ( m_VertexList != NULL)
			SAFE_DELETE_ARRAY(m_VertexList);

		m_VertexList = new AseVERTEX[iCountVertex];		
	}

	
	int iCountTriangle;
	if ( GetValue( "*MESH_NUMFACES" , line , "\t " , iCountTriangle ) )
	{		
		if ( m_FaceList != NULL )
			SAFE_DELETE_ARRAY( m_FaceList );

		m_FaceList = new AseFACE[iCountTriangle];

	}	

	if ( CheckKeyword( "*MESH_VERTEX_LIST" , line ) )
	{
		DecodeMESH_VERTEX_LIST( line , m_VertexList );
	}

	if ( CheckKeyword( "*MESH_FACE_LIST" , line ) )
	{
		DecodeMESH_FACE_LIST( line , m_FaceList );
	}
	
 	while ( true )
 	{
		int iNumOfTVertex;
		if ( GetValue( "*MESH_NUMTVERTEX" , line , "\t " , iNumOfTVertex ) )
		{
			if ( iNumOfTVertex > 0 )
			{
				//u v 
				if ( m_TVertex != NULL )
					SAFE_DELETE_ARRAY(m_TVertex);
				m_TVertex = new TEXCOORDFLOAT[iNumOfTVertex];
				DecodeMESH_TVERTLIST( line , m_TVertex );
			}			
		}			
 	
		int iNumOfTVFaces;
		GetValue( "*MESH_NUMTVFACES" , line , "\t " , iNumOfTVFaces );
		if ( iNumOfTVFaces > 0 )		
 			DecodeMESH_TFACELIST( line );

// 		int iNumOfCVertex;
// 		GetValue( "*MESH_NUMCVERTEX" , line , "\t " , iNumOfCVertex );
// 		if ( iNumOfCVertex > 0 )		
// 			DecodeMESH_CVERTEX( line , pNode );

		if ( CheckKeyword( "*MESH_NORMALS" , line ) )
			DecodeMESH_NORMALS( line );
 		
 		if( CheckKeyword( "}" , line ) ) 
		{
			pNode->GetProperty()->GetMesh()->SetMaterial( m_vecMaterialList.at( m_FaceList[0].MaterialID ) );
			pNode->GetProperty()->GetMesh()->SetFVF( sizeof( AseVERTEX::FVF ) );
			pNode->GetProperty()->GetMesh()->SetVertexSize( sizeof( AseVERTEX ) );
			pNode->GetProperty()->GetMesh()->SetVertexMaxCount( iCountVertex );
			pNode->GetProperty()->GetMesh()->SetIndexMaxCount( iCountTriangle );

			MakeVertex( pNode->GetProperty()->GetMesh()->GetVBBuffer() , iCountVertex );
			MakeIndex( pNode->GetProperty()->GetMesh()->GetIBBuffer() , iCountTriangle );

			return true;
		}

		++line; 
 	}

	return false;
}

void GdsResASE::MakeVertex( LPDIRECT3DVERTEXBUFFER9* vb , int icount_vertex )
{
	LPDIRECT3DDEVICE9 device = RENDERER.GetDevice();
	ASSERT( device );
	ASSERT( !(*vb) );

	if( FAILED(device->CreateVertexBuffer( icount_vertex*sizeof(AseVERTEX),0, AseVERTEX::FVF,
		D3DPOOL_DEFAULT, vb, NULL)))
	{
		return;
	}

	VOID* pVertices;
	if(FAILED( (*vb)->Lock(0, icount_vertex*sizeof(AseVERTEX), (void**)&pVertices,0)))
	{
		return;
	}

	memcpy(pVertices, m_VertexList , icount_vertex*sizeof(AseVERTEX));

	(*vb)->Unlock();

}

void GdsResASE::MakeIndex( LPDIRECT3DINDEXBUFFER9* ib , int icount_index )
{
	LPDIRECT3DDEVICE9 device = RENDERER.GetDevice();
	ASSERT( device );
	ASSERT( !(*ib) );

	AseINDEX* pIndex = NULL;
	pIndex = new AseINDEX[icount_index];
	for ( size_t i=0 ;i < icount_index ; i++ )
	{
		pIndex[i]._0 = m_FaceList[i].VertexIndex[0];
		pIndex[i]._1 = m_FaceList[i].VertexIndex[1];
		pIndex[i]._2 = m_FaceList[i].VertexIndex[2];
	}

	if( FAILED( device->CreateIndexBuffer( icount_index*sizeof(AseINDEX),	0,  D3DFMT_INDEX32 ,
		D3DPOOL_DEFAULT, ib, NULL ) ) )
	{
		return;
	}

	/// 정점버퍼를 값으로 채운다. 
	/// 정점버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
	VOID* pIndices;
	if( FAILED( (*ib)->Lock( 0, icount_index*sizeof(AseINDEX), (void**)&pIndices, 0 ) ) )
		return;

	memcpy( pIndices , pIndex , icount_index*sizeof(AseINDEX) );

	(*ib)->Unlock();

	SAFE_DELETE_ARRAY( pIndex );
}


bool GdsResASE::DecodeMESH_TVERTLIST( LineContainerA::iterator& line , TEXCOORDFLOAT* pTVertex )
{
	if ( CheckKeyword( "*MESH_TVERTLIST" , line ) ) 
	{
		do 
		{
			float index , u , v , w; 
			if ( GetValue( "*MESH_TVERT" , line , "\t " , index , u , v ,w ) )
			{
				int iIndex = static_cast<int>(index);
				pTVertex[iIndex].u = u;
				pTVertex[iIndex].v = 1.0f - v;
			}
			else if ( CheckKeyword( "}" , line ) )
				return true;

			++line;
		}
		while ( true );
	} 

	return false;
}

bool GdsResASE::DecodeMESH_TFACELIST( LineContainerA::iterator& line )
{
	if ( CheckKeyword( "*MESH_TFACELIST" , line ) )
	{
		do 
		{
			float index , x , y , z;
			if ( GetValue( "*MESH_TFACE" , line , "\t " , index , x , z , y ) )
			{
				int iIndex = static_cast<int>( index );
				int ix = static_cast<int>( x );
				int iy = static_cast<int>( y );
				int iz = static_cast<int>( z );
// 				m_FaceList[iIndex].VertexTexture[0].u = m_TVertex[ix].u;
// 				m_FaceList[iIndex].VertexTexture[0].v = m_TVertex[ix].v;
// 				m_FaceList[iIndex].VertexTexture[1].u = m_TVertex[iy].u;
// 				m_FaceList[iIndex].VertexTexture[1].v = m_TVertex[iy].v;
// 				m_FaceList[iIndex].VertexTexture[2].u = m_TVertex[iz].u;
// 				m_FaceList[iIndex].VertexTexture[2].v = m_TVertex[iz].v;

				int vertex_index_x = m_FaceList[iIndex].VertexIndex[0];
				int vertex_index_y = m_FaceList[iIndex].VertexIndex[1];
				int vertex_index_z = m_FaceList[iIndex].VertexIndex[2];

				m_VertexList[vertex_index_x].u = m_TVertex[ix].u;
				m_VertexList[vertex_index_x].v = m_TVertex[ix].v;

				m_VertexList[vertex_index_y].u = m_TVertex[iy].u;
				m_VertexList[vertex_index_y].v = m_TVertex[iy].v;

				m_VertexList[vertex_index_z].u = m_TVertex[iz].u;
				m_VertexList[vertex_index_z].v = m_TVertex[iz].v;

			}
			else if ( CheckKeyword( "}" , line ) )
				return true;

		} while ( true );
	}	

	++line;

	return false;
}

bool GdsResASE::DecodeMESH_CVERTEX( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodeMESH_NORMALS( LineContainerA::iterator& line )
{
	float index , x , y , z;

	do
	{
		if( GetValue( "*MESH_FACENORMAL" , line , "\t " , index , x , z , y ) )
		{
			m_FaceList[(int)index].FaceNormal.x = x;
			m_FaceList[(int)index].FaceNormal.y = y;
			m_FaceList[(int)index].FaceNormal.z = z;

			if ( GetValue( "*MESH_VERTEXNORMAL" , line , "\t " , index , x, z ,y ) )
			{
				// 			m_FaceList[(int)index].VertexNormal[0].x = x;
				// 			m_FaceList[(int)index].VertexNormal[0].y = y;
				// 			m_FaceList[(int)index].VertexNormal[0].z = z;
			}

			if ( GetValue( "*MESH_VERTEXNORMAL" , line , "\t " , index , x, z ,y ) )
			{
				// 			m_FaceList[(int)index].VertexNormal[2].x = x;
				// 			m_FaceList[(int)index].VertexNormal[2].y = y;
				// 			m_FaceList[(int)index].VertexNormal[2].z = z;
			}

			if ( GetValue( "*MESH_VERTEXNORMAL" , line , "\t " , index , x, z ,y ) )
			{
				// 			m_FaceList[(int)index].VertexNormal[1].x = x;
				// 			m_FaceList[(int)index].VertexNormal[1].y = y;
				// 			m_FaceList[(int)index].VertexNormal[1].z = z;
			}
		}

		if ( CheckKeyword( "}" , line ) )
			return true;

	}	
	while( true );

	return false;
}


bool GdsResASE::DecodeMESH_VERTEX_LIST( LineContainerA::iterator& line , AseVERTEX* pVertexList )
{
	do 
	{
		float x,y,z,index;
		if ( GetValue( "*MESH_VERTEX" , line , "\t " , index , x , z , y ) )
		{
			int i = static_cast<int>( index );
			pVertexList[i].p.x = x;
			pVertexList[i].p.y = y;
			pVertexList[i].p.z = z;
		}

		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodeMESH_FACE_LIST( LineContainerA::iterator& line , AseFACE* pTriangleList )
{
	int iCount = 0;
	do 
	{
		//*MESH_FACE    0:    A:    0 B:    1 C:    2 AB:    1 BC:    1 CA:    1	 *MESH_SMOOTHING 1 	*MESH_MTLID 0
		int id , indexx , indexy , indexz ;
		if ( GetValue_FACELIST( "*MESH_FACE" , line , "\t " 
						, "A:" , indexx 
						, "B:" , indexz 
						, "C:" , indexy 
						, "*MESH_MTLID" , id ) )
			{
				pTriangleList[iCount].VertexIndex[0] = indexx;
				pTriangleList[iCount].VertexIndex[1] = indexy;
				pTriangleList[iCount].VertexIndex[2] = indexz;
				m_FaceList[iCount].MaterialID = id;
			}


		if ( CheckKeyword( "}" , line ) )
			return true;

		++iCount;

	} while ( true );

	return false;
}



bool GdsResASE::DecodeTM( LineContainerA::iterator& line , GdsNodePtr pNode )
{
// 	fgets(m_line, 256, fp); //NODE_NAME
// 	fgets(m_line, 256, fp); //*INHERIT_POS
// 	fgets(m_line, 256, fp); //*INHERIT_ROT
// 	fgets(m_line, 256, fp); //*INHERIT_SCL
// 	fgets(m_line, 256, fp); //*TM_ROW0
	D3DXMATRIXA16 dxMat;
	float fScale;

	do 
	{
		GetValue( "*TM_ROW0" , line , "\t " , dxMat._11 , dxMat._13 , dxMat._12 );
		GetValue( "*TM_ROW1" , line , "\t " , dxMat._31 , dxMat._33 , dxMat._32 );
		GetValue( "*TM_ROW2" , line , "\t " , dxMat._21 , dxMat._23 , dxMat._22 );
		GetValue( "*TM_ROW3" , line , "\t " , dxMat._41 , dxMat._43 , dxMat._42 );	
		GetValue( "*TM_SCALE" , line , "\t " , fScale );

		if ( CheckKeyword( "}" , line ) )
		{
			pNode->GetRotate().SetRow( 0 , dxMat._11 , dxMat._12 , dxMat._13 );
			pNode->GetRotate().SetRow( 1 , dxMat._21 , dxMat._22 , dxMat._23 );
			pNode->GetRotate().SetRow( 2 , dxMat._31 , dxMat._32 , dxMat._33 );
			pNode->GetTranslate().x = dxMat._41;
			pNode->GetTranslate().y = dxMat._42;
			pNode->GetTranslate().z = dxMat._43;
			pNode->SetScale( fScale );
			break;
		}

		++line;
	} while (true);
	

	//GetValue( "*TM_SCALE" , line , "\t " , (float*)( pNode->GetScale() ) );

// 	fgets(m_line, 256, fp); //*TM_ROTAXIS
// 	sscanf(m_line, "%s%f%f%f",m_string, &fX, &fZ, &fY);
// 	fgets(m_line, 256, fp); //*TM_ROTANGLE
// 	sscanf(m_line,"%s%f", m_string, &fW);
// 	//fW = -fW;
// 	m_CurMesh->m_TmRot.x = (float)sinf(fW / 2.0f) * fX;
// 	m_CurMesh->m_TmRot.y = (float)sinf(fW / 2.0f) * fY;
// 	m_CurMesh->m_TmRot.z = (float)sinf(fW / 2.0f) * fZ;
// 	m_CurMesh->m_TmRot.w = (float)cosf(fW / 2.0f);
// 	fgets(m_line, 256, fp);//*TM_SCALE
// 	sscanf(m_line, "%s%f%f%f",m_string, &(m_CurMesh->m_TmScale.x), &(m_CurMesh->m_TmScale.z), &(m_CurMesh->m_TmScale.y));
// 	fgets(m_line, 256, fp);//*TM_SCALEAXIS
// 	sscanf(m_line, "%s%f%f%f",m_string, &fX,&fZ,&fY);
// 	fgets(m_line, 256, fp);//*TM_SCALEAXISANG
// 	sscanf(m_line, "%s%f",m_string, &fW);
// 	m_CurMesh->m_TmScaleRot.x = (float)sin(fW / 2.0f) * fX;
// 	m_CurMesh->m_TmScaleRot.y = (float)sin(fW / 2.0f) * fY;
// 	m_CurMesh->m_TmScaleRot.z = (float)sin(fW / 2.0f) * fZ;
// 	m_CurMesh->m_TmScaleRot.w = (float)cos(fW / 2.0f);
// 	fgets(m_line, 256, fp);//}
	return true;
}

bool GdsResASE::CheckKeyword( const char* keyword , LineContainerA::iterator& line )
{
	char* in_str = *line;
	size_t len = strlen( in_str ) + 1;
	
	char* str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );

	sscanf_s( in_str , "%s" , str , len );
	
	bool bflag = static_cast<bool>( _stricmp( keyword , str ) );
	
	FRAMEMEMORY.Free( len , str );

	//찾으면 한줄 점프
	if ( bflag == false )
		++line;

	return !bflag;
}

bool GdsResASE::DecodeSCENE( LineContainerA::iterator& line )
{
	//skip *SCENE_FILENAME
	++line;

	//*SCENE_FIRSTFRAME
	GetValue( "*SCENE_FIRSTFRAME" , line , " " , m_fFirstFrame );
	//getValue( "*SCENE_FIRSTFRAME" , line , " " , m_fFirstFrame ) ;

	//*SCENE_LASTFRAME
	GetValue( "*SCENE_LASTFRAME" , line , " " , m_fLastFrame );

	//*SCENE_FRAMESPEED
	GetValue( "*SCENE_FRAMESPEED" , line , " " , m_fFrameSpeed );

	//*SCENE_TICKSPERFRAME	
	GetValue( "*SCENE_TICKSPERFRAME" , line , " " , m_fTickPerFrame );

	return true;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	token = strtok_s( dest_str , SEP ,&context );
	
	bool bRet = false;
	if( _stricmp( keyword , token ) == false )
	{
		fvalue = static_cast<float>( atof( context ) );
		//한줄 점프~
		++line;
		bRet = true;
	}	

	FRAMEMEMORY.Free( len , dest_str );
	
	return bRet;
}


bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3 )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	bool bRet = false;
	token = strtok_s( dest_str , SEP , &context );
	if( _stricmp( keyword , token ) == false )
	{
		token = strtok_s( NULL , SEP , &context );
		fvalue1 = static_cast<float>( atof( token ) );

		token = strtok_s( NULL , SEP , &context );
		fvalue2 = static_cast<float>( atof( token ) );

		token = strtok_s( NULL , SEP , &context );
		fvalue3 = static_cast<float>( atof( token ) );

		//한줄 점프~
		++line;
		bRet = true;
	}

	FRAMEMEMORY.Free( len , dest_str );

	return bRet;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3 , float& fvalue4 )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	bool bRet = false;
	token = strtok_s( dest_str , SEP , &context );
	if( _stricmp( keyword , token ) == false )
	{
		token = strtok_s( NULL , SEP , &context );
		fvalue1 = static_cast<float>( atof( token ) );

		token = strtok_s( NULL , SEP , &context );
		fvalue2 = static_cast<float>( atof( token ) );

		token = strtok_s( NULL , SEP , &context );
		fvalue3 = static_cast<float>( atof( token ) );

		token = strtok_s( NULL , SEP , &context );
		fvalue4 = static_cast<float>( atof( token ) );

		//한줄 점프~
		++line;
		bRet = true;
	}

	FRAMEMEMORY.Free( len , dest_str );

	return bRet;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , int& ivalue )
{	
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;
	token = strtok_s( dest_str , SEP ,&context );

	bool bRet = false;
	if( _stricmp( keyword , token ) == false )
	{
		ivalue = static_cast<int>( atoi( context ) );
		//한줄 점프~
		++line;
		bRet = true;
	}

	FRAMEMEMORY.Free( len , dest_str );

	return bRet;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , std::string& str )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	bool bRet = false;
	token = strtok_s( dest_str , SEP ,&context );
	if( _stricmp( keyword , token ) == false )
	{
		std::string token( context );

		token.erase(0,1);
		token.erase( token.length()-1, token.length() );
		str = token;
		//한줄 점프~
		++line;
		bRet = true;
	}	

	FRAMEMEMORY.Free( len , dest_str );

	return bRet;
}

bool GdsResASE::GetValue_FACELIST(  const char* keyword , LineContainerA::iterator& line , const char* SEP 						
						, const char* subkeyword1 , int& ivalue1  
						, const char* subkeyword2 , int& ivalue2 
						, const char* subkeyword3 , int& ivalue3
						, const char* subkeyword4 , int& ivalue4 
						)
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	bool bRet = false;
	bool bRet1 = false;
	bool bRet2 = false;
	bool bRet3 = false;
	bool bRet4 = false;

	token = strtok_s( dest_str , SEP ,&context );
	if( _stricmp( keyword , token ) == false )
	{
		while( token )
		{
			if ( _stricmp( subkeyword1 , token ) == false )
			{
				token = strtok_s( NULL , SEP ,&context );
				ivalue1 = atoi( token );

				bRet1 = true;
			}

			if ( _stricmp( subkeyword2 , token ) == false )
			{
				token = strtok_s( NULL , SEP ,&context );
				ivalue2 = atoi( token );

				bRet2 = true;
			}

			if ( _stricmp( subkeyword3 , token ) == false )
			{
				token = strtok_s( NULL , SEP ,&context );
				ivalue3 = atoi( token );

				bRet3 = true;
			}

			if ( _stricmp( subkeyword4 , token ) == false )
			{
				token = strtok_s( NULL , SEP ,&context );
				ivalue4 = atoi( token );

				bRet4 = true;
			}

			token = strtok_s( NULL , SEP ,&context );
		}

		//한줄 점프~
		++line;

		if ( bRet1 && bRet2 && bRet3 && bRet4 )
			bRet = true;
	}	

	FRAMEMEMORY.Free( len , dest_str );

	return bRet;

}

bool GdsResASE::DecodeMATERIAL_LIST( LineContainerA::iterator& line )
{
	int iCountMaterial;
	GetValue( "*MATERIAL_COUNT" , line , " " , iCountMaterial );
	m_vecMaterialList.reserve( iCountMaterial );

	do
	{		
		int iIndex;
		if ( GetValue( "*MATERIAL" , line , "\t " , iIndex ) )
		{
			GdsMaterialPtr material = GdsMaterialPtr( new GdsMaterial );
			DecodeMaterial( line , material );
			m_vecMaterialList.push_back( material );
		}
		else if( CheckKeyword( "}" , line ) )
		{
			break;
		}
		else
		{
			++line;
		}	
	
	}while ( true );


	return true;
}

bool GdsResASE::DecodeMaterial( LineContainerA::iterator& line , GdsMaterialPtr Material )
{
	do 
	{
		if ( CheckKeyword( "*SUBMATERIAL" , line ) )
		{
			GetValue( "*SUBMATERIAL" , line , " " , m_iSubMaterial );
			GdsMaterialPtr subMaterial = GdsMaterialPtr( new GdsMaterial );
			Material->AddSubMaterial( subMaterial );
			DecodeMaterial( line , subMaterial );
		}

		float fAmbientR , fAmbientG , fAmbientB;
		float fDiffuseR , fDiffuseG , fDiffuseB;
		float fSpecularR , fSpecularG , fSpecularB;

		GetValue( "*NUMSUBMTLS" ,line , "\t " , m_iCountSubMaterial );		

		if ( GetValue( "*MATERIAL_AMBIENT" ,line , "\t " , fAmbientR , fAmbientG , fAmbientB ) )
			Material->SetAmbientColor( fAmbientR , fAmbientG , fAmbientB );

		if ( GetValue( "*MATERIAL_DIFFUSE" ,line , "\t " , fDiffuseR , fDiffuseG , fDiffuseB ) )
			Material->SetDiffuesColor( fDiffuseR , fDiffuseG , fDiffuseB );

		if ( GetValue( "*MATERIAL_SPECULAR" ,line , "\t " , fSpecularR , fSpecularG , fSpecularB ) )
			Material->SetSpecularColor( fSpecularR , fSpecularG , fSpecularB );		

		if( CheckKeyword( "*MAP_DIFFUSE" , line ) )
		{
			//CurMaterial->bUseTexture = true;
			DecodeMap( line , Material );
		}

		if( CheckKeyword( "*MAP_OPACITY" , line ) )
		{
// 			CurMaterial->bUseTexture = true;
// 			CurMaterial->bUseOpacity = true;
			DecodeMap( line , Material );
		}
		if( CheckKeyword( "*MAP_SELFILLUM" , line ) )
		{
			DecodeMap( line , Material );
		}

		if( CheckKeyword( "*MAP_REFLECT" , line ) )
		{
			DecodeMap( line , Material );
		}

		if( CheckKeyword( "*MAP_SPECULAR" , line ) )
		{
			DecodeMap( line , Material );
		}

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	} while (true);

	return true;
}

bool GdsResASE::DecodeMap( LineContainerA::iterator& line , GdsMaterialPtr Material )
{
	do{
		std::string path;
		if ( GetValue( "*BITMAP" , line , "\t " , path ) )
		{
			for_each( path.begin() , path.end() , functor::ToLowerA() );

			size_t poscomma = path.rfind( "/" );
			tstring filename= util::string::mb2wc( path.substr( poscomma+1 , path.length() ).c_str() );

			GdsResTexturePtr tex = boost::shared_dynamic_cast< GdsResTexture >( RESMGR.Get( filename.c_str() ) );
			if ( tex != NULL )
			{
				Material->SetTexture( tex );
			}

		}

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	}while ( true );

	return true;
}

