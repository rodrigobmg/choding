#ifndef _CAMERA_H_
#define _CAMERA_H_
#pragma once

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class Camera
{
public:
	Camera(){};
	~Camera(){};


public:

	uint32 Height;
	uint32 Width;

	Vec3 Position;
	Vec3 Direction;
	Vec3 Up;
	Vec3 Target;

	Matrix44 View;
	Matrix44 Projection;

};

#endif _CAMERA_H_

