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



// �ֻ��� Ŭ������ �߰��ϴ� RTTI ���� ��ũ��
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

// DeclareRootRTTI�� ���Ե� ����Ŭ������ ��� �޴� �ڽ� Ŭ�������� �߰��ϴ� ���� ��ũ��
#define DeclareRTTI \
	public: \
	static const sunRTTI ms_RTTI; \
	virtual const sunRTTI* GetRTTI() const {return &ms_RTTI;}


// Ŭ���� ������ ����
#define ImplementRootRTTI(classname) \
	const sunRTTI classname::ms_RTTI(L#classname, NULL)
#define ImplementRTTI( classname, baseclassname) \
	const sunRTTI classname::ms_RTTI(L#classname, &baseclassname::ms_RTTI)

// �ش� ������Ʈ�� �ش� Ŭ������ �´��� �� ��ũ��
#define IsExactKindOf(classname, pObject) \
	classname::IsExactKindOfRTTI(&classname::ms_RTTI, pObject)
#define IsKindOf(classname, pObject) \
	classname::IsKindOfRTTI(&classname::ms_RTTI, pObject)
#define DynamicCast(classname, pObject) \
	((classname*) classname::DynamicCastRTTI(&classname::ms_RTTI, pObject))






































//�޸� Ǯ�� ��ũ��
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
