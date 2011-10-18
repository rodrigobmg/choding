#ifndef _INPUTSYSTEM_MAIN_H_
#define _INPUTSYSTEM_MAIN_H_

#ifdef _WINDLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#include <map>

class DLL InputSystem
{
public:

	enum KEY_VALUE
	{
		VK_A = 0x41,
		VK_B = 0x42,
		VK_C = 0x43,
		VK_D = 0x44,
		VK_E = 0x45,
		VK_F = 0x46,
		VK_G = 0x47,
		VK_H = 0x48,
		VK_I = 0x49,
		VK_J = 0x4a,
		VK_K = 0x4b,
		VK_L = 0x4c,
		VK_M = 0x4d,
		VK_N = 0x4e,
		VK_O = 0x4f,
		VK_P = 0x50,
		VK_Q = 0x51,
		VK_R = 0x52,
		VK_S = 0x53,
		VK_T = 0x54,
		VK_U = 0x55,
		VK_V = 0x56,
		VK_W = 0x57,
		VK_X = 0x58,
		VK_Y = 0x59,
		VK_Z = 0x5a,
		
		VM_LBTN	= 1,
		VM_RBTN = 2,
		VM_MBTN = 3
	};

	InputSystem();
	~InputSystem();

private:

	std::map<int , int > m_map;

};

#endif