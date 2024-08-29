#pragma once
#include "GraphicsSystem.h"
#include "Enums.h"
#include "Config.h"
#include <memory>

class Engine
{
private:
    std::unique_ptr<GraphicsSystem> graphicsSystem;

    Engine(const Engine&) = delete;
    Engine(Engine&&) = delete;
    void operator=(const Engine&) = delete;
    void operator=(Engine&&) = delete;
public:
    bool isInitialized = false;

    Engine(const GraphicsBackendEnum& graphicsBackend, const GraphicsConfig& graphicsConfig);
    ~Engine() = default;
    void playGame();
};