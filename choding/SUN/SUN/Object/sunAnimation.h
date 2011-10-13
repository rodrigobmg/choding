#ifndef _SUN_OBJECT_SUNANIMATION_H_
#define _SUN_OBJECT_SUNANIMATION_H_



struct stBaseKey
{
	int		iFrame;
};

struct stKeyPos : public stBaseKey
{
	D3DXVECTOR3 vPos;
};

struct stKeyRot : public stBaseKey
{
	D3DXQUATERNION qRot;
};

struct stKeyScl : public stBaseKey
{
	D3DXVECTOR3 vScale;
};


typedef struct _stKeys
{
	vector<stKeyPos> vecPos;
	vector<stKeyRot> vecRot;
	vector<stKeyScl> vecScl;

}stKeys;


class DLL_API sunTrack : public sunRef
{
public:
	sunTrack();
	virtual ~sunTrack();


public:
	void	AddKey( const stKeys& key);
	float	GetAlpha(float f1, float f2, float fFrame);
	const	D3DXMATRIX& GetAniMatrix();

	void    SetStartFrame(float fStartFrame);
	void    SetEndFrame(float fEndFrame);

public:
	void	Get2Keys(float frame, vector<stKeyPos>&keys, stKeyPos*& k1, stKeyPos*& k2);
	void	Get2Keys(float frame, vector<stKeyRot>&keys, stKeyRot*& k1, stKeyRot*& k2);
	void    Get2Keys(float frame, vector<stKeyScl>&keys, stKeyScl*& k1, stKeyScl*& k2);

	BOOL	GetPosKey( float frame, D3DXVECTOR3* pv );
	BOOL	GetRotKey( float fFrame, D3DXQUATERNION* pqRot );
	BOOL	GetSclKey( float fframe, D3DXVECTOR3* pvScale );

	void	UpdateTrack( float fFrame );

public:
	stKeys&	GetKeys();

protected:
	//vector< stKeys > m_keyFrames_Vector;

	stKeys			m_kKeys;
	
	D3DXMATRIX		m_matAni;
	float			m_fStartFrame;
	float			m_fEndFrame;
	float			m_fAccumulationTime;
};


class DLL_API sunAnimation : public sunRef
{
public:
	sunAnimation();
	virtual ~sunAnimation();

public:
	void AddTrack ( sunTrackPtr spTrack );

public:
	//const D3DXMATRIX& UpdateTrack( float fTime );

protected:
	vector< sunTrackPtr > m_TrackList_Vector;

};

#endif