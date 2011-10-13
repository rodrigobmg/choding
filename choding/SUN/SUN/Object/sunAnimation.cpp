#include "Precompiled.h"
#include "sunAnimation.h"


sunTrack::sunTrack():
m_fStartFrame(0.0f),
m_fEndFrame(0.0f),
m_fAccumulationTime(0.0f)
{
	D3DXMatrixIdentity(&m_matAni);
}

sunTrack::~sunTrack()
{
	
}


void sunTrack::AddKey(const stKeys& kKeys)
{
	m_kKeys = kKeys;
}


void sunTrack::UpdateTrack( float fFrame )
{
	//m_fAccumulationTime += fTime;

	D3DXVECTOR3		vPos;
	D3DXQUATERNION	qRot;
	D3DXVECTOR3		vScale;

	D3DXMatrixIdentity(&m_matAni);

	if( fFrame > m_fEndFrame ) fFrame   = m_fEndFrame;
	if( fFrame < m_fStartFrame ) fFrame = m_fStartFrame;


	if( GetRotKey( fFrame, &qRot ) )
		D3DXMatrixRotationQuaternion( &m_matAni, &qRot );

	if( GetPosKey( fFrame, &vPos ) )
	{
		m_matAni._41 = vPos.x;
		m_matAni._42 = vPos.y;
		m_matAni._43 = vPos.z;
	}

	if( GetSclKey( fFrame, &vScale ) )
	{
		m_matAni._11 *= vScale.x;
		m_matAni._22 *= vScale.y;
		m_matAni._33 *= vScale.z;
	}
	
}


const D3DXMATRIX& sunTrack::GetAniMatrix()
{
	return m_matAni;
}

void sunTrack::SetStartFrame(float fStartFrame)
{
	m_fStartFrame = fStartFrame;
}

void sunTrack::SetEndFrame(float fEndFrame)
{
	m_fEndFrame = fEndFrame;
}

stKeys& sunTrack::GetKeys()
{

	return m_kKeys;
}


float sunTrack::GetAlpha(float f1, float f2, float fFrame)
{
	return ( (fFrame-f1) / (f2-f1) );
}

void sunTrack::Get2Keys(float frame, vector<stKeyPos>&keys, stKeyPos*& k1, stKeyPos*& k2)
{
	int		mkey;
	int		lkey = 0;
	int		rkey = (int)keys.size() - 1;

	if( keys.size() > 1 )
	{
		if( frame > keys[rkey].iFrame )
		{
			k1 = &keys[rkey];
			k2 = &keys[rkey];
			return;
		}
		if( frame < keys[lkey].iFrame)
		{
			k1 = &keys[lkey];
			k2 = &keys[lkey];
			return;
		}

		while( rkey >= lkey )
		{
			mkey = (rkey + lkey)/2;
			if( keys[mkey].iFrame <= frame && keys[mkey+1].iFrame >= frame )
			{
				k1 = &keys[mkey];
				k2 = &keys[mkey+1];
				return;
			}
			if( keys[mkey].iFrame > frame )
				rkey = mkey;
			else
				lkey = mkey;
		}
	}

	k1 = k2 = NULL;
}

void sunTrack::Get2Keys( float frame, vector<stKeyRot>&keys, stKeyRot*& k1, stKeyRot*& k2 )
{
	int		mkey;
	int		lkey = 0;
	int		rkey = (int)keys.size() - 1;

	if( keys.size() > 1 )
	{
		if( frame > keys[rkey].iFrame )
		{
			k1 = &keys[rkey];
			k2 = &keys[rkey];
			return;
		}
		if( frame < keys[lkey].iFrame)
		{
			k1 = &keys[lkey];
			k2 = &keys[lkey];
			return;
		}

		while( rkey >= lkey )
		{
			mkey = (rkey + lkey)/2;
			if( keys[mkey].iFrame <= frame && keys[mkey+1].iFrame >= frame )
			{
				k1 = &keys[mkey];
				k2 = &keys[mkey+1];
				return;
			}
			if( keys[mkey].iFrame > frame )
				rkey = mkey;
			else
				lkey = mkey;
		}
	}

	k1 = k2 = NULL;
}

