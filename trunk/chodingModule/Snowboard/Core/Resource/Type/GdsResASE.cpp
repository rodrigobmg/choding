#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"
#include "../../../System/Logger/logger.h"
#include "../../../System/FrameMemory/GdsFrameMemory.h"

GdsResASE::GdsResASE()
{
	SetName( OBJECT_RES_ASE );
	vClear();
	m_RootNode = GdsNodePtr( (GdsNode*)NULL );
}

GdsResASE::~GdsResASE()
{
	m_vecNodeList.clear();
	m_vecMaterialList.clear();
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
			return true;

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
	GetValue( "*MESH_NUMVERTEX" , line , "\t " , iCountVertex );
	int iCountTriangle;
	GetValue( "*MESH_NUMFACES" , line , "\t " , iCountTriangle );
	
	if ( CheckKeyword( "*MESH_VERTEX_LIST" , line ) )
	{
		DecodeMESH_VERTEX_LIST( line , pNode );
	}

	if ( CheckKeyword( "*MESH_FACE_LIST" , line ) )
	{
		DecodeMESH_FACE_LIST( line , pNode );
	}
	
 	while ( true )
 	{
		int iNumOfTVertex;
		GetValue( "*MESH_NUMTVERTEX" , line , "\t " , iNumOfTVertex );
		if ( iNumOfTVertex > 0 )
			DecodeMESH_TVERTLIST( line , pNode );
 	
		int iNumOfTVFaces;
		GetValue( "*MESH_NUMTVFACES" , line , "\t " , iNumOfTVFaces );
		if ( iNumOfTVFaces > 0 )		
 			DecodeMESH_TFACELIST( line , pNode );

		int iNumOfCVertex;
		GetValue( "*MESH_NUMCVERTEX" , line , "\t " , iNumOfCVertex );
		if ( iNumOfCVertex > 0 )		
			DecodeMESH_CVERTEX( line , pNode );

		if ( CheckKeyword( "*MESH_NORMALS" , line ) )
			DecodeMESH_NORMALS( line , pNode );
 		
 		if( CheckKeyword( "}" , line ) ) 
			break;

		++line; 
 	}
 
	if ( CheckKeyword( "}" , line ) )
 		return true;

	return false;
}

bool GdsResASE::DecodeMESH_TVERTLIST( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodeMESH_TFACELIST( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

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

bool GdsResASE::DecodeMESH_NORMALS( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}


bool GdsResASE::DecodeMESH_VERTEX_LIST( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{
		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
	} while ( true );

	return false;
}

bool GdsResASE::DecodeMESH_FACE_LIST( LineContainerA::iterator& line , GdsNodePtr pNode )
{
	do 
	{

		if ( CheckKeyword( "}" , line ) )
			return true;

		++line;
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
			tstring strpath = util::string::mb2wc( path.c_str() );
			Material->SetTexturePath( strpath );
		}

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	}while ( true );

	return true;
}


