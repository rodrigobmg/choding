#pragma once 

class Vertex
{
public:
	//Components of a Vertex
	Vec3 Position;
	Vec3 Normal;
	Vec2 UV;
	Vec3 Tangent;
	Vec3 BiTangent;

	//Constructor
	Vertex(Vec3 Position, Vec3 Normal, Vec2 UV, Vec3 Tangent, Vec3 BiTangent)
	{
		this->Position = Position;
		this->Normal = Normal;
		this->UV = UV;
		this->Tangent = Tangent;
		this->BiTangent = BiTangent;
	}
};