#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include "GraphicsSystem.h"
#include "Config.h"

class DX11Backend : public GraphicsSystem
{
private:
    GraphicsConfig m_config;
    HINSTANCE m_hInstance;
    HWND m_hWnd;

    unsigned int createShader(const unsigned int shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    unsigned int parseShaderFile(const std::string& filename);
    bool renderFrame();

public:
    DX11Backend(const GraphicsConfig& config);
    ~DX11Backend();
    bool isInitialized();
    void playGameLoop();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);