#pragma once

#include "GraphicsSystem/GraphicsSystem.h"
#include "ContextManager/ContextManager.h"
#include "Config.h"
#include <memory>
#include <Windows.h>

class Engine
{
public:
    Engine(const GraphicsConfig& graphicsConfig);
    ~Engine() = default;
    void startGame();
    bool isInitialized();
private:
    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&) = delete;

    std::unique_ptr<GraphicsSystem> m_graphicsSystem;
    std::unique_ptr<ContextManager> m_contextManager;
    HWND m_hWnd;
};