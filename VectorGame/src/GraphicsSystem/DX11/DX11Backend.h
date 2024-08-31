#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define WIN32_LEAN_AND_MEAN

#include "Config.h"
#include "GraphicsSystem/GraphicsSystem.h"
#include <string>
#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class DX11Backend : public GraphicsSystem
{
public:
    DX11Backend(const GraphicsConfig& config, const HWND& hWnd);
    ~DX11Backend();
    bool isInitialized();
    bool renderFrame();
    void BeginScene(float, float, float, float);
    void EndScene();
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    void GetProjectionMatrix(DirectX::XMMATRIX&);
    void GetWorldMatrix(DirectX::XMMATRIX&);
    void GetOrthoMatrix(DirectX::XMMATRIX&);
    void GetVideoCardInfo(char*, int&);
    void SetBackBufferRenderTarget();
    void ResetViewport();

private:
    DX11Backend(const DX11Backend&) = delete;
    DX11Backend(DX11Backend&&) = delete;
    DX11Backend& operator=(const DX11Backend&) = delete;
    DX11Backend& operator=(DX11Backend&&) = delete;

    unsigned int createShader(const unsigned int shaderType, const std::string& shaderSource);
    unsigned int createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);

    GraphicsConfig m_config;
    int m_gpuMemory;
    char m_gpuDescription[128];
    IDXGISwapChain* m_swapChain;
    ID3D11Device* m_device;
    ID3D11DeviceContext* m_deviceContext;
    ID3D11RenderTargetView* m_renderTargetView;
    ID3D11Texture2D* m_depthStencilBuffer;
    ID3D11DepthStencilState* m_depthStencilState;
    ID3D11DepthStencilView* m_depthStencilView;
    ID3D11RasterizerState* m_rasterState;
    DirectX::XMMATRIX m_projectionMatrix;
    DirectX::XMMATRIX m_worldMatrix;
    DirectX::XMMATRIX m_orthoMatrix;
    D3D11_VIEWPORT m_viewport;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);