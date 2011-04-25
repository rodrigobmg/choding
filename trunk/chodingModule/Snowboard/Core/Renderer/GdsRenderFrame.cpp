#include "GdsRenderFrame.h"

void GdsRenderFrame::AddRenderObject( GdsRenderObjectPtr pRenderObject )
{
	m_RenderFrame.push_back( pRenderObject );
}

void GdsRenderFrame::SetRenderState( int renderstste_key , int renderstate_value )
{
	VALUE opt( renderstste_key , renderstate_value );
	m_Opt_RenderState.push_back( opt );
}

void GdsRenderFrame::vRender( LPDIRECT3DDEVICE9 device )
{
	OPT_RENDERSTATE::iterator	it = m_Opt_RenderState.begin();
	for ( ; it != m_Opt_RenderState.end() ; ++it )
	{
		device->SetRenderState( static_cast< D3DRENDERSTATETYPE>( it->first ) , it->second );
	}

	OPT_MULTITEXTURE::iterator	it_tex = m_Opt_MultiTexturing.begin();
	for ( ; it_tex != m_Opt_MultiTexturing.end() ; ++it_tex )
	{		
		device->SetTextureStageState( it_tex->first , static_cast<D3DTEXTURESTAGESTATETYPE>( it_tex->second.first ) , it_tex->second.second );
	}

	OPT_MULTITEXTURE::iterator	it_samplerstate = m_Opt_SamplerState.begin();
	for ( ; it_samplerstate != m_Opt_SamplerState.end() ; ++it_samplerstate )
	{		
		device->SetSamplerState( it_samplerstate->first 
								, static_cast< D3DSAMPLERSTATETYPE >( it_samplerstate->second.first ) 								
								, static_cast< D3DTEXTUREFILTERTYPE >( it_samplerstate->second.second )
								);
	}
 	
 	
 	//if( SUCCEEDED( device->BeginScene() ) )
 	{	  
		RENDEROBJECT::iterator it_render = m_RenderFrame.begin();
		for ( ; it_render != m_RenderFrame.end() ; ++it_render )
		{
			(*it_render)->vRender( device );
		}
 	}
 
 	//device->Present( NULL , NULL , NULL , NULL );
}

void GdsRenderFrame::SetTextureStageState( int index , int iop1 , int iop2 )
{
	VALUE val( iop1 , iop2 );
	MULTI_VALUE multi_value( index , val );
	m_Opt_MultiTexturing.push_back( multi_value );
}

void GdsRenderFrame::SetSamplerState( int index , int iop1 , int iop2 )
{	
	VALUE val( iop1 , iop2 );
	MULTI_VALUE multi_value( index , val );
	m_Opt_SamplerState.push_back( multi_value );
}