#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "..\Type\CommonType.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "Winmm.lib")

class Camera
{

private:
	Vec3 Position;
	Vec3 Direction;
	Vec3 Up;

	float Speed;

public:
	Matrix44 View;
	Matrix44 Projection;

	//Constructor
	Camera(Vec3 Position, Vec3 Target, Vec3 Up, float Width, float Height)
	{
		this->Position = Position;
		this->Direction = Vec3(Target - Position); D3DXVec3Normalize(&this->Direction, &this->Direction);
		this->Up = Up;

		this->Speed = 0.10f;

		D3DXMatrixLookAtLH(&View, &this->Position, &Target, &this->Up);
		D3DXMatrixPerspectiveFovLH(&Projection, (D3DX_PI / 4.0f), (Width / Height), 0.10f, 1000.0f);
	}

	//onDeviceReset
	void onDeviceReset(float Width, float Height)
	{
		D3DXMatrixPerspectiveFovLH(&Projection, (D3DX_PI / 4.0f), (Width / Height), 1.0f, 1000.0f);
	}

	//Update
	void Update(float fpsMod)
	{
		//Keyboard bools
		bool W = (GetAsyncKeyState(0x0057) & 0x8000) ? true : false;
		bool S = (GetAsyncKeyState(0x0053) & 0x8000) ? true : false;
		bool A = (GetAsyncKeyState(0x0041) & 0x8000) ? true : false;
		bool D = (GetAsyncKeyState(0x0044) & 0x8000) ? true : false;

		//Mouse position
		POINT Mouse;
		GetCursorPos(&Mouse);

		//Set cursor back to middle
		SetCursorPos(600, 600);

		Vec3 cAxis;
		D3DXVec3Cross(&cAxis, &Up, &Direction);

		if(W) Position += Direction * (Speed * fpsMod);
		if(S) Position -= Direction * (Speed * fpsMod);
		if(A) Position -= cAxis * (Speed * fpsMod);
		if(D) Position +=  cAxis * (Speed * fpsMod);

		Vec4 TransformTemp; // Vec3Transform returns Vector4, use this to convert

		//Yaw Quaternion
		Quat RXq;
		D3DXQuaternionRotationAxis(&RXq, &Up, ( (D3DX_PI / 4.0f) / 150) * (Mouse.x - 600));

		//Converted to Matrix
		Matrix44 RX;
		D3DXMatrixRotationQuaternion(&RX, &RXq);

		//Transform Direction
		D3DXVec3Transform(&TransformTemp, &Direction, &RX);
		Direction.x = TransformTemp.x; Direction.y = TransformTemp.y; Direction.z = TransformTemp.z;

		//Pitch Quaternion
		Quat RYq;
		D3DXVec3Cross(&cAxis, &Up, &Direction);
		D3DXQuaternionRotationAxis(&RYq, &cAxis, ( (D3DX_PI / 4.0f) / 150) * (Mouse.y - 600));

		//Convert to Matrix
		Matrix44 RY;
		D3DXMatrixRotationQuaternion(&RY, &RYq);

		//Transform Direction
		D3DXVec3Transform(&TransformTemp, &Direction, &RY);
		Direction.x = TransformTemp.x; Direction.y = TransformTemp.y; Direction.z = TransformTemp.z;

		//Calculate Target
		Vec3 Target = Position + Direction;

		//Calculate View Matrix
		D3DXMatrixLookAtLH(&View, &this->Position, &Target, &this->Up);
	}
};

#endif _CAMERA_H_

