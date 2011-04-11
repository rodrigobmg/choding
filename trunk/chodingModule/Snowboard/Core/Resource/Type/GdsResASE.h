#ifndef _GDS_CORE_RESOURCE_TYPE_ASE_H_
#define _GDS_CORE_RESOURCE_TYPE_ASE_H_

#include "GdsResBaseType.h"
#include "..\..\..\System\FileSystem\GdsFile.h"
#include "../../SceneNode\GdsNode.h"
#include "../../Property/GdsMaterialProperty.h"

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
	bool				DecodeMaterial( LineContainerA::iterator& line , GdsMaterialPropertyPtr Material );
	bool				DecodeMap( LineContainerA::iterator& line, GdsMaterialPropertyPtr Material );

	bool				DecodeGEOMOBJECT( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeTM( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_VERTEX_LIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_FACE_LIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_TVERTLIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_TFACELIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_CVERTEX( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_NORMALS( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeANIMATION( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodePOS_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeSCALE_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeROT_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );

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

	int					m_iSubMaterial;
	int					m_iCountSubMaterial;

	int					m_iMeshRef;
	
	GdsNodePtr			m_RootNode;

	typedef std::vector<GdsNodePtr> NODE_LIST;
	NODE_LIST			m_vecNodeList;

	typedef std::vector<GdsMaterialPropertyPtr> MATERIAL_LIST;
	MATERIAL_LIST		m_vecMaterialList;

	bool				attachNode( GdsNodePtr childNode , tstring& parentname );
	bool				parseNode( NODE_LIST& nodelist );
	bool				parseMaterial( NODE_LIST& nodelist , LPDIRECT3DDEVICE9 device );

public:
	GdsResASE();
	virtual ~GdsResASE();
	
	virtual void		vClear();
	virtual HRESULT		vRelease();
	virtual HRESULT		vLoadResource( LPDIRECT3DDEVICE9 device );

	GdsNodePtr			GetNode(){ return m_RootNode; }
		
};

typedef boost::shared_ptr< GdsResASE >	GdsResASEPtr;

#endif
