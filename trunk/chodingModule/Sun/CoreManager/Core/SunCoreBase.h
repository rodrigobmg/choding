#pragma once


class DLL_API Sun_CCoreBase
{
public:
	Sun_CCoreBase();
	virtual ~Sun_CCoreBase();

public:
	virtual int  InitializeCore() = 0;
	virtual void DestoryCore()	  = 0;
	virtual void UpdateCore()	  = 0;

public:

	inline	void  SetCoreName(const tstring& strCoreName);
	inline  const tstring& GetCoreName();

protected:
	tstring m_strCoreName;

};