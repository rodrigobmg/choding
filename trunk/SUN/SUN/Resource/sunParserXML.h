#ifndef _SUN_RESOURCE_PARSERXML_H_
#define _SUN_RESOURCE_PARSERXML_H_


class sunParserXML : public sunStream
{
	typedef struct _stElemInfo
	{
		int iNode;
		int iParentNode;
		sunNodePtr spRenderNode;

		vector<_stVector3f> vecVertex;
		vector<_stVector3f> vecVertexNormal;
		vector<_stVector3i> vecIndex;

		_stElemInfo()
		{
			iNode = 0;
			iParentNode = 0;
			spRenderNode = NULL;

			vecVertex.clear();
			vecVertexNormal.clear();
		}
	} stElemInfo;


public:
	sunParserXML();
	~sunParserXML();


public:
	virtual bool Load(const tstring& strFileName);	
	
	bool	ParseObject(TiXmlNode* pRootNode);
	bool	ParseParent(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo) ;
	bool	ParseTM(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo);
	bool	ParseBoundingBox(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo);
	bool	ParseVertex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	bool	ParseVertexNormal(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	
	bool	ParseTriIndex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);


public:
	sunNodePtr GetLoadedEntity();

public:
	list<stElemInfo> m_MeshInfolist;


private:
	TiXmlDocument pXmlDoc;
	
	sunNodePtr	  m_spEntity;


private:
	bool	SortNodes();


};




#endif