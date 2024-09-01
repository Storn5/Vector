#pragma once

class Renderer
{
public:
	virtual bool renderFrame() = 0;
	virtual bool isInitialized() = 0;
};