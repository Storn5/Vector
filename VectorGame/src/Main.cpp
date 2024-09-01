#define WIN32_LEAN_AND_MEAN

#include "Config.h"
#include "Engine.h"
#include "Enums.h"
#include <Windows.h>
#include <stdexcept>
#include <string>

// Indicates to hybrid graphics systems to prefer the discrete part by default
extern "C"
{
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

int parseIntArg(const char* arg, const char* argName, unsigned int& argToSet)
{
	try {
		argToSet = std::stoi(arg, nullptr);
	}
	catch (std::invalid_argument const& ex) {
		fprintf(stderr, "Error parsing command-line arguments. %s must be a number\n", argName);
		return 1;
	}
	catch (std::out_of_range const& ex) {
		fprintf(stderr, "Error parsing command-line arguments. %s out of range\n", argName);
		return 1;
	}
	return 0;
}

int parseArgs(int argc, const char** argv, GraphicsConfig& graphicsConfig)
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "opengl") == 0)
			graphicsConfig.renderer = RendererEnum::OPENGL;
		else if (strcmp(argv[1], "vulkan") == 0)
			graphicsConfig.renderer = RendererEnum::VULKAN;
		else if (strcmp(argv[1], "dx11") == 0)
			graphicsConfig.renderer = RendererEnum::DX11;
		else if (strcmp(argv[1], "dx12") == 0)
			graphicsConfig.renderer = RendererEnum::DX12;
		else
		{
			fprintf(stderr, "Error parsing first command-line argument. Options: opengl, vulkan, dx11, dx12. Default is opengl\n");
			return 1;
		}
	}

	if (argc > 2)
	{
		for (int i = 2; i < argc; i++)
		{
			if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fullscreen") == 0)
				graphicsConfig.isFullscreen = true;
			else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--vsync") == 0)
				graphicsConfig.vSync = true;
			else if (strcmp(argv[i], "-x") == 0 || strcmp(argv[i], "--xresolution") == 0)
			{
				++i;
				if (i >= argc)
				{
					fprintf(stderr, "Error parsing command-line arguments. X resolution must have a value\n");
					return 1;
				}
				if (parseIntArg(argv[i], "X resolution", graphicsConfig.xRes) == 1)
					return 1;
			}
			else if (strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "--yresolution") == 0)
			{
				++i;
				if (i >= argc)
				{
					fprintf(stderr, "Error parsing command-line arguments. Y resolution must have a value\n");
					return 1;
				}
				if (parseIntArg(argv[i], "Y resolution", graphicsConfig.yRes) == 1)
					return 1;
			}
			else
				return 1;
		}
	}

	return 0;
}

int main(int argc, const char** argv)
{
	// Configure default settings
	GraphicsConfig graphicsConfig
	{
		"Vector",				// Window name
		RendererEnum::OPENGL,	// Graphics API
		false,					// Fullscreen
		false,					// V-Sync
		800,					// X resolution
		600,					// Y resolution
		60,						// Refresh rate
		0.3f,					// Near clipping plane
		1000.0f					// Far clipping plane
	};
	// Read settings from command-line args
	int errorCode = parseArgs(argc, argv, graphicsConfig);
	if (errorCode != 0)
	{
		fprintf(stderr, "\nUsage: Vector.exe {opengl|dx11|dx12|vulkan} [options]\n"
						"  options:\n"
						"\t-f, --fullscreen\tStart in fullscreen mode\n"
						"\t-v, --vsync\tEnable vertical synchronization (OpenGL might have this enabled by default)\n"
						"\t-x <x>, --xresolution <x>\tSet horizontal resolution (only in windowed mode), default %d\n"
						"\t-y <y>, -yresolution <y>\tSet vertical resolution (only in windowed mode), default %d",
			800, 600);
		return 1;
	}

	// Initialize the game engine (including the window manager, graphics system, input system and audio system)
	Engine engine(graphicsConfig);
	if (!engine.isInitialized())
	{
		fprintf(stderr, "Game engine failed to initialize\n");
		return 1;
	}

	// Start the main game loop
	engine.startGame();
}