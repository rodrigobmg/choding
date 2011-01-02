#pragma once


class DLL_API Sun_CGraphicCore : Sun_CCoreBase
{
public:
	Sun_CGraphicCore();
	virtual ~Sun_CGraphicCore();

public:
	virtual int  InitializeCore();
	virtual void DestoryCore();
	virtual void UpdateCore();

protected:

};