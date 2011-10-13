#include "Precompiled.h"
#include "sunParserXML.h"



sunParserXML::sunParserXML() :
m_spEntity(NULL)
{
	m_vecMeshInfo.clear();
	m_MaterialInfo_List.clear();

	m_vecMeshInfo.reserve(50);
}

sunParserXML::~sunParserXML()
{

}

bool sunParserXML::Load(const tstring& strFileName)
{
	if(strFileName.length() <= 0)
	{
		//에러 로그 남기고 
		assert(0 && "FileName.length <= 0");
		return false;
	}

	sunStream::SetFileName(strFileName);

	// 파싱 시작.~~	
		
	if( false == pXmlDoc.LoadFile(_W2M(strFileName ) ) )
	{
		//에러 로그
		assert(0 && " pXmlDoc.LoadFile is Failed ");
		return false;
	
	}
	
	TiXmlNode* pRootNode;

	pRootNode = pXmlDoc.FirstChild("Body");

	if( NULL == pRootNode)
	{
		// 에러 로그 출력
		assert(0 && " pRootNode is NULL ");
		return false;
	}
	
	m_vecMeshInfo.clear();
		
	if(false == ParseMaterialInfo(pRootNode))
	{
		assert( 0 && "ParseObject is false");

	}

	if(false == ParseObject(pRootNode))
	{
		assert( 0 && "ParseObject is false");

	}

	if(false == SortNodes())
		assert(0 && "SortNodes is  Failed");

	return true;
}

bool sunParserXML::ParseMaterialInfo(TiXmlNode* pRootNode)
{

	TiXmlNode* pMaterialNode = pRootNode->FirstChild("Material");

	if( false == pMaterialNode)
	{
		//에러 로그
		assert(0 && " pMaterialNode is NULL ");
		return false;
	}	
	
	TiXmlNode* pSlotNode = pMaterialNode->FirstChild("Slot");
	int iMaterialIndex = -1;

	while(pSlotNode)
	{

		//메테리얼 Info 객체 생성
		stMaterialInfo* pMaterialInfo = new stMaterialInfo;

		assert(pMaterialInfo);

		pSlotNode->ToElement()->QueryIntAttribute("Index", &iMaterialIndex);
		m_MaterialInfo_List.insert(make_pair(iMaterialIndex, pMaterialInfo));

		// Diffuse Parsing
		TiXmlNode* pDiffuseNode = pSlotNode->FirstChild("Diffuse");
		if(pDiffuseNode)
			ParseColor(pDiffuseNode, pMaterialInfo->kMaterial.Diffuse);

		// Ambient Parsing
		TiXmlNode* pAmbientNode = pSlotNode->FirstChild("Ambient");
		if(pAmbientNode)
			ParseColor(pAmbientNode, pMaterialInfo->kMaterial.Ambient);

		// Specular Parsing
		TiXmlNode* pSpecularNode = pSlotNode->FirstChild("Specular");
		if(pSpecularNode)
			ParseColor(pSpecularNode, pMaterialInfo->kMaterial.Specular);

		// Emissive Parsing
		TiXmlNode* pEmissiveNode = pSlotNode->FirstChild("Emissive");
		if(pEmissiveNode)
			ParseColor(pEmissiveNode, pMaterialInfo->kMaterial.Emissive);

	
		const char* pStrDiffuseMap;

		// DiffuseMap 파싱
		TiXmlNode* pDiffuseMapNode = pSlotNode->FirstChild("DiffuseMap");
		
		
		char szDrive[_MAX_DRIVE] = {0,};
		char szDir[_MAX_DIR]	 = {0,};
		char szFname[_MAX_FNAME] = {0,};
		char szExt[_MAX_EXT]	 = {0,};
		
		string  strFileName;

		if(pDiffuseMapNode)
		{
			pStrDiffuseMap = _GetTextFromXML(pDiffuseMapNode);					

			_splitpath_s(pStrDiffuseMap, szDrive, szDir, szFname, szExt); strFileName = szFname; strFileName += szExt;
			pMaterialInfo->strFileName = _M2W(strFileName.c_str());
		}

		m_MaterialInfo_List.insert(make_pair(iMaterialIndex,pMaterialInfo) );
			
		pSlotNode = pSlotNode->NextSibling();
	}


	return true;
}

