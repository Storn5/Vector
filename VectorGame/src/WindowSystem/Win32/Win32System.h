#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include "WindowSystem/WindowSystem.h"
#include "Config.h"

class Win32System : public WindowSystem
{
private:
    GraphicsConfig m_config;
    HINSTANCE m_hInstance;
    HWND m_hWnd;

public:
    Win32System(const GraphicsConfig& config);
    ~Win32System();
    bool isInitialized();
    bool preFrame();
    bool postFrame();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);