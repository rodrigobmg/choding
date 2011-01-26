#include "ResMgr.h"

#include "Type/ResTexture.h"
#include "../CoreMgr.h"


CResMrg::CResMrg()
{
	SetName( OBJECT_RES_MGR );
	Clear();
}

CResMrg::~CResMrg()
{

}

void CResMrg::Clear()
{
	m_mapRes.clear();
}

bool CResMrg::Load( const wchar_t* filename , LPDIRECT3DDEVICE9 pDevice )
{
	if ( pDevice == NULL )
		return false;

	if ( !isExist( filename ) )
	{
		//없네?
		LPDIRECT3DTEXTURE9 pTexture;
		CResTexture* pResTex = new CResTexture;		
		if ( SUCCEEDED( D3DXCreateTextureFromFile( pDevice , filename , pResTex->GetPtr() ) ) )
		{
			m_mapRes.insert( make_pair( filename , pResTex ) );
		}
		else
		{
			LPVOID lpMsgBuf;
			LPVOID lpDisplayBuf;
			DWORD dw = GetLastError(); 

			FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				dw,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR) &lpMsgBuf,
				0, NULL );

			// Display the error message and exit the process

			lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
				(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR) filename ) + 40) * sizeof(TCHAR)); 
			StringCchPrintf((LPTSTR)lpDisplayBuf, 
				LocalSize(lpDisplayBuf) / sizeof(TCHAR),
				TEXT("%s failed with error %d: %s"), 
				filename , dw, lpMsgBuf); 
			MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

			LocalFree(lpMsgBuf);
			LocalFree(lpDisplayBuf);

		}
	}
	else
	{
		CResTexture* pResTex = new CResTexture;
		//있네?
		if ( SUCCEEDED( D3DXCreateTextureFromFile( pDevice , filename , pResTex->GetPtr() ) ) )
			m_mapRes.insert( make_pair( filename , pResTex ) );
	}
	return true;
}

bool CResMrg::isExist( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return true;
	else
		return false;
}

CSnow* CResMrg::Get( const wchar_t* filename )
{
	HASHMAPRes_ITERATOR it = m_mapRes.find( filename );
	if ( it != m_mapRes.end() )
		return it->second;
	else
		return NULL;
}

void CResMrg::Create()
{
	Clear();
}