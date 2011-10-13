#ifndef _TESTAPP_SCENE_TESTSCENE_H_
#define _TESTAPP_SCENE_TESTSCENE_H_


class Triangle;

class TestScene : public sunSceneBase
{
	DeclareRTTI
public:
	TestScene();
	~TestScene();
public:

	virtual int  InitializeScene();
	virtual void DestroyScene();

	virtual void BeginUpdateScene();
	virtual void UpdateScene();
	virtual void EndUpdateScene() ;


	virtual void AppendScene(sunNodePtr spScene );
	virtual void PrependScene(sunNodePtr spScene );
	virtual void RemoveScene(sunNodePtr spScene );
	virtual void UpdateSceneList( );




public:
	

protected:
	sunCameraPtr	m_spCamera;
	Grid3D*			m_pGrid;
	Triangle*		m_pTriangle;

};



#endif