#ifndef _SUN_CLASS_SMARTPOINTER_H_
#define _SUN_CLASS_SMARTPOINTER_H_

//
template <class T> 
class sunCSmartPointer
{
public:
	//생성자 소멸자.
	inline sunCSmartPointer(T* pObject = (T*)0);
	inline sunCSmartPointer(const sunCSmartPointer& ptr);
	inline ~sunCSmartPointer();



	//연산자 오버로딩
	inline operator T*() const;
	inline T& operator*() const;
	inline T* operator->() const;

	inline sunCSmartPointer& operator=(const sunCSmartPointer& ptr);
	inline sunCSmartPointer& operator=(T* pObject);

	inline bool operator ==(T* pObject) const;
	inline bool operator !=(T* pObject) const;
	inline bool operator ==(const sunCSmartPointer& ptr) const;
	inline bool operator !=(const sunCSmartPointer& ptr) const;

protected:
	T* m_pObject;
};

#define SmartPointerCast(type, smartptr)((type*)(void*)(smartptr))
#define SmartPointer(classname)\
	typedef sunCSmartPointer<classname> classname##Ptr


template <class T>
inline sunCSmartPointer<T>::sunCSmartPointer(T* pObject)
{
	m_pObject = pObject;
	if (m_pObject)
		m_pObject->IncRefCount();
}

template <class T>
inline sunCSmartPointer<T>::sunCSmartPointer(const sunCSmartPointer& ptr)
{
	m_pObject = ptr.m_pObject;
	if (m_pObject)
		m_pObject->IncRefCount();
}

template <class T>
inline sunCSmartPointer<T>::~sunCSmartPointer()
{
	if (m_pObject)
		m_pObject->DecRefCount();
}

template <class T>
inline sunCSmartPointer<T>::operator T*() const
{
	return m_pObject;
}

template <class T>
inline T& sunCSmartPointer<T>::operator*() const
{
	return *m_pObject;
}

template <class T>
inline T* sunCSmartPointer<T>::operator->() const
{
	return m_pObject;
}

template <class T>
inline sunCSmartPointer<T>& sunCSmartPointer<T>::operator=(const sunCSmartPointer& ptr)
{
	if (m_pObject != ptr.m_pObject)
	{
		if (m_pObject)
			m_pObject->DecRefCount();
		m_pObject = ptr.m_pObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}
	return *this;
}

template <class T>
inline sunCSmartPointer<T>& sunCSmartPointer<T>::operator=(T* pObject)
{
	if (m_pObject != pObject)
	{
		if (m_pObject)
			m_pObject->DecRefCount();
		m_pObject = pObject;
		if (m_pObject)
			m_pObject->IncRefCount();
	}
	return *this;
}

template <class T>
inline bool sunCSmartPointer<T>::operator==(T* pObject) const
{
	return (m_pObject == pObject);
}

template <class T>
inline bool sunCSmartPointer<T>::operator!=(T* pObject) const
{
	return (m_pObject != pObject);
}

template <class T>
inline bool sunCSmartPointer<T>::operator==(const sunCSmartPointer& ptr) const
{
	return (m_pObject == ptr.m_pObject);
}

template <class T>
inline bool sunCSmartPointer<T>::operator!=(const sunCSmartPointer& ptr) const
{
	return (m_pObject != ptr.m_pObject);
}

#endif