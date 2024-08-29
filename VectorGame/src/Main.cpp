#include "Engine.h"
#include "Enums.h"
#include "Config.h"
#include <stdio.h>
#include <string.h>

int validateArgs(int argc, const char** argv, GraphicsBackendEnum& graphicsBackend, GraphicsConfig& config)
{
	if (argc < 2)
	{
		graphicsBackend = GraphicsBackendEnum::OPENGL;
	}
	for (int i = 1; i < argc; i++)
	{
		if (i == 1)
		{
			if (strcmp(argv[i], "opengl") == 0)
				graphicsBackend = GraphicsBackendEnum::OPENGL;
			else if (strcmp(argv[i], "vulkan") == 0)
				graphicsBackend = GraphicsBackendEnum::VULKAN;
			else if (strcmp(argv[i], "dx11") == 0)
				graphicsBackend = GraphicsBackendEnum::DX11;
			else if (strcmp(argv[i], "dx12") == 0)
				graphicsBackend = GraphicsBackendEnum::DX12;

			if (graphicsBackend == GraphicsBackendEnum::NONE)
			{
				fprintf(stderr, "Error parsing first commandline argument. Options: opengl, vulkan, dx11, dx12. Default is opengl\n");
				return 1;
			}
		}
	}
	return 0;
}

int main(int argc, const char** argv)
{
	auto graphicsBackend = GraphicsBackendEnum::NONE;
	GraphicsConfig graphicsConfig
	{
		"Vector",	// Window name
		1920,		// X resolution
		1080		// Y resolution
	};
	int errorCode = validateArgs(argc, argv, graphicsBackend, graphicsConfig);
	if (errorCode != 0)
	{
		fprintf(stderr, "Error parsing commandline arguments. Usage: Vector.exe [opengl|vulkan|dx11|dx12]\n");
		return 1;
	}

	Engine engine(graphicsBackend, graphicsConfig);
	if (!engine.isInitialized)
	{
		fprintf(stderr, "Game engine failed to initialize\n");
		return 1;
	}
	engine.playGame();
}