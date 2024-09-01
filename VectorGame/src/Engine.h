#pragma once

#include "Renderer/Renderer.h"
#include "ContextManager/ContextManager.h"
#include "Config.h"
#include <memory>

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

    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<ContextManager> m_contextManager;
};