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

	int posX = 0, posY = 0;

	GLFWmonitor* monitor = nullptr;
	if (m_config.isFullscreen)
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		m_config.xRes = mode->width;
		m_config.yRes = mode->height;
		m_config.refreshRate = mode->refreshRate;
		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	}
	else
	{
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		posX = (mode->width - m_config.xRes) / 2;
		posY = (mode->height - m_config.yRes) / 2;
		monitor = nullptr;
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

	// Configure swap interval depending on vSync
	if (m_config.vSync)
	{
		// Lock to screen refresh rate
		glfwSetWindowMonitor(m_window, monitor, posX, posY, m_config.xRes, m_config.yRes, m_config.refreshRate);
		glfwSwapInterval(1);
	}
	else
	{
		// Present as fast as possible
		glfwSetWindowMonitor(m_window, monitor, posX, posY, m_config.xRes, m_config.yRes, GLFW_DONT_CARE);
		glfwSwapInterval(0);
	}
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
	// Swap front and back buffers, if vSync is enabled this will wait for vSync
	glfwSwapBuffers(m_window);

	// Poll for and process events
	glfwPollEvents();

	return true;
}