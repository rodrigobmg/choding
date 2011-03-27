#ifndef _SUN_CLASS_REF_H_
#define _SUN_CLASS_REF_H_


class DLL_API sunRef
{
public:
	sunRef();
	virtual ~sunRef();
	virtual void Destory();

public:
	void IncRefCount();
	void DecRefCount();
	UINT GetRefCount() const ;

private:
	UINT m_uiRefCount;



};











#endif