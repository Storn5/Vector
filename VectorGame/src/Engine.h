#pragma once
#include "GraphicsSystem/GraphicsSystem.h"
#include "WindowSystem/WindowSystem.h"
#include "Config.h"
#include <memory>

class Engine
{
private:
    std::unique_ptr<GraphicsSystem> m_graphicsSystem;
    std::unique_ptr<WindowSystem> m_windowSystem;

    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    void operator=(const Engine&) = delete;
    void operator=(Engine&&) = delete;
public:
    Engine(const GraphicsConfig& graphicsConfig);
    ~Engine() = default;
    void startGame();
    bool isInitialized();
};