#ifndef _SNOWBOARD_FRAMEWORK_INPUTSYSTEM_MAIN_H_
#define _SNOWBOARD_FRAMEWORK_INPUTSYSTEM_MAIN_H_

#define VK_A 0x41  
#define VK_B 0x42  
#define VK_C 0x43  
#define VK_D 0x44  
#define VK_E 0x45
#define VK_F 0x46
#define VK_G 0x47
#define VK_H 0x48  
#define VK_I 0x49  
#define VK_J 0x4a  
#define VK_K 0x4b  
#define VK_L 0x4c
#define VK_M 0x4d
#define VK_N 0x4e
#define VK_O 0x4f
#define VK_P 0x50  
#define VK_Q 0x51  
#define VK_R 0x52 
#define VK_S 0x53  
#define VK_T 0x54  
#define VK_U 0x55
#define VK_V 0x56  
#define VK_W 0x57 
#define VK_X 0x58  
#define VK_Y 0x59  
#define VK_Z 0x5a 

#define VM_LBTN	1
#define VM_RBTN 2
#define VM_MBTN 3

class GdsInputSystem
{
public:

	GdsInputSystem();
	~GdsInputSystem();

	void			SetDefaultEvent();
	void			MappingEvent( int ikey , int ievent );

	void			Record( HWND hwnd , UINT msg , WPARAM wParam , LPARAM lParam );

	bool			GetKeyIsDown( int ikey );
	bool			GetKeyIsUp( int ikey );

	bool			GetMouseIsDown( int ikey );
	bool			GetMouseIsUp( int ikey );

	void			GetMousePosDelta( int& x , int& y , int& z );

protected:
private:

	struct EVENT
	{
		HWND hwnd;
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
	};

	int					m_iPreMousePosX;
	int					m_iPreMousePosY;
	int					m_iPreMousePosZ;

	int					m_iMouseDeltaX;
	int					m_iMouseDeltaY;
	int					m_iMouseDeltaZ;

	EVENT				m_EventDataUp;
	EVENT				m_EventDataDown;
	EVENT				m_EventMouseUp;
	EVENT				m_EventMouseDown;

	typedef GdsLookupTable< int , int >	MAP_EVENT;
	MAP_EVENT					m_mapEvent;

	void			clear( EVENT& token );

};

#define INPUTSYSTEM	singleton_holder< GdsInputSystem >::instance()

#endif