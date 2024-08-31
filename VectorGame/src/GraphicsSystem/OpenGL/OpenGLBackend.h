#pragma once

#include "Config.h"
#include "GraphicsSystem/GraphicsSystem.h"
#include "ShaderFile.h"
#include <chrono>
#include <glew.h>
#include <string>

class OpenGLBackend : public GraphicsSystem
{
public:
    OpenGLBackend(const GraphicsConfig& config);
    ~OpenGLBackend();
    bool isInitialized();
    bool renderFrame();

private:
    OpenGLBackend(const OpenGLBackend&) = delete;
    OpenGLBackend(OpenGLBackend&&) = delete;
    OpenGLBackend& operator=(const OpenGLBackend&) = delete;
    OpenGLBackend& operator=(OpenGLBackend&&) = delete;

    bool testOpenGL();
    unsigned int createShader(const GLenum shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(ShaderFile file);

    GraphicsConfig m_config;
    std::chrono::steady_clock::time_point m_startTime;
};