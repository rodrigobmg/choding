#include "Precompiled.h"
#include "Bridge.h"


#pragma managed

using namespace SUNBridgeDLL;


//ListBox^ CBridge::m_LogBox = nullptr;

CBridge::CBridge(): m_pMessanger(NULL)
{

}

CBridge::~CBridge()
{
}

void CBridge::Create(IntPtr hWnd)
{
	m_hWnd = (HWND)hWnd.ToPointer();


	//messanger�� ��ü�� �Ѱ��ָ� ��
	m_pMessanger = new CBridgeMessenger;
	m_pMessanger->m_pBridge = this;

		

}

void CBridge::SetLogBox(ListBox^ LogBox)
{
	m_LogBox = LogBox;

}



//Messager�� ���� ȣ�� �Ǵ� �Լ���.

void CBridge::WriteLog( const TCHAR* strLog )
{

	assert( m_LogBox);

	String^ str = gcnew String( strLog );
	m_LogBox->Items->Add(str);
	m_LogBox->SetSelected(m_LogBox->Items->Count -1, true);
}


// Messanger Class
CBridgeMessenger::CBridgeMessenger()
{
	m_pBridge = nullptr;
}

CBridgeMessenger::~CBridgeMessenger()
{

}

void CBridgeMessenger::WriteLog( LPCTSTR strLog )
{
	assert( m_pBridge);
	m_pBridge->WriteLog( strLog );

}


