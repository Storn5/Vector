#include "Engine.h"
#include "Enums.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Renderer/DX11/DX11Renderer.h"
#include "ContextManager/Win32/Win32Manager.h"
#include "ContextManager/GLFW/GLFWManager.h"

Engine::Engine(const GraphicsConfig& graphicsConfig)
{
	// Initialize window, graphics, audio and input systems
	switch (graphicsConfig.renderer)
	{
	case RendererEnum::OPENGL:
		m_contextManager = std::make_unique<GLFWManager>(graphicsConfig);
		m_renderer = std::make_unique<OpenGLRenderer>(graphicsConfig);
		break;
	case RendererEnum::DX11:
		m_contextManager = std::make_unique<Win32Manager>(graphicsConfig);
		m_renderer = std::make_unique<DX11Renderer>(graphicsConfig, *m_contextManager);
		break;
	default:
		return;
		fprintf(stderr, "NOT IMPLEMENTED\n");
		return;
	}
}

bool Engine::isInitialized()
{
	return m_renderer->isInitialized() && m_contextManager->isInitialized();
}

void Engine::startGame()
{
	// Main game loop
	while (1)
	{
		if (!m_contextManager->preFrame())
			break;

		if (!m_renderer->renderFrame())
			break;

		if (!m_contextManager->postFrame())
			break;
	}
}