#pragma once

#include "Enums.h"

struct GraphicsConfig
{
	GraphicsBackendEnum graphicsBackend;
	ContextManagerEnum contextManager;
	const char* windowName;
	bool isFullscreen;
	unsigned int xRes = 0, yRes = 0;
	bool vSync;
	float nearPlane, farPlane;
};