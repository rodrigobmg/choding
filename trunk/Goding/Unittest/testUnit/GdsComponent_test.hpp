#pragma once

class GdsComponentTest : public ::testing::Test
{
public:
	GdsComponentTest()
	{
	}

	virtual ~GdsComponentTest()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
		//¼Ò¸êÀÚ
	}

	virtual void TestBody()
	{		
	}
};

TEST_F( GdsComponentTest , VisualRect )
{
	GdsComVisualRect com_visual_rect;
	EXPECT_EQ( COMPONENT_VISUAL_FAMILY , com_visual_rect.familyID() );
	EXPECT_EQ( COMPONENT_VISUAL_RECT , com_visual_rect.componentID() );

	GdsComVisualRect* p_compo_visual = new GdsComVisualRect;
	p_compo_visual->Render( 0.f );

	GdsComVisualRect* p_compo_visual1 = new GdsComVisualRect;
	p_compo_visual1->Render( 0.f );

	GdsComVisualRect* p_compo_visual2 = new GdsComVisualRect;
	p_compo_visual2->Render( 0.f );

	GdsComVisualRect* p_compo_visual3 = new GdsComVisualRect;
	p_compo_visual3->Render( 0.f );

	delete p_compo_visual;
	delete p_compo_visual1;
	delete p_compo_visual2;
	delete p_compo_visual3;
}