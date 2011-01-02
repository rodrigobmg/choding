#pragma once


class Sun_CCoreBase;

class DLL_API Sun_CCoreManager
{
public:
	Sun_CCoreManager(HWND hWnd);
	virtual ~Sun_CCoreManager();


public:
	Sun_CCoreBase* RegisterCore(const tstring& strName, Sun_CCoreBase* pCore);
	void	DestroyCore();


	inline Sun_CCoreBase* GetCore(const tstring& strName);

protected:
	typedef stdext::hash_map<tstring, Sun_CCoreBase*>			 Core_HASHMAP;
	typedef stdext::hash_map<tstring, Sun_CCoreBase*>::iterator  Core_HASHMAP_ITERATOR;
	Core_HASHMAP m_CoreList;




protected:
	HWND		m_hWnd;

};