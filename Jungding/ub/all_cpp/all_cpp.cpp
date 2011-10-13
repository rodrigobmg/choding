// all_cpp.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

std::vector< project_info_table > g_all_project_info;
std::vector< std::string > g_except_file_table;

#define ATTRIBUTE_NAME "Condition"
#define ELEMENT_NAME "ExcludedFromBuild"
#define ATTRIBUTE_TEXT_DEBUG "'$(Configuration)|$(Platform)'=='Debug|Win32'"
#define ATTRIBUTE_TEXT_DEBUG_UNITY "'$(Configuration)|$(Platform)'=='Debug_unitybuild|Win32'"
#define ATTRIBUTE_TEXT_RELEASE "'$(Configuration)|$(Platform)'=='Profile|Win32'"
#define ATTRIBUTE_TEXT_RELEASE_UNITY "'$(Configuration)|$(Platform)'=='Profile_unitybuild|Win32'"
#define STR_TRUE "true"
#define STR_FALSE "false"

template<int N>
void close_files(FILE* (&arr)[N])
{
	for (int i=0; i<N; ++i) {
		if (arr[i]) {
			fclose(arr[i]);
			arr[i] = NULL;
		}
	}
}

void print_unity_x( project_info_table& info )
{
	FILE* arr[UNITY_NUM] = {NULL,};
	char fname[MAX_PATH];
	
	for (int i=0; i<UNITY_NUM; ++i)
	{
		std::string name = util::string::wc2mb( info.project_dir_path.c_str() );
		sprintf_s(fname, "%sUnity_%d.cpp", name.c_str() , i+1);
		fopen_s(&arr[i], fname, "wt");

		if (!arr[i])
		{
			close_files(arr);
			return;
		}
		
		fputs("#include \"stdafx.h\"\n\n", arr[i]);
	}

	typedef vector<std::string>::const_iterator iterator;

	int i=0;
	for (iterator itr=info.filepath_table.begin(); itr!=info.filepath_table.end(); ++itr) 
	{
		bool val = false;
		for( iterator except = g_except_file_table.begin(); except!=g_except_file_table.end(); ++except)
		{
			if( *except == *itr )
			{
				val = true;
				break;
			}
		}

		if( !val )
			fprintf(arr[i], "#include \"%s\"\n", (*itr).c_str());

		++i;
		if (i>=UNITY_NUM)
			i=0;
	}

	close_files(arr);
}


bool search_solution( const TCHAR* dirpath , bool bRecursive = true )
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	wstring dirspec( dirpath );
	dirspec += L"\\*";

	hFind = FindFirstFile( dirspec.c_str(), &fd);
	if (INVALID_HANDLE_VALUE == hFind) {
		return false;
	}

	do {
		if ( fd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY && bRecursive )
		{
			if (   _tcscmp(fd.cFileName, L".")
				&& _tcscmp(fd.cFileName, L"..") 
				&& _tcscmp(fd.cFileName, L".svn")
				&& _tcscmp(fd.cFileName, L".git")
				)
			{
				TCHAR curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				_stprintf_s( curpath , L"%s\\%s" , dirpath , fd.cFileName );

				search_solution( curpath , bRecursive );
			}
		}
		else if ( fd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY )
		{
			wstring wstr( fd.cFileName );
			size_t pos = wstr.find( L"." );
			if ( pos > 0 )
			{
				TCHAR curpath[MAX_PATH];
				ZeroMemory( curpath , sizeof( curpath ) );
				_stprintf_s( curpath , L"%s\\%s" , dirpath , fd.cFileName );
				wstring wfilepath( curpath );
				for_each( wstr.begin() , wstr.end() , ToLower() );

				size_t poscomma = wstr.rfind( L"." );
				wstring ext		= wstr.substr( poscomma + 1 , wstr.length() );
				if ( ext == L"vcxproj")
				{
					project_info_table info;
					info.project_dir_path = wstring(dirpath) + L"\\";
					info.project_file_path = wfilepath;
					g_all_project_info.push_back( info );
				}
			}	
		}

	} while (FindNextFile(hFind, &fd));

	DWORD err = GetLastError();
	FindClose(hFind);
	
	return true;
}

