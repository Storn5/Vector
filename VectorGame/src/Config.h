#pragma once

#include "Enums.h"

struct GraphicsConfig
{
	GraphicsBackendEnum graphicsBackend;
	WindowSystemEnum windowSystem;
	const char* windowName;
	bool isFullscreen;
	unsigned int xRes = 0, yRes = 0;
};