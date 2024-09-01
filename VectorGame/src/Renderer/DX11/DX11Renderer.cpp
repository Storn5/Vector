#include "Renderer/DX11/DX11Renderer.h"
#include "ContextManager/Win32/Win32Manager.h"
#include "Utils/Helpers.h"
#include <Config.h>
#include <string>
#include <vector>

DX11Renderer::DX11Renderer(const GraphicsConfig& config, const ContextManager& contextManager)
    : m_config(config)
{
	// Initialize all pointers & other variables needed for DirectX initialization
	m_swapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	m_renderTargetView = nullptr;
	m_depthStencilBuffer = nullptr;
	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_rasterState = nullptr;
	m_gpuMemory = 0;

	HRESULT result = 0;
	IDXGIFactory* factory = nullptr;
	IDXGIAdapter* adapter = nullptr;
	IDXGIOutput* adapterOutput;
	unsigned int numModes = 0, numerator = 0, denominator = 0;
	unsigned long long stringLength = 0;
	DXGI_MODE_DESC* displayModeList = nullptr;
	DXGI_ADAPTER_DESC adapterDesc;
	int error = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Texture2D* backBufferPtr = nullptr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	float fieldOfView = 0.0f, screenAspect = 0.0f;



	/*                                        Getting refresh rate and graphics card description                                        */



	// Create a DirectX graphics interface factory
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	ASSERT(!FAILED(result))

	// Use the factory to the adapters for the primary graphics interface (graphics cards)
	int i = 0;
	std::vector <IDXGIAdapter*> adapters;
	while (factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		// Get the adapter (graphics card) description
		result = adapter->GetDesc(&adapterDesc);
		ASSERT(!FAILED(result))

		// Convert the name of the graphics card to a character array and store it
		error = wcstombs_s(&stringLength, m_gpuDescription, GPU_DESC_LENGTH, adapterDesc.Description, GPU_DESC_LENGTH);
		ASSERT(error == 0)

		fprintf(stdout, "%d. GPU description (wide): %S\n", i, adapterDesc.Description);

		// Store the graphics card memory in megabytes
		m_gpuMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		fprintf(stdout, "%d. GPU memory: %d MB\n", i, m_gpuMemory);

		adapters.push_back(adapter);
		++i;
	}

	// Let user choose the GPU if there are multiple
	if (adapters.size() > 1)
	{
		fprintf(stdout, "\nSelect GPU: ");
		scanf_s("%d", &i);

		ASSERT(i >= 0 && i <= adapters.size())
		adapter = adapters[i];

		result = adapter->GetDesc(&adapterDesc);
		ASSERT(!FAILED(result))

		m_gpuMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);
		error = wcstombs_s(&stringLength, m_gpuDescription, GPU_DESC_LENGTH, adapterDesc.Description, GPU_DESC_LENGTH);
		ASSERT(error == 0)

		fprintf(stdout, "Selected GPU %d %s\n", i, m_gpuDescription);
	}

	// Enumerate the primary adapter output (monitor)
	result = adapter->EnumOutputs(0, &adapterOutput);
	ASSERT(!FAILED(result))

	// Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor)
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	ASSERT(!FAILED(result))

	// Create a list to hold all the possible display modes for this monitor/graphics card combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	ASSERT(displayModeList)

	// Now fill the display mode list structures
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	ASSERT(!FAILED(result))

	// Now go through all the display modes and find the one that matches the screen width and height
	for (i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == m_config.xRes)
		{
			if (displayModeList[i].Height == m_config.yRes)
			{
				// Store the numerator and denominator of the refresh rate
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				m_config.refreshRate = numerator / denominator;
				break;
			}
		}
	}

	// Clean up unnecessary memory
	delete[] displayModeList;
	displayModeList = nullptr;
	adapterOutput->Release();
	adapterOutput = nullptr;
	adapter->Release();
	adapter = nullptr;
	factory->Release();
	factory = nullptr;



	/*                                        Initializing the swap chain                                        */



	// Initialize the swap chain description
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer
	swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer
	swapChainDesc.BufferDesc.Width = m_config.xRes;
	swapChainDesc.BufferDesc.Height = m_config.yRes;

	// Set regular 32-bit surface for the back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer
	if (m_config.vSync)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to
	swapChainDesc.OutputWindow = ((const Win32Manager&)contextManager).getWindowHandle();

	// Turn multisampling off
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode
	if (m_config.isFullscreen)
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// Set the scan line ordering and scaling to unspecified
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags
	swapChainDesc.Flags = 0;

	// Create the swap chain, Direct3D device, and Direct3D device context
	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);
	ASSERT(!FAILED(result))

	// Get the pointer to the back buffer
	result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	ASSERT(!FAILED(result))

	// Create the render target view with the back buffer pointer
	result = m_device->CreateRenderTargetView(backBufferPtr, nullptr, &m_renderTargetView);
	ASSERT(!FAILED(result))

	// Release pointer to the back buffer as we no longer need it
	backBufferPtr->Release();
	backBufferPtr = nullptr;

	// Initialize the description of the depth buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer
	depthBufferDesc.Width = m_config.xRes;
	depthBufferDesc.Height = m_config.yRes;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description
	result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
	ASSERT(!FAILED(result))

	// Initialize the description of the stencil state
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state
	result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
	ASSERT(!FAILED(result))

	// Set the depth stencil state
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Initialize the depth stencil view
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view
	result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
	ASSERT(!FAILED(result))

	// Bind the render target view and depth stencil buffer to the output render pipeline
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out
	result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
	ASSERT(!FAILED(result))

	// Now set the rasterizer state
	m_deviceContext->RSSetState(m_rasterState);

	// Setup the viewport for rendering
	m_viewport.Width = (float)m_config.xRes;
	m_viewport.Height = (float)m_config.yRes;
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;

	// Create the viewport
	m_deviceContext->RSSetViewports(1, &m_viewport);

	// Setup the projection matrix
	fieldOfView = 3.141592654f / 4.0f;
	screenAspect = (float)m_config.xRes / (float)m_config.yRes;

	// Create the projection matrix for 3D rendering
	m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, m_config.nearPlane, m_config.farPlane);

	// Initialize the world matrix to the identity matrix
	m_worldMatrix = DirectX::XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering
	m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)m_config.xRes, (float)m_config.yRes, m_config.nearPlane, m_config.farPlane);
}

