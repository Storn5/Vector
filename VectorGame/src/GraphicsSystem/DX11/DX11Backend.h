#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include "GraphicsSystem/GraphicsSystem.h"
#include "Config.h"

class DX11Backend : public GraphicsSystem
{
private:
    GraphicsConfig m_config;

    unsigned int createShader(const unsigned int shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

public:
    DX11Backend(const GraphicsConfig& config);
    ~DX11Backend();
    bool isInitialized();
    bool renderFrame();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);