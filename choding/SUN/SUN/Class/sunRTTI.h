#ifndef _SUN_CLASS_RTTI_H_
#define	_SUN_CLASS_RTTI_H_


// RTTI ( Run-Time Type Infomation )
class DLL_API sunRTTI
{
public:
	sunRTTI( const tstring strClassName, const sunRTTI* pBaseRTTI );
	const tstring& GetClassName() const;
	const sunRTTI* GetBaseRTTI() const;

protected:
	const tstring		m_strClassName;
	const sunRTTI*		m_pBaseRTTI;
};



// 최상위 클래스에 추가하는 RTTI 선언 매크로
#define DeclareRootRTTI(classname) \
	public: \
	static const sunRTTI ms_RTTI; \
	virtual const sunRTTI* GetRTTI() const { return &ms_RTTI; } \
	static bool IsExactKindOfRTTI( const sunRTTI* pRTTI, const classname *pObject ) \
{ \
	if( NULL == pObject ) \
{ \
	return false; \
} \
	return pObject->IsExactKindOfRTTI( pRTTI ); \
} \
	bool IsExactKindOfRTTI( const sunRTTI* pRTTI ) const \
{ \
	return ( GetRTTI() == pRTTI ); \
} \
	static bool IsKindOfRTTI( const sunRTTI* pRTTI, const classname *pObject ) \
{ \
	if( NULL == pObject ) \
{ \
	return false; \
} \
	return pObject->IsKindOfRTTI( pRTTI ); \
} \
	bool IsKindOfRTTI( const sunRTTI* pRTTI ) const \
{ \
	const sunRTTI* pTmp = GetRTTI(); \
	while( NULL != pTmp ) \
{ \
	if (pTmp == pRTTI) \
{ \
	return true; \
} \
	pTmp = pTmp->GetBaseRTTI(); \
} \
	return false; \
} \
	static classname* DynamicCastRTTI( const sunRTTI* pRTTI, \
	const classname* pObject ) \
{ \
	if( !pObject ) \
{ \
	return NULL; \
} \
	return pObject->DynamicCastRTTI( pRTTI ); \
} \
	classname* DynamicCastRTTI( const sunRTTI* pRTTI ) const \
{ \
	return ( IsKindOfRTTI( pRTTI ) ? ( classname* ) this : 0 ); \
}

// DeclareRootRTTI가 삽입된 상위클래스를 상속 받는 자식 클래스에게 추가하는 선언 매크로
#define DeclareRTTI \
	public: \
	static const sunRTTI ms_RTTI; \
	virtual const sunRTTI* GetRTTI() const {return &ms_RTTI;}


// 클래스 본문에 삽입
#define ImplementRootRTTI(classname) \
	const sunRTTI classname::ms_RTTI(L#classname, NULL)
#define ImplementRTTI( classname, baseclassname) \
	const sunRTTI classname::ms_RTTI(L#classname, &baseclassname::ms_RTTI)

// 해당 오브젝트가 해당 클래스가 맞는지 비교 매크로
#define IsExactKindOf(classname, pObject) \
	classname::IsExactKindOfRTTI(&classname::ms_RTTI, pObject)
#define IsKindOf(classname, pObject) \
	classname::IsKindOfRTTI(&classname::ms_RTTI, pObject)
#define DynamicCast(classname, pObject) \
	((classname*) classname::DynamicCastRTTI(&classname::ms_RTTI, pObject))






































//메모리 풀링 매크로
#define ImplementBPool(class) boost::pool<> class::bpool(sizeof(class))
#define DeclareBPool \
public: \
	void* operator new(size_t s) \
{ \
	return bpool.malloc(); \
} \
	void operator delete(void *p) \
{ \
	bpool.free(p); \
} \
private: \
	static boost::pool<> bpool;

#endif