DX11Renderer::~DX11Renderer()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, nullptr);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}
}

void DX11Renderer::startFrame(float red, float green, float blue, float alpha)
{
	float color[4] = { red, green, blue, alpha };

	// Clear the back buffer with background color
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

	// Clear the depth buffer
	m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX11Renderer::endFrame()
{
	// Swap the back buffer to screen, lock to refresh rate if vSync enabled
	if (m_config.vSync)
		m_swapChain->Present(1, 0);
	else
		m_swapChain->Present(0, 0);
}

ID3D11Device* DX11Renderer::GetDevice()
{
	return m_device;
}

ID3D11DeviceContext* DX11Renderer::GetDeviceContext()
{
	return m_deviceContext;
}

void DX11Renderer::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
	projectionMatrix = m_projectionMatrix;
}

void DX11Renderer::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
	worldMatrix = m_worldMatrix;
}

void DX11Renderer::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
	orthoMatrix = m_orthoMatrix;
	return;
}

void DX11Renderer::GetVideoCardInfo(char* cardName, int& memory)
{
	strcpy_s(cardName, GPU_DESC_LENGTH, m_gpuDescription);
	memory = m_gpuMemory;
	return;
}

void DX11Renderer::SetBackBufferRenderTarget()
{
	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void DX11Renderer::ResetViewport()
{
	// Set the viewport.
	m_deviceContext->RSSetViewports(1, &m_viewport);
}

unsigned int DX11Renderer::createShader(const unsigned int shaderType, const std::string& shaderSource)
{
	return 0;
}

unsigned int DX11Renderer::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	return 0;
}

bool DX11Renderer::renderFrame()
{
	startFrame(0.1f, 0.8f, 0.2f, 1.0f);
	endFrame();

	return true;
}

bool DX11Renderer::isInitialized()
{
	return true;
}