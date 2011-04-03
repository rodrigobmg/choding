#ifndef _SUN_RESOURCE_SUNCONVERT_H_
#define _SUN_RESOURCE_SUNCONVERT_H_


#define _M2W(p) _strConvM2W(p)
#define _W2M(p) _strConvW2M(p)



//멀티바이트 -> 유니코드
inline wstring _strConvM2W(const string& _src)
{
	LPWSTR szTextW = new WCHAR[_src.size() + 1];
	MultiByteToWideChar(CP_ACP, 0, _src.c_str(), -1, szTextW, (int)_src.size()+1);
	wstring strW = szTextW;
	delete[] szTextW;
	return strW;
}

//유니코드 -> 멀티바이트

inline string _strConvW2M(const wstring& _wsrc)
{
	int len = WideCharToMultiByte(CP_ACP, 0, _wsrc.c_str(), -1, NULL, 0, NULL, NULL);
	LPSTR szText = new CHAR[len];
	WideCharToMultiByte(CP_ACP, 0, _wsrc.c_str(), -1, szText, len , 0, 0);
	string str = szText;
	delete[] szText;
	return str;
}

// 문자열 -> D3DXVECTOR4
inline D3DXVECTOR4 _StrToVector4(const char* str)
{
	D3DXVECTOR4		vec4;
	char*			sep = ",";
	char*			buff = const_cast<char*>(str);
	char*			context;

	vec4.x = (float)atof( strtok_s( buff, sep, &context )) ;
	vec4.y = (float)atof( strtok_s( NULL, sep, &context )) ;
	vec4.z = (float)atof( strtok_s( NULL, sep, &context )) ;
	vec4.w = (float)atof( strtok_s( NULL, sep, &context )) ;

	return vec4;
}


// 문자열 -> D3DXVECTOR3
inline D3DXVECTOR3 _StrToVector3(const char* str)
{
	D3DXVECTOR3		vec3;
	char*			sep = ",";
	char*			buff = const_cast<char*>(str);
	char*			context;

	vec3.x = (float)atof( strtok_s( buff, sep, &context )) ;
	vec3.y = (float)atof( strtok_s( NULL, sep, &context )) ;
	vec3.z = (float)atof( strtok_s( NULL, sep, &context )) ;

	return vec3;
}

// 문자열 -> stVector3f
inline stVector3f _StrToVector3f(const char* str)
{
	stVector3f		vec3;
	char*			sep = "[],\n";
	char*			buff = const_cast<char*>(str);
	char*			context;

	vec3.x = (float)atof( strtok_s( buff, sep, &context )) ;
	vec3.y = (float)atof( strtok_s( NULL, sep, &context )) ;
	vec3.z = (float)atof( strtok_s( NULL, sep, &context )) ;

	return vec3;
}

// 문자열 -> stVector3i
inline stVector3i _StrToVector3i(const char* str)
{
	stVector3i		vec3;
	char*			sep = "[],\n";
	char*			buff = const_cast<char*>(str);
	char*			context;

	vec3.x = (int)atoi( strtok_s( buff, sep, &context )) ;
	vec3.y = (int)atoi( strtok_s( NULL, sep, &context )) ;
	vec3.z = (int)atoi( strtok_s( NULL, sep, &context )) ;

	return vec3;
}


// 문자열 -> D3DXVECTOR2
inline D3DXVECTOR2 _StrToVector2(const char* str)
{

	if(str == NULL)
		return D3DXVECTOR2(0.0f, 0.0f);

	D3DXVECTOR2		vec2;
	char*			sep = ",";
	char*			buff = const_cast<char*>(str);
	char*			context;

	vec2.x = (float)atof( strtok_s( buff, sep, &context )) ;
	vec2.y = (float)atof( strtok_s( NULL, sep, &context )) ;

	return vec2;
}

// 문자열 -> 정점 인덱스 [3]
inline void _StrToIndex( const char* str, WORD* wIndex )
{
	char*	sep = ",";
	char*	buff = const_cast<char*>(str);
	char*	context;

	wIndex[0] = atoi( strtok_s( buff, sep, &context ) );
	wIndex[1] = atoi( strtok_s( NULL, sep, &context ) );
	wIndex[2] = atoi( strtok_s( NULL, sep, &context ) );	
}

// XML 노드로부터 Text 값 읽어오기 2
inline const char* _GetTextFromXML(TiXmlNode* pNode)
{

	if(pNode != NULL)
		return pNode->FirstChild()->ToText()->Value();
	else
		return NULL;
}



// D3DXVECTOR4(Trans) -> D3DXMATRIX

inline D3DXMATRIXA16 _GetVect4toMatrixTran(D3DXVECTOR3 vVec3)
{

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);


	mat._41 = vVec3.x;
	mat._42 = vVec3.y;
	mat._43 = vVec3.z;



	return mat;

}

inline D3DXMATRIXA16 _GetVect4toMatrixScale(D3DXVECTOR3 vVec3)
{

	D3DXMATRIXA16 mat;
	D3DXMatrixIdentity(&mat);


	mat._11 = vVec3.x;
	mat._22 = vVec3.y;
	mat._33 = vVec3.z;


	return mat;

}


inline stVector4f _StrToVector4f(const char *_pStr )
{
	stVector4f		vec4f;
	char*			sep = "[],\n";
	char*			buff = const_cast<char*>(_pStr);
	char*			context;


	vec4f.x = static_cast<float>(atof( strtok_s(buff, sep ,&context) ) );
	vec4f.y = static_cast<float>(atof( strtok_s(NULL, sep ,&context) ) );
	vec4f.z = static_cast<float>(atof( strtok_s(NULL, sep ,&context) ) );
	vec4f.w = static_cast<float>(atof( strtok_s(NULL, sep ,&context) ) );

	return vec4f;
}

#endif