void sunTrack::Get2Keys(float frame, vector<stKeyScl>&keys, stKeyScl*& k1, stKeyScl*& k2) 
{
	int		mkey;
	int		lkey = 0;
	int		rkey = (int)keys.size() - 1;

	if( keys.size() > 1 )
	{
		if( frame > keys[rkey].iFrame )
		{
			k1 = &keys[rkey];
			k2 = &keys[rkey];
			return;
		}
		if( frame < keys[lkey].iFrame)
		{
			k1 = &keys[lkey];
			k2 = &keys[lkey];
			return;
		}

		while( rkey >= lkey )
		{
			mkey = (rkey + lkey)/2;
			if( keys[mkey].iFrame <= frame && keys[mkey+1].iFrame >= frame )
			{
				k1 = &keys[mkey];
				k2 = &keys[mkey+1];
				return;
			}
			if( keys[mkey].iFrame > frame )
				rkey = mkey;
			else
				lkey = mkey;
		}
	}

	k1 = k2 = NULL;
}

BOOL sunTrack::GetPosKey( float fFrame, D3DXVECTOR3* pvPos )
{
	float	fA;
	stKeyPos *p1, *p2;

	Get2Keys( fFrame, m_kKeys.vecPos, p1, p2 );

	if( !p1 || !p2 ) return FALSE;

	if( p1 == p2 )
	{
		pvPos->x = p1->vPos.x;
		pvPos->y = p1->vPos.y;
		pvPos->z = p1->vPos.z;
	}
	else
	{
		fA = GetAlpha( (float)p1->iFrame, (float)p2->iFrame, fFrame );
		D3DXVec3Lerp( pvPos, &p1->vPos, &p2->vPos, fA );
	}

	return TRUE;
}

BOOL sunTrack::GetRotKey( float fFrame, D3DXQUATERNION* pqRot )
{
	float	fA;
	stKeyRot *p1,*p2;

	Get2Keys( fFrame, m_kKeys.vecRot, p1, p2 );

	if( !p1 || !p2 ) return FALSE;

	if( p1 == p2 )
	{
		pqRot->x = p2->qRot.x;
		pqRot->y = p2->qRot.y;
		pqRot->z = p2->qRot.z;
		pqRot->w = p2->qRot.w;
	}
	else
	{
		fA = GetAlpha( (float)p1->iFrame, (float)p2->iFrame, fFrame );
		D3DXQuaternionSlerp( pqRot, &p1->qRot, &p2->qRot, fA );
	}

	return TRUE;
}

BOOL sunTrack::GetSclKey( float fframe, D3DXVECTOR3* pvScale )
{
	float	fA;
	stKeyScl *p1, *p2;

	Get2Keys( fframe, m_kKeys.vecScl, p1, p2 );

	if( !p1 || !p2 ) return FALSE;

	if( p1 == p2 )
	{
		pvScale->x = p1->vScale.x;
		pvScale->y = p1->vScale.y;
		pvScale->z = p1->vScale.z;
	}
	else
	{
		fA = GetAlpha((float)p1->iFrame, (float)p2->iFrame, fframe );
		D3DXVec3Lerp( pvScale, &p1->vScale, &p2->vScale, fA );
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

sunAnimation::sunAnimation()
{
	m_TrackList_Vector.clear();
	m_TrackList_Vector.reserve(30);
}

sunAnimation::~sunAnimation()
{
	m_TrackList_Vector.clear();
}

void sunAnimation::AddTrack( sunTrackPtr spTrack )
{
	assert(spTrack);
	m_TrackList_Vector.push_back(spTrack);
}

//const D3DXMATRIX& sunAnimation::UpdateTrack( float fTime )
//{
//	BOOST_FOREACH( sunTrackPtr spTrack, m_TrackList_Vector)
//	{
//		spTrack->UpdateTrack( fTime );
//	}
//	
//	D3DXMATRIX matTemp;
//
//	return matTemp;
//}