bool filter_file( const char* file )
{
	// pre filter
	if ( file == "" || file == NULL )
		return false;

	std::string str(file);
	for_each( str.begin() , str.end() , functor::ToLower() );

	size_t pos = str.rfind("\\");
	std::string preheader = str.substr( pos+1 , str.length() );
	if ( preheader == "stdafx.cpp" 
		|| preheader == "stdafx.h" 
		)
		return false;	
	
	pos = str.rfind("_");
	std::string unity_file = str.substr( 0 , pos );
	if ( unity_file == "unity" )
	{
		return false;
	}
	
	size_t poscomma = str.rfind( "." );
	std::string ext		= str.substr( poscomma + 1 , str.length() );
	if ( ext == "cpp" )
		return true;
	else
		return false;
}

void push_excludedfrombuild( TiXmlNode* node ,
							const char* elementname , 
							const char* attribute , 
							const char* attributedata ,
							const char* value )
{
	TiXmlElement* fileconfiguration_debug = new TiXmlElement(elementname);
	fileconfiguration_debug->SetAttribute( attribute , attributedata );

	TiXmlText* option_value = new TiXmlText( value );
	fileconfiguration_debug->LinkEndChild( option_value );

	node->LinkEndChild( fileconfiguration_debug );
}

void find_fileconfiguration( TiXmlElement* pElement , const char* filepath )
{	
	char buffer[128] = STR_FALSE;
	TiXmlNode* pNode = pElement->FirstChild();
	while( pNode )
	{		
		if ( strcmp( pNode->Value() , ELEMENT_NAME ) )
		{
			TiXmlNode* ptemp = pNode->NextSibling();
			pNode = ptemp;
			continue;
		}

		const char* text = pNode->FirstChild()->ToText()->Value();
		const char* check_str = pNode->ToElement()->Attribute( ATTRIBUTE_NAME );

		if ( !strcmp( check_str , ATTRIBUTE_TEXT_DEBUG ) )
		{
			strcpy_s( buffer , pNode->FirstChild()->ToText()->Value() );
			if ( !strcmp( buffer , STR_TRUE ) )
			{		
				g_except_file_table.push_back( filepath );
			}
		}

		TiXmlNode* ptemp = pNode->NextSibling();
		pElement->RemoveChild( pNode );
		pNode = ptemp;
	}

	push_excludedfrombuild( pElement , ELEMENT_NAME, ATTRIBUTE_NAME, ATTRIBUTE_TEXT_DEBUG, buffer );
	push_excludedfrombuild( pElement , ELEMENT_NAME, ATTRIBUTE_NAME, ATTRIBUTE_TEXT_DEBUG_UNITY, "true" );

	push_excludedfrombuild( pElement , ELEMENT_NAME, ATTRIBUTE_NAME, ATTRIBUTE_TEXT_RELEASE, buffer );
	push_excludedfrombuild( pElement , ELEMENT_NAME, ATTRIBUTE_NAME, ATTRIBUTE_TEXT_RELEASE_UNITY, "true" );	

}

void search_node( TiXmlNode* pNode , std::vector<std::string>& file_table )
{
	if ( pNode == NULL )
		return;

	for ( pNode ; pNode ; pNode = pNode->NextSibling( "ItemGroup" ) )
	{
		TiXmlNode* pChild = pNode->FirstChild();
		for( pChild ; pChild ; pChild = pChild->NextSibling("ClCompile" ) )
		{
			const char* filepath = 
				pChild->ToElement()->Attribute( "Include" );
			if ( filter_file( filepath ) )
			{
				find_fileconfiguration( pChild->ToElement() , filepath );
				file_table.push_back( std::string( filepath ) );
			}
		}
	}
}

