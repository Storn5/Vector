#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include "DX11/DX11Backend.h"
#include "Utils/Helpers.h"

DX11Backend::DX11Backend(const GraphicsConfig& config)
    : m_config(config)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	unsigned long wStyle = 0;
	int posX, posY;

	// Get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// Setup the window class
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_config.windowName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class
	RegisterClassEx(&wc);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode
	if (m_config.isFullscreen)
	{
		// Determine the screen resolution
		m_config.xRes = GetSystemMetrics(SM_CXSCREEN);
		m_config.yRes = GetSystemMetrics(SM_CYSCREEN);

		// Set the screen to maximum size of the users desktop and 32bit
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)m_config.xRes;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_config.yRes;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);
		wStyle = WS_POPUP;

		// Set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		// Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - m_config.xRes) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - m_config.yRes) / 2;
		wStyle = WS_TILEDWINDOW;
	}

	// Create the window with the screen settings and get the handle to it
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_config.windowName, m_config.windowName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | wStyle | WS_VISIBLE,
		posX, posY, m_config.xRes, m_config.yRes, NULL, NULL, m_hInstance, NULL);

	// Set focus
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Don't hide the mouse cursor
	ShowCursor(true);

	return;
}

DX11Backend::~DX11Backend()
{
	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (m_config.isFullscreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Destroy the window
	DestroyWindow(m_hWnd);
	m_hWnd = NULL;

	// Destroy the instance
	UnregisterClass(m_config.windowName, m_hInstance);
	m_hInstance = NULL;
}

// Win32 callback function to handle messages from Windows
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			PostQuitMessage(0);
			return 0;
		break;

	/*
	case WM_KEYDOWN:
		m_Input->KeyDown((unsigned int)wParam);
		return 0;
	case WM_KEYUP:
		m_Input->KeyUp((unsigned int)wParam);
		return 0;
	*/

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

unsigned int DX11Backend::createShader(const unsigned int shaderType, const std::string& shaderSource)
{
	return 0;
}

unsigned int DX11Backend::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	return 0;
}

unsigned int DX11Backend::parseShaderFile(const std::string& filename)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::ifstream filestream(filename);

    if (!filestream)
    {
        fprintf(stderr, "Error parsing shader: file %s cannot be read\n", filename.c_str());
        return 0;
    }

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(filestream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else
            {
                fprintf(stderr, "Error parsing shader: invalid shader type\n");
                return 0;
            }
        }
        else
            ss[(int)type] << line << "\n";
    }

    return createShaderProgram(ss[0].str(), ss[1].str());
}

bool DX11Backend::renderFrame()
{
	/*
	// Check if the user pressed escape and wants to exit the application.
	if (m_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the application class object.
	bool result = m_Application->Frame();
	if (!result)
	{
		return false;
	}
	*/

	return true;
}

bool DX11Backend::isInitialized()
{
	if (m_hWnd == nullptr)
		return false;

	return true;
}

void DX11Backend::playGameLoop()
{
	MSG msg;
	bool done, result;

	// Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user
	done = false;
	while (!done)
	{
		// Handle Windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If Windows signals to end the application then exit out
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = renderFrame();
			if (!result)
			{
				done = true;
			}
		}
	}
}