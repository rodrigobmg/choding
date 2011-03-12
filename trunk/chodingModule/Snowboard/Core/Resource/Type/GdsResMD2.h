#ifndef _GDS_CORE_RESOURCE_TYPE_MD2_H_
#define _GDS_CORE_RESOURCE_TYPE_MD2_H_

#include "GdsResBaseType.h"

class GdsResMD2 : public GdsResBase{
	
private:

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DTEXTURE9			m_pTexture;
	DWORD						m_dFVF;
	UINT						m_uPrimitive;

public:

	typedef struct 
	{
		int		ident;				//MD2 ���� �ĺ��� "IDP2"
		int		version;			//8
		int		skinwidth;			//�ؽ��� �ʺ�
		int		skinheight;			//�ؽ��� ����
		int		framesize;			//Ű �����Ӵ� ����Ʈ ��
		int		numSkins;			//�ؽ��� ����
		int		numXYZ;				//���ؽ� ��ǥ ����
		int		numST;				//�ؽ��� ��ǥ ����
		int		numTris;			//�ﰢ�� ����
		int		numGLcmds;			//OpenGL ���� ���� ����
		int		numFrames;			//�� ������ ��
		int		offsetSkins;		//��Ų �̸���(�̸� �ϳ��� 64����Ʈ)���� ���� �ּ�
		int		offsetST;			//�ؽ��� ����Ʈ ���� �ּ�
		int		offsetTris;			//�ε��� ����Ʈ ���� �ּ�
		int		offsetFrames;		//���ؽ� ����Ʈ ���� �ּ�
		int		offsetGLcmds;		//����� OpenGL ���ɵ��� ����
		int		offsetEnd;			//������ ��
	} MD2HEADER; //68����Ʈ

	typedef struct 
	{
		float	scale[3];			//�����ϰ�
		float	translate[3];		//�̵���
		char	name[16];			//�ؽ��� �̸�
	} MD2MATRIX; //40����Ʈ

	typedef struct
	{
		BYTE	v[3];				//��ǥ x, z, y
		BYTE	normal;				//�븻 ����(������)
	} MD2VERTEX; //4����Ʈ

	typedef struct
	{
		short	tu;					//�ؽ��� ��ǥ tu
		short	tv;					//�ؽ��� ��ǥ tv
	} MD2TEXTURE; //4����Ʈ

	typedef struct
	{
		short	vertex[3];			//���ؽ� �ε���
		short	texture[3];			//�ؽ��� �ε���
	} MD2INDICES; //12����Ʈ

	union MD2_VERTEX
	{
		struct
		{
			float x, y, z;
			float tu, tv;
		};
		struct
		{
			D3DXVECTOR3 v;
			D3DXVECTOR2 t;
		};
	};

	DeclareBoostPool	

public:
	GdsResMD2();
	virtual ~GdsResMD2();

	virtual void		Clear();
	virtual HRESULT		Create();
	virtual HRESULT		Release();
	virtual HRESULT		LoadResource( const TCHAR* path  , LPDIRECT3DDEVICE9 device );
		
};

typedef boost::shared_ptr< GdsResMD2 >	GdsResMD2Ptr;

#endif