void clear_unity_file( TiXmlNode* pRoot )
{
	if ( pRoot == NULL )
		return;

	bool check = false;
	TiXmlNode* pNode = pRoot->FirstChild( "ItemGroup" );
	while( pNode )
	{
		TiXmlNode* pChild = pNode->FirstChild();
		for( pChild ; pChild ; pChild = pChild->NextSibling("ClCompile" ) )
		{
			const char* filepath = pChild->ToElement()->Attribute( "Include" );
			
			if( filepath == NULL ) 
				continue;

			if( !strcmp( filepath , "Unity_1.cpp" ) )
			{
				check = true;			
				break;
			}
		}		

		TiXmlNode* ptemp = pNode->NextSibling();		
		if ( check )
		{
			pRoot->RemoveChild( pNode );
		}
		pNode = ptemp;

	}
}

void add_unity_file( TiXmlNode* pRoot )
{

	TiXmlNode* punity_Itemgroup = new TiXmlElement("ItemGroup");

	for( int i=0 ; i < UNITY_NUM ; i++ )
	{
		char fname[MAX_PATH];
		sprintf_s(fname, "Unity_%d.cpp", i+1);
		TiXmlElement* unity_file = new TiXmlElement("ClCompile");
		unity_file->SetAttribute( "Include" , fname );		


		push_excludedfrombuild( unity_file
			, ELEMENT_NAME 
			, ATTRIBUTE_NAME
			, ATTRIBUTE_TEXT_DEBUG
			, "true" );

		push_excludedfrombuild( unity_file
			, ELEMENT_NAME
			, ATTRIBUTE_NAME
			, ATTRIBUTE_TEXT_DEBUG_UNITY
			, "false" );

		push_excludedfrombuild( unity_file
			, ELEMENT_NAME
			, ATTRIBUTE_NAME
			, ATTRIBUTE_TEXT_RELEASE
			, "true" );

		push_excludedfrombuild( unity_file
			, ELEMENT_NAME
			, ATTRIBUTE_NAME
			, ATTRIBUTE_TEXT_RELEASE_UNITY
			, "false" );

		punity_Itemgroup->LinkEndChild( unity_file );

	}		

	pRoot->LinkEndChild( punity_Itemgroup );
}

void make_solution_for_ub( project_info_table& info )
{	
	TiXmlDocument root;
	std::string path = util::string::wc2mb( info.project_file_path.c_str() );
	root.LoadFile( path.c_str() );

	std::string backup_path = path + "_backup";
	root.SaveFile( backup_path.c_str() );

	if ( root.Error() == true )
	{
		printf( "loading error file : %s errorID : %d , errorRow : %d errorCol : %d"
				, path.c_str()
				, root.ErrorId()
				, root.ErrorRow()
				, root.ErrorCol()
				);

		return;
	}

	TiXmlNode* pRoot = root.FirstChildElement("Project");
	if ( pRoot == NULL )
		return;

	TiXmlNode* pItemGroup = pRoot->FirstChild("ItemGroup");
	if ( pItemGroup == NULL )
		return;

	
	search_node( pItemGroup , info.filepath_table );

	print_unity_x( info );
	
	clear_unity_file( pRoot );
	add_unity_file( pRoot );


	root.SaveFile( path.c_str() );
	root.Clear();
}

int _tmain(int argc, _TCHAR* argv[])
{
	const TCHAR* path = NULL;
	
	if ( argc < 2 )
	{
		TCHAR directory[MAX_PATH];	
		::GetCurrentDirectory(MAX_PATH, directory);
		path = directory;
	}
	else
	{
		path = argv[1];
	}
	
	g_all_project_info.clear();
	search_solution( path );
	
	for( int i=0 ; i < (int)g_all_project_info.size() ; i++ )
	{
		g_except_file_table.clear();
		project_info_table& info = g_all_project_info.at(i);		
		make_solution_for_ub( info );		
	}
	
	return 0;
}
