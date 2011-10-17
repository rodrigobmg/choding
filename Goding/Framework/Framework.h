#pragma once

#include "..\Input\InputSystem.h"
#include "..\EntitySystem\EntitySystem.h"


class DLLTYPE Framework
{
public:
	
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
	EntitySystem*	m_pEntitySystem;
};

