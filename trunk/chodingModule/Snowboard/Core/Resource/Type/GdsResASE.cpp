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

	++it;

	for ( ; it != it_end ; ++it )
	{ 		
		if ( CheckKeyword( "*SCENE" , it ) )
		{
			++it;

			DecodeSCENE( it );
		}

		if ( CheckKeyword( "*MATERIAL_LIST" , it ) )
		{
			++it;

			DecodeMATERIAL_LIST( it );
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
	return !bflag;
}

bool GdsResASE::DecodeSCENE( LineContainerA::iterator& line )
{
	//*SCENE_FILENAME
	++line;

	//*SCENE_FIRSTFRAME
	GetValue( line , " " , m_fFirstFrame );

	//*SCENE_LASTFRAME
	GetValue( line , " " , m_fLastFrame );

	//*SCENE_FRAMESPEED
	GetValue( line , " " , m_fFrameSpeed );

	//*SCENE_TICKSPERFRAME	
	GetValue( line , " " , m_fTickPerFrame );

	return true;
}

bool GdsResASE::GetValue( LineContainerA::iterator& line , const char* SEP , float& fvalue )
{
	char* in_str = *line;
	char* context = NULL;

	strtok_s( in_str , SEP ,&context );
	fvalue = atof( context );
	
	//한줄 점프~
	++line;

	return true;
}

bool GdsResASE::GetValue( LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3 )
{
	char* in_str = *line;
	char* context = NULL;
	char* token = NULL;

	token = strtok_s( in_str , SEP , &context );

	token = strtok_s( NULL , SEP , &context );
	fvalue1 = atof( token );

	token = strtok_s( NULL , SEP , &context );
	fvalue2 = atof( token );

	token = strtok_s( NULL , SEP , &context );
	fvalue3 = atof( token );

	//한줄 점프~
	++line;

	return true;
}

bool GdsResASE::GetValue( LineContainerA::iterator& line , const char* SEP , int& ivalue )
{
	char* in_str = *line;
	char* context = NULL;

	strtok_s( in_str , SEP ,&context );
	ivalue = atoi( context );

	//한줄 점프~
	++line;

	return true;
}

bool GdsResASE::DecodeMATERIAL_LIST( LineContainerA::iterator& line )
{
	GetValue( line , " " , m_iCountMaterial );

	int iCheck = 0;
	
	do
	{
		
		if( CheckKeyword( "*MATERIAL" , line ) )
		{
			//한줄 점프~~
			++line;

			DecodeMaterial( line );
			++iCheck;
		}

		if( CheckKeyword( "}" , line ) )
		{
			//한줄 점프~~
			++line;

			break;
		}

	}while(1);


	return true;
}

bool GdsResASE::DecodeMaterial( LineContainerA::iterator& line )
{
	do 
	{
		if ( CheckKeyword( "*SUBMATERIAL" , line ) )
		{
			GetValue( line , " " , m_iSubMaterial );
			DecodeMaterial( line );
			continue;
		}

		if ( CheckKeyword( "*NUMSUBMTLS" , line ) )
		{
			GetValue( line , " " , m_iCountSubMaterial );
			continue;
		}

		if ( CheckKeyword( "*MATERIAL_AMBIENT" , line ) )
		{
			GetValue( line , "\t " , m_fAmbientR , m_fAmbientG , m_fAmbientB );
			continue;
		}

		if ( CheckKeyword( "*MATERIAL_DIFFUSE" , line ) )
		{
			GetValue( line , "\t " , m_fDiffuseR , m_fDiffuseG , m_fDiffuseB );
			continue;
		}

		if ( CheckKeyword( "*MATERIAL_SPECULAR" , line ) )
		{
			GetValue( line , "\t " , m_fSpecularR , m_fSpecularG , m_fSpecularB );
			continue;
		}

		if( CheckKeyword( "}" , line ) )
		{
			break;
		}

		++line;

	} while (1);

	return true;
}
