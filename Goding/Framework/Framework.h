#pragma once
#include <vector>

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL Framework
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

	HWND			m_hWnd;
};

