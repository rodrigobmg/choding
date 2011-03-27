#ifndef _SUN_COREMANAGER_CORE_COREBASE_H_
#define _SUN_COREMANAGER_CORE_COREBASE_H_


class DLL_API sunCoreBase
{
public:
	sunCoreBase();
	virtual ~sunCoreBase();


public:
	virtual int  InitializeCore() = 0;
	virtual void DestoryCore() = 0;
	virtual void UpdateCore() = 0;


public:
	void SetDestroyOrder(int iOrder);
	int  GetDestoryOrder() ;
	void  SetCoreName(const tstring& strCoreName);
	const tstring& GetCoreName();

protected:
	int		m_iDestroyOrder;
	tstring m_strCoreName;


};


#endif