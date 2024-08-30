#include "Engine.h"
#include "Enums.h"
#include "Config.h"
#include <stdio.h>
#include <string.h>

int validateArgs(int argc, const char** argv, GraphicsConfig& graphicsConfig)
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "opengl") == 0)
			graphicsConfig.graphicsBackend = GraphicsBackendEnum::OPENGL;
		else if (strcmp(argv[1], "vulkan") == 0)
			graphicsConfig.graphicsBackend = GraphicsBackendEnum::VULKAN;
		else if (strcmp(argv[1], "dx11") == 0)
			graphicsConfig.graphicsBackend = GraphicsBackendEnum::DX11;
		else if (strcmp(argv[1], "dx12") == 0)
			graphicsConfig.graphicsBackend = GraphicsBackendEnum::DX12;
		else
		{
			fprintf(stderr, "Error parsing first command-line argument. Options: opengl, vulkan, dx11, dx12. Default is opengl\n");
			return 1;
		}
	}

	if (argc > 2)
	{
		if (strcmp(argv[2], "-f") == 0)
			graphicsConfig.isFullscreen = true;
		else
		{
			return 1;
		}
	}

	if (graphicsConfig.graphicsBackend == GraphicsBackendEnum::OPENGL)
		graphicsConfig.contextManager == ContextManagerEnum::GLFW;
	else
		graphicsConfig.contextManager = ContextManagerEnum::Win32;
	
	return 0;
}

int main(int argc, const char** argv)
{
	// Configure default settings
	GraphicsConfig graphicsConfig
	{
		GraphicsBackendEnum::OPENGL,
		ContextManagerEnum::GLFW,
		"Vector",	// Window name
		false,		// Fullscreen
		800,		// X resolution
		600			// Y resolution
	};
	// Read settings from command-line args
	int errorCode = validateArgs(argc, argv, graphicsConfig);
	if (errorCode != 0)
	{
		fprintf(stderr, "Error parsing commandline arguments. Usage: Vector.exe [opengl|vulkan|dx11|dx12] [-f]\n");
		return 1;
	}

	// Initialize the game engine (including the window manager, graphics system, input system and audio system)
	Engine engine(graphicsConfig);
	if (!engine.isInitialized())
	{
		fprintf(stderr, "Game engine failed to initialize\n");
		return 1;
	}

	// Start the game loop
	engine.startGame();
}