#ifndef _UNITTEST_GDS_MATERIAL_H_
#define _UNITTEST_GDS_MATERIAL_H_

class GdsMaterialTest : public ::testing::Test
{
public:
	GdsMaterialTest()
	{
	}

	virtual ~GdsMaterialTest()
	{
	}

	virtual void SetUp()
	{
		//持失切
	}

	virtual void TearDown()
	{
		//社瑚切
	}

	virtual void TestBody()
	{	
	}
};

TEST_F( GdsMaterialTest ,  GdsMaterial )
{
	GdsMaterialPtr material = GdsMaterialPtr( new GdsMaterial );
	GdsMaterialPtr submaterial1 = GdsMaterialPtr( new GdsMaterial );
	GdsMaterialPtr submaterial2 = GdsMaterialPtr( new GdsMaterial );
	GdsMaterialPtr submaterial3 = GdsMaterialPtr( new GdsMaterial );

	material->AddSubMaterial( submaterial1 );
	material->AddSubMaterial( submaterial2 );
	material->AddSubMaterial( submaterial3 );

	D3DMATERIAL9 mat = material->GetSubMaterial(2)->GetDxMaterial();

	EXPECT_EQ( 3 , material->GetCountSubMaterial() );

	EXPECT_EQ( submaterial2 , material->GetSubMaterial(1) );

	
}

#endif	