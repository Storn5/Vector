#include "Engine.h"
#include "GraphicsSystem/OpenGL/OpenGLBackend.h"
#include "GraphicsSystem/DX11/DX11Backend.h"
#include "WindowSystem/Win32/Win32System.h"
#include "WindowSystem/GLFW/GLFWSystem.h"

Engine::Engine(const GraphicsConfig& graphicsConfig)
{
	// Initialize window system
	switch (graphicsConfig.windowSystem)
	{
	case WindowSystemEnum::Win32:
		m_windowSystem = std::make_unique<Win32System>(graphicsConfig);
		break;
	case WindowSystemEnum::GLFW:
		m_windowSystem = std::make_unique<GLFWSystem>(graphicsConfig);
		break;
	default:
		fprintf(stderr, "NOT IMPLEMENTED\n");
		return;
	}

	// Initialize graphics system
	switch (graphicsConfig.graphicsBackend)
	{
	case GraphicsBackendEnum::OPENGL:
		m_graphicsSystem = std::make_unique<OpenGLBackend>(graphicsConfig);
		break;
	case GraphicsBackendEnum::DX11:
		m_graphicsSystem = std::make_unique<DX11Backend>(graphicsConfig);
		break;
	default:
		fprintf(stderr, "NOT IMPLEMENTED\n");
		return;
	}
}

bool Engine::isInitialized()
{
	return m_graphicsSystem->isInitialized() && m_windowSystem->isInitialized();
}

void Engine::startGame()
{
	// Main game loop
	while (1)
	{
		if (!m_windowSystem->preFrame())
			break;

		if (!m_graphicsSystem->renderFrame())
			break;

		if (!m_windowSystem->postFrame())
			break;
	}
}