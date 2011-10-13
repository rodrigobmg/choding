#ifndef _SUN_COREMANAGER_CORE_SCENECORE_H_
#define _SUN_COREMANAGER_CORE_SCENECORE_H_


class sunSceneBase;

//¾À °ü¸® ÄÚ¾î °´Ã¼
class DLL_API sunSceneCore : public sunCoreBase
{
public:
	sunSceneCore();
	virtual ~sunSceneCore();

public:
	virtual int  InitializeCore();
	virtual void DestoryCore();
	virtual void UpdateCore();


public:
	static sunSceneCore* GetCore();


public:
	void					SetNextScene( sunSceneBase *pScene );
	inline  sunSceneBase*	GetCurrentScene();
	inline  sunCameraPtr	GetProgressCamera();


public:
	void RegistRenderList();
	void RegistRenderList(sunNodePtr spNode);

	void BeginUpdate();
	void Update();
	void EndUpdate();

private:
	sunGraphicCore*	m_pGraphicCore;

	sunSceneBase*	m_pCurrentScene;
	sunSceneBase*	m_pNextScene;

	sunCameraPtr	m_spProgressCamera;

};


// ¾À º£ÀÌ½º Å¬·¡½º
class DLL_API sunSceneBase
{
	DeclareRootRTTI(sunSceneBase)


public:
	sunSceneBase();
	virtual ~sunSceneBase();

public:
	virtual int  InitializeScene( void ) = 0;
	virtual void DestroyScene( void ) = 0;

	virtual void BeginUpdateScene( void ) = 0;
	virtual void UpdateScene( void ) = 0;
	virtual void EndUpdateScene( void ) = 0;

public:

	inline sunNodePtr GetRootNode( void );

	void SetDefaultCamera( sunCameraPtr spCamera );
	inline sunCameraPtr GetDefaultCamera( void );


public:
	virtual void AppendScene( sunNodePtr spScene );
	virtual	void PrependScene( sunNodePtr spScene );
	virtual	void RemoveScene( sunNodePtr spScene );
	virtual	void UpdateSceneList( void );

	
protected:
	sunNodePtr			m_spRootNode;
	sunCameraPtr		m_spDefaultCamera;

};


#endif