bool sunParserXML::ParseColor(TiXmlNode* pColorNode, D3DCOLORVALUE& kMaterialColor)
{

	stVector3f kColor = _StrToColor3f(_GetTextFromXML(pColorNode));
	D3DCOLORVALUE kColorValue =  {kColor.x, kColor.y, kColor.z, 0};
	kMaterialColor = kColorValue;	

	return true;
}


bool sunParserXML::ParseObject(TiXmlNode* pRootNode)	
{
	TiXmlNode* pObjectNode = pRootNode->FirstChild("Object");
	
	if( false == pObjectNode)
	{
		//에러 로그
		assert(0 && " pObjectNode is NULL ");
		return false;
	}
		

	int iNodeID   = -1;
	tstring strNodeName;

	while(pObjectNode)
	{
		stElemInfo elementInfo;

		pObjectNode->ToElement()->QueryIntAttribute("ID", &iNodeID);
		elementInfo.iNode = iNodeID;


		sunRenderNodePtr spRenderNode = new sunRenderNode;
		assert(NULL != spRenderNode);

		elementInfo.spRenderNode = SmartPointerCast(sunNode, spRenderNode);
			
		strNodeName = _strConvM2W( pObjectNode->ToElement()->Attribute("Name") );
		elementInfo.spRenderNode->SetName(strNodeName);
		

		if(false == ParseParent(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseParent is false");
			//에러 로그
			return false;
		}

		if(false == ParseTM(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseTM is false" );
			//에러 로그
			return false;
		}

		if(false == ParseBoundingBox(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseBoundingBox is false" );
			//에러 로그
			return false;
		}
			
		if(false == ParseVertex(pObjectNode, &elementInfo))
		{
			//임시로 이렇게 하자.
//			m_vecMeshInfo.push_back(elementInfo);
			pObjectNode = pObjectNode->NextSibling();
			continue;
	//		assert(0 && "ParseVertex is false"); Vertex가 없을 수도 있다.
			//에러 로그
			//return false;
		}

		if(false == ParseVertexNormal(pObjectNode, &elementInfo))
		{
			//임시로 이렇게 하자.
//			m_vecMeshInfo.push_back(elementInfo);
//			pObjectNode = pObjectNode->NextSibling();
//			continue;
	//		assert(0 && "ParseVertexNormal is false"); VertexNormal가 없을 수도 있다.
			//에러 로그
			//return false;
		}
		
		if(false == ParseTVertex(pObjectNode, &elementInfo))
		{

		}
			
		if(false == ParseTriIndex(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseTriIndex return false"); 
			//에러 로그
			return false;
		}	

		if(false == ParseTexIndex(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseTexIndex return false"); 
			//에러 로그
			return false;
		}	

		ParseKey(pObjectNode, &elementInfo);

		// 버텍스가 최소 한개 존재해야 Mesh 생성
		if( elementInfo.vecVertex.size() <= 0)
			continue;
			
		sunMeshPtr spMesh = new sunMesh;
		
		spRenderNode->SetMesh(spMesh);
		spMesh->SetVertexInfo(&elementInfo.vecVertex, &elementInfo.vecVertexNormal, &elementInfo.vecTVertex);
		spMesh->SetIndexInfo(&elementInfo.vecIndex);
		spMesh->CreateVIB();

		if(NULL != elementInfo.spTrack)
			spRenderNode->SetTrack(elementInfo.spTrack);

		MaterialInfo_ITERATOR Material_IT = m_MaterialInfo_List.find(elementInfo.iMaterialID);
		if(Material_IT != m_MaterialInfo_List.end())
		{

			stMaterialInfo *kMaterialInfo = static_cast<stMaterialInfo*>(Material_IT->second);
			
			sunMaterialPtr spMaterial = new sunMaterial;

			spMaterial->SetMaterialInfo(kMaterialInfo->kMaterial);

			// 텍스쳐가 있으면.
			if(kMaterialInfo->strFileName.length() > 0)
			{
				sunTexturePtr spTexture = new sunTexture;

				if(true == spTexture->Load(kMaterialInfo->strFileName) )
				{
					spMaterial->SetTexture(spTexture, 0);
				}
			}

			spRenderNode->SetMaterial(spMaterial);
			

		}

		m_vecMeshInfo.push_back(elementInfo);
		pObjectNode = pObjectNode->NextSibling();
	}

	return true;
}

bool sunParserXML::ParseParent(TiXmlNode* pObjectNode,stElemInfo* pElemInfo)
{
	TiXmlNode* pParentNode = pObjectNode->FirstChild("Parent");
	int iParentID = -1;

	if(NULL == pParentNode || NULL == pElemInfo)
	{
		//에러 로그
		assert( 0 && "pParentNode == NULL || pElemInfo == NULL");
		return false;
	}

	pParentNode->ToElement()->QueryIntAttribute("ID",&iParentID);
	pElemInfo->iParentNode = iParentID;


	return true;
}


bool sunParserXML::ParseTM(TiXmlNode* pObjectNode,stElemInfo* pElemInfo)
{
	TiXmlNode* pLocalNode = pObjectNode->FirstChild("LocalTM");

	if(NULL == pLocalNode || NULL == pElemInfo)
	{
		//에러 로그
		assert(0 && " NULL == pLocalNode || NULL == pElemInfo ");
		return false;
	}
	
	TiXmlNode *pLocalValuexNode = pLocalNode->FirstChild("Value");

	if(pLocalValuexNode)
	{
		//로컬 좌표 파싱
		stMat16 kLocalMat16;
		for(int i =0 ; NULL != pLocalValuexNode; ++i)
		{
			assert(i < 4 && "행렬 크기 오버플로우.");

			kLocalMat16.kRow[i] = _StrToVector4f( _GetTextFromXML(pLocalValuexNode) );

			pLocalValuexNode = pLocalValuexNode->NextSibling();
		}
		// 로컬 좌표 세팅
		pElemInfo->spRenderNode->SetLocalMatrix(kLocalMat16);

	}


	// 월드 좌표 파싱
	stMat16 kWorldMat16;
	TiXmlNode* pWorldNode = pObjectNode->FirstChild("WorldTM");
	
	if(pWorldNode)
	{
		TiXmlNode *pWorldValuexNode = pWorldNode->FirstChild("Value");

		for(int i =0 ; NULL != pWorldValuexNode; ++i)
		{
			assert(i < 4 && "행렬 크기 오버플로우.");

			kWorldMat16.kRow[i] = _StrToVector4f( _GetTextFromXML(pWorldValuexNode) );	

			pWorldValuexNode = pWorldValuexNode->NextSibling();

		}

		// 월드 좌표 세팅
		pElemInfo->spRenderNode->SetWorldMatrix(kWorldMat16);
	}


	return true;
}

bool sunParserXML::ParseBoundingBox(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo)
{
	TiXmlNode* pBoundingBoxNode = pObjectNode->FirstChild("BoundingBox");

	if(NULL == pBoundingBoxNode || NULL == pElemInfo)
	{
		//에러 로그
		assert( 0 && "NULL == pBoundingBoxNode || NULL == pElemInfo");
		return false;
	}

	return true;
}

bool sunParserXML::ParseVertex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{

	TiXmlNode* pVertexNode = pObjectNode->FirstChild("Vertex");

	if(NULL == pVertexNode || NULL == pElemInfo)
	{
		return false;
	}
	
	TiXmlNode *pVertexValueNode = pVertexNode->FirstChild("Value");

	
	while(pVertexValueNode)
	{
		stVector3f kvertex3f;
		kvertex3f = _StrToVector3f( _GetTextFromXML(pVertexValueNode) );

		pElemInfo->vecVertex.push_back(kvertex3f);

		pVertexValueNode = pVertexValueNode->NextSibling();
	}

	return true;
}

bool sunParserXML::ParseVertexNormal(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{

	TiXmlNode* pVertexNormalNode = pObjectNode->FirstChild("VertexNormal");

	if(NULL == pVertexNormalNode || NULL == pElemInfo)
	{
// 		에러 로그
// 		assert( 0 && "NULL == pVertexNode || NULL == pElemInfo");
		return false;
	}

	TiXmlNode *pVertexNormalValueNode = pVertexNormalNode->FirstChild("Value");

	while (pVertexNormalValueNode)
	{
		stVector3f kVertex3f;
		kVertex3f = _StrToVector3f( _GetTextFromXML(pVertexNormalValueNode) );

		pElemInfo->vecVertexNormal.push_back(kVertex3f);

		pVertexNormalValueNode = pVertexNormalValueNode->NextSibling();
	}

	return true;
}


bool sunParserXML::ParseTVertex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{

	TiXmlNode* pTVertexNode = pObjectNode->FirstChild("TexVertex");

	if(NULL == pTVertexNode || NULL == pElemInfo)
	{
		// 		에러 로그
		// 		assert( 0 && "NULL == pVertexNode || NULL == pElemInfo");
		return false;
	}

	TiXmlNode *pTVertexValueNode = pTVertexNode->FirstChild("Value");
	
	while(pTVertexValueNode)
	{
		stVector2f kVertex2f;
		kVertex2f = _StrToVector2f( _GetTextFromXML(pTVertexValueNode));
		
		pElemInfo->vecTVertex.push_back(kVertex2f);

		pTVertexValueNode = pTVertexValueNode->NextSibling();
	}

	return true;
}

bool sunParserXML::ParseTriIndex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{
	
	TiXmlNode* pTriIndexNode = pObjectNode->FirstChild("TriIndex");

	if(NULL == pTriIndexNode || NULL == pElemInfo)
	{
		// 에러 로그
		assert( 0 && "NULL == pTriIndexNode || NULL == pElemInfo");
		return false;
	}

	TiXmlNode *pTriIndexValueNode = pTriIndexNode->FirstChild("Value");
	

	int iMaterialID = -1;
	pTriIndexValueNode->ToElement()->QueryIntAttribute("MaterialID", &iMaterialID);
	pElemInfo->iMaterialID = iMaterialID;

	while(pTriIndexValueNode)
	{
		stVector3i kVectex3i;
		kVectex3i = _StrToVector3i( _GetTextFromXML(pTriIndexValueNode) );			
		
		pElemInfo->vecIndex.push_back(kVectex3i);
			
		pTriIndexValueNode = pTriIndexValueNode->NextSibling();
	}

	return true;
}

bool sunParserXML::ParseTexIndex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{

	//TiXmlNode* pTriIndexNode = pObjectNode->FirstChild("TriIndex");

	//if(NULL == pTriIndexNode || NULL == pElemInfo)
	//{
	//	// 에러 로그
	//	assert( 0 && "NULL == pTriIndexNode || NULL == pElemInfo");
	//	return false;
	//}

	//TiXmlNode *pTriIndexValueNode = pTriIndexNode->FirstChild("Value");


	//int iMaterialID = -1;
	//pTriIndexValueNode->ToElement()->QueryIntAttribute("MaterialID", &iMaterialID);
	//pElemInfo->iMaterialID = iMaterialID;

	//while(pTriIndexValueNode)
	//{
	//	stVector3i kVectex3i;
	//	kVectex3i = _StrToVector3i( _GetTextFromXML(pTriIndexValueNode) );			

	//	pElemInfo->vecIndex.push_back(kVectex3i);

	//	pTriIndexValueNode = pTriIndexValueNode->NextSibling();
	//}

	return true;
}


bool sunParserXML::ParseKey(TiXmlNode* pObjectNode, stElemInfo* pElemInfo)
{
	TiXmlNode* pkeyNode = pObjectNode->FirstChild("Key");
	
	if(NULL == pkeyNode || NULL == pElemInfo)
	{
		
	//	assert( 0 && "NULL == pkeyNode || NULL == pElemInfo");
		return false;
	}
	
	sunTrackPtr spTrack = new sunTrack;
	stKeys kKeys;

	float fAniStart = 0.0f;
	float fAniEnd	= 0.0f;

	TiXmlNode* pAniStartNode = pkeyNode->FirstChild("AnimationStart");
	TiXmlNode* pAniEndNode	 = pkeyNode->FirstChild("AnimationEnd");
	

	fAniStart = (float)atof(_GetTextFromXML(pAniStartNode));	
	fAniEnd	  = (float)atof(_GetTextFromXML(pAniEndNode));	

	spTrack->SetStartFrame(fAniStart);
	spTrack->SetEndFrame(fAniEnd);

	
	TiXmlNode* pPositionNode = pkeyNode->FirstChild("Position");
	
	if(pPositionNode)
	{
		int			iFrame = -1;
		D3DXVECTOR3 vPosition(0.0f,0.0f,0.0f);

		TiXmlNode* pPositionValueNode = pPositionNode->FirstChild("Value");
		
		while(pPositionValueNode)
		{
			stKeyPos kPosition;

			pPositionValueNode->ToElement()->QueryIntAttribute("Frame", &iFrame);
			vPosition =   _StrToVector3( _GetTextFromXML(pPositionValueNode) );
			
			kPosition.iFrame = iFrame;
			kPosition.vPos	 = vPosition;
		
			spTrack->GetKeys().vecPos.push_back(kPosition);

			pPositionValueNode = pPositionValueNode->NextSibling();
		}
	}


	TiXmlNode* pQuaternionNode = pkeyNode->FirstChild("Quaternion");
	if(pQuaternionNode)
	{
		int			iFrame = -1;
		D3DXVECTOR4 qRot(0.0f,0.0f,0.0f,0.0f);

		TiXmlNode* pQuaternionValueNode = pQuaternionNode->FirstChild("Value");

		while(pQuaternionValueNode)
		{
			stKeyRot kRot;

			pQuaternionValueNode->ToElement()->QueryIntAttribute("Frame", &iFrame);
			qRot =   _StrToVector4( _GetTextFromXML(pQuaternionValueNode) );

			kRot.iFrame = iFrame;
			kRot.qRot	= D3DXQUATERNION(qRot.x,qRot.y, qRot.z, qRot.w) ;

			spTrack->GetKeys().vecRot.push_back(kRot);

			pQuaternionValueNode = pQuaternionValueNode->NextSibling();
		}
	}


	TiXmlNode* pScaleNode = pkeyNode->FirstChild("Scale");
	if(pScaleNode)
	{
		int			iFrame = -1;
		D3DXVECTOR3 vScale(0.0f,0.0f,0.0f);

		TiXmlNode* pScaleValueNode = pScaleNode->FirstChild("Value");

		while(pScaleValueNode)
		{
			stKeyScl kScale;

			pScaleValueNode->ToElement()->QueryIntAttribute("Frame", &iFrame);
			vScale =   _StrToVector3( _GetTextFromXML(pScaleValueNode) );

			kScale.iFrame	= iFrame;
			kScale.vScale	= vScale;

			spTrack->GetKeys().vecScl.push_back(kScale);

			pScaleValueNode = pScaleValueNode->NextSibling();
		}
	}

	pElemInfo->spTrack = spTrack;

	return true;
}



bool sunParserXML::SortNodes()
{
	m_spEntity = new sunNode;
	
	bool bFoundParent = false;
	
	BOOST_FOREACH( stElemInfo elemInfo, m_vecMeshInfo)
	{
		if( -1 == elemInfo.iParentNode)
		{
			m_spEntity->AddChild( SmartPointerCast(sunNode, elemInfo.spRenderNode) );
			continue;
		}

		bFoundParent = false;
		BOOST_FOREACH( stElemInfo parentElemInfo, m_vecMeshInfo)
		{
			if( elemInfo.iParentNode == parentElemInfo.iNode)
			{
				parentElemInfo.spRenderNode->AddChild(SmartPointerCast(sunNode, elemInfo.spRenderNode));
				bFoundParent = true;
				break;
			}
		}

		if( false == bFoundParent)
		{
			assert(0 && "ParentNode found't !!");
			return false;
		}

	}

	return true;
}

sunNodePtr sunParserXML::GetLoadedEntity()
{
	assert( m_spEntity && "m_spEntity is NULL ");

	return m_spEntity;
}


void sunParserXML::Destory()
{
	MaterialInfo_ITERATOR it = m_MaterialInfo_List.begin();

	for( ; it != m_MaterialInfo_List.end(); ++it)
	{
		stMaterialInfo* pkMaterialInfo = (stMaterialInfo*) it->second;
		Safe_Delete(pkMaterialInfo);
	}
}