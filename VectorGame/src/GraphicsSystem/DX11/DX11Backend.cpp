#define _USE_MATH_DEFINES
#include <cmath>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <chrono>
#include "GraphicsSystem/DX11/DX11Backend.h"
#include "Utils/Helpers.h"

DX11Backend::DX11Backend(const GraphicsConfig& config)
    : m_config(config)
{
	
}

DX11Backend::~DX11Backend()
{
	
}

unsigned int DX11Backend::createShader(const unsigned int shaderType, const std::string& shaderSource)
{
	return 0;
}

unsigned int DX11Backend::createShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource)
{
	return 0;
}

bool DX11Backend::renderFrame()
{
	return true;
}

bool DX11Backend::isInitialized()
{
	return true;
}