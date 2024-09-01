#pragma once

#include "Config.h"
#include "Renderer/Renderer.h"
#include <chrono>
#include <glew.h>
#include <string>

struct ShaderFile
{
    const std::string vertexShaderSource, fragmentShaderSource;
};

class OpenGLRenderer : public Renderer
{
public:
    OpenGLRenderer(const GraphicsConfig& config);
    ~OpenGLRenderer();
    bool isInitialized();
    bool renderFrame();

private:
    OpenGLRenderer(const OpenGLRenderer&) = delete;
    OpenGLRenderer(OpenGLRenderer&&) = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

    bool testOpenGL();
    unsigned int createShader(const GLenum shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(ShaderFile file);
    ShaderFile parseShaderFile(const std::string& filename);

    GraphicsConfig m_config;
    std::chrono::steady_clock::time_point m_startTime;
};