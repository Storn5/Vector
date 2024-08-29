#pragma once
#include <string>
#include <glew.h>
#include <glfw3.h>
#include "GraphicsSystem.h"
#include "Config.h"

class OpenGLBackend : public GraphicsSystem
{
private:
    GraphicsConfig m_config;
    GLFWwindow* m_window;

    bool testOpenGL();
    unsigned int createShader(const GLenum shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    unsigned int parseShaderFile(const std::string& filename);

public:
    OpenGLBackend(const GraphicsConfig& config);
    ~OpenGLBackend();
    bool isInitialized();
    void playGameLoop();
};