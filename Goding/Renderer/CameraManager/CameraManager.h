#ifndef _CAMERAMANAGER_H_
#define _CAMERAMANAGER_H_
#pragma once


#include "Camera.h"


class CameraManager
{
public:
	//Constructor
	CameraManager();
	~CameraManager();

	//onDeviceReset
	void onDeviceReset();

	//Update
	void Update(float fpsMod);

	Camera			GetCurCam(){ return *m_pCam; }
	void			SetCurCam( Camera* cam );

private:

	Camera*			m_pCam;
};

#endif _CAMERAMANAGER_H_

