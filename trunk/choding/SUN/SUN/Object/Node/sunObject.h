#ifndef _SUN_OBJECT_NODE_OBJECT_H_
#define _SUN_OBJECT_NODE_OBJECT_H_


class DLL_API sunObject : public sunRef
{

	DeclareRootRTTI(sunObject)


public:
	sunObject();
	virtual ~sunObject();
	void Destory();

public:
	inline void  SetName( const tstring& strName);
	inline const tstring& GetName();
	inline bool  IsName( const tstring& strName);


public:
	virtual sunObject* CloneMe();


protected:
	tstring m_strName;

};


inline void sunObject::SetName( const tstring& strName)
{
	m_strName = strName;
}

inline const tstring& sunObject::GetName()
{
	return m_strName;
}

inline bool sunObject::IsName( const tstring& strName)
{
	return ( m_strName.compare( strName ) == 0 );
}



#endif