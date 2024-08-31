#include "ContextManager/GLFW/GLFWManager.h"
#include "Utils/Helpers.h"

GLFWManager::GLFWManager(const GraphicsConfig& config)
    : m_window(nullptr), m_config(config)
{
	// Initialize the library
	if (!glfwInit())
	{
		fprintf(stderr, "Error while initializing GLFW\n");
		return;
	}

	// Configure the window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = nullptr;
	if (m_config.isFullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		m_config.xRes = mode->width;
		m_config.yRes = mode->height;
	}

	// Create a window and its OpenGL context
	m_window = glfwCreateWindow(m_config.xRes, m_config.yRes, m_config.windowName, monitor, nullptr);
	if (!m_window)
	{
		fprintf(stderr, "Error creating a window in GLFW\n");
		glfwTerminate();
		return;
	}

	// Make the window's context current
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);
}

GLFWManager::~GLFWManager()
{
	glfwTerminate();
}

bool GLFWManager::isInitialized()
{
	if (m_window == nullptr)
		return false;

	const char* glfwError;
	if (glfwGetError(&glfwError) != GLFW_NO_ERROR)
	{
		fprintf(stderr, "Error after initializing GLFW: %s\n", glfwError);
		return false;
	}

	return true;
}

bool GLFWManager::preFrame()
{
    // Loop until the user closes the window
	return !glfwWindowShouldClose(m_window);
}

bool GLFWManager::postFrame()
{
	// Swap front and back buffers
	glfwSwapBuffers(m_window);

	// Poll for and process events
	glfwPollEvents();

	return true;
}