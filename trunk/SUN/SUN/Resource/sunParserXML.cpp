#include "Precompiled.h"
#include "sunParserXML.h"



sunParserXML::sunParserXML() :
m_spEntity(NULL)
{
	m_MeshInfolist.clear();

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
	
	m_MeshInfolist.clear();

	if(false == ParseObject(pRootNode))
	{
		assert( 0 && "ParseObject is false");

	}

	if(false == SortNodes())
		assert(0 && "SortNodes is  Failed");

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
//			m_MeshInfolist.push_back(elementInfo);
			pObjectNode = pObjectNode->NextSibling();
			continue;
	//		assert(0 && "ParseVertex is false"); Vertex가 없을 수도 있다.
			//에러 로그
			//return false;
		}

		if(false == ParseVertexNormal(pObjectNode, &elementInfo))
		{
			//임시로 이렇게 하자.
//			m_MeshInfolist.push_back(elementInfo);
			pObjectNode = pObjectNode->NextSibling();
			continue;
	//		assert(0 && "ParseVertexNormal is false"); VertexNormal가 없을 수도 있다.
			//에러 로그
			//return false;
		}
		
		if(false == ParseTriIndex(pObjectNode, &elementInfo))
		{
			assert(0 && "ParseTriIndex return false"); 
			//에러 로그
			return false;
		}	

		// 버텍스가 최소 한개 존재해야 Mesh 생성
		if( elementInfo.vecVertex.size() <= 0)
			continue;
			
		sunMeshPtr spMesh = new sunMesh;
		
		spRenderNode->SetMesh(spMesh);
		spMesh->SetVertexInfo(&elementInfo.vecVertex, &elementInfo.vecVertexNormal);
		spMesh->SetIndexInfo(&elementInfo.vecIndex);
		spMesh->CreateVIB();

		m_MeshInfolist.push_back(elementInfo);
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


	// 월드 좌표 파싱
	stMat16 kWorldMat16;
	TiXmlNode* pWorldNode = pObjectNode->FirstChild("WorldTM");
	
	TiXmlNode *pWorldValuexNode = pWorldNode->FirstChild("Value");

	for(int i =0 ; NULL != pWorldValuexNode; ++i)
	{
		assert(i < 4 && "행렬 크기 오버플로우.");
		
		kWorldMat16.kRow[i] = _StrToVector4f( _GetTextFromXML(pWorldValuexNode) );	

		pWorldValuexNode = pWorldValuexNode->NextSibling();

	}

	// 월드 좌표 세팅
	pElemInfo->spRenderNode->SetWorldMatrix(kWorldMat16);


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

	TiXmlNode* pVertexNode = pObjectNode->FirstChild("VertexNormal");

	if(NULL == pVertexNode || NULL == pElemInfo)
	{
// 		에러 로그
// 		assert( 0 && "NULL == pVertexNode || NULL == pElemInfo");
		return false;
	}

	TiXmlNode *pVertexNormalValueNode = pVertexNode->FirstChild("Value");

	while (pVertexNormalValueNode)
	{
		stVector3f kVertex3f;
		kVertex3f = _StrToVector3f( _GetTextFromXML(pVertexNormalValueNode) );

		pElemInfo->vecVertexNormal.push_back(kVertex3f);

		pVertexNormalValueNode = pVertexNormalValueNode->NextSibling();
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

	while(pTriIndexValueNode)
	{
		stVector3i kVectex3i;
		kVectex3i = _StrToVector3i( _GetTextFromXML(pTriIndexValueNode) );			
		
		pElemInfo->vecIndex.push_back(kVectex3i);
			
		pTriIndexValueNode = pTriIndexValueNode->NextSibling();
	}

	return true;
}


bool sunParserXML::SortNodes()
{
	m_spEntity = new sunNode;
	
	bool bFoundParent = false;
	
	BOOST_FOREACH( stElemInfo elemInfo, m_MeshInfolist)
	{
		if( -1 == elemInfo.iParentNode)
		{
			m_spEntity->AddChild( SmartPointerCast(sunNode, elemInfo.spRenderNode) );
			continue;
		}

		bFoundParent = false;
		BOOST_FOREACH( stElemInfo parentElemInfo, m_MeshInfolist)
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
