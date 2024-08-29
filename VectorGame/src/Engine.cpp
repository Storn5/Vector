#include "Engine.h"
#include "OpenGL/OpenGLBackend.h"
#include <stdio.h>

Engine::Engine(const GraphicsBackendEnum& graphicsBackend, const GraphicsConfig& graphicsConfig)
{

	if (graphicsBackend == GraphicsBackendEnum::OPENGL)
		graphicsSystem = std::make_unique<OpenGLBackend>(graphicsConfig);
	else if (graphicsBackend == GraphicsBackendEnum::VULKAN || graphicsBackend == GraphicsBackendEnum::DX11 || graphicsBackend == GraphicsBackendEnum::DX12)
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

void Engine::playGame()
{
	graphicsSystem->playGameLoop();
}