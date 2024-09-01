#include "Engine.h"
#include "Enums.h"
#include "GraphicsSystem/OpenGL/OpenGLBackend.h"
#include "GraphicsSystem/DX11/DX11Backend.h"
#include "ContextManager/Win32/Win32Manager.h"
#include "ContextManager/GLFW/GLFWManager.h"

Engine::Engine(const GraphicsConfig& graphicsConfig)
{
	// Initialize window system
	switch (graphicsConfig.contextManager)
	{
	case ContextManagerEnum::Win32:
		m_contextManager = std::make_unique<Win32Manager>(graphicsConfig);
		break;
	case ContextManagerEnum::GLFW:
		m_contextManager = std::make_unique<GLFWManager>(graphicsConfig);
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
		m_graphicsSystem = std::make_unique<DX11Backend>(graphicsConfig, *m_contextManager);
		break;
	default:
		return;
		fprintf(stderr, "NOT IMPLEMENTED\n");
		return;
	}
}

bool Engine::isInitialized()
{
	return m_graphicsSystem->isInitialized() && m_contextManager->isInitialized();
}

void Engine::startGame()
{
	// Main game loop
	while (1)
	{
		if (!m_contextManager->preFrame())
			break;

		if (!m_graphicsSystem->renderFrame())
			break;

		if (!m_contextManager->postFrame())
			break;
	}
}