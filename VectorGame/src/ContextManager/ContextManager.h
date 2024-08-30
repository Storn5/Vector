#pragma once

class ContextManager
{
public:
	virtual bool preFrame() = 0;
	virtual bool postFrame() = 0;
	virtual bool isInitialized() = 0;
};