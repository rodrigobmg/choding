#ifndef _GDS_CORE_RESOURCE_TYPE_ASE_H_
#define _GDS_CORE_RESOURCE_TYPE_ASE_H_

#include "GdsResBaseType.h"
#include "..\..\..\System\FileSystem\GdsFile.h"
#include "../../SceneNode\GdsNode.h"

class GdsResASE : public GdsResBase
{
// 	struct getValue
// 	{
// 		bool operator ()( LineContainerA::iterator& line , const char* SEP , float& fvalue )
// 		{
// 			char* in_str = *line;
// 			char* context = NULL;
// 
// 			strtok_s( in_str , SEP ,&context );
// 			fvalue = atof( context );
// 
// 			//한줄 점프~
// 			++line;
// 
// 			return true;
// 		};
// 	};

	bool				DecodeSCENE( LineContainerA::iterator& line );
	bool				DecodeMATERIAL_LIST( LineContainerA::iterator& line );
	bool				DecodeGEOMOBJECT( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMaterial( LineContainerA::iterator& line );
	bool				DecodeMap( LineContainerA::iterator& line );

	bool				CheckKeyword( const char* keyword , LineContainerA::iterator& line );
//	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , getValue functor );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3);
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , int& ivalue );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , std::string& const str );

	int					m_iCountBone;

	float				m_fFirstFrame;
	float				m_fLastFrame;
	float				m_fFrameSpeed;
	float				m_fTickPerFrame;

	int					m_iCountMaterial;
	int					m_iSubMaterial;
	int					m_iCountSubMaterial;

	float				m_fAmbientR;
	float				m_fAmbientG;
	float				m_fAmbientB;

	float				m_fDiffuseR;
	float				m_fDiffuseG;
	float				m_fDiffuseB;

	float				m_fSpecularR;
	float				m_fSpecularG;
	float				m_fSpecularB;

	std::string				m_strTextureName;

	int					m_iMeshRef;
	
	GdsNodePtr			m_RootNode;

public:
	GdsResASE();
	virtual ~GdsResASE();
	
	virtual void		vClear();
	virtual HRESULT		vRelease();
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device );
		
};

typedef boost::shared_ptr< GdsResASE >	GdsResASEPtr;

#endif
