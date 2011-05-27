#ifndef _GDS_CORE_RESOURCE_INDEXBUFFER_H_
#define _GDS_CORE_RESOURCE_INDEXBUFFER_H_

#include "../GdsResBaseType.h"

class GdsIndexBuffer : public GdsResBase
{
private:

	LPDIRECT3DINDEXBUFFER9		m_pIB;
	std::vector<GDSINDEX>		m_pBuffer;

	int							m_Index_Maxcount;
	int							m_iStartIndex;
	int							m_iEndIndex;
	int							m_ePrimitiveType;
	

protected:
	
	virtual void				vClear();
	virtual HRESULT				vRelease();
	virtual HRESULT				vLoadResource( LPDIRECT3DDEVICE9 device );

public:
	GdsIndexBuffer();
	virtual ~GdsIndexBuffer();	

	LPDIRECT3DINDEXBUFFER9		Get(){ return m_pIB; }

	void						Alloc();
	void						Free();

	void						AddIndex( GDSINDEX& index );
	int							GetTotalIndex(){ return m_Index_Maxcount; }
	bool						IsBufferEmpty(){ return m_pBuffer.empty(); }

	int							GetIndexMaxCount(){ return m_Index_Maxcount; }
	void						SetStartIndex( int index ){ m_iStartIndex = index; }
	void						SetEndIndex( int index ){ m_iEndIndex = index; }

};

typedef boost::shared_ptr< GdsIndexBuffer >	GdsIndexBufferPtr;

#endif
