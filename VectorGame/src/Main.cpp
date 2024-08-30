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
		if (strcmp(argv[2], "win32") == 0)
			graphicsConfig.windowSystem = WindowSystemEnum::Win32;
		else if (strcmp(argv[2], "glfw") == 0)
			graphicsConfig.windowSystem = WindowSystemEnum::GLFW;
		else
		{
			fprintf(stderr, "Error parsing second command-line argument. Options: win32, glfw (only for OpenGL). Default is glfw\n");
			return 1;
		}
	}
	
	// Check incompatible settings
	if (graphicsConfig.windowSystem == WindowSystemEnum::GLFW && graphicsConfig.graphicsBackend != GraphicsBackendEnum::OPENGL)
	{
		fprintf(stderr, "Error parsing second command-line argument. The glfw option is only available for OpenGL\n");
		return 1;
	}
	
	return 0;
}

int main(int argc, const char** argv)
{
	// Configure default settings
	GraphicsConfig graphicsConfig
	{
		GraphicsBackendEnum::OPENGL,
		WindowSystemEnum::GLFW,
		"Vector",	// Window name
		true,		// Fullscreen
		800,		// X resolution
		600			// Y resolution
	};
	// Read settings from command-line args
	int errorCode = validateArgs(argc, argv, graphicsConfig);
	if (errorCode != 0)
	{
		fprintf(stderr, "Error parsing commandline arguments. Usage: Vector.exe [opengl|vulkan|dx11|dx12] [win32|glfw]\n");
		return 1;
	}

	// Initialize the game engine (including the window, graphics system, input system and audio system)
	Engine engine(graphicsConfig);
	if (!engine.isInitialized())
	{
		fprintf(stderr, "Game engine failed to initialize\n");
		return 1;
	}

	// Start the game loop
	engine.startGame();
}