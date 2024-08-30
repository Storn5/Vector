#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <glfw3.h>
#include "ContextManager/ContextManager.h"
#include "Config.h"

class GLFWManager : public ContextManager
{
private:
    GraphicsConfig m_config;
    GLFWwindow* m_window;

public:
    GLFWManager(const GraphicsConfig& config);
    ~GLFWManager();
    bool isInitialized();
    bool preFrame();
    bool postFrame();
};