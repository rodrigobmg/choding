#ifndef _GODING_FRAMEWORK_H_
#define _GODING_FRAMEWORK_H_

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

	void			OnIdle();

	HWND			m_hWnd;	
};

#endif