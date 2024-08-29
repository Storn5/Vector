#pragma once

class GraphicsSystem
{
public:
	virtual void playGameLoop() = 0;
	virtual bool isInitialized() = 0;
};