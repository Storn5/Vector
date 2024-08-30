#pragma once

class WindowSystem
{
public:
	virtual bool preFrame() = 0;
	virtual bool postFrame() = 0;
	virtual bool isInitialized() = 0;
};