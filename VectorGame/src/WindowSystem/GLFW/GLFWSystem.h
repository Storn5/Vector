#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <glfw3.h>
#include "WindowSystem/WindowSystem.h"
#include "Config.h"

class GLFWSystem : public WindowSystem
{
private:
    GraphicsConfig m_config;
    GLFWwindow* m_window;

public:
    GLFWSystem(const GraphicsConfig& config);
    ~GLFWSystem();
    bool isInitialized();
    bool preFrame();
    bool postFrame();
};