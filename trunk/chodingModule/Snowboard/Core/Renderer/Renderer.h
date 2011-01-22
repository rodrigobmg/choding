#pragma once

class CRenderer{

private:

	void Begin();
	void End();
	void Render();

public:
	CRenderer();
	virtual ~CRenderer();
	
	void UpdateFrame();
};