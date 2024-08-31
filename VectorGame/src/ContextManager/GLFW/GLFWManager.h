#pragma once

#include "Config.h"
#include "ContextManager/ContextManager.h"
#include <glfw3.h>

class GLFWManager : public ContextManager
{
public:
    GLFWManager(const GraphicsConfig& config);
    ~GLFWManager();
    bool isInitialized();
    bool preFrame();
    bool postFrame();

private:
    GLFWManager(const GLFWManager&) = delete;
    GLFWManager(GLFWManager&&) = delete;
    GLFWManager& operator=(const GLFWManager&) = delete;
    GLFWManager& operator=(GLFWManager&&) = delete;

    GraphicsConfig m_config;
    GLFWwindow* m_window;
};