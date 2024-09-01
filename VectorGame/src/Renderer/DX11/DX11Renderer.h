#pragma once

#define WIN32_LEAN_AND_MEAN

#include "Config.h"
#include "Renderer/Renderer.h"
#include "ContextManager/ContextManager.h"
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class DX11Renderer : public Renderer
{
public:
    DX11Renderer(const GraphicsConfig& config, const ContextManager& contextManager);
    ~DX11Renderer();
    bool isInitialized();
    bool renderFrame();
    void startFrame(float, float, float, float);
    void endFrame();
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    void GetProjectionMatrix(DirectX::XMMATRIX&);
    void GetWorldMatrix(DirectX::XMMATRIX&);
    void GetOrthoMatrix(DirectX::XMMATRIX&);
    void GetVideoCardInfo(char*, int&);
    void SetBackBufferRenderTarget();
    void ResetViewport();

private:
    DX11Renderer(const DX11Renderer&) = delete;
    DX11Renderer(DX11Renderer&&) = delete;
    DX11Renderer& operator=(const DX11Renderer&) = delete;
    DX11Renderer& operator=(DX11Renderer&&) = delete;

    unsigned int createShader(const unsigned int shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

    // Configuration
    GraphicsConfig m_config;
    
    // Device
    const static unsigned int gpuDescLength = 128;
    int m_gpuMemory;
    char m_gpuDescription[gpuDescLength];
    IDXGISwapChain* m_swapChain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;

    // Main target view texture and stencil buffer
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;

    // Matrices
    DirectX::XMMATRIX m_projectionMatrix;
    DirectX::XMMATRIX m_worldMatrix;
    DirectX::XMMATRIX m_orthoMatrix;
    D3D11_VIEWPORT m_viewport;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);