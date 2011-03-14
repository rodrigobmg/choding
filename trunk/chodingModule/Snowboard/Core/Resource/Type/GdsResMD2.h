#ifndef _GDS_CORE_RESOURCE_TYPE_MD2_H_
#define _GDS_CORE_RESOURCE_TYPE_MD2_H_

#include "GdsResBaseType.h"

class GdsResMD2 : public GdsResBase{

public:

	typedef struct 
	{
		int		ident;				//MD2 파일 식별자 "IDP2"
		int		version;			//8
		int		skinwidth;			//텍스쳐 너비
		int		skinheight;			//텍스쳐 높이
		int		framesize;			//키 프레임당 바이트 수
		int		numSkins;			//텍스쳐 개수
		int		numXYZ;				//버텍스 좌표 개수
		int		numST;				//텍스쳐 좌표 개수
		int		numTris;			//삼각형 개수
		int		numGLcmds;			//OpenGL 명령 종류 개수
		int		numFrames;			//총 프레임 수
		int		offsetSkins;		//스킨 이름들(이름 하나가 64바이트)로의 시작 주소
		int		offsetST;			//텍스쳐 리스트 시작 주소
		int		offsetTris;			//인덱스 리스트 시작 주소
		int		offsetFrames;		//버텍스 리스트 시작 주소
		int		offsetGLcmds;		//사용할 OpenGL 명령들의 종류
		int		offsetEnd;			//파일의 끝
	} MD2HEADER; //68바이트

	typedef struct 
	{
		float	scale[3];			//스케일값
		float	translate[3];		//이동값
		char	name[16];			//텍스쳐 이름
	} MD2MATRIX; //40바이트

	typedef struct
	{
		BYTE	v[3];				//좌표 x, z, y
		BYTE	normal;				//노말 벡터(라이팅)
	} MD2VERTEX; //4바이트

	typedef struct
	{
		short	tu;					//텍스쳐 좌표 tu
		short	tv;					//텍스쳐 좌표 tv
	} MD2TEXTURE; //4바이트

	typedef struct
	{
		short	vertex[3];			//버텍스 인덱스
		short	texture[3];			//텍스쳐 인덱스
	} MD2INDICES; //12바이트

	union MD2_VERTEX
	{
		struct
		{
			float x, y, z;
			float nx , ny , nz;
			float tu, tv;
		};
		struct
		{
			D3DXVECTOR3 v;
			D3DXVECTOR3 n; //노멀 벡터 추가
			D3DXVECTOR2 t;
		};
	};

	DeclareBoostPool	

private:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DTEXTURE9			m_pTexture;
	DWORD						m_dFVF;
	UINT						m_uPrimitive;

public:
	GdsResMD2();
	virtual ~GdsResMD2();
	
	void						SetVB( LPDIRECT3DVERTEXBUFFER9 vb );
	LPDIRECT3DVERTEXBUFFER9		GetVB();

	void						SetFVF( DWORD flag );
	DWORD						GetFVF();

	void						SetTexture( LPDIRECT3DTEXTURE9 texture );
	LPDIRECT3DTEXTURE9			GetTexture();

	void						SetPrimitive( UINT uPrimitive );
	UINT						GetPrimitive();


	virtual void		vClear();
	virtual HRESULT		vCreate();
	virtual HRESULT		vRelease();
	virtual HRESULT		vReCreate( LPDIRECT3DDEVICE9 device );
	virtual HRESULT		vLoadResource( const TCHAR* path  , LPDIRECT3DDEVICE9 device );
		
};

typedef boost::shared_ptr< GdsResMD2 >	GdsResMD2Ptr;

#endif
