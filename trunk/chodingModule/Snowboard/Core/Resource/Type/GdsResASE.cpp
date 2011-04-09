#include "GdsResASE.h"
#include "../../../System/FileSystem/GdsFile.h"
#include "../../../System/Logger/logger.h"
#include "../../../System/FrameMemory/GdsFrameMemory.h"

GdsResASE::GdsResASE()
{
	SetName( OBJECT_RES_ASE );
	vClear();
}

GdsResASE::~GdsResASE()
{
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

	for ( ; it != it_end ; ++it )
	{ 		
		if ( CheckKeyword( "*SCENE" , it ) )
		{
			DecodeSCENE( it );
		}

		if ( CheckKeyword( "*MATERIAL_LIST" , it ) )
		{
			DecodeMATERIAL_LIST( it );
		}

		if ( CheckKeyword( "*GEOMOBJECT" , it ) )
		{

		}

	}	

	return true;
}

bool GdsResASE::CheckKeyword( const char* keyword , LineContainerA::iterator& line )
{
	char* in_str = *line;
	size_t len = strlen( in_str ) + 1;
	
	char* str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );

	sscanf_s( in_str , "%s" , str , len );
	
	bool bflag = _stricmp( keyword , str );		
	
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
	
	if( _stricmp( keyword , token ) == false )
	{
		fvalue = atof( context );
		//한줄 점프~
		++line;
	}	

	FRAMEMEMORY.Free( len , dest_str );
	
	return true;
}


bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3 )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	token = strtok_s( dest_str , SEP , &context );
	if( _stricmp( keyword , token ) == false )
	{
		token = strtok_s( NULL , SEP , &context );
		fvalue1 = atof( token );

		token = strtok_s( NULL , SEP , &context );
		fvalue2 = atof( token );

		token = strtok_s( NULL , SEP , &context );
		fvalue3 = atof( token );

		//한줄 점프~
		++line;
	}

	FRAMEMEMORY.Free( len , dest_str );

	return true;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , int& ivalue )
{	
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;
	token = strtok_s( dest_str , SEP ,&context );
	if( _stricmp( keyword , token ) == false )
	{
		ivalue = atoi( context );
		//한줄 점프~
		++line;
	}

	FRAMEMEMORY.Free( len , dest_str );

	return true;
}

bool GdsResASE::GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , std::string& const str )
{
	size_t len = strlen( *line ) + 1;
	char* dest_str = static_cast< char* >( FRAMEMEMORY.Alloc( len ) );
	strcpy_s( dest_str , len ,  *line );

	char* context = NULL;
	char* token = NULL;

	token = strtok_s( dest_str , SEP ,&context );
	if( _stricmp( keyword , token ) == false )
	{
		std::string token( context );
		str = token;
		//한줄 점프~
		++line;
	}	

	FRAMEMEMORY.Free( len , dest_str );

	return true;
}

bool GdsResASE::DecodeMATERIAL_LIST( LineContainerA::iterator& line )
{
	GetValue( "*MATERIAL_COUNT" , line , " " , m_iCountMaterial );

	int iCheck = 0;	
	do
	{		
		if( CheckKeyword( "*MATERIAL" , line ) )
		{
			DecodeMaterial( line );
			++iCheck;
			continue;
		}
		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;
	
	}while(1);


	return true;
}

bool GdsResASE::DecodeMaterial( LineContainerA::iterator& line )
{
	do 
	{
		if ( CheckKeyword( "*SUBMATERIAL" , line ) )
		{
			GetValue( "*SUBMATERIAL" , line , " " , m_iSubMaterial );
			DecodeMaterial( line );
		}

		GetValue( "*NUMSUBMTLS" ,line , "\t " , m_iCountSubMaterial );		
		GetValue( "*MATERIAL_AMBIENT" ,line , "\t " , m_fAmbientR , m_fAmbientG , m_fAmbientB );
		GetValue( "*MATERIAL_DIFFUSE" ,line , "\t " , m_fDiffuseR , m_fDiffuseG , m_fDiffuseB );
		GetValue( "*MATERIAL_SPECULAR" ,line , "\t " , m_fSpecularR , m_fSpecularG , m_fSpecularB );

		if( CheckKeyword( "*MAP_DIFFUSE" , line ) )
		{
			//CurMaterial->bUseTexture = true;
			DecodeMap( line );
		}

		if( CheckKeyword( "*MAP_OPACITY" , line ) )
		{
// 			CurMaterial->bUseTexture = true;
// 			CurMaterial->bUseOpacity = true;
			DecodeMap( line );
		}
		if( CheckKeyword( "*MAP_SELFILLUM" , line ) )
		{
			DecodeMap( line );
		}

		if( CheckKeyword( "*MAP_REFLECT" , line ) )
		{
			DecodeMap( line );
		}

		if( CheckKeyword( "*MAP_SPECULAR" , line ) )
		{
			DecodeMap( line );
		}

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	} while (1);

	return true;
}

bool GdsResASE::DecodeMap( LineContainerA::iterator& line )
{
	do{

		GetValue( "*BITMAP" , line , "\t " , m_strTextureName );

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	}while(1);

	return true;
}
