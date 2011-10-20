#pragma once

class GdsGameObjectTest : public ::testing::Test
{
public:
	GdsGameObjectTest()
	{
	}

	virtual ~GdsGameObjectTest()
	{
	}

	virtual void SetUp()
	{
		//entity = new GdsGameObject( GAMEOBJECT_DUMMY );
		entity = GdsGameObjectPtr( new GdsGameObject( GAMEOBJECT_DUMMY ) );
		
		int step = 10;
		GdsGameObjectPtr pNode = entity;
		for ( int i=0; i < step ; ++i )
		{
			for ( int j=0 ; j<100 ; ++j )
			{
				//GdsGameObject* pChild = new GdsGameObject( GAMEOBJECT_DUMMY );
				GdsGameObjectPtr pChild = GdsGameObjectPtr( new GdsGameObject( GAMEOBJECT_DUMMY ) );
				pNode->AttchChild( pChild );
			}			
			pNode = pNode->GetChild( GAMEOBJECT_DUMMY );
		}	
	}

	virtual void TearDown()
	{
		//¼Ò¸êÀÚ
		//SAFE_DELETE( entity );
	}

	virtual void TestBody()
	{			
	}

	bool_chk Test()
	{
		int a = 0;
	}

	GdsGameObjectPtr entity;
};

TEST_F( GdsGameObjectTest , update_test )
{
// 	GdsComponentMaker component_maker;
// 	GdsGameObject entity( GAMEOBJECT_DUMMY );
// 	GdsComVisualRect* visual_rect = static_cast< GdsComVisualRect*>( component_maker.MakeComponent( COMPONENT_VISUAL_RECT ) );
// 	entity.SetComponent( visual_rect );
// 	GdsComVisualRect* p_visual_rect = static_cast< GdsComVisualRect*>( entity.GetComponent( COMPONENT_VISUAL_FAMILY ) );
// 	EXPECT_EQ( p_visual_rect , visual_rect );	
// 	
// 	GdsGameObject* pentity = visual_rect->GetOwnerGO();
// 	EXPECT_EQ( pentity , &entity );
// 
// 	p_visual_rect->Update( 0.f );
// 	
//  	{
//  		GdsGameObjectPtr ptr = GdsGameObjectPtr( new GdsGameObject(1) );
//  		ptr->Init( GAMEOBJECT_DUMMY );
//  	}
	entity->Update( 0.f );
}
