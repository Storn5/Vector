#pragma once

#include "Enums.h"

struct GraphicsConfig
{
	const char* windowName;
	GraphicsBackendEnum graphicsBackend;
	bool isFullscreen;
	bool vSync;
	unsigned int xRes = 0, yRes = 0;
	unsigned int refreshRate;
	float nearPlane, farPlane;
};