#include "stdafx.h"

GdsRendererDX9::GdsRendererDX9() :
m_bWireMode( true ) ,
m_pd3dDevice( NULL ) ,
m_pD3D( NULL )
{
}

GdsRendererDX9::~GdsRendererDX9()
{
	Release();
}

HRESULT GdsRendererDX9::Release()
{
	SAFE_RELEASE(m_pD3D);
	SAFE_RELEASE(m_pd3dDevice);	
	return true;
}

void GdsRendererDX9::Clear()
{

}

bool GdsRendererDX9::Create( HWND hWnd )
{
	// Create the D3D object.
	if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		LOG_INFO_F( "Fail to create d3d\n");
		return false;
	}

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// Create the D3DDevice
	if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pd3dDevice ) ) )
	{
		LOG_INFO_F("Fail to create device\n");
		return false;
	}
	
	m_RenderFrameList = GdsRenderFramePtr( new GdsRenderFrame );

	return true;
}

void GdsRendererDX9::RenderFrame( float fAccumtime )
{	
	if ( m_bWireMode )
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	else
		m_pd3dDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	m_pd3dDevice->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB( 128, 128, 128 ) , 1.f , 0 );

	if( SUCCEEDED( m_pd3dDevice->BeginScene() ) )
	{	
 		m_pd3dDevice->SetTransform( D3DTS_VIEW , &(CAMMGR.GetCurCam()->GetViewMat()));
 		m_pd3dDevice->SetTransform( D3DTS_PROJECTION, &(CAMMGR.GetCurCam()->GetProjMat()) );

		m_RenderFrameList->Render( m_pd3dDevice );

		if( !m_listAxisData.empty() || !m_listBoxData.empty() )
		{
			drawEtc();			
		}

 		m_pd3dDevice->EndScene();
	}

	m_pd3dDevice->Present( NULL , NULL , NULL , NULL );
}

void GdsRendererDX9::drawAxis(  D3DXMATRIX& mat , D3DXVECTOR3& point , ID3DXLine* Line )
{
	D3DXVECTOR3 lineLBN[2];
	lineLBN[0].x = 0.0f; lineLBN[0].y = 0.0f; lineLBN[0].z = 0.0f;
	lineLBN[1].x = 10.0f; lineLBN[1].y = 0.0f; lineLBN[1].z = 0.0f;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0].x = 0.0f; lineLTN[0].y = 0.0f; lineLTN[0].z = 0.0f;
	lineLTN[1].x = 0.0f; lineLTN[1].y = 10.0f; lineLTN[1].z = 0.0f;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0].x = 0.0f; lineLBF[0].y = 0.0f; lineLBF[0].z = 0.0f;
	lineLBF[1].x = 0.0f; lineLBF[1].y = 0.0f; lineLBF[1].z = 10.0f;

	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 1.0f , 0.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 0.0f , 1.0f , 1.0f ));
}

