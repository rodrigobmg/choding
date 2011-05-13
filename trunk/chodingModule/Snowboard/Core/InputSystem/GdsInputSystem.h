
#ifndef _GDS_CORE_INPUTSYSTEM_H_
#define _GDS_CORE_INPUTSYSTEM_H_

#include "..\Base\GdsObject.h"
#include "..\..\System\Util\Utility.h"

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
#define VK_V 0x56  
#define VK_W 0x57 
#define VK_X 0x58  
#define VK_Y 0x59  
#define VK_Z 0x5a 

class GdsInputSystem : public GdsObject
{
public:

	GdsInputSystem();
	~GdsInputSystem();

	void			SetDefaultEvent();
	void			MappingEvent( int ikey , int ievent );

	void			Record( UINT msg , WPARAM wParam , LPARAM lParam );

	bool			GetKeyIsDown( int ikey );
	bool			GetKeyIsUp( int ikey );

protected:
private:

	struct EVENT
	{
		UINT msg;
		WPARAM wParam;
		LPARAM lParam;
	};

	EVENT				m_EventData;

	typedef GdsLookupTable< int , int >	MAP_EVENT;
	MAP_EVENT					m_mapEvent;

// 	GdsMouse		m_Mouse;
// 	GdsKeyboard		m_Keyboard;

};

#define INPUTSYSTEM	singleton_holder< GdsInputSystem >::instance()

#endif