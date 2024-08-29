#include "Engine.h"
#include "OpenGL/OpenGLBackend.h"
#include "DX11/DX11Backend.h"
#include <stdio.h>

Engine::Engine(const GraphicsBackendEnum& graphicsBackend, const GraphicsConfig& graphicsConfig)
{
	// Initialize appropriate graphics system depending on configuration
	if (graphicsBackend == GraphicsBackendEnum::OPENGL)
		graphicsSystem = std::make_unique<OpenGLBackend>(graphicsConfig);
	else if (graphicsBackend == GraphicsBackendEnum::DX11)
		graphicsSystem = std::make_unique<DX11Backend>(graphicsConfig);
	else if (graphicsBackend == GraphicsBackendEnum::VULKAN || graphicsBackend == GraphicsBackendEnum::DX12)
	{
		fprintf(stderr, "NOT IMPLEMENTED\n");
		return;
	}

	if (!graphicsSystem->isInitialized())
	{
		fprintf(stderr, "Graphics system failed to initialize\n");
		return;
	}

	isInitialized = true;
}

void Engine::startGame()
{
	graphicsSystem->playGameLoop();
}