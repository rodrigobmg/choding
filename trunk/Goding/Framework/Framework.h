#pragma once
/*

#include "..\EntitySystem\EntitySystem.h"
*/


#include "..\Input\InputSystem.h"
#pragma comment(lib , "Input.dll" )

class Framework
{
public:
	
	Framework( HWND hWnd );
	virtual ~Framework();

public:
	
	void			Init();
	void			Destory();
	void			MsgIdle();	

private:

	std::vector<int>	m_vec;
	void			OnIdle();

	HWND			m_hWnd;	/*
	InputSystem		inputsystem;
	EntitySystem*	m_pEntitySystem;*/
};

