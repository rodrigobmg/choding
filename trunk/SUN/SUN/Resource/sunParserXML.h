#ifndef _SUN_RESOURCE_PARSERXML_H_
#define _SUN_RESOURCE_PARSERXML_H_


class sunParserXML : public sunStream
{
	typedef struct _stElemInfo
	{
		int iNode;
		int iParentNode;
		int iMaterialID;
		sunNodePtr spRenderNode;

		vector<_stVector3f> vecVertex;
		vector<_stVector3f> vecVertexNormal;
		vector<_stVector3i> vecIndex;
		vector<_stVector2f> vecTVertex;

		sunTrackPtr spTrack;

		_stElemInfo()
		{
			iNode = 0;
			iParentNode = 0;
			iMaterialID = 0;
			spRenderNode = NULL;
			spTrack		 = NULL;

			vecVertex.clear();
			vecVertexNormal.clear();
		}
	} stElemInfo;

	
	typedef struct _stMaterialInfo
	{
		
		D3DMATERIAL9	kMaterial;
		tstring			strFullPath;
		tstring			strFileName;

		_stMaterialInfo()
		{
			ZeroMemory(&kMaterial, sizeof(kMaterial));	
		}
	} stMaterialInfo ;
	


public:
	sunParserXML();
	~sunParserXML();

	void Destory();

public:
	virtual bool Load(const tstring& strFileName);	
	

	bool    ParseMaterialInfo(TiXmlNode* pRootNode);
	bool	ParseColor(TiXmlNode* pColorNode, D3DCOLORVALUE& kMaterialColor);


	bool	ParseObject(TiXmlNode* pRootNode);
	bool	ParseParent(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo) ;
	bool	ParseTM(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo);
	bool	ParseBoundingBox(TiXmlNode* pObjectNode ,stElemInfo* pElemInfo);
	bool	ParseVertex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	bool	ParseVertexNormal(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	bool	ParseTVertex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);

	bool	ParseTriIndex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	bool	ParseTexIndex(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);
	bool    ParseKey(TiXmlNode* pObjectNode, stElemInfo* pElemInfo);



public:
	sunNodePtr GetLoadedEntity();

public:

	vector<stElemInfo> m_vecMeshInfo;

	typedef stdext::hash_map<int, stMaterialInfo*>  MaterialInfo;
	typedef stdext::hash_map<int, stMaterialInfo*>::const_iterator 	MaterialInfo_ITERATOR;
	MaterialInfo	m_MaterialInfo_List;


private:
	TiXmlDocument pXmlDoc;
	
	sunNodePtr	  m_spEntity;


private:
	bool	SortNodes();


};




#endif