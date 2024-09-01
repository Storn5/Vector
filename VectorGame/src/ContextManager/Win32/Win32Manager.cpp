#include "ContextManager/Win32/Win32Manager.h"
#include <fstream>

Win32Manager::Win32Manager(const GraphicsConfig& config)
    : m_config(config)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	unsigned long wStyle = 0;
	int posX = 0, posY = 0;

	// Get the instance of this application
	m_hInstance = GetModuleHandle(nullptr);

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
	wc.lpszMenuName = nullptr;
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
		posX, posY, m_config.xRes, m_config.yRes, NULL, NULL, m_hInstance, nullptr);

	// Set focus
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	// Don't hide the mouse cursor
	ShowCursor(true);
}

Win32Manager::~Win32Manager()
{
	// Show the mouse cursor
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode
	if (m_config.isFullscreen)
	{
		ChangeDisplaySettings(nullptr, 0);
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

bool Win32Manager::isInitialized()
{
	if (m_hWnd == nullptr)
	{
		fprintf(stderr, "Error creating a Win32 window handle\n");
		return false;
	}

	return true;
}

bool Win32Manager::preFrame()
{
	MSG msg;

	// Initialize the message structure
	ZeroMemory(&msg, sizeof(MSG));

	// Handle Windows messages
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// If Windows signals to end the application then exit out
	if (msg.message == WM_QUIT)
	{
		return false;
	}

	return true;
}

bool Win32Manager::postFrame()
{
	return true;
}

HWND Win32Manager::getWindowHandle() const
{
	return m_hWnd;
}