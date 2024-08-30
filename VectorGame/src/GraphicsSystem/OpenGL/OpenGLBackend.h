#pragma once
#include <string>
#include <chrono>
#include <glew.h>
#include "GraphicsSystem/GraphicsSystem.h"
#include "Config.h"
#include "ShaderFile.h"

class OpenGLBackend : public GraphicsSystem
{
private:
    GraphicsConfig m_config;
    std::chrono::steady_clock::time_point m_startTime;

    bool testOpenGL();
    unsigned int createShader(const GLenum shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(ShaderFile file);

public:
    OpenGLBackend(const GraphicsConfig& config);
    ~OpenGLBackend();
    bool isInitialized();
    bool renderFrame();
};