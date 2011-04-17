#ifndef _GDS_CORE_RESOURCE_TYPE_ASE_H_
#define _GDS_CORE_RESOURCE_TYPE_ASE_H_

#include "../GdsResBaseType.h"
#include "..\..\..\System\FileSystem\GdsFile.h"
#include "../../SceneNode\GdsNode.h"

class GdsResASE : public GdsResBase
{

	struct sVERTEX
	{
		enum _FVF { FVF= (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 ) };

		D3DXVECTOR3 p;
		D3DXVECTOR3 n;
		float u, v;
		//	DWORD color;
		sVERTEX():p(0,0,0),n(0,0,0),u(0),v(0)//,color(0xffffffff)
		{
		}
	};

	struct TEXCOORDFLOAT{
		union{
			float u;
			float uz;
		};
		union{
			float v;
			float vz;
		};
		float oz;
	};

	struct TRIANGLE{
		int VertexIndex[3];
		DWORD VertexColor[3];
		D3DXVECTOR3 VertexNormal[3];
		D3DXVECTOR3 FaceNormal;
		TEXCOORDFLOAT VertexTexture[3];
		int MaterialID;
		bool bView;
		
		void Init()
		{
			for(int i=0; i < 3; i++)
			{
				VertexIndex[i] = 0;
				VertexColor[i] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				VertexNormal[i] = D3DXVECTOR3(0, 0, 0);
				FaceNormal = D3DXVECTOR3(0, 0, 0);
				VertexTexture[i].u = 0;
				VertexTexture[i].v = 0;
			}

			MaterialID = 0;

			bView = true;
		}
	};

	D3DXVECTOR3*		m_VertexList;
	TRIANGLE*			m_TriangleList;

	bool				DecodeSCENE( LineContainerA::iterator& line );
	bool				DecodeMATERIAL_LIST( LineContainerA::iterator& line );	
	bool				DecodeMaterial( LineContainerA::iterator& line , GdsMaterialPtr Material );
	bool				DecodeMap( LineContainerA::iterator& line, GdsMaterialPtr Material );

	bool				DecodeGEOMOBJECT( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeTM( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_VERTEX_LIST( LineContainerA::iterator& line , D3DXVECTOR3* pVertexList );
	bool				DecodeMESH_FACE_LIST( LineContainerA::iterator& line , TRIANGLE* pTriangleList );
	bool				DecodeMESH_TVERTLIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_TFACELIST( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_CVERTEX( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeMESH_NORMALS( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeANIMATION( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodePOS_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeSCALE_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );
	bool				DecodeROT_TRACK( LineContainerA::iterator& line , GdsNodePtr pNode );

	bool				CheckKeyword( const char* keyword , LineContainerA::iterator& line );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3);
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , int& ivalue );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , std::string& str );
	bool				GetValue( const char* keyword , LineContainerA::iterator& line , const char* SEP , float& fvalue1, float& fvalue2, float& fvalue3 , float& favlue4 );
	bool				GetValue_FACELIST( const char* keyword , LineContainerA::iterator& line , const char* SEP 
										, const char* subkeyword1 , int& ivalue1 
										, const char* subkeyword2 , int& ivalue2 
										, const char* subkeyword3 , int& ivalue3 
										, const char* subkeyword4 , int& ivalue4 
										);

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

	typedef std::vector<GdsMaterialPtr> MATERIAL_LIST;
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
