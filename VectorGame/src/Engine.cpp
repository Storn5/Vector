#include "Engine.h"
#include "Enums.h"
#include "GraphicsSystem/OpenGL/OpenGLBackend.h"
#include "GraphicsSystem/DX11/DX11Backend.h"
#include "ContextManager/Win32/Win32Manager.h"
#include "ContextManager/GLFW/GLFWManager.h"

Engine::Engine(const GraphicsConfig& graphicsConfig)
{
	// Initialize window, graphics, audio and input systems
	switch (graphicsConfig.graphicsBackend)
	{
	case GraphicsBackendEnum::OPENGL:
		m_contextManager = std::make_unique<GLFWManager>(graphicsConfig);
		m_graphicsSystem = std::make_unique<OpenGLBackend>(graphicsConfig);
		break;
	case GraphicsBackendEnum::DX11:
		m_contextManager = std::make_unique<Win32Manager>(graphicsConfig);
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