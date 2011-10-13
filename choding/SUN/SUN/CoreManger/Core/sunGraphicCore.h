#ifndef _SUN_COREMANAGER_CORE_GRAPHICCORE_H_
#define _SUN_COREMANAGER_CORE_GRAPHICCORE_H_


class DLL_API sunGraphicCore : public sunCoreBase
{
public:
	sunGraphicCore();
	virtual ~sunGraphicCore();


public:			 
	virtual int  InitializeCore();
	virtual void DestoryCore();
	virtual void UpdateCore();


public:
	static sunGraphicCore* GetCore();

public:
	sunRenderer* GetRenderer();

public:
	


public:
	void RegistRenderList( const NodeList& SceneList );
	void ClearRenderList();

	void BeginRender();
	void Render();
	void EndRender();


protected:
	sunRenderer*						m_pRenderer;	

	vector<sunRenderNodePtr>			m_RenderList_Vector;


};



#endif