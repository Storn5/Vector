#pragma once

class GraphicsSystem
{
public:
	virtual bool renderFrame() = 0;
	virtual bool isInitialized() = 0;
};