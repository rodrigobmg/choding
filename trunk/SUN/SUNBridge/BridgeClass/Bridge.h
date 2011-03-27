#ifndef _SUNBRIDGE_BRIDGE_H_
#define _SUNBRIDGE_BRIDGE_H_


using namespace System;
using namespace System::Data;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace SUNBridgeDLL{
	class CBridgeMessenger;

	//c# -> c++를 위한 랩핑 클래스
	public ref class CBridge
	{
	public:
		CBridge();
		~CBridge();

	public:
		void Create(IntPtr hWnd);
		void SetLogBox(ListBox^ LogBox);
		
	
		//Messager에 의해 호출 되는 함수
	public:
		void WriteLog( LPCTSTR strLog );

	

	private:
		HWND m_hWnd;

		CBridgeMessenger* m_pMessanger;
			
	private:
		static ListBox^	m_LogBox;


	};

	// c++ -> C#을 위한 랩핑 클래스
	public class CBridgeMessenger : public CMessengerToCSharp
	{
	public:
		CBridgeMessenger();
		virtual ~CBridgeMessenger();

	public:
		virtual void WriteLog( LPCTSTR strLog );
	
	public:
		gcroot<CBridge^> m_pBridge;

	};



}



#endif