#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <Windows.h>
#include "ContextManager/ContextManager.h"
#include "Config.h"

class Win32Manager : public ContextManager
{
private:
    GraphicsConfig m_config;
    HINSTANCE m_hInstance;
    HWND m_hWnd;

public:
    Win32Manager(const GraphicsConfig& config);
    ~Win32Manager();
    bool isInitialized();
    bool preFrame();
    bool postFrame();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);