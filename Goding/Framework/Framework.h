#ifndef _GODING_FRAMEWORK_H_
#define _GODING_FRAMEWORK_H_

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

class DLL Framework
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
};

#endif