#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Config.h"
#include "ContextManager/ContextManager.h"
#include <Windows.h>

class Win32Manager : public ContextManager
{
public:
    Win32Manager(const GraphicsConfig& config, HWND* hWnd);
    ~Win32Manager();
    bool isInitialized();
    bool preFrame();
    bool postFrame();

private:
    Win32Manager(const Win32Manager&) = delete;
    Win32Manager(Win32Manager&&) = delete;
    Win32Manager& operator=(const Win32Manager&) = delete;
    Win32Manager& operator=(Win32Manager&&) = delete;

    GraphicsConfig m_config;
    HINSTANCE m_hInstance;
    HWND m_hWnd;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);