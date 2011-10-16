#pragma once
#ifdef _WINDLL
#define DLLTYPE __declspec(dllexport)
#else
#define DLLTYPE __declspec(dllimport)
#endif



#include "..\Input\InputSystem.h"


class DLLTYPE Framework
{
public:
	Framework();
	Framework( HWND hWnd );
	virtual ~Framework();

public:
	
	void			Init();
	void			Destory();
	void			MsgIdle();	

private:

	void			OnIdle();

	HWND			m_hWnd;	
	InputSystem		inputsystem;
};