void GdsRendererDX9::drawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos , ID3DXLine* Line )
{
	D3DXVECTOR3 lineLBN[2];
	lineLBN[0] = minPos;
	lineLBN[1].x = minPos.x; lineLBN[1].y = maxPos.y; lineLBN[1].z = minPos.z;

	D3DXVECTOR3 lineLTN[2];
	lineLTN[0] = lineLBN[1];
	lineLTN[1].x = minPos.x; lineLTN[1].y = maxPos.y; lineLTN[1].z = maxPos.z;

	D3DXVECTOR3 lineLBF[2];
	lineLBF[0] = lineLTN[1];
	lineLBF[1].x = minPos.x; lineLBF[1].y = minPos.y; lineLBF[1].z = maxPos.z;

	D3DXVECTOR3 lineLTF[2];
	lineLTF[0] = lineLBF[1];
	lineLTF[1] = minPos;

	D3DXVECTOR3 lineRBN[2];
	lineRBN[0].x = maxPos.x; lineRBN[0].y = minPos.y; lineRBN[0].z = minPos.z;
	lineRBN[1].x = maxPos.x; lineRBN[1].y = maxPos.y; lineRBN[1].z = minPos.z;

	D3DXVECTOR3 lineRTN[2];
	lineRTN[0] = lineRBN[1];
	lineRTN[1].x = maxPos.x; lineRTN[1].y = maxPos.y; lineRTN[1].z = maxPos.z;

	D3DXVECTOR3 lineRBF[2];
	lineRBF[0] = lineRTN[1];
	lineRBF[1].x = maxPos.x; lineRBF[1].y = minPos.y; lineRBF[1].z = maxPos.z;

	D3DXVECTOR3 lineRTF[2];
	lineRTF[0] = lineRBF[1];
	lineRTF[1] = lineRBN[0];

	D3DXVECTOR3 lineBN[2];
	lineBN[0] = minPos;
	lineBN[1].x = maxPos.x; lineBN[1].y = minPos.y; lineBN[1].z = minPos.z;

	D3DXVECTOR3 lineTN[2];
	lineTN[0].x = minPos.x; lineTN[0].y = maxPos.y; lineTN[0].z = minPos.z;
	lineTN[1].x = maxPos.x; lineTN[1].y = maxPos.y; lineTN[1].z = minPos.z;

	D3DXVECTOR3 lineBF[2];
	lineBF[0].x = minPos.x; lineBF[0].y = minPos.y; lineBF[0].z = maxPos.z;
	lineBF[1].x = maxPos.x; lineBF[1].y = minPos.y; lineBF[1].z = maxPos.z;

	D3DXVECTOR3 lineTF[2];
	lineTF[0].x = minPos.x; lineTF[0].y = maxPos.y; lineTF[0].z = maxPos.z;
	lineTF[1] = maxPos;

	Line->DrawTransform( lineLBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineLTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineRTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTN , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineBF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));
	Line->DrawTransform( lineTF , 2, &mat, D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ));	
}

void GdsRendererDX9::DrawBox( D3DXMATRIX& mat , D3DXVECTOR3& minPos , D3DXVECTOR3& maxPos )
{
	BOXDATA token( mat , minPos , maxPos );
	m_listBoxData.push_back( token );
}

void GdsRendererDX9::DrawAxis( D3DXMATRIX& mat , D3DXVECTOR3& point )
{
	AXISDATA token( mat , point );
	m_listAxisData.push_back( token );
}

void GdsRendererDX9::drawEtc()
{
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProj;
	m_pd3dDevice->GetTransform( D3DTS_VIEW  , &matView );
	m_pd3dDevice->GetTransform( D3DTS_PROJECTION , &matProj );	

	ID3DXLine* Line;
	D3DXCreateLine( m_pd3dDevice , &Line );
	Line->SetWidth( 1 );
	Line->SetAntialias( true );
	Line->Begin();

	LIST_AXIS::iterator itAxis = m_listAxisData.begin();
	LIST_AXIS::iterator itAxis_end = m_listAxisData.end();
	for( ; itAxis != itAxis_end ; ++itAxis )
	{
		D3DXMATRIX matWorld = (boost::tuples::get<0>(*itAxis))*matView*matProj;
		drawAxis( matWorld , boost::tuples::get<1>(*itAxis) , Line );
	}

	LIST_BOX::iterator itBox = m_listBoxData.begin();
	LIST_BOX::iterator itBox_end = m_listBoxData.end();
	for ( ; itBox != itBox_end ; ++itBox )
	{
		D3DXMATRIX matWorld = (boost::tuples::get<0>(*itBox))*matView*matProj;
		drawBox( matWorld , boost::tuples::get<1>(*itBox) , boost::tuples::get<2>(*itBox) , Line );
	}

	Line->End();
	Line->Release();

	m_listAxisData.clear();
	m_listBoxData.clear();
}